#pragma once
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/numpy.h>

namespace py = pybind11;
using namespace py::literals;

namespace DC_Data
{
	enum DataTypes
	{
		DEFAULT_DATA, TIME_DATA
	};
}

using namespace DC_Data;

template<typename T, size_t dim, DataTypes bind>
inline static py::array_t<T> getBEMC();

//This class is to be used only in the python client side -> see src/core/api/include/dcscan.h for more info
//This class references a generic data type to be transfered over to client side
//Only supported dim is 1
template<typename T, size_t dim>
class DataChunk
{
public:
	template<typename T, size_t dim, DataTypes bind>
	friend inline static py::array_t<T> getBEMC();

	DataChunk(size_t size, DataTypes bind);
	~DataChunk();

	inline size_t resize(const size_t bytes);

	inline size_t set(const T* data);
	inline size_t add(const T* data, const size_t size);
	inline size_t add(const T data);

	inline size_t dis();

private:
	typedef T Data[dim];
	Data* data = { nullptr }; //This is the buffer that will be passed to scripts (e.g. python)
	size_t data_size = 0;
	size_t heap = 0;
	size_t total_ptr = 0;
	std::mutex mtx;

	Data* curr_ptr = nullptr;

	inline static std::unordered_map<DataTypes, DataChunk*> binds;
};

#pragma warning( push )
#pragma warning( disable : 26812)
template<typename T, size_t dim>
DataChunk<T, dim>::DataChunk(size_t size, DataTypes bind)
{
	//Same as calloc for the data
	data = new Data[size]();
	curr_ptr = data;
	this->heap = size * sizeof(T);
	data_size = size;
	DataChunk::binds.try_emplace(bind, this);
}
#pragma warning( pop ) 

template<typename T, size_t dim>
DataChunk<T, dim>::~DataChunk()
{
	delete[] data;
}

template<typename T, size_t dim>
inline size_t DataChunk<T, dim>::resize(const size_t bytes)
{
	return size_t();
}

template<typename T, size_t dim>
inline size_t DataChunk<T, dim>::set(const T* data)
{
	std::lock_guard<std::mutex> lock(mtx);
	std::memcpy(this->data, data, heap);
	return heap;
}

template<typename T, size_t dim>
inline size_t DataChunk<T, dim>::add(const T* data, const size_t size)
{
	std::lock_guard<std::mutex> lock(mtx);
	//Check if it is possible to add the data // else return error
	//TODO: Allocate more space instead
	size_t local = size * sizeof(T);
	assert(total_ptr + local <= heap);
	total_ptr += local;

	std::memcpy(this->curr_ptr, data, local);
	this->curr_ptr += local;
	return heap - total_ptr;
}

template<typename T, size_t dim>
inline size_t DataChunk<T, dim>::add(const T data)
{
	return add(&data, 1);
}

template<typename T, size_t dim>
inline size_t DataChunk<T, dim>::dis()
{
	total_ptr = 0;
	this->curr_ptr = this->data;
	return heap;
}

template<typename T, size_t dim, DataTypes bind>
inline py::array_t<T> getBEMC()
{
#pragma warning( push )
#pragma warning( disable : 26812)
	DataChunk<T, dim>* ptr = nullptr;
	try
	{
		ptr = DataChunk<T, dim>::binds.at(bind);
	}
	catch (std::out_of_range & oor)
	{
		return py::array_t<T>();
	}
#pragma warning( pop ) 

	py::array_t<T> out = py::array_t<T>(ptr->heap);

	py::buffer_info buffer = out.request();

	T* dataptr = (T*)buffer.ptr;

	std::lock_guard<std::mutex> lock(ptr->mtx);
	for (int i = 0; i < ptr->data_size; i++)
	{
		dataptr[i] = ptr->data[i][0];
	}

	return out;
}


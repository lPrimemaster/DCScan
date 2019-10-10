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

	DataChunk(size_t heap, DataTypes bind);
	~DataChunk();

	inline size_t set(const T* data);

private:
	typedef T Data[dim];
	Data* data = { nullptr }; //This is the buffer that will be passed to scripts (e.g. python)
	size_t data_size = 0;
	size_t heap = 0;
	std::mutex mtx;

	inline static std::unordered_map<DataTypes, DataChunk*> binds;
};

#pragma warning( push )
#pragma warning( disable : 26812)
template<typename T, size_t dim>
DataChunk<T, dim>::DataChunk(size_t heap, DataTypes bind)
{
	//Same as calloc for the data
	data = new Data[heap]();
	this->heap = heap;
	data_size = heap / sizeof(T);
	DataChunk::binds.try_emplace(bind, this);
}
#pragma warning( pop ) 

template<typename T, size_t dim>
DataChunk<T, dim>::~DataChunk()
{
	delete[] data;
}

template<typename T, size_t dim>
inline size_t DataChunk<T, dim>::set(const T* data)
{
	std::lock_guard<std::mutex> lock(mtx);
	std::memcpy(this->data, data, heap);
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


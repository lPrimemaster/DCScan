#pragma once
#include <iostream>

//This class is to be used only in the python client side -> see src/core/api/include/dcscan.h for more info
//This class references a generic data type to be transfered over to client side
template<typename T, size_t heap, size_t dim>
class DataChunk
{
public:
	DataChunk();
	~DataChunk();

private:
	typedef T Data[dim];
	Data* data = { nullptr }; //This is the buffer that will be passed to scripts (e.g. python)
	size_t data_size = 0;
};

template<typename T, size_t heap, size_t dim>
DataChunk<T, heap, dim>::DataChunk()
{
	//Same as calloc for the data
	data = new Data[heap]();
}

template<typename T, size_t heap, size_t dim>
DataChunk<T, heap, dim>::~DataChunk()
{
	delete[] data;
}


/*************************************************************************
    > File Name: DataBlob.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:55:19 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"DataBlob.h"
//#include<CL/cl.h>

template <typename Dtype>
void 
DataBlob<Dtype>::Reshape_HOST(const int n, const int c, const int h, const int w)
{
	num_ = n;
	channels_ = c;
	height_ = h;
	width_ = w;
    count_ = num_ * channels_ * height_ * width_;
	if (count_ > capacity_ )
	{
		capacity_ = count_;
	    host_data_ptr_ =(Dtype *) malloc(capacity_ * sizeof(Dtype));
	}
}

template <typename Dtype>
void
DataBlob<Dtype>::ReshapeLike(const int n, const int c, const int h, const int w)
{
    num_ = n;
    channels_ = c;
    height_ = h;
    width_ = w;
    count_ = num_ * channels_ * height_* width_;
    capacity_ =count_;
    
}
template <typename Dtype>
void
DataBlob<Dtype>::CopyFromMemory(void* data)
{
    host_data_ptr_ = data;    
}
/*
template <typename Dtype>
void 
DataBlob<Dtype>::Reshape_DEVICE(const int n, const int c, const int h, const int w)
{
	num_ = n;
	channels_ = c;
	height_ = h;
	width_ = w;
    count_ = num_ * channels_ * height_ * width_;
	if (count_ > capacity_ )
	{
		capacity_ = count_;
	    device_data_ptr_ = malloc(sizeof(cl_mem));
		*device_data_ptr_ = clCreateBuffer(device_->DeviceContext(), CL_ALLOC_HOST_PTR, NULL, NULL);
	}  
}
*/

template <typename Dtype>
DataBlob<Dtype>::DataBlob(const int n, const int c, const int h, const int w)
	: capacity_(0)
{
    Reshape_HOST(n, c , h ,w);
	data_state_ = HOST;	
}
/*
template <typename Dtype>
DataBlob<Dtype>::DataBlob(const int n, const int c, const int h, const int w, BaseDevice* device)
	: capacity_(0)
{
	if (device == NULL)
	{
		Reshape_HOST(n, c , h ,w);
		data_state_ = HOST;
	}
	else 
	{
		data_state_ = DEVICE;
		device_ = device;
		Reshape_DEVICE(n, c, h ,w);
	}
	
}
*/

/*
template <typename Dtype>
void
DataBlob<Dtype>::CopyFromCPUtoGPU()
{
    switch(data_state_)
    {
        case DEVICE :
        {
            
            break;
        }
        case HOST:
        {
            Reshape_DEVICE()
        }
    }
}
*/
template class DataBlob<float>;
template class DataBlob<double>;
template class DataBlob<int>;
template class DataBlob<long>;

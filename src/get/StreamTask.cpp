/*************************************************************************
    > File Name: StreamTask.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:53:07 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"StreamTask.h"
#include<CL/cl.h>

template <typename Dtype>
StreamTask<Dtype>::~StreamTask()
{
    datas_.clear();
    results_.clear();

    for(int i = 0; i < device_buffers_.size(); i++)
    {
        clReleaseMemObject(device_buffers_[i]);
    }
    device_buffers_.clear();
    inner_datas_.clear();
    inner_results_.clear();
}

template <typename Dtype>
void
StreamTask<Dtype>::AddtoDatas(DataBlob<Dtype> data)
{
    datas_.push_back(data);
}

template <typename Dtype>
void 
StreamTask<Dtype>::AddtoResults(DataBlob<Dtype> data)
{
    results_.push_back(data);
}

template class StreamTask<float>;
template class StreamTask<double>;
template class StreamTask<int>;
template class StreamTask<long>;

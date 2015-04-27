/*************************************************************************
    > File Name: BaseTask.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:53:07 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"BaseTask.h"
#include<CL/cl.h>

template <typename Dtype>
BaseTask<Dtype>::~BaseTask()
{
    datas_.clear();
    results_.clear();

    for(int i = 0; i < device_buffers_.size(); i++)
    {
        clReleaseMemObject(device_buffers_[i]);
    }
    device_buffers_.clear();
}

template <typename Dtype>
void
BaseTask<Dtype>::AddtoDatas(DataBlob<Dtype> data)
{
    datas_.push_back(data);
}

template <typename Dtype>
void 
BaseTask<Dtype>::AddtoResults(DataBlob<Dtype> data)
{
    results_.push_back(data);
}

template class BaseTask<float>;
template class BaseTask<double>;
template class BaseTask<int>;
template class BaseTask<long>;

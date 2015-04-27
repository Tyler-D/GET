/*************************************************************************
    > File Name: StreamTask.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Mon 23 Mar 2015 11:58:41 PM EDT
 ************************************************************************/
#ifndef GET_STREAM_TASK_HEADER
#define GET_STREAM_TASK_HEADER
#include<iostream>
using namespace std;
#include"BaseDevice.h"
#include"DataBlob.h"
#include<CL/cl.h>


template <typename Dtype>
class StreamTask
{
    public:
    ~StreamTask();
    explicit StreamTask(BaseDevice* device) 
    {
        device_ = device;
    }
    //function to start Task
    void AddtoDatas(DataBlob<Dtype> data);
    void AddtoResults(DataBlob<Dtype> data);
   

    virtual void PreCompute() {};
    virtual void Compute() {};
    virtual void PostCompute() {};
    //virtual void SetParams(TaskParam param);
    
    inline void TaskOn()
    {
        PreCompute();
        Compute();
        PostCompute();
    }

    protected:
    vector<DataBlob<Dtype> > datas_;
    BaseDevice* device_;
    vector<DataBlob<Dtype> > results_;
    vector<cl_mem> device_buffers_;
    vector<DataBlob<Dtype> > inner_datas_;
    vector<DataBlob<Dtype> > inner_results_;
    long data_per_block_;
    int blocks_num_;
    
};
#endif

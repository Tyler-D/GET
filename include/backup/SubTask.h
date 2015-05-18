/*************************************************************************
    > File Name: SubTask.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Mon 23 Mar 2015 11:58:50 PM EDT
 ************************************************************************/
#ifndef GET_SUB_TASK_HEADER
#define GET_SUB_TASK_HEADER
#include<iostream>
#include<CL/cl.h>
#include"BaseTask.h"
#include"BaseDevice.h"
#include"proto/GET.pb.h"

using namespace std;


//template <typename Dtype>
//using BaseTask<Dtype>::datas_;
//using BaseTask::device_;
//using BaseTask::results_;
//using BaseTask::device_buffers_;
template <typename Dtype>
class SubTask : public BaseTask<Dtype>
{
    public:
    ~SubTask() {};
    //explicit AddTask(BaseDevice* device) {device_ = device;}
    SubTask(BaseDevice* device):
        BaseTask<Dtype>(device)
    {

    }
    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(GET::SubParam param) {};
    void SetParams(int channels, int height, int width)
    {
        channels_ = channels;
        height_ = (cl_int)height;
        width_ = (cl_int)width;
    }

    protected:
    cl_int height_, width_;
    int channels_;
};
#endif

/*************************************************************************
    > File Name: BaseTask.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Mon 23 Mar 2015 11:58:41 PM EDT
 ************************************************************************/
#ifndef GET_BASE_TASK_HEADER
#define GET_BASE_TASK_HEADER
#include<iostream>
using namespace std;
#include"BaseDevice.h"
#include"DataBlob.h"
#include<CL/cl.h>
#include<pthread.h>
#include"proto/GET.pb.h"

template <typename Dtype>
class BaseTask
{
    public:
    ~BaseTask();
    explicit BaseTask(BaseDevice* device) {device_ = device;}
    //function to start Task
    void AddtoDatas(DataBlob<Dtype> data);
    void AddtoResults(DataBlob<Dtype> data);
   

    virtual void PreCompute() {};
    virtual void Compute() {};
    virtual void PostCompute() {};
    //virtual void SetParams(TaskParam param);
    

    protected:
    vector<DataBlob<Dtype> > datas_;
    BaseDevice* device_;
    vector<DataBlob<Dtype> > results_;
    vector<cl_mem> device_buffers_;
    
};

template <typename Dtype>
static void * 
BaseTaskOn(void *args)
{
    BaseTask<Dtype>* task = (BaseTask<Dtype>*)args;
    task->PreCompute();
    task->Compute();
    task->PostCompute();
    pthread_exit(NULL);
}

template <typename Dtype>
class AddTask : public BaseTask<Dtype>
{
    public:
    ~AddTask() {};
    //explicit AddTask(BaseDevice* device) {device_ = device;}
    AddTask(BaseDevice* device):
        BaseTask<Dtype>(device)
    {

    }
    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(GET::AddParam param) {};
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

template <typename Dtype>
class MulTask : public BaseTask<Dtype>
{
    public:
    ~MulTask() {};
    //explicit AddTask(BaseDevice* device) {device_ = device;}
    MulTask(BaseDevice* device):
        BaseTask<Dtype>(device)
    {

    }
    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(GET::MulParam param) {};
    void SetParams(int channels, int M, int K, int N)
    {
        channels_ = channels;
        M_ = (cl_uint)M;
        K_ = (cl_uint)K;
        N_ = (cl_uint)N;
    }

    protected:
    cl_uint M_, K_, N_;
    int channels_;
};


template <typename Dtype>
class ConvTask : public BaseTask<Dtype>
{
    public:
    ~ConvTask() {};
    ConvTask(BaseDevice* device):
        BaseTask<Dtype>(device)
    {

    }

    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(GET::ConvParam param);
    void SetParams(int channels, int data_h, int data_w, int filter_h, int filter_w, int stride_h, int stride_w, int pad_h, int pad_w)
    {
        channels_ = (cl_uint)channels;
        data_h_ = (cl_uint)data_h;
        data_w_ = (cl_uint)data_w;
        filter_h_ = (cl_uint)filter_h;
        filter_w_ = (cl_uint)filter_w;
        stride_h_ = (cl_uint)stride_h;
        stride_w_ = (cl_uint)stride_w;
        pad_h_ = (cl_uint)pad_h;
        pad_w_ = (cl_uint)pad_w;
        output_h_ = (data_h_ - filter_h_) / stride_h_ + 1;
        output_w_ = (data_w_ - filter_w_) / stride_w_ + 1;     
    }

    protected:
    cl_uint data_h_, data_w_;
    cl_uint filter_h_, filter_w_;
    cl_uint stride_h_, stride_w_;
    cl_uint pad_h_, pad_w_;
    cl_uint output_h_, output_w_;
    cl_uint channels_;

};

template <typename Dtype>
class PoolTask : public BaseTask<Dtype>
{
    public:
    ~PoolTask() {};
    PoolTask(BaseDevice* device):
        BaseTask<Dtype>(device)
    {

    }

    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(GET::PoolParam param);
    void SetParams(int channels, int data_h, int data_w, int kernel_h, int kernel_w, int stride_h, int stride_w, int pad_h, int pad_w)
    {
        channels_ = (cl_uint)channels;
        data_h_ = (cl_uint)data_h;
        data_w_ = (cl_uint)data_w;
        kernel_h_ = (cl_uint)kernel_h;
        kernel_w_ = (cl_uint)kernel_w;
        stride_h_ = (cl_uint)stride_h;
        stride_w_ = (cl_uint)stride_w;
        pad_h_ = (cl_uint)pad_h;
        pad_w_ = (cl_uint)pad_w;
        output_h_ = (data_h_ - kernel_h_) / stride_h_ + 1;
        output_w_ = (data_w_ - kernel_w_) / stride_w_ + 1;     
    }

    protected:
    cl_uint data_h_, data_w_;
    cl_uint kernel_h_, kernel_w_;
    cl_uint stride_h_, stride_w_;
    cl_uint pad_h_, pad_w_;
    cl_uint output_h_, output_w_;
    cl_uint channels_;

};

template <typename Dtype>
class ReLUTask : public BaseTask<Dtype>
{
    public:
    ~ReLUTask() {};
    //explicit AddTask(BaseDevice* device) {device_ = device;}
    ReLUTask(BaseDevice* device):
        BaseTask<Dtype>(device)
    {

    }
    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(GET::ReLUParam param) {};
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

template <typename Dtype>
class SigmoidTask : public BaseTask<Dtype>
{
    public:
    ~SigmoidTask() {};
    //explicit AddTask(BaseDevice* device) {device_ = device;}
    SigmoidTask(BaseDevice* device):
        BaseTask<Dtype>(device)
    {

    }
    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(GET::ReLUParam param) {};
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

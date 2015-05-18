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
#include"proto/GET.pb.h"
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

template <typename Dtype>
class AddStreamTask: public StreamTask<Dtype>
{
    public:
        ~AddStreamTask();
        explicit AddStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(int channels, int height, int width)
    {
        channels_ = channels;
        height_ = height;
        width_ = width;
    }
    protected:
    cl_int height_, width_;
    int channels_;

};

template <typename Dtype>
class SubStreamTask: public StreamTask<Dtype>
{
    public:
        ~SubStreamTask();
        explicit SubStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(int channels, int height, int width)
    {
        channels_ = channels;
        height_ = height;
        width_ = width;
    }
    protected:
    cl_int height_, width_;
    int channels_;

};

template <typename Dtype>
class MulStreamTask: public StreamTask<Dtype>
{
    public:
        ~MulStreamTask();
        explicit MulStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
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
class ConvStreamTask: public StreamTask<Dtype>
{
    public:
        ~ConvStreamTask();
        explicit ConvStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
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
class PoolStreamTask: public StreamTask<Dtype>
{
    public:
        ~PoolStreamTask();
        explicit PoolStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
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
class ReLUStreamTask: public StreamTask<Dtype>
{
    public:
        ~ReLUStreamTask();
        explicit ReLUStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(int channels, int height, int width)
    {
        channels_ = channels;
        height_ = height;
        width_ = width;
    }
    protected:
    cl_int height_, width_;
    int channels_;

};

template <typename Dtype>
class SigmoidStreamTask: public StreamTask<Dtype>
{
    public:
        ~SigmoidStreamTask();
        explicit SigmoidStreamTask(BaseDevice* device):
        StreamTask<Dtype>(device)
        { 
            this->data_per_block_ =device->GlobalMemory() / 6;
        }

    void PreCompute();
    void Compute();
    void PostCompute();
    void SetParams(int channels, int height, int width)
    {
        channels_ = channels;
        height_ = height;
        width_ = width;
    }
    protected:
    cl_int height_, width_;
    int channels_;

};
#endif

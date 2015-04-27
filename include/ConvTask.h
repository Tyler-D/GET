/*************************************************************************
	> File Name: ConvTask.h
	> Author: 
	> Mail: 
	> Created Time: Thu 23 Apr 2015 09:21:10 AM EDT
 ************************************************************************/

#ifndef GET_CONVTASK_H
#define GET_CONVTASK_H
#include"BaseDevice.h"
#include"DataBlob.h"
#include<CL/cl.h>
#include"proto/GET.pb.h"

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

    protected:
    cl_int data_h_, data_w_;
    cl_int filter_h_. filter_w_;
    cl_int stride_h_, stride_w_;
    cl_int pad_h_, pad_w_;
    cl_int output_h_, output_w_;

}

#endif

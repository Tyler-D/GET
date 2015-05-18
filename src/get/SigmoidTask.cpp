/*************************************************************************
    > File Name: SigmoidTask.cpp
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Tue 24 Mar 2015 02:55:48 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"BaseTask.h"
#include<CL/cl.h>
#include"opencl_z.h"
#include<typeinfo>

#define GLOBAL_WORKGROUP_HEIGHT 4096
#define GLOBAL_WORKGROUP_WIDTH 4096
#define LOCAL_WORKGROUP_HEIGHT 16
#define LOCAL_WORKGROUP_WIDTH 16

template <typename Dtype>
void
SigmoidTask<Dtype>::PreCompute()
{
    //set device buffer
    cl_mem temp_buffer;
    cl_int status;

    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_READ_ONLY, height_*width_*sizeof(Dtype), NULL, &status);
    cl_pStatus(status, "clCreateBuffer 1");
    
    this->device_buffers_.push_back(temp_buffer);


    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_WRITE_ONLY, height_*width_*sizeof(Dtype), NULL, &status);
    cl_pStatus(status, "clCreateBuffer 2");
    
    this->device_buffers_.push_back(temp_buffer);
    
    //copy data from host to device
    status = clEnqueueWriteBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[0], CL_TRUE, 0, height_*width_*sizeof(Dtype), (Dtype*)(this->datas_[0].host_data()), 0, NULL, NULL );
    cl_pStatus(status, "clEnqueueWriteBuffer");

}

template <typename Dtype>
void
SigmoidTask<Dtype>::Compute()
{
   //read program source and create kernel
    cl_int status;
    
    char* program_source =NULL;

    if (typeid(Dtype) == typeid(int))
         program_source = cl_readSource("/home/zy/GET/src/get/sig_task_int.cl");
    if (typeid(Dtype) == typeid(long))
         program_source = cl_readSource("/home/zy/GET/src/get/sig_task_long.cl");
    if (typeid(Dtype) == typeid(float))
         program_source = cl_readSource("/home/zy/GET/src/get/sig_task_float.cl");
    if (typeid(Dtype) == typeid(double))
         program_source = cl_readSource("/home/zy/GET/src/get/sig_task_double.cl");

    cl_program program = clCreateProgramWithSource(this->device_->DeviceContext(), 1, (const char**)&program_source, NULL, &status);
    cl_pStatus(status, "clCreateProgramWithSource");

    cl_device_id temp_device = this->device_->DeviceID();
    status = clBuildProgram(program, 1, &temp_device, NULL, NULL, NULL);
    if (status != CL_SUCCESS)
    {
         char buildLog[16384];
         clGetProgramBuildInfo(program, temp_device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
         cout << "Error in Kernel : "<<endl;
         cout <<buildLog;     
    }
    cl_pStatus(status, "clBuildProgram");

    cl_kernel kernel = NULL;
    kernel = clCreateKernel(program, "sigMatrix", &status);
    cl_pStatus(status, "clCreateKernel");
    
    cl_uint thread_division_row = 1;
    cl_uint thread_division_col = 1;
    if (height_ > GLOBAL_WORKGROUP_HEIGHT)
        thread_division_row = (height_ + GLOBAL_WORKGROUP_HEIGHT - 1) / GLOBAL_WORKGROUP_HEIGHT;
           
    if (width_ > GLOBAL_WORKGROUP_WIDTH)
        thread_division_col = (width_ + GLOBAL_WORKGROUP_WIDTH - 1) / GLOBAL_WORKGROUP_WIDTH;

   //set kernel arg
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &(this->device_buffers_[0]));
    cl_pStatus(status, "clSetKernelArg_0");
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &(this->device_buffers_[1]));
    cl_pStatus(status, "clSetKernelArg_1");
    status = clSetKernelArg(kernel, 2, sizeof(cl_int), &height_);
    cl_pStatus(status, "clSetKernelArg_2");
    status = clSetKernelArg(kernel, 3, sizeof(cl_int), &width_);
    cl_pStatus(status, "clSetKernelArg_3");
    status = clSetKernelArg(kernel, 4, sizeof(cl_uint), &thread_division_row);
    cl_pStatus(status, "clSetKernelArg_4");
    status = clSetKernelArg(kernel, 5, sizeof(cl_uint), &thread_division_col);
    cl_pStatus(status, "clSetKernelArg_5");
    
   //start computing
    size_t global_worksize[2] = {GLOBAL_WORKGROUP_HEIGHT , GLOBAL_WORKGROUP_WIDTH};
    size_t local_worksize[2] = {LOCAL_WORKGROUP_HEIGHT, LOCAL_WORKGROUP_WIDTH};

    status = clEnqueueNDRangeKernel(this->device_->DeviceCmdQueA(), kernel , 2, NULL , global_worksize, local_worksize, 0, NULL, NULL );
    cl_pStatus(status, "clEnqueueNDRangeKernel");
    cout<<"task start on GPU"<<endl;
    clFinish(this->device_->DeviceCmdQueA());
}

template <typename Dtype>
void
SigmoidTask<Dtype>::PostCompute()
{
    //copy memory from device to host
    cl_int status; 
    status = clEnqueueReadBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[1], CL_TRUE, 0, height_*width_*sizeof(Dtype), (Dtype*)(this->results_[0].host_data()), 0, NULL, NULL );
    cl_pStatus(status, "clEnqueueReadBuffer");
    clFinish(this->device_->DeviceCmdQueA());
   // clReleaseMemObject(this->device_buffers_[0]);
   // clReleaseMemObject(this->device_buffers_[1]);
   // clReleaseMemObject(this->device_buffers_[2]);
}

template class SigmoidTask<int>;
template class SigmoidTask<float>;
template class SigmoidTask<double>;
template class SigmoidTask<long>;


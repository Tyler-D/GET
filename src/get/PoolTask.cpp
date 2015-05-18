/*************************************************************************
    > File Name: ReLUTask.cpp
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
PoolTask<Dtype>::PreCompute()
{
    //set device buffer
    cl_mem temp_buffer;
    cl_int status;

    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_READ_ONLY, data_h_*data_w_*sizeof(Dtype), NULL, &status);
    cl_pStatus(status, "clCreateBuffer 1");
    
    this->device_buffers_.push_back(temp_buffer);


    temp_buffer = clCreateBuffer(this->device_->DeviceContext(), CL_MEM_WRITE_ONLY, output_h_*output_w_*sizeof(Dtype), NULL, &status);
    cl_pStatus(status, "clCreateBuffer 2");
    
    this->device_buffers_.push_back(temp_buffer);
    
    //copy data from host to device
    status = clEnqueueWriteBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[0], CL_TRUE, 0, data_h_*data_w_*sizeof(Dtype), (Dtype*)(this->datas_[0].host_data()), 0, NULL, NULL );
    cl_pStatus(status, "clEnqueueWriteBuffer");

}

template <typename Dtype>
void
PoolTask<Dtype>::Compute()
{
   //read program source and create kernel
    cl_int status;
    
    char* program_source =NULL;

    if (typeid(Dtype) == typeid(int))
         program_source = cl_readSource("/home/zy/GET/src/get/pool_task_int.cl");
    if (typeid(Dtype) == typeid(long))
         program_source = cl_readSource("/home/zy/GET/src/get/pool_task_long.cl");
    if (typeid(Dtype) == typeid(float))
         program_source = cl_readSource("/home/zy/GET/src/get/pool_task_float.cl");
    if (typeid(Dtype) == typeid(double))
         program_source = cl_readSource("/home/zy/GET/src/get/pool_task_double.cl");

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
    kernel = clCreateKernel(program, "poolMatrix", &status);
    cl_pStatus(status, "clCreateKernel");
    
    
    int padding_pixels = (int)(kernel_w_ / 2) * 2;

    unsigned int global_itemh = RoundUp(data_h_ - padding_pixels , LOCAL_WORKGROUP_HEIGHT);
    unsigned int global_itemw = RoundUp(data_w_ - padding_pixels, LOCAL_WORKGROUP_WIDTH);
    size_t global_worksize[2] = {global_itemh , global_itemw};
    size_t local_worksize[2] = {LOCAL_WORKGROUP_HEIGHT, LOCAL_WORKGROUP_WIDTH};

    cl_uint local_height = LOCAL_WORKGROUP_HEIGHT + padding_pixels;
    cl_uint local_width = LOCAL_WORKGROUP_WIDTH + padding_pixels;

    size_t local_mem = local_height * local_width * sizeof(Dtype);
   //set kernel arg
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &(this->device_buffers_[0]));
    cl_pStatus(status, "clSetKernelArg_0");
    status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &(this->device_buffers_[1]));
    cl_pStatus(status, "clSetKernelArg_1");
    status = clSetKernelArg(kernel, 2, sizeof(cl_int), &output_h_);
    cl_pStatus(status, "clSetKernelArg_3");
    status = clSetKernelArg(kernel, 3, sizeof(cl_int), &output_w_);
    cl_pStatus(status, "clSetKernelArg_3");
    status = clSetKernelArg(kernel, 4, sizeof(cl_int), &data_h_);
    cl_pStatus(status, "clSetKernelArg_4");
    status = clSetKernelArg(kernel, 5, sizeof(cl_int), &data_w_);
    cl_pStatus(status, "clSetKernelArg_5");
    status = clSetKernelArg(kernel, 6, sizeof(cl_uint), &kernel_w_);
    cl_pStatus(status, "clSetKernelArg_6");
    status = clSetKernelArg(kernel, 7, local_mem, NULL);
    cl_pStatus(status, "clSetKernelArg_7");
    status = clSetKernelArg(kernel, 8, sizeof(cl_uint), &local_height);
    cl_pStatus(status, "clSetKernelArg_8");
    status = clSetKernelArg(kernel, 9, sizeof(cl_uint), &local_width);
    cl_pStatus(status, "clSetKernelArg_9");
   //start computing

    status = clEnqueueNDRangeKernel(this->device_->DeviceCmdQueA(), kernel , 2, NULL , global_worksize, local_worksize, 0, NULL, NULL );
    cl_pStatus(status, "clEnqueueNDRangeKernel");
    cout<<"task start on GPU"<<endl;
    clFinish(this->device_->DeviceCmdQueA());
}

template <typename Dtype>
void
PoolTask<Dtype>::PostCompute()
{
    //copy memory from device to host
    cl_int status; 
    status = clEnqueueReadBuffer(this->device_->DeviceCmdQueA(),this->device_buffers_[1], CL_TRUE, 0, output_h_*output_w_*sizeof(Dtype), (Dtype*)(this->results_[0].host_data()), 0, NULL, NULL );
    cl_pStatus(status, "clEnqueueReadBuffer");
    clFinish(this->device_->DeviceCmdQueA());
   // clReleaseMemObject(this->device_buffers_[0]);
   // clReleaseMemObject(this->device_buffers_[1]);
   // clReleaseMemObject(this->device_buffers_[2]);
}

template class PoolTask<int>;
template class PoolTask<float>;
template class PoolTask<double>;
template class PoolTask<long>;


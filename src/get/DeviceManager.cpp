/*************************************************************************
  > File Name: DeviceManager.cpp
  > Author: crows
  > Mail: 136211494@qq.com 
  > Created Time: Tue 24 Mar 2015 02:53:21 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"DeviceManager.h"

void
DeviceManager::Init()
{
	InitPlatforms();
	int num_platforms = platforms_index_.size();
	for(int i = 0; i<num_platforms ; i++)
	{
		InitDevices(platforms_index_[i]);
	}
}

void
DeviceManager::InitPlatforms()
{
	cl_uint numPlatforms;
	cl_platform_id *platforms = NULL;
	string name;

	clGetPlatformIDs(0, NULL, &numPlatforms);

	platforms = (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));

	clGetPlatformIDs(numPlatforms, platforms, NULL);

	for(int i = 0; i<numPlatforms; i++)
	{
		platforms_index_.push_back(platforms[i]);
		char plat_name[128]={"\0"};
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 128, plat_name, NULL);
		platforms_names_.insert(make_pair(i, name.assign(plat_name)));
	}
    cout << "Platform initial success !" <<endl; 

}

void
DeviceManager::InitDevices(cl_platform_id platform)
{

	cl_uint numDevices = 0;
	cl_device_id *devices = NULL;
	cl_int status;

// initial devices
	status = clGetDeviceIDs(platform,CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);

	devices = (cl_device_id*)malloc(numDevices*sizeof(cl_device_id));

	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);

//create context
	cl_context_properties cps[3] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platform, 0};  
	cl_context ctx = NULL;
	ctx = clCreateContext(cps, numDevices, devices, NULL, NULL, &status);

    for(int i = 0; i< numDevices ; i++)
	{
        //create command queue
		cl_command_queue cmdQueue1, cmdQueue2;
		cmdQueue1 = clCreateCommandQueue(ctx, devices[i], CL_QUEUE_PROFILING_ENABLE, &status); 
		cmdQueue2 = clCreateCommandQueue(ctx, devices[i], CL_QUEUE_PROFILING_ENABLE, &status);

        BaseDevice device(devices[i], i, ctx, cmdQueue1,cmdQueue2);
        device.Init();
        devices_index_.insert(make_pair(i, device));
        devices_status_.insert(make_pair(i, DEV_IDLE));
	}
    
    map<int , DeviceStatus>::iterator l_it = devices_status_.begin();
    while(l_it != devices_status_.end())
    {
        cout<<"Device "<<l_it->first<<" 's status is "<<l_it->second<<endl;
        ++l_it;
    }
    cout<<"Device initial success !" <<endl;
}

BaseDevice*
DeviceManager::GetAvailableDevice()
{
    map<int, DeviceStatus>::iterator l_it = devices_status_.begin();
    while(l_it != devices_status_.end())
    {
        if(l_it->second == DEV_IDLE)
        {
            l_it->second = DEV_BUSY;
            cout<< "Device "<<l_it->first<<" Get Busy"<<endl;
            return &(devices_index_[l_it->first]);
        }
        ++l_it;
    }
    return NULL;

}

void
DeviceManager::FreeDevice(int device_index)
{
    if (devices_status_[device_index] == DEV_BUSY)
    {
        devices_status_[device_index] = DEV_IDLE;
    }
}

/*************************************************************************
    > File Name: DeviceManager.h
    > Author: crows
    > Mail: 136211494@qq.com 
    > Created Time: Mon 23 Mar 2015 11:59:00 PM EDT
 ************************************************************************/
#ifndef GET_DEVICE_MANAGER_HEAD
#define GET_DEVICE_MANAGER_HEAD
#include<iostream>
using namespace std;
#include<map>
#include<string>
#include<CL/cl.h>
#include"BaseDevice.h"
#include<vector>
#include<map>

class DeviceManager
{
	public:
		~DeviceManager() {};
		DeviceManager()
			:devices_index_(), devices_status_(), device_info_(),platforms_index_(),platforms_names_() {};
	
    void Init();
	BaseDevice* GetAvailableDevice();
    void FreeDevice(int device_index); 
    string GetDeviceInfo(int device_id);	
    typedef enum {DEV_IDLE, DEV_BUSY} DeviceStatus;

    map<int, BaseDevice> devices_index_;
    map<int, DeviceStatus> devices_status_;

	string device_info_;
    vector<cl_platform_id> platforms_index_;
	map<int, string> platforms_names_;

    protected :
	void InitPlatforms();
	void InitDevices(cl_platform_id platform);

};
#endif

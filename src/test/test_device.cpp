/*************************************************************************
	> File Name: test_device.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu 02 Apr 2015 09:36:00 PM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"DeviceManager.h"
#include"BaseDevice.h"

int 
main()
{
    DeviceManager dm;

    dm.Init();

    cout << "Platform_id : " <<dm.platforms_index_[0]<<endl;
    cout << "platform_name : "<<dm.platforms_names_[0]<<endl;
    int numde = 0;
    cout << "num of devices : "<<(numde = (int)(dm.devices_index_.size()))<<endl;
    for(int i = 0; i<numde ; i++)
    {
        cout<< "device "<< i << " ID: "<<dm.devices_index_[i].DeviceID() << " status: " << dm.devices_status_[i] <<endl;     
        cout<< "global_memory : " <<dm.devices_index_[i].GlobalMemory() <<endl; 
        cout<< "block_size :" << dm.devices_index_[i].MaxBlockSize() <<endl; 
    }

    BaseDevice* device;
    
    device = dm.GetAvailableDevice();
    cout<<"Device "<<device->DeviceIndex()<<" is got";
    cout<<" status :" <<dm.devices_status_[device->DeviceIndex()] << endl;
    
    dm.FreeDevice(device->DeviceIndex());
    cout<<"Device "<<device->DeviceIndex()<<" is free";
    cout<<" status :" <<dm.devices_status_[device->DeviceIndex()] << endl;

}

/*************************************************************************
	> File Name: test_basetask.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon 13 Apr 2015 10:50:10 PM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"BaseTask.h"
#include"DeviceManager.h"
#include"DataBlob.h"
int 
main()
{
    DeviceManager dm;

    dm.Init();

    BaseDevice* device;

    device = dm.GetAvailableDevice();
    
    BaseTask<float> task(device);

    DataBlob<float> data(1,1,1024,1024);
    
    task.AddtoDatas(&data);
    task.AddtoResults(&data);
    task.TaskOn();
    cout<<"TaskFinished"<<endl;

}

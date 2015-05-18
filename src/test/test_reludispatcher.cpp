/*************************************************************************
	> File Name: test_adddispatcher.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed 15 Apr 2015 10:32:17 PM EDT
 ************************************************************************/

#include<iostream>
#include<stdlib.h>
#include<stdio.h>
using namespace std;
#include "TaskDispatcher.h" 
#include<string>
#include<stdint.h>
int main(int argc, char** argv)
{
    DeviceManager dm;

    dm.Init();

    cout << "Number of Devices = "<<dm.devices_status_.size() <<endl;
    
    

    BaseDevice* device;


    int h = atoi(argv[1]);
    int w = h;
    GET::TaskParam task_param;

    int* in = (int*)malloc(h*w*sizeof(int));
    int* out = (int*)malloc(h*w*sizeof(int));

    for(int i = 0; i < h*w ; i++)
    {
        in[i] = 1;
        out[i] = 0;
    }


    uint64_t a = reinterpret_cast<uint64_t>(in);
    uint64_t b = reinterpret_cast<uint64_t>(out);

 
    task_param.add_sourcem(a);
    task_param.add_resultm(b);


    cout<< "SOURCE num = "<<task_param.sourcem_size() <<endl;
    //cout<< "source 1 address : "<<(void*)atoi((task_param.source(0).c_str()))<<endl;
    //cout<< "source 2 address : "<<(void*)atoi((task_param.source(1).c_str()))<<endl;
    //cout<< "result 1 address : "<<(void*)atoi((task_param.result(0).c_str()))<<endl;

    task_param.set_type(GET::TaskParam_TaskType_RELU);
    GET::ReLUParam *pReLUParam = task_param.mutable_relu_param();

    pReLUParam->set_height(h);
    pReLUParam->set_width(w);

    ReLUTaskDispatcher<int> rlu(task_param, &dm);

    rlu.TaskOn();

    int count = 0;

    if (count == 0)
       cout << "Right Answer" <<endl;
    else 
       cout << "Wrong Answer : "<<count<<endl;

}

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

    int* A = (int*)malloc(h*w*sizeof(int));
    cout<< "A address: "<< A<<endl;
    int* B = (int*)malloc(h*w*sizeof(int));
    cout<< "B address: "<< B<<endl;
    int* C = (int*)malloc(h*w*sizeof(int));
    cout<< "C address: "<< C<<endl;

    for(int i = 0; i < h*w ; i++)
    {
        A[i] = 1;
        B[i] = 2;
        C[i] = 0;
    }


    uint64_t a = reinterpret_cast<uint64_t>(A);
    uint64_t b = reinterpret_cast<uint64_t>(B);
    uint64_t c = reinterpret_cast<uint64_t>(C);

 
    task_param.add_sourcem(a);
    task_param.add_sourcem(b);
    task_param.add_resultm(c);


    cout<< "SOURCE num = "<<task_param.sourcem_size() <<endl;
    //cout<< "source 1 address : "<<(void*)atoi((task_param.source(0).c_str()))<<endl;
    //cout<< "source 2 address : "<<(void*)atoi((task_param.source(1).c_str()))<<endl;
    //cout<< "result 1 address : "<<(void*)atoi((task_param.result(0).c_str()))<<endl;

    task_param.set_type(GET::TaskParam_TaskType_ADD);
    GET::AddParam *pAddParam = task_param.mutable_add_param();

    pAddParam->set_height(h);
    pAddParam->set_width(w);

    AddTaskDispatcher<int> atd(task_param, &dm);

    atd.TaskOn();

    int count = 0;
    for(int i = 0; i<h*w ; i++)
    {
        if (C[i] != A[i]+B[i]) 
        {
            count++;
        }
    }

    if (count == 0)
       cout << "Right Answer" <<endl;
    else 
       cout << "Wrong Answer : "<<count<<endl;

}

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
    

    int n = atoi(argv[1]);
    int m = n - 2;
    GET::TaskParam task_param;

    int* in = (int*)malloc(n*n*sizeof(int));
    cout<< "A address: "<< in<<endl;
    int f[9] = {
        1, 1, 1 ,
        1, 1, 1 ,
        1, 1, 1 ,
    };
    int* out = (int*)malloc(m*m*sizeof(int));
    //cout<< "C address: "<< C<<endl;

    for(int i = 0; i < n*n ; i++)
    {
        in[i] = 1;
    }
    
    for(int i = 0; i < m*m ; i++)
    {
        out[i] = 0;
    }


    uint64_t a = reinterpret_cast<uint64_t>(in);
    uint64_t b = reinterpret_cast<uint64_t>(f);
    uint64_t c = reinterpret_cast<uint64_t>(out);

 
    task_param.add_sourcem(a);
    task_param.add_sourcem(b);
    task_param.add_resultm(c);


    cout<< "SOURCE num = "<<task_param.sourcem_size() <<endl;
    //cout<< "source 1 address : "<<(void*)atoi((task_param.source(0).c_str()))<<endl;
    //cout<< "source 2 address : "<<(void*)atoi((task_param.source(1).c_str()))<<endl;
    //cout<< "result 1 address : "<<(void*)atoi((task_param.result(0).c_str()))<<endl;

    task_param.set_type(GET::TaskParam_TaskType_CONVOLUTION);
    GET::ConvParam *pConvParam = task_param.mutable_conv_param();

    pConvParam->set_data_h(n);
    pConvParam->set_data_w(n);
    pConvParam->set_filter_h(3);
    pConvParam->set_filter_w(3);
 

    ConvTaskDispatcher<int> ctd(task_param, &dm);

    ctd.TaskOn();
/*
    int count = 0;
    int answer = 0 ;
    for (int i = 0; i < K ; i++)
        answer += A[i]*B[i];
    for(int i = 0; i<M*N ; i++)
    {
        if (C[i] != answer) 
        {
            count++;
        }
    }

    if (count == 0)
       cout << "Right Answer" <<endl;
    else 
       cout << "Wrong Answer : "<<count<<endl;
*/
}

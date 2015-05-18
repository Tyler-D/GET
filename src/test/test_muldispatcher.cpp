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
    

    int M = atoi(argv[1]);
    int K = atoi(argv[2]);
    int N = atoi(argv[3]);
    GET::TaskParam task_param;

    int* A = (int*)malloc(M*K*sizeof(int));
    cout<< "A address: "<< A<<endl;
    int* B = (int*)malloc(K*N*sizeof(int));
    cout<< "B address: "<< B<<endl;
    int* C = (int*)malloc(M*N*sizeof(int));
    cout<< "C address: "<< C<<endl;

    for(int i = 0; i < M*K ; i++)
    {
        A[i] = 1;
    }

    for(int i = 0; i< K*N ; i++)
    {
        B[i] = 2; 
    }
    
    for(int i = 0; i < M*N ; i++)
    {
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

    task_param.set_type(GET::TaskParam_TaskType_MULTI);
    GET::MulParam *pMulParam = task_param.mutable_mul_param();

    pMulParam->set_m(M);
    pMulParam->set_k(K);
    pMulParam->set_n(N);

    MulTaskDispatcher<int> mtd(task_param, &dm);

    mtd.TaskOn();

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

}

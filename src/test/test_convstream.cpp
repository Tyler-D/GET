/*************************************************************************
	> File Name: test_addtask.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 14 Apr 2015 02:43:26 AM EDT
 ************************************************************************/

#include<iostream>
#include<stdlib.h>
using namespace std;
#include"BaseTask.h"
#include"StreamTask.h"
#include"DeviceManager.h"
#include<pthread.h>

int 
main(int argc ,char** argv)
{
    DeviceManager dm;

    dm.Init();

    BaseDevice* device;

    device = dm.GetAvailableDevice();

    ConvStreamTask<int> task(device);
    
    int n = atoi(argv[1]);
    int f = 3;
    int inh = n;
    int inw = n;
    int outh = inh - f + 1;
    int outw = inw - f + 1;
    
    int* in = (int*)malloc(inh*inw*sizeof(int));
    int filter[9] = {
        1 , 1 , 1 ,
        1 , 1 , 1 ,
        1 , 1 , 1
    };
    int* out = (int*)malloc(outh*outw*sizeof(int));
    
    

    for(int i = 0; i < inh*inw ; i++)
    {
        in[i] = 1 ;
    }
    for(int i = 0; i < outh*outw; i++)
    {
        out[i] = 0;
    }
    
    DataBlob<int> data[3];

    data[0].ReshapeLike(1 , 1, inh, inw);
    data[1].ReshapeLike(1 , 1, f , f);
    data[2].ReshapeLike(1 , 1, outh, outw);

    data[0].CopyFromMemory((void *)in);
    data[1].CopyFromMemory((void *)filter);
    data[2].CopyFromMemory((void *)out);

    task.SetParams(1, inh , inw, f, f, 1, 1, 0, 0);
    task.AddtoDatas(data[0]);
    task.AddtoDatas(data[1]);
    task.AddtoResults(data[2]);

    task.TaskOn();

    cout<<"task finished"<<endl;
/*
    for(int i =0 ; i < outh; i++)
    {
        for(int j = 0; j < outw ; j++)
           cout<< out[i * outw + j] << " ";
        cout << endl;
    }

*/
    cout<<"Right Answer"<<endl;
}

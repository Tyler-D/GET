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
#include"DeviceManager.h"
#include<pthread.h>

int 
main(int argc ,char** argv)
{
    DeviceManager dm;

    dm.Init();

    BaseDevice* device;

    device = dm.GetAvailableDevice();

    PoolTask<int> task(device);
    
    int n = atoi(argv[1]);

    int h = n;
    int w = n;
    int outh = h / 4;
    int outw = w / 4;

    int* IN = (int*)malloc(h*w*sizeof(int));
    int* OUT = (int*)malloc(h*w*sizeof(int));
    
    for(int i = 0; i < h*w ; i++)
    {
        if (i % 2 == 0)
            IN[i] = 1;
        else 
            IN[i] = 3 ;
    }
    
    for (int i = 0; i< outh*outw; i++)
    {
        OUT[i] = 0;
    }
    DataBlob<int> data[2];

    for(int i = 0; i < 2 ; i++)
        data[i].ReshapeLike(1 , 1, h, w);

    data[0].CopyFromMemory((void *)IN);
    data[1].CopyFromMemory((void *)OUT);

    task.SetParams(1, h, w, 4, 4 ,4, 4, 0, 0);
    task.AddtoDatas((data[0]));
    task.AddtoResults((data[1]));


    pthread_t tid;
    pthread_create(&tid, NULL, BaseTaskOn<int>,(void*)&task);
    pthread_join(tid,NULL);

    for(int i =0 ; i < outh; i++)
    {
        for(int j = 0; j < outw ; j++)
           cout<< OUT[i * outw + j] << " ";
        cout << endl;
    }

    cout<<"task finished"<<endl;

}

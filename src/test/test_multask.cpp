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

    MulTask<int> task(device);
    
    int n = atoi(argv[1]);

    int h = n;
    int w = n;

    int* A = (int*)malloc(h*w*sizeof(int));
    int* B = (int*)malloc(h*w*sizeof(int));
    int* C = (int*)malloc(h*w*sizeof(int));
    
    for(int i = 0; i < h*w ; i++)
    {
        A[i] = 2;
        B[i] = 1;
        C[i] = 0;
    }
    
    DataBlob<int> data[3];

    for(int i = 0; i < 3 ; i++)
        data[i].ReshapeLike(1 , 1, h, w);

    data[0].CopyFromMemory((void *)A);
    data[1].CopyFromMemory((void *)B);
    data[2].CopyFromMemory((void *)C);

    task.SetParams(1, n, n ,n);
    task.AddtoDatas(data[0]);
    task.AddtoDatas(data[1]);
    task.AddtoResults(data[2]);

    pthread_t tid;
    pthread_create(&tid, NULL, BaseTaskOn<int>,(void*)&task);
    pthread_join(tid,NULL);
    cout<<"task finished"<<endl;

   
    int answer = 0;
    for (int i = 0; i < n ; i++)
        answer += A[i]*B[i];
    for(int i = 0; i< h*w ; i++)
       if (C[i] != answer)
        { 
            cout << "Wrong Answer" <<endl;
            exit(0);
        }

    cout<<"Right Answer"<<endl;
}

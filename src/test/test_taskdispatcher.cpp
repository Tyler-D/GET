/*************************************************************************
	> File Name: test_taskdispatcher.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 14 Apr 2015 05:08:33 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include"TaskDispatcher.h"
#include"DeviceManager.h"

int 
main()
{
    DeviceManager dm;
    dm.Init();
    GET::TaskParam task_param;

    

    TaskDispatcher<int> td(task_param, &dm);

    td.TaskOn();
}


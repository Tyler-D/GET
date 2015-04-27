/*************************************************************************
	> File Name: test_stream.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 24 Apr 2015 11:05:21 AM EDT
 ************************************************************************/

#include<iostream>
using namespace std;
#include "StreamTask.h"
#include "DeviceManager.h"
#include "DataBlob.h"

int 
main() 
{
    DeviceManager dm;
    dm.Init();

    BaseDevice* device = dm.GetAvailableDevice();
    StreamTask<int> st(device);
    
    DataBlob<int> blobs(1,1,10,10);

    st.AddtoDatas(blobs);
    st.AddtoResults(blobs);

    st.TaskOn();

}

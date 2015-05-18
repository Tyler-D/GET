#pragma OPENCL EXTENSION cl_khr_fp64 : enable  
__kernel                        
void convMatrix(
	           __global long* imageIn, 
			   __global long* imageOut,  
               __constant long* filter, 
		       unsigned int  rowsout, 
			   unsigned int  colsout,
               unsigned int  rowsin,
               unsigned int  colsin,
			   unsigned int  filterWidth, 
			   __local long* localImage,
               unsigned int localHeight,
               unsigned int localWidth)
{	 
    int filterRadius = (filterWidth/2);
    int padding = filterRadius * 2;

    int groupStartCol = get_group_id(1)*get_local_size(1);
    int groupStartRow = get_group_id(0)*get_local_size(0);

    int localCol = get_local_id(1);    
    int localRow = get_local_id(0);

    int globalCol = groupStartCol + localCol;    
    int globalRow = groupStartRow + localRow;

    for(int i = localRow; i < localHeight; i += get_local_size(0))
    {       
        int curRow = groupStartRow+i;                        
        for(int j = localCol; j < localWidth; j += get_local_size(1)) 
        {   
            int curCol = groupStartCol+j;                                    
            if(curRow < rowsin && curCol < colsin) 
            {                
                localImage[i*localWidth + j] =  imageIn[curRow*colsin+curCol];            
            }        
        }    
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    if(globalRow < rowsout && globalCol < colsout)
    {
        float sum = 0.0f;
        int filterIdx = 0;
        for(int i = localRow; i < localRow+filterWidth; i++)
        {
            int offset = i*localWidth;
            for(int j = localCol; j < localCol+filterWidth; j++)
            {
               sum += localImage[offset+j] * filter[filterIdx++];         
            }
        }
    imageOut[(globalRow)*colsout +(globalCol)] = sum;
    }
    
}

#pragma OPENCL EXTENSION cl_khr_fp64 : enable  
__kernel                        
void sigMatrix(__global int* Input,
                __global int* Output,
		       int  height, 
			   int  width, 
			   unsigned int divT_r,
			   unsigned int divT_c
	           )  
{	 
	int col_t = get_global_id(1)*divT_c;
	int row_t = get_global_id(0)*divT_r;
	int iterations = divT_c*divT_r;
    int index = row_t*width+col_t;
	if ((row_t < height)&&(col_t < width))
	{
        for(int i = 0 ; i < iterations ; i++)
        {
            Output[index+i] = (int)(1 / (1 + exp((float)-Input[index+i]))) ;
        }   
    }   
}

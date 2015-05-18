#pragma OPENCL EXTENSION cl_khr_fp64 : enable  
__kernel                        
void subMatrix(__global int* OutputC, 
		       int  width, 
			   int height, 
			   unsigned int divT_r,
			   unsigned int divT_c,
	           __global int* InputA, 
			   __global int* InputB)  
{	 
	int col_t = get_global_id(1)*divT_c;
	int row_t = get_global_id(0)*divT_r;
	
	if ((row_t < height)&&(col_t < width))
	{
		for(int row = row_t; row < row_t+divT_r; row++)
		{
			for (int col = col_t; col< col_t+divT_c; col++)
			{
				if ((row < height)&&(col < width))
				{
					OutputC[row*width+col] = InputA[row*width+col]-InputB[row*width+col];
				}
			}
		}
	}

}

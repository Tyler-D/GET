#pragma OPENCL EXTENSION cl_khr_fp64 : enable  
__kernel                        
void addMatrix(__global long* OutputC, 
		       int  width, 
			   int height, 
			   unsigned int divT_r,
			   unsigned int divT_c,
	           __global long* InputA, 
			   __global long* InputB)  
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
					OutputC[row*width+col] = InputA[row*width+col]+InputB[row*width+col];
				}
			}
		}
	}

}

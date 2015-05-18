#pragma OPENCL EXTENSION cl_khr_fp64 : enable  
__kernel                        
void mulMatrix(
	           __global double* InputA, 
			   __global double* InputB,  
               __global double* OutputC, 
		       unsigned int  M, 
			   unsigned int  K, 
			   unsigned int  N, 
			   unsigned int divT_r,
			   unsigned int divT_c)
{	 
	int col_t = get_global_id(1)*divT_c;
	int row_t = get_global_id(0)*divT_r;
	double sum = 0.0f;
	
    if ((row_t < M)&&(col_t < N))
	{
		for(int row = row_t; row < row_t+divT_r; row++)
		{
			for (int col = col_t; col< col_t+divT_c; col++)
			{
			   if ((row < M)&&(col <N))
			   {
					for(int i = 0;i < K; i++)
					{				
						sum += InputA[row*K+i]*InputB[i*N+col];
					}
					OutputC[row*N+col] = sum;
					sum = 0.0f;
			   }
			}
		}
	}

}

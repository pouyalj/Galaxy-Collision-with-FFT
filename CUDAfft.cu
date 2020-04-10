#include <cufft.h>
#include <stdio.h>
#include <assert.h>
#include <cuda_runtime_api.h>
#include <math.h>

__global__
void fft3D(int xN, int yN, int zN, float Lx, float Ly, float Lz, float den_array[xN][yN][zN], float grav_po[xN][yN][zN])
{
	/*
	does the dang thing.
	*/

	double __complex__ p_sq;
	double __complex__ q_sq;
	double __complex__ r_sq;
	
	cufftHandle plan;
	cufftComplex *d_in, *d_out;
	
	size_t ds = xN*yN*zN*sizeof(cufftComplex);
	cudaMalloc((void **)&d_in,  ds);
	cudaMalloc((void **)&d_out, ds);
	
	cufftResult res = cufftPlan3d(&plan, xN, yN, zN, CUFFT_C2C);           //, 1);
	assert(res == CUFFT_SUCCESS);
	
	cudaMemcpy(d_in, den_array, ds, cudaMemcpyHostToDevice);
	res = cufftExecC2C(plan, d_in, d_out, CUFFT_FORWARD);
	assert(res == CUFFT_SUCCESS);
	
	cudaMemcpy(grav_po, d_out, ds, cudaMemcpyDeviceToHost);
	
	#pragma omp parallel for
	for (int i = 0; i < xN; i ++)
	{
		for (int j = 0; j < yN; j++)
		{
			for (int k = 0; k < zN; k++)
			{
				if (i-(xN/2) == 0 && j-(yN/2) == 0 && k-(zN/2) == 0)
				{
					grav_po[i][j][k] = grav_po[i][j][k]/0.00001;
				}
				
				else
				{
					p_sq = -1.0j*2*3.141592*(i-(xN/2))/Lx;
					q_sq = -1.0j*2*3.141592*(j-(yN/2))/Ly;
					r_sq = -1.0j*2*3.141592*(k-(zN/2))/Lz;
					grav_po[i][j][k] = grav_po[i][j][k]/(p_sq*p_sq + q_sq*q_sq + r_sq*r_sq);
				}
			}
		}
	}
	
	cudaMemcpy(d_in, grav_po, ds, cudaMemcpyHostToDevice);
	res = cufftExecC2C(plan, d_in, d_out, CUFFT_INVERSE);
	assert(res == CUFFT_SUCCESS);
	
	cudaMemcpy(grav_po, d_out, ds, cudaMemcpyDeviceToHost);
	
	cufftDestroy(plan);
}

int main(void)
{
	return 0;
}

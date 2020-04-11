#include <cufft.h>
#include <stdio.h>
#include <assert.h>
#include <cuda_runtime_api.h>
#include <math.h>

void fft3D(int xN, int yN, int zN, float Lx, float Ly, float Lz, float den_array[xN][yN][zN], float grav_po[xN][yN][zN])
{
	/*
	does the dang thing.
	*/
	int i;
	int j;
	int k;
	
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
	for (i = 0; i < xN; i ++)
	{
		for (j = 0; j < yN; j++)
		{
			for (k = 0; k < zN; k++)
			{
				if (i-(xN/2) == 0 && j-(yN/2) == 0 && k-(zN/2) == 0)
				{
					grav_po[i][j][k] = grav_po[i][j][k]/0.00001;
				}
				
				else
				{
					float p_sq = 2*3.141592*(i-(xN/2))/Lx;
					float q_sq = 2*3.141592*(j-(yN/2))/Ly;
					float r_sq = 2*3.141592*(k-(zN/2))/Lz;
					grav_po[i][j][k] = grav_po[i][j][k]/((pow(p_sq, 2) + pow(q_sq, 2) + pow(r_sq, 2))*-1);
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
	float den[64][64][8];
	float grav_po[64][64][8];
	int i, j, k;
	
	#pragma omp parallel for
	for (i = 0; i < 64; i ++)
	{
		for (j = 0; j < 64; j ++)
		{
			for (k = 0; k < 8, k++)
			{
				den[i][j][k] = 0.0;
				grav_po[i][j][k] = 0.0
			}
		}
	}
	
	den[32][32][4] = 500.0
	
	fft3D(64, 64, 8, 64000, 64000, 8000, den, grav_po);
	
	return 0;
}
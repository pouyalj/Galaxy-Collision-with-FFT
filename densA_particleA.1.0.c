// Generate and populate 3d array
// --------------------------------
// Useful link: https://www.techiedelight.com/dynamically-allocate-memory-for-3d-array/
// ---------------------------------------------------------------------------------------
// Compile with gcc densA_particleA.1.0.c -lm -o densA_particleA.1.0.out -mcmodel=large
// --------------------------------------------------------------------------------------------------------
// Can be used given a design decision to be made by the group
// --------------------------------------------------------------


#include <stdio.h>
#include <math.h>
#include<time.h>
#include<stdlib.h>

// M x N x O matrix
// #define M 64
// // #define N 64
#define M 100000000
#define N 8

#define I 64
#define J 64
#define K 8



// Dynamically Allocate Memory for 3D Array
void densArray(float **particleArray, float threedArray[I][J][K]) {
	int i, j, k = 0;
    // dynamically allocate memory of size M*N*O
    printf("density array intitiation complete\n");

    // assign values to allocated memory
	for (i; i < M; i++) {
        threedArray[(int)floorf(particleArray[i][0])][(int)floorf(particleArray[i][1])][(int)floorf(particleArray[i][2])] =
        threedArray[(int)floorf(particleArray[i][0])][(int)floorf(particleArray[i][1])][(int)floorf(particleArray[i][2])] + 1;
	}
    // // print the 3D array
	// for (i = 0; i < I; i++)
	// {
	// 	for (j = 0; j < J; j++)
	// 	{
	// 		for (k = 0; k < K; k++)
	// 			printf("%f\n", threedArray[i][j][k]);
	//    	}
	// }
    printf("Density Array completed\n");
}


// Dynamically Allocate Memory for 3D Array
void create_particle_Array(float **particleArray) {
	int i, j, k;
    // dynamically allocate memory of size M*N*O
    

    // assign values to allocated memory
	for (i = 0; i < M; i++) {
		particleArray[i] = (float *)malloc(N * sizeof(float*));

		if (particleArray[i] == NULL) {
			fprintf(stderr, "Out of memory");
			exit(0);
		}
	}

    printf("Starting to populate the particle array\n");
    // assign values to allocated memory
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			particleArray[i][j] = rand() % 8;
        }
    }


    // // print the 2D array
	// for (i = 0; i < M; i++)
	// {
	// 	for (j = 0; j < N; j++) {
    //         printf("%f\n", particleArray[i][j]);

	//    	}
	// 	printf("\n");
	// }

	// // deallocate memory
	// for (i = 0; i < M; i++) {
    //     free(particleArray[i]);
	// }
    // free(particleArray);
}



void main(){
    int i;
    float **particleArray = (float **)malloc(M * sizeof(float *));
    float threedArray[I][J][K];
    create_particle_Array(particleArray);
    printf("Particle Array completed\n");
    densArray(particleArray, threedArray);
    // deallocate memory
	for (i = 0; i < M; i++) {
        free(particleArray[i]);
	}
    free(particleArray);
}
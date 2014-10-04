// includes, system
#include <stdio.h>
#include <stdlib.h>

// includes, project
#include <cutil_inline.h>

#define MATRIX_SIZE 16
#define BLOCK_SIZE 4

__global__ void multiply_matrix(float* matrix_a, float* matrix_b, float* matrix_c) {
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;
	int n = y * MATRIX_SIZE + x;

	float result = 0.0f;
	for(int i = 0; i < MATRIX_SIZE / BLOCK_SIZE; i++) {
		__shared__ float shared_a[BLOCK_SIZE][BLOCK_SIZE];
		__shared__ float shared_b[BLOCK_SIZE][BLOCK_SIZE];

		int ax = i * BLOCK_SIZE + threadIdx.x;
		int ay = blockIdx.y * BLOCK_SIZE + threadIdx.y;
		int bx = blockIdx.x * BLOCK_SIZE + threadIdx.x;
		int by = i * BLOCK_SIZE + threadIdx.y;

		shared_a[threadIdx.y][threadIdx.x] = matrix_a[ay * MATRIX_SIZE + ax];
		shared_b[threadIdx.y][threadIdx.x] = matrix_b[by * MATRIX_SIZE + bx];

		__syncthreads();

		for(int k = 0; k < BLOCK_SIZE; k++) {
			result += shared_a[threadIdx.y][k] * shared_b[k][threadIdx.x];
		}

		__syncthreads();
	}
	matrix_c[n] = result;
}

int main() {
	size_t size = MATRIX_SIZE * MATRIX_SIZE;
	size_t byte_size = size * sizeof(float);

	float* host_matrix_a = (float*)malloc(byte_size);
	float* host_matrix_b = (float*)malloc(byte_size);
	float* host_matrix_c = (float*)malloc(byte_size);
	float* device_matrix_a;
	float* device_matrix_b;
	float* device_matrix_c;
	cudaMalloc(&device_matrix_a, byte_size);
	cudaMalloc(&device_matrix_b, byte_size);
	cudaMalloc(&device_matrix_c, byte_size);

	FILE* input = fopen("matrix_a.txt", "r+");
	for (int i = 0; i < size; i++) {
		fscanf(input, "%f", &host_matrix_a[i]);
	}
	fclose(input);

	input = fopen("matrix_b.txt", "r+");
	for (int i = 0; i < size; i++) {
		fscanf(input, "%f", &host_matrix_b[i]);
	}
	fclose(input);

	cudaMemcpy(device_matrix_a, host_matrix_a, byte_size, cudaMemcpyHostToDevice);
	cudaMemcpy(device_matrix_b, host_matrix_b, byte_size, cudaMemcpyHostToDevice);

	dim3 block_size(BLOCK_SIZE, BLOCK_SIZE);
	dim3 grid_size(MATRIX_SIZE / BLOCK_SIZE, MATRIX_SIZE / BLOCK_SIZE);
	multiply_matrix<<<grid_size, block_size>>>(device_matrix_a, device_matrix_b, device_matrix_c);

	cudaMemcpy(host_matrix_c, device_matrix_c, byte_size, cudaMemcpyDeviceToHost);

	FILE* output = fopen("matrix_c.txt", "w+");
	for (int i = 0; i < size; i++) {
		fprintf(output, "%.1f ", host_matrix_c[i]);
		if((i + 1) % MATRIX_SIZE == 0) fprintf(output, "\n");
	}
	fclose(output);

	free(host_matrix_a);
	free(host_matrix_b);
	free(host_matrix_c);
	cudaFree(device_matrix_a);
	cudaFree(device_matrix_b);
	cudaFree(device_matrix_c);
}
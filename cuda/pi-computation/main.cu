#include <stdio.h>
#include <stdlib.h>

#include <cuda.h>

__global__ void compute_pi(float* result) {
	int i = blockDim.x * blockIdx.x + threadIdx.x;
	int n = gridDim.x * blockDim.x;

	float dx = 1.0f / (float)n;
	float x = (float)i / (float)n + dx / 2;
	float y = sqrt(1 - x * x);

	if (i < n) {
		result[i] = dx * y;
	}
}

int main() {
	int grid_size = 256;
	int block_size = 256;
	size_t size = grid_size * block_size * sizeof(float);

	float* host_memory = (float*)malloc(size);
	float* device_memory;
	cudaMalloc(&device_memory, size);

	cudaThreadSynchronize();
	cudaError_t status = cudaGetLastError();
	if(status != cudaSuccess) {
		printf("%s\n", cudaGetErrorString(status));
		return 0;
	}

	compute_pi<<<grid_size, block_size>>>(device_memory);

	cudaMemcpy(host_memory, device_memory, size, cudaMemcpyDeviceToHost);

	float result = 0.0f;
	for (int i = 0; i < grid_size * block_size; i++) {
		result += host_memory[i];
	}

	printf("%f\n", result * 4);

	free(host_memory);
	cudaFree(device_memory);
}
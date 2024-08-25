#include "ex1.h"

void v_add_naive(double* x, double* y, double* z) {
    #pragma omp parallel for
        for(int i=0; i<ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
    // TODO: Implement this function
    // Do NOT use the `for` directive here!
    #pragma omp parallel
	{
        int num_threads = omp_get_num_threads();
        int id = omp_get_thread_num();
		for (int i = id; i < ARRAY_SIZE; i += num_threads)
			z[i] = x[i] + y[i];
	}
}

// Chunks Method
void v_add_optimized_chunks(double* x, double* y, double* z) {
    // TODO: Implement this function
    // Do NOT use the `for` directive here!
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int id = omp_get_thread_num();
        int l = ARRAY_SIZE / num_threads * id, r = ARRAY_SIZE / num_threads * (id + 1);
        if (id == num_threads - 1) r = ARRAY_SIZE;
        for (int i = l; i < r; i ++) {
            z[i] = x[i] + y[i];
        }
    }
}

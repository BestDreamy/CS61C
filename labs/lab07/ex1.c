#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "ex1.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    int *a = (int*)malloc(sizeof(int) * 4);
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        unsigned int i = 0;
        for(; i < NUM_ELEMS / 4 * 4; i += 4) {
            __m128i data = _mm_loadu_si128((__m128i*)(vals + i));
            __m128i mask = _mm_cmpgt_epi32(data, _127);

            data = _mm_and_si128(data, mask);

            _mm_storeu_si128((__m128i*)a, data);
            for(int j = 0; j < 4; j ++) {
                result += a[j];
            }
        }

        for(; i < NUM_ELEMS; i ++) {
            if(vals[i] > 127) result += vals[i];
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    int *a = (int*)malloc(sizeof(int) * 4);
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        unsigned int i = 0;
        for(; i < NUM_ELEMS / 8 * 8; i += 8) {
            __m128i data0 = _mm_loadu_si128((__m128i*)(vals + i));
            __m128i data1 = _mm_loadu_si128((__m128i*)(vals + i + 4));
            // __m128i data2 = _mm_loadu_si128((__m128i*)(vals + i + 8));
            // __m128i data3 = _mm_loadu_si128((__m128i*)(vals + i + 12));

            __m128i mask0 = _mm_cmpgt_epi32(data0, _127);
            __m128i mask1 = _mm_cmpgt_epi32(data1, _127);
            // __m128i mask2 = _mm_cmpgt_epi32(data2, _127);
            // __m128i mask3 = _mm_cmpgt_epi32(data3, _127);

            data0 = _mm_and_si128(data0, mask0);
            data1 = _mm_and_si128(data1, mask1);
            // data2 = _mm_and_si128(data2, mask2);
            // data3 = _mm_and_si128(data3, mask3);

            __m128i sum = _mm_add_epi32(data0, data1);

            _mm_storeu_si128((__m128i*)a, sum);
            // _mm_storeu_si128((__m128i*)(a + 4), data1);
            // _mm_storeu_si128((__m128i*)(a + 8), data2);
            // _mm_storeu_si128((__m128i*)(a + 12), data3);

            for(int j = 0; j < 4; j ++) {
                result += a[j];
            }
        }

        for(; i < NUM_ELEMS; i ++) {
            if(vals[i] > 127) result += vals[i];
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

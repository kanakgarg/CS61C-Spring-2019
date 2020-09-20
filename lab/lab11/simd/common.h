#ifndef COMMON_H
#define COMMON_H

#include <x86intrin.h>

#define NUM_ELEMS ((1 << 16) + 10)
#define OUTER_ITERATIONS (1 << 16)

long long int sum(unsigned int vals[NUM_ELEMS]) {
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

long long int sum_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
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

long long int sum_simd(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				// This is where you should put your final result!
											// DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE.
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* YOUR CODE GOES HERE */
		int result_arr[4] = {0, 0, 0, 0};
		__m128i sum_v = _mm_setzero_si128();

		for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i+= 4){
			__m128i curr_vals = _mm_loadu_si128((__m128i *) &vals[i]);
			__m128i bigger = _mm_cmpgt_epi32(curr_vals, _127);
			__m128i masked = _mm_and_si128(bigger, curr_vals);

			sum_v = _mm_add_epi32(sum_v, masked);
		}
		_mm_storeu_si128((__m128i *) &result_arr, sum_v);

		/* You'll need a tail case. */
		for (unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++){
			if (vals[i] >= 128){
				result_arr[0] += vals[i];
			}
		}
		int tmp_sum = (result_arr[0] + result_arr[1] + result_arr[2] + result_arr[3]);
		result += tmp_sum;
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(unsigned int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* COPY AND PASTE YOUR sum_simd() HERE */
		/* MODIFY IT BY UNROLLING IT */
		int result_arr[4] = {0, 0, 0, 0};
		__m128i sum_v = _mm_setzero_si128();

		for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i+= 16){
			__m128i curr_vals_1 = _mm_loadu_si128((__m128i *) &vals[i]);
			__m128i bigger_1 = _mm_cmpgt_epi32(curr_vals_1, _127);
			__m128i masked_1 = _mm_and_si128(bigger_1, curr_vals_1);

			sum_v = _mm_add_epi32(sum_v, masked_1);

			__m128i curr_vals_2 = _mm_loadu_si128((__m128i *) &vals[i + 4]);
			__m128i bigger_2 = _mm_cmpgt_epi32(curr_vals_2, _127);
			__m128i masked_2 = _mm_and_si128(bigger_2, curr_vals_2);

			sum_v = _mm_add_epi32(sum_v, masked_2);

			__m128i curr_vals_3 = _mm_loadu_si128((__m128i *) &vals[i + 8]);
			__m128i bigger_3 = _mm_cmpgt_epi32(curr_vals_3, _127);
			__m128i masked_3 = _mm_and_si128(bigger_3, curr_vals_3);

			sum_v = _mm_add_epi32(sum_v, masked_3);

			__m128i curr_vals_4 = _mm_loadu_si128((__m128i *) &vals[i + 12]);
			__m128i bigger_4 = _mm_cmpgt_epi32(curr_vals_4, _127);
			__m128i masked_4 = _mm_and_si128(bigger_4, curr_vals_4);

			sum_v = _mm_add_epi32(sum_v, masked_4);
		}
		_mm_storeu_si128((__m128i *) &result_arr, sum_v);

		/* You'll need a tail case. */
		for (unsigned int i = NUM_ELEMS / 16 * 16; i < NUM_ELEMS; i++){
			if (vals[i] >= 128){
				result_arr[0] += vals[i];
			}
		}
		int tmp_sum = (result_arr[0] + result_arr[1] + result_arr[2] + result_arr[3]);
		result += tmp_sum;

		/* You'll need 1 or maybe 2 tail cases here. */

	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

/* DON'T TOUCH THIS FUNCTION */
int int_comparator(const void* a, const void* b) {
	if(*(unsigned int*)a == *(unsigned int*)b) return 0;
	else if(*(unsigned int*)a < *(unsigned int*)b) return -1;
	else return 1;
}

#endif

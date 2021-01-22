#ifndef MEASURE_H
#define MEASURE_H

#include <chrono>

// Returns the average runtime in milliseconds of 'func', executed 'runs' times
long long measure(void (*func)(int), int num_elements, int runs) {
	long long milliseconds = 0;

	for (int run = 0; run < runs; ++run) {
		auto start = std::chrono::high_resolution_clock::now();

		(*func)(num_elements);

		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

		milliseconds += microseconds / 1000;
	}

	return milliseconds / runs;
}

#endif
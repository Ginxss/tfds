#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "../../tfds/tf_vector.hpp"

void print_vector_performance(int num_elements, int runs) {
	long long std_insert_ms = 0;
	long long tf_insert_ms = 0;

	long long std_insert_res_ms = 0;
	long long tf_insert_res_ms = 0;

	long long std_get_mu = 0;
	long long tf_get_mu = 0;

	for (int run = 0; run < runs; ++run) {
		std::vector<std::string> std_vector;
		tf::vector<std::string> tf_vector;

		// INSERT

		// std
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			std_vector.emplace_back(std::to_string(i));
		}

		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		std_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_vector.add(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// INSERT (reserved)

		std::vector<std::string> std_vector_2;
		std_vector_2.reserve(num_elements);
		tf::vector<std::string> tf_vector_2(num_elements);

		// std
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			std_vector_2.emplace_back(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		std_insert_res_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_vector_2.add(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_insert_res_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// GET

		// std
		start = std::chrono::high_resolution_clock::now();

		for (size_t i = 0; i < std_vector.size(); ++i) {
			std_vector.at(i);
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		std_get_mu += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (size_t i = 0; i < std_vector.size(); ++i) {
			tf_vector.get(i);
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_get_mu += std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	}

	std_insert_ms /= runs;
	tf_insert_ms /= runs;

	std_insert_res_ms /= runs;
	tf_insert_res_ms /= runs;

	std_get_mu /= runs;
	tf_get_mu /= runs;

	std::cout << "| VECTOR |" << std::endl << std::endl;

	std::cout << "Inserting " << num_elements << " std::strings:" << std::endl;
	std::cout << "std::vector: " << std_insert_ms << " ms" << std::endl;
	std::cout << "tf::vector: " << tf_insert_ms << " ms" << std::endl << std::endl;

	std::cout << "Inserting " << num_elements << " std::strings (with reserved space):" << std::endl;
	std::cout << "std::vector: " << std_insert_res_ms << " ms" << std::endl;
	std::cout << "tf::vector: " << tf_insert_res_ms << " ms" << std::endl << std::endl;

	std::cout << "Accessing " << num_elements << " std::strings:" << std::endl;
	std::cout << "std::vector: " << std_get_mu << " micros" << std::endl;
	std::cout << "tf::vector: " << tf_get_mu << " micros" << std::endl << std::endl;
}

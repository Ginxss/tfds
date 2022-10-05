#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
#include "../../tfds/tf_hash_table.hpp"

void print_table_performance(int num_elements, int runs) {
	long long std_insert_ms = 0;
	long long tf_insert_ms = 0;

	long long std_get_ms = 0;
	long long tf_get_ms = 0;

	for (int run = 0; run < runs; ++run) {
		std::unordered_map<std::string, int> std_map;
		std_map.reserve(num_elements);
		tf::hash_table<std::string, int> tf_table(num_elements);

		// INSERT

		// std
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			std_map[std::to_string(i)] = i;
		}

		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		std_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_table.insert(std::to_string(i), i);
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// GET

		//std
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < std_map.size(); ++i) {
			std_map.at(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		std_get_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// INSERT

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_table.get(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_get_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	}

	std_insert_ms /= runs;
	tf_insert_ms /= runs;

	std_get_ms /= runs;
	tf_get_ms /= runs;

	std::cout << "| HASH TABLE |" << std::endl << std::endl;

	std::cout << "Inserting " << num_elements << " (std::string, int) pairs:" << std::endl;
	std::cout << "std::unordered_map: " << std_insert_ms << " milliseconds" << std::endl;
	std::cout << "tf::hash_table: " << tf_insert_ms << " milliseconds" << std::endl << std::endl;

	std::cout << "Accessing " << num_elements << " (std::string, int) pairs:" << std::endl;
	std::cout << "std::unordered_map: " << std_get_ms << " milliseconds" << std::endl;
	std::cout << "tf::hash_table: " << tf_get_ms << " milliseconds" << std::endl << std::endl;
}

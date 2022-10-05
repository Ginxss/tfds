#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include "../../tfds/tf_search_tree.hpp"

void print_tree_performance(int num_elements, int runs) {
	long long std_insert_ms = 0;
	long long tf_insert_ms = 0;

	long long std_get_ms = 0;
	long long tf_get_ms = 0;

	for (int run = 0; run < runs; ++run) {
		std::map<int, std::string> std_map;
		tf::search_tree<int, std::string> tf_tree;

		// INSERT

		// std
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			std_map[i] = std::to_string(i);
		}

		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		std_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_tree.insert(i, std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// GET

		// std
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			std_map.at(i);
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		std_get_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_tree.get(i);
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_get_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	}

	std_insert_ms /= runs;
	tf_insert_ms /= runs;

	std_get_ms /= runs;
	tf_get_ms /= runs;
	
	std::cout << "| SEARCH TREE |" << std::endl << std::endl;

	std::cout << "Inserting " << num_elements << " (int, std::string) pairs:" << std::endl;
	std::cout << "std::map: " << std_insert_ms << " milliseconds" << std::endl;
	std::cout << "tf::search_tree: " << tf_insert_ms << " milliseconds" << std::endl << std::endl;

	std::cout << "Accessing " << num_elements << " (int, std::string) pairs:" << std::endl;
	std::cout << "std::map: " << std_get_ms << " milliseconds" << std::endl;
	std::cout << "tf::search_tree: " << tf_get_ms << " milliseconds" << std::endl << std::endl;	
}

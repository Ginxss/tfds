#include <iostream>
#include <string>
#include <list>
#include <chrono>
#include "../../tfds/tf_linked_list.hpp"

void print_list_performance(int num_elements, int runs) {
	long long std_insert_ms = 0;
	long long tf_insert_ms = 0;

	long long std_iterate_ms = 0;
	long long tf_iterate_ms = 0;

	long long std_remove_ms = 0;
	long long tf_remove_ms = 0;

	for (int run = 0; run < runs; ++run) {
		std::list<std::string> std_list;
		tf::linked_list<std::string> tf_list;

		// INSERT

		// std
		auto start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			std_list.emplace_back(std::to_string(i));
		}

		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		std_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_list.add_back(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_insert_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// ITERATE

		// std
		start = std::chrono::high_resolution_clock::now();

		for (auto it = std_list.begin(); it != std_list.end(); ++it) {
			*it;
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		std_iterate_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (auto it = tf_list.begin(); it.has_value(); ++it) {
			*it;
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_iterate_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// REMOVE

		// std
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			std_list.remove(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		std_remove_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

		// tf
		start = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < num_elements; ++i) {
			tf_list.remove(std::to_string(i));
		}

		elapsed = std::chrono::high_resolution_clock::now() - start;
		tf_remove_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
	}

	std_insert_ms /= runs;
	tf_insert_ms /= runs;

	std_iterate_ms /= runs;
	tf_iterate_ms /= runs;

	std_remove_ms /= runs;
	tf_remove_ms /= runs;

	std::cout << "| LINKED LIST |" << std::endl << std::endl;
	
	std::cout << "Inserting " << num_elements << " std::strings:" << std::endl;
	std::cout << "std::list: " << std_insert_ms << " ms" << std::endl;
	std::cout << "tf::linked_list: " << tf_insert_ms << " ms" << std::endl << std::endl;

	std::cout << "Iterating over " << num_elements << " std::strings:" << std::endl;
	std::cout << "std::list: " << std_iterate_ms << " ms" << std::endl;
	std::cout << "tf::linked_list: " << tf_iterate_ms << " ms" << std::endl << std::endl;

	std::cout << "Removing " << num_elements << " std::strings from front:" << std::endl;
	std::cout << "std::list: " << std_remove_ms << " ms" << std::endl;
	std::cout << "tf::linked_list: " << tf_remove_ms << " ms" << std::endl << std::endl;
}

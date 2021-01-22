#include <iostream>
#include <string>
#include <unordered_map>
#include "../../tfds/tf_hash_table.hpp"
#include "measure.hpp"

void std_unordered_map_insert(int num_elements) {
	std::unordered_map<std::string, int> map;
	map.reserve(num_elements);
	for (int i = 0; i < num_elements; ++i) {
		map[std::to_string(i)] = i;
	}
}

void tf_table_insert(int num_elements) {
	tf::hash_table<std::string, int> table(num_elements);
	for (int i = 0; i < num_elements; ++i) {
		table.insert(std::to_string(i), i);
	}
}

void std_unordered_map_insert_get(int num_elements) {
	std::unordered_map<std::string, int> map;
	map.reserve(num_elements);
	for (int i = 0; i < num_elements; ++i) {
		map[std::to_string(i)] = i;
	}

	for (int i = 0; i < num_elements; ++i) {
		int value = map.at(std::to_string(i));
	}
}

void tf_table_insert_get(int num_elements) {
	tf::hash_table<std::string, int> table(num_elements);
	for (int i = 0; i < num_elements; ++i) {
		table.insert(std::to_string(i), i);
	}

	for (int i = 0; i < num_elements; ++i) {
		int value = table.get(std::to_string(i));
	}
}

void print_table_performance(int num_elements, int runs) {
	std::cout << "std::unordered_map inserted " << num_elements << " (std::string, int) pairs : " <<
		measure(std_unordered_map_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::hash_table inserted " << num_elements << " (std::string, int) pairs : " <<
		measure(tf_table_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;

	std::cout << "std::unordered_map inserted and accessed " << num_elements << " (std::string, int) pairs : " <<
		measure(std_unordered_map_insert_get, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::hash_table inserted and accessed " << num_elements << " (std::string, int) pairs : " <<
		measure(tf_table_insert_get, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;
}

/* int main(int argc, char *argv[]) {
	int num_elements = 100000;
	int runs = 10;

	print_table_performance(num_elements, runs);

	return 0;
} */
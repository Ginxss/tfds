#include <iostream>
#include <string>
#include <map>
#include "../../tfds/tf_search_tree.hpp"
#include "measure.hpp"

void std_map_insert(int num_elements) {
	std::map<int, std::string> map;
	for (int i = 0; i < num_elements; ++i) {
		map[i] = std::to_string(i);
	}
}

void tf_tree_insert(int num_elements) {
	tf::search_tree<int, std::string> tree;
	for (int i = 0; i < num_elements; ++i) {
		tree.insert(i, std::to_string(i));
	}
}

void std_map_insert_get(int num_elements) {
	std::map<int, std::string> map;
	for (int i = 0; i < num_elements; ++i) {
		map[i] = std::to_string(i);
	}

	for (int i = 0; i < num_elements; ++i) {
		std::string value = map.at(i);
	}
}

void tf_tree_insert_get(int num_elements) {
	tf::search_tree<int, std::string> tree;
	for (int i = 0; i < num_elements; ++i) {
		tree.insert(i, std::to_string(i));
	}

	for (int i = 0; i < num_elements; ++i) {
		std::string value = tree.get(i);
	}
}

void print_tree_performance(int num_elements, int runs) {
	std::cout << "std::map inserted " << num_elements << " (int, std::string) pairs : " <<
		measure(std_map_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::search_tree inserted " << num_elements << " (int, std::string) pairs : " <<
		measure(tf_tree_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;

	std::cout << "std::map inserted and accessed " << num_elements << " (int, std::string) pairs : " <<
		measure(std_map_insert_get, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::search_tree inserted and accessed " << num_elements << " (int, std::string) pairs : " <<
		measure(tf_tree_insert_get, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;
}

/* int main(int argc, char *argv[]) {
	int num_elements = 100000;
	int runs = 10;

	print_tree_performance(num_elements, runs);

	return 0;
} */
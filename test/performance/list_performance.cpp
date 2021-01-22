#include <iostream>
#include <string>
#include <list>
#include "../../tfds/tf_linked_list.hpp"
#include "measure.hpp"

void std_list_insert(int num_elements) {
	std::list<std::string> list;
	for (int i = 0; i < num_elements; ++i) {
		list.emplace_back(std::to_string(i));
	}
}

void tf_list_insert(int num_elements) {
	tf::linked_list<std::string> list;
	for (int i = 0; i < num_elements; ++i) {
		list.add_back(std::to_string(i));
	}
}

void std_list_insert_iterate(int num_elements) {
	std::list<std::string> list;
	for (int i = 0; i < num_elements; ++i) {
		list.emplace_back(std::to_string(i));
	}

	for (auto it = list.begin(); it != list.end(); ++it) {
		std::string value = *it;
	}
}

void tf_list_insert_iterate(int num_elements) {
	tf::linked_list<std::string> list;
	for (int i = 0; i < num_elements; ++i) {
		list.add_back(std::to_string(i));
	}

	for (auto it = list.begin(); it.condition(); ++it) {
		std::string value = *it;
	}
}

void std_list_insert_pop(int num_elements) {
	std::list<std::string> list;
	for (int i = 0; i < num_elements; ++i) {
		list.emplace_back(std::to_string(i));
	}

	while (!list.empty()) {
		std::string value = list.back();
		list.pop_back();
	}
}

void tf_list_insert_pop(int num_elements) {
	tf::linked_list<std::string> list;
	for (int i = 0; i < num_elements; ++i) {
		list.add_back(std::to_string(i));
	}

	while (!list.empty()) {
		std::string value = list.pop_back();
	}
}

void print_list_performance(int num_elements, int runs) {
	std::cout << "std::list inserted " << num_elements << " std::strings : " <<
		measure(std_list_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::linked_list inserted " << num_elements << " std::strings : " <<
		measure(tf_list_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;

	std::cout << "std::list inserted and iterated over " << num_elements << " std::strings : " <<
		measure(std_list_insert_iterate, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::linked_list inserted and iterated over " << num_elements << " std::strings : " <<
		measure(tf_list_insert_iterate, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;

	std::cout << "std::list inserted and popped " << num_elements << " std::strings : " <<
		measure(std_list_insert_pop, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::linked_list inserted and popped " << num_elements << " std::strings : " <<
		measure(tf_list_insert_pop, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;
}

/* int main(int argc, char *argv[]) {
	int num_elements = 100000;
	int runs = 10;

	print_list_performance(num_elements, runs);

	return 0;
} */
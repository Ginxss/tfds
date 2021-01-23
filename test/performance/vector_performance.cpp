#include <iostream>
#include <string>
#include <vector>
#include "../../tfds/tf_vector.hpp"
#include "measure.hpp"

void std_vector_insert(int num_elements) {
	std::vector<std::string> vector;
	for (int i = 0; i < num_elements; ++i) {
		vector.emplace_back(std::to_string(i));
	}
}

void tf_vector_insert(int num_elements) {
	tf::vector<std::string> vector;
	for (int i = 0; i < num_elements; ++i) {
		vector.add(std::to_string(i));
	}
}

void std_vector_insert_reserved(int num_elements) {
	std::vector<std::string> vector;
	vector.reserve(num_elements);
	for (int i = 0; i < num_elements; ++i) {
		vector.emplace_back(std::to_string(i));
	}
}

void tf_vector_insert_reserved(int num_elements) {
	tf::vector<std::string> vector(num_elements);
	for (int i = 0; i < num_elements; ++i) {
		vector.add(std::to_string(i));
	}
}

void std_vector_insert_get(int num_elements) {
	std::vector<std::string> vector;
	for (int i = 0; i < num_elements; ++i) {
		vector.emplace_back(std::to_string(i));
	}

	for (int i = 0; i < vector.size(); ++i) {
		std::string value = vector.at(i);
	}
}

void tf_vector_insert_get(int num_elements) {
	tf::vector<std::string> vector;
	for (int i = 0; i < num_elements; ++i) {
		vector.add(std::to_string(i));
	}

	for (int i = 0; i < vector.size(); ++i) {
		std::string value = vector.get(i);
	}
}

void print_vector_performance(int num_elements, int runs) {
	std::cout << "| VECTOR |" << std::endl << std::endl;
	
	std::cout << "std::vector inserted " << num_elements << " std::strings : " <<
		measure(std_vector_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::vector inserted " << num_elements << " std::strings : " <<
		measure(tf_vector_insert, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;

	std::cout << "std::vector inserted " << num_elements << " std::strings (with reserved space) : " <<
		measure(std_vector_insert_reserved, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::vector inserted " << num_elements << " std::strings (with reserved space) : " <<
		measure(tf_vector_insert_reserved, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;

	std::cout << "std::vector inserted and accessed " << num_elements << " std::strings : " <<
		measure(std_vector_insert_get, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << "tf::vector inserted and accessed " << num_elements << " std::strings : " <<
		measure(tf_vector_insert_get, num_elements, runs) << " milliseconds" << std::endl;
	std::cout << std::endl;
}

/* int main(int argc, char *argv[]) {
	int num_elements = 100000;
	int runs = 10;

	print_vector_performance(num_elements, runs);

	return 0;
} */
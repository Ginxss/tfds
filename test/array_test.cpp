#include <cassert>
#include <iostream>
#include "../tfds/tf_array.hpp"

// Helper function for manual testing
void print_array_info(const tf::array<std::string> &a, const std::string &message = "Array info") {
	std::cout << message << ":" << std::endl << std::endl;
	std::cout << "Capacity: " << a.capacity() << std::endl;
	std::cout << "Auto-Reallocating: " << a.auto_reallocating() << std::endl;

	std::cout << "Content: ";
	for (int i = 0; i < a.capacity(); ++i) {
		std::cout << a.get(i) << " | ";
	}
	
	std::cout << std::endl << std::endl << "----------------------------------------" << std::endl << std::endl;
}

tf::array<std::string> test_construction() {
	tf::array<std::string> a1;
	assert(a1.capacity() == 10);
	assert(a1.auto_reallocating() == true);

	tf::array<std::string> a2(5, false);
	assert(a2.capacity() == 5);
	assert(a2.auto_reallocating() == false);

	tf::array<std::string> a3(4);
	assert(a3.capacity() == 4);
	assert(a3.auto_reallocating() == true);

	return a3;
}

void test_insert_and_get(tf::array<std::string> &a) {
	assert(a.capacity() == 4);
	assert(a.auto_reallocating() == true);

	a.insert(0, "zero");
	assert(a.get(0) == "zero");

	a.insert(1, "one");
	assert(a[1] == "one");

	a[2] = "two";
	assert(a.get(2) == "two");

	a[3] = "three";
	assert(a[3] == "three");
}

void test_reallocation(tf::array<std::string> &a) {
	assert(a.capacity() == 4);
	assert(a.auto_reallocating() == true);
	assert(a.get(0) == "zero");
	assert(a.get(1) == "one");
	assert(a.get(2) == "two");
	assert(a.get(3) == "three");

	a.insert(4, "four");
	assert(a.capacity() == 8);
	assert(a.get(4) == "four");

	a[20] = "twenty";
	assert(a.capacity() == 24);
	assert(a.get(20) == "twenty");
}

void test_copying(const tf::array<std::string> &a) {
	assert(a.capacity() == 24);
	assert(a.auto_reallocating() == true);
	assert(a.get(0) == "zero");
	assert(a.get(1) == "one");
	assert(a.get(2) == "two");
	assert(a.get(3) == "three");
	assert(a.get(4) == "four");
	assert(a.get(20) == "twenty");

	const tf::array<std::string> a2 = a;
	assert(a2.capacity() == 24);
	assert(a2.auto_reallocating() == true);
	assert(a2.get(0) == "zero");
	assert(a2.get(1) == "one");
	assert(a2.get(2) == "two");
	assert(a2.get(3) == "three");
	assert(a2.get(4) == "four");
	assert(a2.get(20) == "twenty");

	tf::array<std::string> a3;
	a3 = a;
	assert(a3.capacity() == 24);
	assert(a3.auto_reallocating() == true);
	assert(a3.get(0) == "zero");
	assert(a3.get(1) == "one");
	assert(a3.get(2) == "two");
	assert(a3.get(3) == "three");
	assert(a3.get(4) == "four");
	assert(a3.get(20) == "twenty");

	tf::array<std::string> a4(std::move(a3));
	assert(a4.capacity() == 24);
	assert(a4.auto_reallocating() == true);
	assert(a4.get(0) == "zero");
	assert(a4.get(1) == "one");
	assert(a4.get(2) == "two");
	assert(a4.get(3) == "three");
	assert(a4.get(4) == "four");
	assert(a4.get(20) == "twenty");

	assert(a3.capacity() == 1);
	assert(a3.auto_reallocating() == 1);
}

void test_set_all(tf::array<std::string> &a) {
	assert(a.capacity() == 24);
	assert(a.auto_reallocating() == true);
	assert(a.get(0) == "zero");
	assert(a.get(1) == "one");
	assert(a.get(2) == "two");
	assert(a.get(3) == "three");
	assert(a.get(4) == "four");
	assert(a.get(20) == "twenty");

	a.set_all("all");
	for (int i = 0; i < a.capacity(); ++i) {
		assert(a[i] == "all");
	}
}

int main(int argc, char *argv[]) {
	try {
		tf::array<std::string> result = test_construction();
		test_insert_and_get(result);
		test_reallocation(result);
		test_copying(result);
		test_set_all(result);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
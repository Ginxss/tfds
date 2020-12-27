#include <cassert>
#include <iostream>
#include "../tfds/tf_vector.hpp"

tf::vector<std::string> test_construction() {
	tf::vector<std::string> v1;
	assert(v1.size() == 0);
	assert(v1.empty() == true);
	assert(v1.current_capacity() == 10);

	tf::vector<std::string> v2(4);
	assert(v2.size() == 0);
	assert(v2.empty() == true);
	assert(v2.current_capacity() == 4);

	return v2;
}

void test_add_and_get(tf::vector<std::string> &v) {
	assert(v.size() == 0);
	assert(v.empty() == true);
	assert(v.current_capacity() == 4);

	v.add("zero");
	assert(v.get(0) == "zero");

	v.add("one");
	assert(v[1] == "one");

	assert(v.size() == 2);
	assert(v.empty() == false);
	assert(v.current_capacity() == 4);
}

void test_reallocation(tf::vector<std::string> &v) {
	assert(v.size() == 2);
	assert(v.empty() == false);
	assert(v.current_capacity() == 4);

	v.add("two");
	v.add("three");
	assert(v.size() == 4);
	assert(v.empty() == false);
	assert(v.current_capacity() == 4);

	v.add("four");
	assert(v.size() == 5);
	assert(v.current_capacity() == 8);
}

void test_remove(tf::vector<std::string> &v) {
	assert(v.size() == 5);
	assert(v.current_capacity() == 8);

	assert(v.remove(2) == "two");
	assert(v.size() == 4);
	assert(v.current_capacity() == 8);

	assert(v.get(2) == "three");
	assert(v.get(3) == "four");
}

void test_contains(const tf::vector<std::string> &v) {
	assert(v.size() == 4);
	assert(v.current_capacity() == 8);

	assert(v.contains("two") == false);
	assert(v.contains("three") == true);
}

void test_copying(const tf::vector<std::string> &v) {
	assert(v.size() == 4);
	assert(v.empty() == false);
	assert(v.current_capacity() == 8);
	assert(v.get(1) == "one");

	const tf::vector<std::string> v2 = v;
	assert(v2.size() == 4);
	assert(v2.empty() == false);
	assert(v2.current_capacity() == 8);
	assert(v2.get(1) == "one");

	tf::vector<std::string> v3;
	v3 = v;
	assert(v3.size() == 4);
	assert(v3.empty() == false);
	assert(v3.current_capacity() == 8);
	assert(v3.get(1) == "one");

	const tf::vector<std::string> v4(std::move(v3));
	assert(v4.size() == 4);
	assert(v4.empty() == false);
	assert(v4.current_capacity() == 8);
	assert(v4.get(1) == "one");

	assert(v3.size() == 0);
	assert(v3.empty() == true);
	assert(v3.current_capacity() == 1);
}

void test_set_all(tf::vector<std::string> &v) {
	assert(v.size() == 4);
	assert(v.empty() == false);
	assert(v.current_capacity() == 8);
	assert(v.get(1) == "one");

	v.set_all("new");
	assert(v.size() == 4);
	assert(v.empty() == false);
	assert(v.current_capacity() == 8);
	assert(v.get(1) == "new");
}

void test_clear(tf::vector<std::string> &v) {
	assert(v.size() == 4);
	assert(v.empty() == false);
	assert(v.current_capacity() == 8);
	assert(v.get(2) == "new");

	v.clear();
	assert(v.size() == 0);
	assert(v.empty() == true);
	assert(v.current_capacity() == 8);
}

int main(int argc, char *argv[]) {
	try {
		tf::vector<std::string> result = test_construction();
		test_add_and_get(result);
		test_reallocation(result);
		test_remove(result);
		test_contains(result);
		test_copying(result);
		test_set_all(result);
		test_clear(result);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
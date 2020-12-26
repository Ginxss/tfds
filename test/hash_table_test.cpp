#include <cassert>
#include <iostream>
#include "../tfds/tf_hash_table.hpp"

tf::hash_table<std::string, std::string> test_construction() {
	tf::hash_table<std::string, std::string> h;
	assert(h.size() == 0);
	assert(h.empty() == true);
	assert(h.table_size() == 100);
	assert(h.checks_duplicate_keys() == true);

	tf::hash_table<std::string, std::string> h2(10, false);
	assert(h2.size() == 0);
	assert(h2.empty() == true);
	assert(h2.table_size() == 10);
	assert(h2.checks_duplicate_keys() == false);

	tf::hash_table<std::string, std::string> h3(42);
	assert(h3.size() == 0);
	assert(h3.empty() == true);
	assert(h3.table_size() == 42);
	assert(h3.checks_duplicate_keys() == true);

	return h3;
}

void test_insert_and_get(tf::hash_table<std::string, std::string> &h) {
	assert(h.size() == 0);
	assert(h.empty() == true);
	assert(h.table_size() == 42);
	assert(h.checks_duplicate_keys() == true);

	h.insert("key", "value");
	assert(h.size() == 1);
	assert(h.empty() == false);
	assert(h.get("key") == "value");
	assert(h["key"] == "value");

	h.insert("hello", "world");
	assert(h.size() == 2);
	assert(h.get("key") == "value");
	assert(h["key"] == "value");
	assert(h.get("hello") == "world");
	assert(h["hello"] == "world");
}

void test_iterator(const tf::hash_table<std::string, std::string> &h) {
	assert(h.size() == 2);
	assert(h.empty() == false);
	assert(h.table_size() == 42);
	assert(h.checks_duplicate_keys() == true);

	for (auto it = h.begin(); it.condition(); ++it) {
		assert((it.key() == "key" && *it == "value") || (it.key() == "hello" && it.value() == "world"));
	}
}

void test_contains(const tf::hash_table<std::string, std::string> &h) {
	assert(h.size() == 2);
	assert(h.empty() == false);
	assert(h.table_size() == 42);
	assert(h.checks_duplicate_keys() == true);

	assert(h.contains("key") == true);
	assert(h.contains("hello") == true);
	assert(h.contains("hellos") == false);
}

void test_remove(tf::hash_table<std::string, std::string> &h) {
	assert(h.size() == 2);
	assert(h.empty() == false);
	assert(h.table_size() == 42);
	assert(h.checks_duplicate_keys() == true);
	assert(h.contains("key") == true);

	assert(h.remove("key") == "value");
	assert(h.size() == 1);
	assert(h.contains("key") == false);
}

void test_clear(tf::hash_table<std::string, std::string> &h) {
	assert(h.size() == 1);
	assert(h.empty() == false);
	assert(h.table_size() == 42);
	assert(h.checks_duplicate_keys() == true);

	h.clear();
	assert(h.size() == 0);
	assert(h.empty() == true);
	assert(h.table_size() == 42);
	assert(h.checks_duplicate_keys() == true);
}

int main(int argc, char *argv[]) {
	try {
		tf::hash_table<std::string, std::string> result = test_construction();
		test_insert_and_get(result);
		test_iterator(result);
		test_contains(result);
		test_remove(result);
		test_clear(result);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
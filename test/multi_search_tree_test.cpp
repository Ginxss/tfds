#include <cassert>
#include <iostream>
#include "../tfds/tf_multi_search_tree.hpp"

tf::multi_search_tree<int, std::string> test_construction() {
	tf::multi_search_tree<int, std::string> m;
	assert(m.height() == 0);
	assert(m.size() == 0);
	assert(m.empty() == true);
	
	return m;
}

void test_insert_and_get(tf::multi_search_tree<int, std::string> &m) {
	assert(m.height() == 0);
	assert(m.size() == 0);
	assert(m.empty() == true);

	m.insert(34, "string");
	assert(m.get(34) == "string");
	assert(m.height() == 1);
	assert(m.size() == 1);
	assert(m.empty() == false);

	m.insert(34, "another");
	assert(m[34] == "string" || m[34] == "another");
	assert(m.height() == 1);
	assert(m.size() == 2);
	assert(m.empty() == false);

	m.insert(345, "big");
	assert(m.get(345) == "big");
	assert(m.height() == 2);
	assert(m.size() == 3);
	assert(m.empty() == false);
}

void test_iterator(const tf::multi_search_tree<int, std::string> &m) {
	assert(m.height() == 2);
	assert(m.size() == 3);

	int i = 0;
	for (auto it = m.begin(); it.condition(); ++it) {
		switch (i) {
		case 0:
			assert(it.key() == 34);
			assert(*it == "another" || it.value() == "string");
			break;
		case 1:
			assert(it.key() == 34);
			assert(*it == "another" || it.value() == "string");
			break;
		case 2:
			assert(it.key() == 345);
			assert(it.value() == "big");
			break;
		default:
			assert(false);
		}
		i++;
	}
	assert(i == 3);

	for (auto it = m.end(); it.condition(); --it) {
		i--;
		switch (i) {
		case 0:
			assert(it.key() == 34);
			assert(*it == "another" || it.value() == "string");
			break;
		case 1:
			assert(it.key() == 34);
			assert(*it == "another" || it.value() == "string");
			break;
		case 2:
			assert(it.key() == 345);
			assert(it.value() == "big");
			break;
		default:
			assert(false);
		}
	}
	assert(i == 0);
}

void test_remove(tf::multi_search_tree<int, std::string> &m) {
	assert(m.height() == 2);
	assert(m.size() == 3);

	assert(m.remove(345) == "big");
	assert(m.height() == 1);
	assert(m.size() == 2);

	m.insert(17, "three");
	m.insert(190, "five");
	m.insert(190, "six");
	m.insert(1000, "seven");

	std::string value = m.remove(190);
	assert(value == "five" || value == "six");
	assert(m.size() == 5);
	assert(m.height() == 3);

	int i = 0;
	for (auto it = m.begin(); it.condition(); ++it) {
		switch (i) {
		case 0:
			assert(it.key() == 17);
			assert(it.value() == "three");
			break;
		case 1:
			assert(it.key() == 34);
			assert(*it == "another" || *it == "string");
			break;
		case 2:
			assert(it.key() == 34);
			assert(*it == "another" || *it == "string");
			break;
		case 3:
			assert(it.key() == 190);
			assert(*it == "five" || *it == "six");
			break;
		case 4:
			assert(it.key() == 1000);
			assert(it.value() == "seven");
			break;
		default:
			assert(false);
		}
		i++;
	}
	assert(i == 5);

	std::string value2 = m.remove_all(34);
	assert(value2 == "another" || value2 == "string");
	assert(m.size() == 3);
	assert(m.height() == 2);

	m.insert(33, "3");
	m.insert(33, "33");
	m.insert(33, "333");
	m.insert(33, "3333");
	assert(m.size() == 7);
	assert(m.height() == 3);

	assert(m.remove_value(33, "3") == "3");
	assert(m.remove_value(33, "333") == "333");
	
	std::string value3 = m.remove(33);
	assert(value3 == "33" || value3 == "3333");
	assert(m.size() == 4);
	assert(m.height() == 3);

	std::string value4 = m.remove(33);
	assert(value4 == "33" || value4 == "3333");
	assert(m.size() == 3);
	assert(m.height() == 2);
}

void test_pop_min(tf::multi_search_tree<int, std::string> &m) {
	assert(m.size() == 3);
	assert(m.height() == 2);
	
	m.insert(190, "blah");

	assert(m.pop_min() == "three");
	std::string value = m.pop_min();
	assert(value == "blah" || value == "five");
	assert(m.size() == 2);
	assert(m.height() == 2);

	std::string value2 = m.pop_min();
	assert(value2 == "blah" || value2 == "five");
	assert(m.size() == 1);
	assert(m.height() == 1);
}

void test_pop_max(tf::multi_search_tree<int, std::string> &m) {
	assert(m.size() == 1);
	assert(m.height() == 1);

	m.insert(17, "three");
	m.insert(180, "bleh");
	m.insert(180, "bloh");

	assert(m.pop_max() == "seven");
	std::string value = m.pop_max();
	assert(value == "bleh" || value == "bloh");
	std::string value2 = m.pop_max();
	assert(value2 == "bleh" || value2 == "bloh");
	assert(m.size() == 1);
	assert(m.height() == 1);

	m.insert(69, "xd");
	m.insert(42, "ok");
	m.insert(42, "no");
}

void test_contains(const tf::multi_search_tree<int, std::string> &m) {
	assert(m.size() == 4);
	assert(m.height() == 2);

	assert(m.contains(12) == false);
	assert(m.contains(42) == true);
	assert(m.contains(69) == true);
}

void test_copying(const tf::multi_search_tree<int, std::string> &m) {
	assert(m.size() == 4);
	assert(m.height() == 2);
	assert(m.empty() == false);
	assert(m.get(17) == "three");

	const tf::multi_search_tree<int, std::string> m2 = m;
	assert(m2.size() == 4);
	assert(m2.height() == 2);
	assert(m2.empty() == false);
	assert(m2.get(17) == "three");

	tf::multi_search_tree<int, std::string> m3;
	m3 = m;
	assert(m3.size() == 4);
	assert(m3.height() == 2);
	assert(m3.empty() == false);
	assert(m3.get(17) == "three");

	tf::multi_search_tree<int, std::string> m4(std::move(m3));
	assert(m4.size() == 4);
	assert(m4.height() == 2);
	assert(m4.empty() == false);
	assert(m4.get(17) == "three");

	assert(m3.size() == 0);
	assert(m3.height() == 0);
	assert(m3.empty() == true);
}

void test_clear(tf::multi_search_tree<int, std::string> &m) {
	assert(m.size() == 4);
	assert(m.height() == 2);
	assert(m.empty() == false);

	m.clear();
	assert(m.size() == 0);
	assert(m.height() == 0);
	assert(m.empty() == true);
}

int main(int argc, char *argv[]) {
	try {
		tf::multi_search_tree<int, std::string> result = test_construction();
		test_insert_and_get(result);
		test_iterator(result);
		test_remove(result);
		test_pop_min(result);
		test_pop_max(result);
		test_contains(result);
		test_copying(result);
		test_clear(result);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
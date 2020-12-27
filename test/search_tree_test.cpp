#include <cassert>
#include <iostream>
#include "../tfds/tf_search_tree.hpp"

tf::search_tree<int, std::string> test_construction() {
	tf::search_tree<int, std::string> t;
	assert(t.height() == 0);
	assert(t.size() == 0);
	assert(t.empty() == true);

	return t;
}

void test_insert_and_get(tf::search_tree<int, std::string> &t) {
	assert(t.height() == 0);
	assert(t.size() == 0);
	assert(t.empty() == true);

	t.insert(15, "value");
	assert(t.get(15) == "value");
	assert(t.height() == 1);
	assert(t.size() == 1);
	assert(t.empty() == false);

	t.insert(327, "world");
	assert(t[327] == "world");
	assert(t.height() == 2);
	assert(t.size() == 2);

	t.insert(-2, "two");
	assert(t.get(-2) == "two");
	assert(t.height() == 2);
	assert(t.size() == 3);
}

void test_iterator(const tf::search_tree<int, std::string> &t) {
	assert(t.height() == 2);
	assert(t.size() == 3);

	int i = 0;
	for (auto it = t.begin(); it.condition(); ++it) {
		switch (i) {
		case 0:
			assert(it.key() == -2);
			assert(it.value() == "two");
			break;
		case 1:
			assert(it.key() == 15);
			assert(it.value() == "value");
			break;
		case 2:
			assert(it.key() == 327);
			assert(it.value() == "world");
			break;
		default:
			assert(false);
		}
		i++;
	}
	assert(i == 3);

	for (auto it = t.end(); it.condition(); --it) {
		i--;
		switch (i) {
		case 0:
			assert(it.key() == -2);
			assert(it.value() == "two");
			break;
		case 1:
			assert(it.key() == 15);
			assert(it.value() == "value");
			break;
		case 2:
			assert(it.key() == 327);
			assert(it.value() == "world");
			break;
		default:
			assert(false);
		}
	}
	assert(i == 0);
}

void test_remove(tf::search_tree<int, std::string> &t) {
	assert(t.height() == 2);
	assert(t.size() == 3);

	assert(t.remove(15) == "value");
	assert(t.height() == 2);
	assert(t.size() == 2);

	int i = 0;
	for (auto it = t.begin(); it.condition(); ++it) {
		switch (i) {
		case 0:
			assert(it.key() == -2);
			assert(it.value() == "two");
			break;
		case 1:
			assert(it.key() == 327);
			assert(it.value() == "world");
			break;
		default:
			assert(false);
		}
		i++;
	}
	assert(i == 2);

	t.insert(17, "three");
	t.insert(190, "five");
	t.insert(1000, "seven");

	assert(t.remove(190) == "five");
	assert(t.size() == 4);
	assert(t.height() == 3);
}

void test_pop_min(tf::search_tree<int, std::string> &t) {
	assert(t.size() == 4);
	assert(t.height() == 3);

	assert(t.pop_min() == "two");
	assert(t.size() == 3);
	assert(t.height() == 2);
}

void test_pop_max(tf::search_tree<int, std::string> &t) {
	assert(t.size() == 3);
	assert(t.height() == 2);

	assert(t.pop_max() == "seven");
	assert(t.size() == 2);
	assert(t.height() == 2);
}

void test_contains(const tf::search_tree<int, std::string> &t) {
	assert(t.size() == 2);
	assert(t.height() == 2);

	assert(t.contains(126) == false);
	assert(t.contains(327) == true);
	assert(t.contains(17) == true);
}

void test_copying(const tf::search_tree<int, std::string> &t) {
	assert(t.size() == 2);
	assert(t.height() == 2);
	assert(t.empty() == false);
	assert(t.min() == "three");
	assert(t.max() == "world");

	const tf::search_tree<int, std::string> t2 = t;
	assert(t2.size() == 2);
	assert(t2.height() == 2);
	assert(t2.empty() == false);
	assert(t2.min() == "three");
	assert(t2.max() == "world");

	tf::search_tree<int, std::string> t3;
	t3 = t;
	assert(t3.size() == 2);
	assert(t3.height() == 2);
	assert(t3.empty() == false);
	assert(t3.min() == "three");
	assert(t3.max() == "world");

	tf::search_tree<int, std::string> t4(std::move(t3));
	assert(t4.size() == 2);
	assert(t4.height() == 2);
	assert(t4.empty() == false);
	assert(t4.min() == "three");
	assert(t4.max() == "world");

	assert(t3.size() == 0);
	assert(t3.height() == 0);
	assert(t3.empty() == true);
}

void test_clear(tf::search_tree<int, std::string> &t) {
	assert(t.size() == 2);
	assert(t.height() == 2);
	assert(t.empty() == false);

	t.clear();
	assert(t.size() == 0);
	assert(t.height() == 0);
	assert(t.empty() == true);
}

int main(int argc, char *argv[]) {
	try {
		tf::search_tree<int, std::string> result = test_construction();
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
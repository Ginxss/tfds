#include <cassert>
#include <iostream>
#include "../../tfds/tf_hash_table.hpp"

void test_table();
void test_table_default_constructor();
void test_table_insert();
void test_table_get();
void test_table_copy_constructor();
void test_table_swap();
void test_table_move_constructor();
void test_table_copy_assignment();
void test_table_contains();
void test_table_remove();
void test_table_brackets_operator();
void test_table_iteration();
void test_table_empty();
void test_table_clear();


/* int main(int argc, char *argv[]) {
	test_table();

	return 0;
} */

void test_table() {
	test_table_default_constructor();
	test_table_insert();
	test_table_get();
	test_table_copy_constructor();
	test_table_swap();
	test_table_move_constructor();
	test_table_copy_assignment();
	test_table_contains();
	test_table_remove();
	test_table_brackets_operator();
	test_table_iteration();
	test_table_empty();
	test_table_clear();

	std::cout << "HASH TABLE tests successful." << std::endl;
}

// prec: -
void test_table_default_constructor() {
	tf::hash_table<std::string, int> h;
	assert(h.size() == 0);
	assert(h.table_size() == 100);
	assert(h.checks_duplicate_keys() == true);

	tf::hash_table<std::string, int> h2(10);
	assert(h2.size() == 0);
	assert(h2.table_size() == 10);
	assert(h2.checks_duplicate_keys() == true);

	tf::hash_table<std::string, int> h3(0, false);
	assert(h3.size() == 0);
	assert(h3.table_size() == 1);
	assert(h3.checks_duplicate_keys() == false);
}

// prec: default_constructor
void test_table_insert() {
	tf::hash_table<std::string, int> h;

	tf::hash_table<std::string, int> h2(10, false);

	// -- //

	h.insert("One", 1);
	assert(h.size() == 1);

	h.insert("Two", 2);
	assert(h.size() == 2);

	h.insert("Three", 3);
	assert(h.size() == 3);

	h.insert("Four", 4);
	assert(h.size() == 4);

	try {
		h.insert("Two", 22);
		assert(false);
	} catch (tf::exception &) {}

	h2.insert("One", 1);
	h2.insert("One", 11);
	assert(h2.size() == 2);
}

// prec: insert
void test_table_get() {
	tf::hash_table<std::string, int> h;
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);
	h.insert("Four", 4);

	// -- //

	assert(h.get("One") == 1);
	assert(h.get("Two") == 2);
	assert(h.get("Three") == 3);
	assert(h.get("Four") == 4);

	try {
		h.get("Five");
		assert(false);
	} catch (tf::exception &) {}
}

// prec: get
void test_table_copy_constructor() {
	tf::hash_table<std::string, int> h(10, false);
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);
	h.insert("Four", 4);

	// -- //

	tf::hash_table<std::string, int> h2(h);
	assert(h2.size() == 4);
	assert(h2.table_size() == 10);
	assert(h2.checks_duplicate_keys() == false);
	assert(h2.get("One") == 1);
	assert(h2.get("Two") == 2);
	assert(h2.get("Three") == 3);
	assert(h2.get("Four") == 4);

	h2.insert("Five", 5);
	assert(h.size() == 4);
}

// prec: get
void test_table_swap() {
	tf::hash_table<std::string, int> h(10);
	h.insert("One", 1);
	h.insert("Two", 2);

	tf::hash_table<std::string, int> h2(101, false);
	h2.insert("One", 3);
	h2.insert("Two", 3);
	h2.insert("Three", 3);

	// -- //

	swap(h, h2);

	assert(h.size() == 3);
	assert(h.table_size() == 101);
	assert(h.checks_duplicate_keys() == false);
	assert(h.get("One") == 3);
	assert(h.get("Two") == 3);
	assert(h.get("Three") == 3);

	assert(h2.size() == 2);
	assert(h2.table_size() == 10);
	assert(h2.checks_duplicate_keys() == true);
	assert(h2.get("One") == 1);
	assert(h2.get("Two") == 2);
}

// prec: get
void test_table_move_constructor() {
	tf::hash_table<std::string, int> h(10);
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);
	h.insert("Four", 4);

	// -- //

	tf::hash_table<std::string, int> h2(std::move(h));
	assert(h2.size() == 4);
	assert(h2.table_size() == 10);
	assert(h2.checks_duplicate_keys() == true);
	assert(h2.get("One") == 1);
	assert(h2.get("Two") == 2);
	assert(h2.get("Three") == 3);
	assert(h2.get("Four") == 4);
}

// prec: get
void test_table_copy_assignment() {
	tf::hash_table<std::string, int> h(10);
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);
	h.insert("Four", 4);

	// -- //

	tf::hash_table<std::string, int> h2;
	h2 = h;

	assert(h2.size() == 4);
	assert(h2.table_size() == 10);
	assert(h2.checks_duplicate_keys() == true);
	assert(h2.get("One") == 1);
	assert(h2.get("Two") == 2);
	assert(h2.get("Three") == 3);
	assert(h2.get("Four") == 4);
}

// prec: insert
void test_table_contains() {
	tf::hash_table<std::string, int> h;

	// -- //

	assert(h.contains("One") == false);
	h.insert("One", 1);
	assert(h.contains("One") == true);

	assert(h.contains("Two") == false);
	h.insert("Two", 2);
	assert(h.contains("One") == true);
	assert(h.contains("Two") == true);

	assert(h.contains("Three") == false);
	h.insert("Three", 3);
	assert(h.contains("One") == true);
	assert(h.contains("Two") == true);
	assert(h.contains("Three") == true);
}

// prec: contains
void test_table_remove() {
	tf::hash_table<std::string, int> h;
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);
	h.insert("Four", 4);

	// -- //

	try {
		h.remove("Five");
		assert(false);
	} catch (tf::exception &) {}

	assert(h.remove("One") == 1);
	assert(h.size() == 3);
	assert(h.contains("One") == false);

	assert(h.remove("Three") == 3);
	assert(h.size() == 2);
	assert(h.contains("Three") == false);

	assert(h.remove("Four") == 4);
	assert(h.size() == 1);
	assert(h.contains("Four") == false);

	assert(h.remove("Two") == 2);
	assert(h.size() == 0);
	assert(h.contains("Two") == false);

	try {
		h.remove("One");
		assert(false);
	} catch (tf::exception &) {}
}

// prec: insert
void test_table_brackets_operator() {
	tf::hash_table<std::string, int> h;
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);
	h.insert("Four", 4);

	// -- //

	assert(h["One"] == 1);
	assert(h["Two"] == 2);
	assert(h["Three"] == 3);
	assert(h["Four"] == 4);

	try {
		h["Five"];
		assert(false);
	} catch (tf::exception &) {}
}

// prec: insert
void test_table_iteration() {
	tf::hash_table<std::string, int> h;
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);
	h.insert("Four", 4);

	// -- //

	int i = 0;
	for (auto it = h.begin(); it.has_value(); ++it) {
		assert(*it == 1 || *it == 2 || *it == 3 || *it == 4);
		++i;
	}
	assert(i == 4);
}

// prec: remove
void test_table_empty() {
	tf::hash_table<std::string, int> h;

	// -- //

	assert(h.empty() == true);

	h.insert("One", 1);
	assert(h.empty() == false);
	h.insert("Two", 2);
	assert(h.empty() == false);

	h.remove("One");
	assert(h.empty() == false);
	h.remove("Two");
	assert(h.empty() == true);
}

// prec: empty
void test_table_clear() {
	tf::hash_table<std::string, int> h(10);
	h.insert("One", 1);
	h.insert("Two", 2);
	h.insert("Three", 3);

	// -- //

	h.clear();
	assert(h.size() == 0);
	assert(h.table_size() == 10);
	assert(h.empty() == true);
	assert(h.checks_duplicate_keys() == true);

	try {
		h["One"];
		assert(false);
	} catch (tf::exception &) {}
}
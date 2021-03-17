#include <cassert>
#include <iostream>
#include "../../tfds/tf_search_tree.hpp"

void test_tree();
void test_tree_default_constructor();
void test_tree_insert();
void test_tree_get();
void test_tree_copy_constructor();
void test_tree_swap();
void test_tree_move_constructor();
void test_tree_copy_assignment();
void test_tree_brackets_operator();
void test_tree_min();
void test_tree_max();
void test_tree_pop_min();
void test_tree_pop_max();
void test_tree_contains();
void test_tree_contains_value();
void test_tree_remove();
void test_tree_remove_all();
void test_tree_remove_value();
void test_tree_iteration();
void test_tree_empty();
void test_tree_clear();


int main(int argc, char *argv[]) {
	test_tree();

	return 0;
}

void test_tree() {
	test_tree_default_constructor();
	test_tree_insert();
	test_tree_get();
	test_tree_copy_constructor();
	test_tree_swap();
	test_tree_move_constructor();
	test_tree_copy_assignment();
	test_tree_brackets_operator();
	test_tree_min();
	test_tree_max();
	test_tree_pop_min();
	test_tree_pop_max();
	test_tree_contains();
	test_tree_contains_value();
	test_tree_remove();
	test_tree_remove_all();
	test_tree_remove_value();
	test_tree_iteration();
	test_tree_empty();
	test_tree_clear();

	std::cout << "SEARCH TREE tests successful." << std::endl;
}

// prec: -
void test_tree_default_constructor() {
	tf::search_tree<int, std::string> t;
	assert(t.size() == 0);
	assert(t.height() == 0);
	assert(t.allows_duplicate_keys() == false);

	tf::search_tree<int, std::string> t2(true);
	assert(t2.size() == 0);
	assert(t2.height() == 0);
	assert(t2.allows_duplicate_keys() == true);
}

// prec: default_constructor
void test_tree_insert() {
	tf::search_tree<int, std::string> t;
	tf::search_tree<int, std::string> t2(true);

	// -- //

	t.insert(2, "Two");
	assert(t.size() == 1);
	assert(t.height() == 1);

	t.insert(6, "Six");
	assert(t.size() == 2);
	assert(t.height() == 2);

	t.insert(-2, "nTwo");
	assert(t.size() == 3);
	assert(t.height() == 2);

	t.insert(-6, "nSix");
	assert(t.size() == 4);
	assert(t.height() == 3);

	try {
		t.insert(2, "Two2");
		assert(false);
	} catch (tf::exception &) {}

	t2.insert(1, "One");
	t2.insert(1, "One2");
}

// prec: insert
void test_tree_get() {
	tf::search_tree<int, std::string> t;

	// -- //

	t.insert(2, "Two");
	assert(t.get(2) == "Two");

	t.insert(6, "Six");
	assert(t.get(2) == "Two");
	assert(t.get(6) == "Six");

	try {
		t.get(1);
		assert(false);
	} catch (tf::exception &) {}
}

// prec: get
void test_tree_copy_constructor() {
	tf::search_tree<int, std::string> t(true);
	t.insert(2, "Two");
	t.insert(6, "Six");

	// -- //

	tf::search_tree<int, std::string> t2(t);
	assert(t2.size() == 2);
	assert(t2.height() == 2);
	assert(t2.allows_duplicate_keys() == true);
	assert(t2.get(2) == "Two");
	assert(t2.get(6) == "Six");

	t2.insert(2, "Two2");
	assert(t.size() == 2);
}

// prec: get
void test_tree_swap() {
	tf::search_tree<int, std::string> t;
	t.insert(2, "Two");
	t.insert(6, "Six");

	tf::search_tree<int, std::string> t2(true);
	t2.insert(1, "One");
	t2.insert(2, "Two");
	t2.insert(-2, "nTwo");

	// -- //

	swap(t, t2);

	assert(t.size() == 3);
	assert(t.height() == 2);
	assert(t.allows_duplicate_keys() == true);
	assert(t.get(1) == "One");
	assert(t.get(2) == "Two");
	assert(t.get(-2) == "nTwo");

	assert(t2.size() == 2);
	assert(t2.height() == 2);
	assert(t2.allows_duplicate_keys() == false);
	assert(t2.get(2) == "Two");
	assert(t2.get(6) == "Six");
}

// prec: get
void test_tree_move_constructor() {
	tf::search_tree<int, std::string> t(true);
	t.insert(2, "Two");
	t.insert(6, "Six");

	// -- //

	tf::search_tree<int, std::string> t2(std::move(t));
	assert(t2.size() == 2);
	assert(t2.height() == 2);
	assert(t2.allows_duplicate_keys() == true);
	assert(t2.get(2) == "Two");
	assert(t2.get(6) == "Six");
}

// prec: get
void test_tree_copy_assignment() {
	tf::search_tree<int, std::string> t(true);
	t.insert(2, "Two");
	t.insert(6, "Six");

	// -- //

	tf::search_tree<int, std::string> t2;
	t2 = t;

	assert(t2.size() == 2);
	assert(t2.height() == 2);
	assert(t2.allows_duplicate_keys() == true);
	assert(t2.get(2) == "Two");
	assert(t2.get(6) == "Six");
}

// prec: insert
void test_tree_brackets_operator() {
	tf::search_tree<int, std::string> t;
	t.insert(2, "Two");
	t.insert(6, "Six");

	// -- //

	assert(t[2] == "Two");
	assert(t[6] == "Six");

	try {
		t[1];
		assert(false);
	} catch (tf::exception &) {}
}

// prec: insert
void test_tree_min() {
	tf::search_tree<int, std::string> t;

	// -- //

	t.insert(1, "One");
	assert(t.min() == "One");

	t.insert(-1, "nOne");
	assert(t.min() == "nOne");

	t.insert(10, "Ten");
	assert(t.min() == "nOne");
}

// prec: insert
void test_tree_max() {
	tf::search_tree<int, std::string> t;

	// -- //

	t.insert(1, "One");
	assert(t.max() == "One");

	t.insert(-1, "nOne");
	assert(t.max() == "One");

	t.insert(10, "Ten");
	assert(t.max() == "Ten");
}

// prec: insert
void test_tree_pop_min() {
	tf::search_tree<int, std::string> t;
	t.insert(1, "One");
	t.insert(-1, "nOne");
	t.insert(10, "Ten");
	t.insert(60, "Sixty");
	t.insert(-100, "nHundred");
	t.insert(7, "Seven");

	// -- //

	assert(t.pop_min() == "nHundred");
	assert(t.size() == 5);
	assert(t.height() == 3);

	assert(t.pop_min() == "nOne");
	assert(t.size() == 4);
	assert(t.height() == 3);

	assert(t.pop_min() == "One");
	assert(t.size() == 3);
	assert(t.height() == 2);

	assert(t.pop_min() == "Seven");
	assert(t.size() == 2);
	assert(t.height() == 2);

	assert(t.pop_min() == "Ten");
	assert(t.size() == 1);
	assert(t.height() == 1);

	assert(t.pop_min() == "Sixty");
	assert(t.size() == 0);
	assert(t.height() == 0);
}

// prec: insert
void test_tree_pop_max() {
	tf::search_tree<int, std::string> t;
	t.insert(1, "One");
	t.insert(-1, "nOne");
	t.insert(10, "Ten");
	t.insert(60, "Sixty");
	t.insert(-100, "nHundred");
	t.insert(7, "Seven");

	// -- //

	assert(t.pop_max() == "Sixty");
	assert(t.size() == 5);
	assert(t.height() == 3);

	assert(t.pop_max() == "Ten");
	assert(t.size() == 4);
	assert(t.height() == 3);

	assert(t.pop_max() == "Seven");
	assert(t.size() == 3);
	assert(t.height() == 2);

	assert(t.pop_max() == "One");
	assert(t.size() == 2);
	assert(t.height() == 2);

	assert(t.pop_max() == "nOne");
	assert(t.size() == 1);
	assert(t.height() == 1);

	assert(t.pop_max() == "nHundred");
	assert(t.size() == 0);
	assert(t.height() == 0);
}

// prec: insert
void test_tree_contains() {
	tf::search_tree<int, std::string> t;

	// -- //

	assert(t.contains(1) == false);
	t.insert(1, "One");
	assert(t.contains(1) == true);

	assert(t.contains(2) == false);
	t.insert(2, "Two");
	assert(t.contains(1) == true);
	assert(t.contains(2) == true);

	assert(t.contains(3) == false);
	t.insert(3, "Three");
	assert(t.contains(1) == true);
	assert(t.contains(2) == true);
	assert(t.contains(3) == true);

	assert(t.contains(-3) == false);
	t.insert(-3, "nThree");
	assert(t.contains(1) == true);
	assert(t.contains(2) == true);
	assert(t.contains(3) == true);
	assert(t.contains(-3) == true);
}

// prec: insert
void test_tree_contains_value() {
	tf::search_tree<int, std::string> t(true);

	// -- //

	assert(t.contains_value(1, "One") == false);
	t.insert(1, "One2");
	assert(t.contains_value(1, "One") == false);
	t.insert(1, "One");
	assert(t.contains_value(1, "One") == true);

	assert(t.contains_value(2, "Two") == false);
	t.insert(2, "Two");
	assert(t.contains_value(2, "Two") == true);
}

// prec: contains
void test_tree_remove() {
	tf::search_tree<int, std::string> t;
	t.insert(1, "One");
	t.insert(-1, "nOne");
	t.insert(10, "Ten");
	t.insert(60, "Sixty");
	t.insert(-100, "nHundred");
	t.insert(7, "Seven");

	tf::search_tree<int, std::string> t2(true);
	t2.insert(1, "One");
	t2.insert(1, "One2");
	t2.insert(1, "One3");
	t2.insert(10, "Ten");
	t2.insert(10, "Ten2");
	t2.insert(7, "Seven");

	// -- //

	assert(t.remove(1) == "One");
	assert(t.size() == 5);
	assert(t.contains(1) == false);

	assert(t.remove(10) == "Ten");
	assert(t.size() == 4);
	assert(t.contains(10) == false);

	assert(t.remove(7) == "Seven");
	assert(t.size() == 3);
	assert(t.contains(17) == false);

	assert(t.remove(-1) == "nOne");
	assert(t.size() == 2);
	assert(t.contains(-1) == false);

	assert(t.remove(-100) == "nHundred");
	assert(t.size() == 1);
	assert(t.contains(100) == false);

	assert(t.remove(60) == "Sixty");
	assert(t.size() == 0);
	assert(t.contains(60) == false);

	std::string result = t2.remove(1);
	assert(result == "One" || result == "One2" || result == "One3");
	assert(t2.size() == 5);
	assert(t2.contains(1) == true);

	result = t2.remove(1);
	assert(result == "One" || result == "One2" || result == "One3");
	assert(t2.size() == 4);
	assert(t2.contains(1) == true);

	result = t2.remove(1);
	assert(result == "One" || result == "One2" || result == "One3");
	assert(t2.size() == 3);
	assert(t2.contains(1) == false);
}

// prec: contains, get
void test_tree_remove_all() {
	tf::search_tree<int, std::string> t(true);
	t.insert(1, "One");
	t.insert(1, "One2");
	t.insert(1, "One3");
	t.insert(10, "Ten");
	t.insert(10, "Ten2");
	t.insert(7, "Seven");

	// -- //

	std::string result = t.remove_all(1);
	assert(result == "One" || result == "One2" || result == "One3");
	assert(t.size() == 3);
	assert(t.contains(1) == false);

	try {
		t.get(1);
		assert(false);
	} catch (tf::exception &) {}
}

// prec: contains
void test_tree_remove_value() {
	tf::search_tree<int, std::string> t(true);
	t.insert(1, "One");
	t.insert(1, "One2");
	t.insert(1, "One3");
	t.insert(10, "Ten");
	t.insert(10, "Ten2");
	t.insert(7, "Seven");

	// -- //

	
	assert(t.remove_value(1, "One2") == "One2");
	assert(t.size() == 5);
	assert(t.contains(1) == true);

	assert(t.remove_value(1, "One") == "One");
	assert(t.size() == 4);
	assert(t.contains(1) == true);

	assert(t.remove_value(1, "One3") == "One3");
	assert(t.size() == 3);
	assert(t.contains(1) == false);
}

// prec: insert
void test_tree_iteration() {
	tf::search_tree<int, std::string> t(true);
	t.insert(1, "One");
	t.insert(-1, "nOne");
	t.insert(10, "Ten");
	t.insert(10, "Ten2");
	t.insert(60, "Sixty");
	t.insert(-100, "nHundred");
	t.insert(7, "Seven");

	// -- //

	int i = 0;
	for (auto it = t.begin(); it.condition(); ++it) {
		switch (i) {
		case 0: assert(*it == "nHundred"); break;
		case 1: assert(*it == "nOne"); break;
		case 2: assert(*it == "One"); break;
		case 3: assert(*it == "Seven"); break;
		case 4: assert(*it == "Ten" || *it == "Ten2"); break;
		case 5: assert(*it == "Ten" || *it == "Ten2"); break;
		case 6: assert(*it == "Sixty"); break;
		}
		++i;
	}
	assert(i == 7);

	for (auto it = t.end(); it.condition(); --it) {
		--i;
		switch (i) {
		case 6: assert(*it == "Sixty"); break;
		case 5: assert(*it == "Ten" || *it == "Ten2"); break;
		case 4: assert(*it == "Ten" || *it == "Ten2"); break;
		case 3: assert(*it == "Seven"); break;
		case 2: assert(*it == "One"); break;
		case 1: assert(*it == "nOne"); break;
		case 0:
			assert(it.value() == "nHundred");
			it.value() = "New nHundred";
			break;
		}
	}
	assert(i == 0);

	for (auto it = t.begin(); it.condition(); --it) {
		assert(*it == "New nHundred");
	}

	for (auto it = t.end(); it.condition(); ++it) {
		assert(*it == "Sixty");
	}
}

// prec: remove
void test_tree_empty() {
	tf::search_tree<int, std::string> t;

	// -- //

	assert(t.empty() == true);

	t.insert(1, "One");
	assert(t.empty() == false);
	t.insert(2, "Two");
	assert(t.empty() == false);

	t.remove(1);
	assert(t.empty() == false);
	t.remove(2);
	assert(t.empty() == true);
}

// prec: empty();
void test_tree_clear() {
	tf::search_tree<int, std::string> t(true);
	t.insert(1, "One");
	t.insert(-1, "nOne");
	t.insert(10, "Ten");
	t.insert(10, "Ten2");
	t.insert(60, "Sixty");
	t.insert(-100, "nHundred");
	t.insert(7, "Seven");

	// -- //

	t.clear();
	assert(t.size() == 0);
	assert(t.height() == 0);
	
	try {
		t.min();
		assert(false);
	} catch (tf::exception &) {}
}
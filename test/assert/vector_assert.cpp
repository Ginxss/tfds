#include <cassert>
#include <iostream>
#include "../../tfds/tf_vector.hpp"

void test_vector();
void test_vector_default_constuctor();
void test_vector_add();
void test_vector_get();
void test_vector_copy_constructor();
void test_vector_swap();
void test_vector_move_constructor();
void test_vector_copy_assignment();
void test_vector_brackets_operator();
void test_vector_set_all();
void test_vector_contains();
void test_vector_remove();
void test_vector_reallocate();
void test_vector_empty();
void test_vector_clear();


/* int main(int argc, char *argv[]) {
	test_vector();

	return 0;
} */

void test_vector() {
	test_vector_default_constuctor();
	test_vector_add();
	test_vector_get();
	test_vector_copy_constructor();
	test_vector_swap();
	test_vector_move_constructor();
	test_vector_copy_assignment();
	test_vector_brackets_operator();
	test_vector_set_all();
	test_vector_contains();
	test_vector_remove();
	test_vector_reallocate();
	test_vector_empty();
	test_vector_clear();

	std::cout << "VECTOR tests successful." << std::endl;
}

// prec: -
void test_vector_default_constuctor() {
	tf::vector<std::string> v;
	assert(v.size() == 0);
	assert(v.capacity() == 10);

	tf::vector<std::string> v2(1000);
	assert(v2.size() == 0);
	assert(v2.capacity() == 1000);

	tf::vector<std::string> v3(0);
	assert(v3.size() == 0);
	assert(v3.capacity() == 1);
}

// prec: default_constructor
void test_vector_add() {
	tf::vector<std::string> v(2);

	// -- //

	v.add("Zero");
	assert(v.size() == 1);
	assert(v.capacity() == 2);

	v.add("One");
	assert(v.size() == 2);
	assert(v.capacity() == 2);

	v.add("Two");
	assert(v.size() == 3);
	assert(v.capacity() == 4);

	v.add("Three");
	assert(v.size() == 4);
	assert(v.capacity() == 4);

	v.add("Four");
	assert(v.size() == 5);
	assert(v.capacity() == 8);
}

// prec: add
void test_vector_get() {
	tf::vector<std::string> v(2);

	// -- //

	v.add("Zero");
	assert(v.get(0) == "Zero");

	v.add("One");
	assert(v.get(0) == "Zero");
	assert(v.get(1) == "One");

	v.add("Two");
	assert(v.get(0) == "Zero");
	assert(v.get(1) == "One");
	assert(v.get(2) == "Two");

	try {
		v.get(3);
		assert(false);
	} catch (tf::exception &) {}
}

// prec: get
void test_vector_copy_constructor() {
	tf::vector<std::string> v(9);
	v.add("Zero");
	v.add("One");

	// -- //

	tf::vector<std::string> v2(v);
	assert(v.size() == 2);
	assert(v.capacity() == 9);
	assert(v.get(0) == "Zero");
	assert(v.get(1) == "One");

	v2.add("Two");
	assert(v.size() == 2);
}

// prec: get
void test_vector_swap() {
	tf::vector<std::string> v;
	v.add("Zero");
	v.add("One");
	v.add("Two");

	tf::vector<std::string> v2(8);
	v2.add("Four");

	// -- //

	swap(v, v2);

	assert(v.size() == 1);
	assert(v.capacity() == 8);
	assert(v.get(0) == "Four");

	assert(v2.size() == 3);
	assert(v2.capacity() == 10);
	assert(v2.get(0) == "Zero");
	assert(v2.get(1) == "One");
	assert(v2.get(2) == "Two");
}

// prec: get
void test_vector_move_constructor() {
	tf::vector<std::string> v(9);
	v.add("Zero");
	v.add("One");

	// -- //

	tf::vector<std::string> v2(std::move(v));
	assert(v2.size() == 2);
	assert(v2.capacity() == 9);
	assert(v2.get(0) == "Zero");
	assert(v2.get(1) == "One");
}

// prec: get
void test_vector_copy_assignment() {
	tf::vector<std::string> v(9);
	v.add("Zero");
	v.add("One");

	// -- //

	tf::vector<std::string> v2;
	v2 = v;

	assert(v2.size() == 2);
	assert(v2.capacity() == 9);
	assert(v2.get(0) == "Zero");
	assert(v2.get(1) == "One");
}

// prec: add
void test_vector_brackets_operator() {
	tf::vector<std::string> v(9);
	v.add("Zero");
	v.add("One");

	// -- //

	assert(v[0] == "Zero");
	assert(v[1] == "One");

	try {
		v[2];
		assert(false);
	} catch (tf::exception &) {}
}

// prec: brackets_operator
void test_vector_set_all() {
	tf::vector<std::string> v;
	v.add("Zero");
	v.add("One");
	v.add("Two");

	// -- //

	v.set_all("All");

	for (int i = 0; i < 3; ++i) {
		assert(v[i] == "All");
	}
}

// prec: add
void test_vector_contains() {
	tf::vector<std::string> v;

	// -- //

	assert(v.contains("Zero") == false);
	v.add("Zero");
	assert(v.contains("Zero") == true);

	assert(v.contains("One") == false);
	v.add("One");
	assert(v.contains("Zero") == true);
	assert(v.contains("One") == true);

	assert(v.contains("Two") == false);
	v.add("Two");
	assert(v.contains("Zero") == true);
	assert(v.contains("One") == true);
	assert(v.contains("Two") == true);
}

// prec: contains, brackets_operator
void test_vector_remove() {
	tf::vector<std::string> v;
	v.add("Zero");
	v.add("One");
	v.add("Two");

	// -- //

	assert(v.remove(1) == "One");
	assert(v.size() == 2);
	assert(v.capacity() == 10);
	assert(v.contains("One") == false);
	assert(v[0] == "Zero");
	assert(v[1] == "Two");

	try {
		v.remove(2);
		assert(false);
	} catch (tf::exception &) {}

	assert(v.remove(1) == "Two");
	assert(v.size() == 1);
	assert(v.remove(0) == "Zero");
	assert(v.size() == 0);

	try {
		v.remove(0);
		assert(false);
	} catch (tf::exception &) {}
}

// prec: brackets_operator
void test_vector_reallocate() {
	tf::vector<std::string> v;
	v.add("Zero");
	v.add("One");
	v.add("Two");

	// -- //

	v.reallocate(2);
	assert(v.size() == 2);
	assert(v.capacity() == 2);
	assert(v[0] == "Zero");
	assert(v[1] == "One");

	try {
		v[2];
		assert(false);
	} catch (tf::exception &) {}

	v.reallocate(50);
	assert(v.size() == 2);
	assert(v.capacity() == 50);
	assert(v[0] == "Zero");
	assert(v[1] == "One");

	try {
		v[2];
		assert(false);
	} catch (tf::exception &) {}
}

// prec: remove
void test_vector_empty() {
	tf::vector<std::string> v;

	// -- //

	assert(v.empty() == true);

	v.add("Zero");
	assert(v.empty() == false);
	v.add("One");
	assert(v.empty() == false);

	v.remove(0);
	assert(v.empty() == false);
	v.remove(0);
	assert(v.empty() == true);
}

// prec: empty
void test_vector_clear() {
	tf::vector<std::string> v(9);
	v.add("Zero");
	v.add("One");
	v.add("Two");

	// -- //

	v.clear();
	assert(v.size() == 0);
	assert(v.empty() == true);
	assert(v.capacity() == 9);
	
	try {
		v[0];
		assert(false);
	} catch (tf::exception &) {}
}
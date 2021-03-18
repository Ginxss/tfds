#include <cassert>
#include <iostream>
#include "../../tfds/tf_array.hpp"

void test_array();
void test_array_default_constructor();
void test_array_insert();
void test_array_get();
void test_array_copy_constructor();
void test_array_swap();
void test_array_move_constructor();
void test_array_copy_assignment();
void test_array_brackets_operator();
void test_array_set_all();
void test_array_reallocate();


/* int main(int argc, char *argv[]) {
	test_array();

	return 0;
} */

void test_array() {
	test_array_default_constructor();
	test_array_insert();
	test_array_get();
	test_array_copy_constructor();
	test_array_swap();
	test_array_move_constructor();
	test_array_copy_assignment();
	test_array_brackets_operator();
	test_array_set_all();
	test_array_reallocate();

	std::cout << "ARRAY tests successful." << std::endl;
}

// prec: -
void test_array_default_constructor() {
	tf::array<std::string> a;
	assert(a.capacity() == 10);
	assert(a.auto_reallocating() == true);

	tf::array<std::string> a2(1000);
	assert(a2.capacity() == 1000);
	assert(a2.auto_reallocating() == true);

	tf::array<std::string> a3(0, false);
	assert(a3.capacity() == 1);
	assert(a3.auto_reallocating() == false);
}

// prec: default_constructor
void test_array_insert() {
	tf::array<std::string> a;

	// -- //

	a.insert(1, "One");
	assert(a.capacity() == 10);

	a.insert(10, "Ten");
	assert(a.capacity() == 20);

	a.insert(10000, "Tenthousand");
	assert(a.capacity() == 10020);

	try {
		tf::array<std::string> a2(10, false);

		a2.insert(10, "Ten");
		assert(false);
	} catch (tf::exception &) {}
}

// prec: insert
void test_array_get() {
	tf::array<std::string> a;

	// -- //

	a.insert(1, "One");
	assert(a.get(1) == "One");

	a.insert(10, "Ten");
	assert(a.get(1) == "One");
	assert(a.get(10) == "Ten");

	try {
		a.get(0);
		a.get(20);
		assert(false);
	} catch (tf::exception &) {}
}

// prec: get
void test_array_copy_constructor() {
	tf::array<std::string> a;
	a.insert(1, "One");
	a.insert(10, "Ten");

	// -- //

	tf::array<std::string> a2(a);
	assert(a2.capacity() == 20);
	assert(a2.auto_reallocating() == true);
	assert(a2.get(1) == "One");
	assert(a2.get(10) == "Ten");

	a.insert(30, "Thirty");
	assert(a2.capacity() == 20);
}

// prec: get
void test_array_swap() {
	tf::array<std::string> a;
	a.insert(1, "One");
	a.insert(10, "Ten");

	tf::array<std::string> a2(8, false);
	a2.insert(0, "Zero");
	a2.insert(7, "Seven");

	// -- //

	swap(a, a2);
	
	assert(a.capacity() == 8);
	assert(a.auto_reallocating() == false);
	assert(a.get(0) == "Zero");
	assert(a.get(7) == "Seven");

	assert(a2.capacity() == 20);
	assert(a2.auto_reallocating() == true);
	assert(a2.get(1) == "One");
	assert(a2.get(10) == "Ten");
}

// prec: get
void test_array_move_constructor() {
	tf::array<std::string> a;
	a.insert(1, "One");
	a.insert(10, "Ten");

	// -- //

	tf::array<std::string> a2(std::move(a));
	assert(a2.capacity() == 20);
	assert(a2.auto_reallocating() == true);
	assert(a2.get(1) == "One");
	assert(a2.get(10) == "Ten");
}

// prec: get
void test_array_copy_assignment() {
	tf::array<std::string> a;
	a.insert(1, "One");
	a.insert(10, "Ten");

	// -- //

	tf::array<std::string> a2(5, false);
	a2 = a;

	assert(a2.capacity() == 20);
	assert(a2.auto_reallocating() == true);
	assert(a2.get(1) == "One");
	assert(a2.get(10) == "Ten");
}

// prec: insert
void test_array_brackets_operator() {
	tf::array<std::string> a;
	a.insert(1, "One");
	a.insert(10, "Ten");

	// -- //

	assert(a[1] == "One");
	assert(a[10] == "Ten");

	a[1] = "Two";
	a[20] = "Twenty";
	assert(a.capacity() == 40);
	assert(a[1] == "Two");
	assert(a[10] == "Ten");
	assert(a[20] == "Twenty");

	try {
		tf::array<std::string> a2(10, false);

		a2[10];
		assert(false);
	} catch (tf::exception &) {}
}

// prec: brackets_operator
void test_array_set_all() {
	tf::array<std::string> a;
	a.insert(1, "One");
	a.insert(9, "Nine");

	// -- //

	a.set_all("All");

	for (int i = 0; i < 10; ++i) {
		assert(a[i] == "All");
	}
}

// prec: backets_operator
void test_array_reallocate() {
	tf::array<std::string> a(10, false);
	a[1] = "One";
	a[3] = "Three";
	a[9] = "Nine";

	// -- //

	a.reallocate(5);
	assert(a.capacity() == 5);
	assert(a[1] == "One");
	assert(a[3] == "Three");

	try {
		a[9];
		assert(false);
	} catch (tf::exception &) {}
	
	a.reallocate(50);
	assert(a.capacity() == 50);
	assert(a[1] == "One");
	assert(a[3] == "Three");

	try {
		a[49];
		a[50];
		assert(false);
	} catch (tf::exception &) {}
}
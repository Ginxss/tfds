#include <cassert>
#include <iostream>
#include "../../tfds/tf_linked_list.hpp"

void test_list();
void test_list_default_constructor();
void test_list_add_back();
void test_list_pop_back();
void test_list_copy_constructor();
void test_list_swap();
void test_list_move_constructor();
void test_list_copy_assignment();
void test_list_add_front();
void test_list_add_after();
void test_list_add_before();
void test_list_add_back_all();
void test_list_back();
void test_list_front();
void test_list_pop_front();
void test_list_contains();
void test_list_remove();
void test_list_iteration();
void test_list_empty();
void test_list_clear();


/* int main(int argc, char *argv[]) {
	test_list();

	return 0;
} */

void test_list() {
	test_list_default_constructor();
	test_list_add_back();
	test_list_pop_back();
	test_list_copy_constructor();
	test_list_swap();
	test_list_move_constructor();
	test_list_copy_assignment();
	test_list_add_front();
	test_list_add_after();
	test_list_add_before();
	test_list_add_back_all();
	test_list_back();
	test_list_front();
	test_list_pop_front();
	test_list_contains();
	test_list_remove();
	test_list_iteration();
	test_list_empty();
	test_list_clear();

	std::cout << "LINKED LIST tests successful." << std::endl;
}

// prec: -
void test_list_default_constructor() {
	tf::linked_list<std::string> l;
	assert(l.length() == 0);
}

// prec: default_constructor
void test_list_add_back() {
	tf::linked_list<std::string> l;

	// -- //

	l.add_back("One");
	assert(l.length() == 1);

	l.add_back("Two");
	assert(l.length() == 2);

	l.add_back("Three");
	assert(l.length() == 3);

	l.add_back("Four");
	assert(l.length() == 4);
}

// prec: add_back
void test_list_pop_back() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");
	l.add_back("Five");

	// -- //

	assert(l.pop_back() == "Five");
	assert(l.length() == 4);
	assert(l.pop_back() == "Four");
	assert(l.length() == 3);
	assert(l.pop_back() == "Three");
	assert(l.length() == 2);
	assert(l.pop_back() == "Two");
	assert(l.length() == 1);
	assert(l.pop_back() == "One");
	assert(l.length() == 0);

	try {
		l.pop_back();
		assert(false);
	} catch (tf::exception &) {}
}

// prec: pop_back
void test_list_copy_constructor() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");
	l.add_back("Five");

	// -- //

	tf::linked_list<std::string> l2(l);
	assert(l2.length() == 5);
	assert(l2.pop_back() == "Five");
	assert(l2.pop_back() == "Four");
	assert(l2.pop_back() == "Three");
	assert(l2.pop_back() == "Two");
	assert(l2.pop_back() == "One");

	assert(l2.length() == 0);
	assert(l.length() == 5);
}

// prec: pop_back
void test_list_swap() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");

	tf::linked_list<std::string> l2;
	l2.add_back("Five");
	l2.add_back("Six");

	// -- //

	swap(l, l2);

	assert(l.length() == 2);
	assert(l.pop_back() == "Six");
	assert(l.pop_back() == "Five");

	assert(l2.length() == 4);
	assert(l2.pop_back() == "Four");
	assert(l2.pop_back() == "Three");
	assert(l2.pop_back() == "Two");
	assert(l2.pop_back() == "One");
}

// prec: pop_back
void test_list_move_constructor() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");

	// -- //

	tf::linked_list<std::string> l2(std::move(l));
	assert(l2.length() == 4);
	assert(l2.pop_back() == "Four");
	assert(l2.pop_back() == "Three");
	assert(l2.pop_back() == "Two");
	assert(l2.pop_back() == "One");
}

// prec: pop_back
void test_list_copy_assignment() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");

	// -- //

	tf::linked_list<std::string> l2;
	l2 = l;

	assert(l2.length() == 4);
	assert(l2.pop_back() == "Four");
	assert(l2.pop_back() == "Three");
	assert(l2.pop_back() == "Two");
	assert(l2.pop_back() == "One");
}

// prec: pop_back
void test_list_add_front() {
	tf::linked_list<std::string> l;

	// -- //

	l.add_front("One");
	assert(l.length() == 1);
	
	l.add_front("Two");
	assert(l.length() == 2);

	l.add_front("Three");
	assert(l.length() == 3);

	l.add_front("Four");
	assert(l.length() == 4);

	assert(l.pop_back() == "One");
	assert(l.pop_back() == "Two");
	assert(l.pop_back() == "Three");
	assert(l.pop_back() == "Four");
}

// prec: pop_back
void test_list_add_after() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");

	tf::linked_list<std::string> l2;
	l2.add_back("One");
	l2.add_back("Two");
	l2.add_back("Three");

	tf::linked_list<std::string> l3;
	l3.add_back("One");
	l3.add_back("Two");
	l3.add_back("Three");

	// -- //

	l.add_after("Four", "One");
	assert(l.length() == 4);
	assert(l.pop_back() == "Three");
	assert(l.pop_back() == "Two");
	assert(l.pop_back() == "Four");
	assert(l.pop_back() == "One");

	l2.add_after("Four", "Two");
	assert(l2.length() == 4);
	assert(l2.pop_back() == "Three");
	assert(l2.pop_back() == "Four");
	assert(l2.pop_back() == "Two");
	assert(l2.pop_back() == "One");

	l3.add_after("Four", "Three");
	assert(l3.length() == 4);
	assert(l3.pop_back() == "Four");
	assert(l3.pop_back() == "Three");
	assert(l3.pop_back() == "Two");
	assert(l3.pop_back() == "One");
}

// prec: pop_back
void test_list_add_before() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");

	tf::linked_list<std::string> l2;
	l2.add_back("One");
	l2.add_back("Two");
	l2.add_back("Three");

	tf::linked_list<std::string> l3;
	l3.add_back("One");
	l3.add_back("Two");
	l3.add_back("Three");

	// -- //

	l.add_before("Four", "One");
	assert(l.length() == 4);
	assert(l.pop_back() == "Three");
	assert(l.pop_back() == "Two");
	assert(l.pop_back() == "One");
	assert(l.pop_back() == "Four");

	l2.add_before("Four", "Two");
	assert(l2.length() == 4);
	assert(l2.pop_back() == "Three");
	assert(l2.pop_back() == "Two");
	assert(l2.pop_back() == "Four");
	assert(l2.pop_back() == "One");

	l3.add_before("Four", "Three");
	assert(l3.length() == 4);
	assert(l3.pop_back() == "Three");
	assert(l3.pop_back() == "Four");
	assert(l3.pop_back() == "Two");
	assert(l3.pop_back() == "One");
}

// prec: pop_back
void test_list_add_back_all() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");

	tf::linked_list<std::string> l2;
	l2.add_back("Three");
	l2.add_back("Four");
	l2.add_back("Five");

	// -- //

	l.add_back_all(l2);

	assert(l.length() == 5);
	assert(l.pop_back() == "Five");
	assert(l.pop_back() == "Four");
	assert(l.pop_back() == "Three");
	assert(l.pop_back() == "Two");
	assert(l.pop_back() == "One");
}

// prec: add_back(), add_front()
void test_list_back() {
	tf::linked_list<std::string> l;

	// -- //

	try {
		l.back();
		assert(false);
	} catch (tf::exception &) {}

	l.add_back("One");
	assert(l.back() == "One");

	l.add_front("Zero");
	assert(l.back() == "One");

	l.add_back("Two");
	assert(l.back() == "Two");

	l.add_back("Three");
	assert(l.back() == "Three");
}

// prec: add_back(), add_front()
void test_list_front() {
	tf::linked_list<std::string> l;

	// -- //

	try {
		l.front();
		assert(false);
	} catch (tf::exception &) {}

	l.add_front("One");
	assert(l.front() == "One");

	l.add_back("Zero");
	assert(l.front() == "One");

	l.add_front("Two");
	assert(l.front() == "Two");

	l.add_front("Three");
	assert(l.front() == "Three");
}

// prec: add_back
void test_list_pop_front() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");
	l.add_back("Five");

	// -- //

	assert(l.pop_front() == "One");
	assert(l.length() == 4);
	assert(l.pop_front() == "Two");
	assert(l.length() == 3);
	assert(l.pop_front() == "Three");
	assert(l.length() == 2);
	assert(l.pop_front() == "Four");
	assert(l.length() == 1);
	assert(l.pop_front() == "Five");
	assert(l.length() == 0);

	try {
		l.pop_front();
		assert(false);
	} catch (tf::exception &) {}
}

// prec: add_back
void test_list_contains() {
	tf::linked_list<std::string> l;

	// -- //

	assert(l.contains("One") == false);
	l.add_back("One");
	assert(l.contains("One") == true);

	assert(l.contains("Two") == false);
	l.add_back("Two");
	assert(l.contains("One") == true);
	assert(l.contains("Two") == true);

	assert(l.contains("Three") == false);
	l.add_back("Three");
	assert(l.contains("One") == true);
	assert(l.contains("Two") == true);
	assert(l.contains("Three") == true);

	assert(l.contains("Four") == false);
	l.add_back("Four");
	assert(l.contains("One") == true);
	assert(l.contains("Two") == true);
	assert(l.contains("Three") == true);
	assert(l.contains("Four") == true);
}

// prec: contains
void test_list_remove() {
	tf::linked_list<std::string> l;
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");
	l.add_back("Five");

	// -- //

	try {
		l.remove("Not there");
		assert(false);
	} catch (tf::exception &) {}

	l.remove("Three");
	assert(l.contains("Three") == false);
	assert(l.length() == 4);
	assert(l.front() == "One");
	assert(l.back() == "Five");

	l.remove("Two");
	assert(l.contains("Two") == false);
	assert(l.length() == 3);
	assert(l.front() == "One");
	assert(l.back() == "Five");

	l.remove("Five");
	assert(l.contains("Five") == false);
	assert(l.length() == 2);
	assert(l.front() == "One");
	assert(l.back() == "Four");

	l.remove("One");
	assert(l.contains("One") == false);
	assert(l.length() == 1);
	assert(l.front() == "Four");
	assert(l.back() == "Four");

	l.remove("Four");
	assert(l.length() == 0);

	try {
		l.front();
		assert(false);
	} catch (tf::exception &) {}
}

// prec: add_back
void test_list_iteration() {
	tf::linked_list<std::string> l;
	l.add_back("Zero");
	l.add_back("One");
	l.add_back("Two");
	l.add_back("Three");
	l.add_back("Four");

	const tf::linked_list<std::string> l2(l);

	// -- //

	int i = 0;
	for (auto it = l.begin(); it.condition(); ++it) {
		switch (i) {
		case 0: assert(*it == "Zero"); break;
		case 1: assert(*it == "One"); break;
		case 2: assert(*it == "Two"); break;
		case 3: assert(*it == "Three"); break;
		case 4: assert(*it == "Four"); break;
		}
		++i;
	}
	assert(i == 5);

	for (auto it = l.end(); it.condition(); --it) {
		--i;
		switch (i) {
		case 4: assert(it.value() == "Four"); break;
		case 3: assert(it.value() == "Three"); break;
		case 2: assert(it.value() == "Two"); break;
		case 1: assert(it.value() == "One"); break;
		case 0:
			assert(it.value() == "Zero");
			it.value() = "New Zero";
			break;
		}
	}
	assert(i == 0);

	for (auto it = l.begin(); it.condition(); --it) {
		assert(*it == "New Zero");
	}
	for (auto it = l.end(); it.condition(); ++it) {
		assert(*it == "Four");
	}


	for (auto it = l2.begin(); it.condition(); ++it) {
		switch (i) {
		case 0: assert(*it == "Zero"); break;
		case 1: assert(*it == "One"); break;
		case 2: assert(*it == "Two"); break;
		case 3: assert(*it == "Three"); break;
		case 4: assert(*it == "Four"); break;
		}
		++i;
	}
	assert(i == 5);

	for (auto it = l2.end(); it.condition(); --it) {
		--i;
		switch (i) {
		case 4: assert(it.value() == "Four"); break;
		case 3: assert(it.value() == "Three"); break;
		case 2: assert(it.value() == "Two"); break;
		case 1: assert(it.value() == "One"); break;
		case 0:
			assert(it.value() == "Zero");
			// it.value() = "New Zero";
			break;
		}
	}
	assert(i == 0);

	for (auto it = l2.begin(); it.condition(); --it) {
		assert(*it == "Zero");
	}
	for (auto it = l2.end(); it.condition(); ++it) {
		assert(*it == "Four");
	}
}

// prec: remove
void test_list_empty() {
	tf::linked_list<std::string> l;

	// -- //

	assert(l.empty() == true);

	l.add_front("One");
	assert(l.empty() == false);
	l.add_back("Two");
	assert(l.empty() == false);

	l.remove("One");
	assert(l.empty() == false);
	l.remove("Two");
	assert(l.empty() == true);
}

// prec: empty
void test_list_clear() {
	tf::linked_list<std::string> l;
	l.add_back("Zero");
	l.add_back("One");
	l.add_back("Two");
	
	// -- //

	l.clear();
	assert(l.length() == 0);
	assert(l.empty() == true);

	try {
		l.front();
		assert(false);
	} catch (tf::exception &) {}
}
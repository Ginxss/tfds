#include <cassert>
#include <iostream>
#include "../tfds/tf_linked_list.hpp"

void print_list_info(const tf::linked_list<std::string> &l, const std::string &message = "Linked List info") {
	std::cout << message << ":" << std::endl << std::endl;
	std::cout << "Length: " << l.length() << std::endl;
	std::cout << "Empty: " << l.empty() << std::endl;
	
	std::cout << "Content: ";
	for (auto it = l.begin(); it.condition(); ++it) {
		std::cout << *it << " | ";
	}

	std::cout << std::endl << std::endl << "----------------------------------------" << std::endl << std::endl;
}

tf::linked_list<std::string> test_construction() {
	tf::linked_list<std::string> l1;
	assert(l1.length() == 0);
	assert(l1.empty() == true);

	return l1;
}

void test_add_back_and_back(tf::linked_list<std::string> &l) {
	assert(l.length() == 0);
	assert(l.empty() == true);

	l.add_back("three");
	assert(l.back() == "three");

	l.add_back("four");
	assert(l.back() == "four");
}

void test_add_front_and_front(tf::linked_list<std::string> &l) {
	assert(l.length() == 2);
	assert(l.empty() == false);

	l.add_front("two");
	assert(l.front() == "two");

	l.add_front("one");
	assert(l.front() == "one");
}

void test_add_after(tf::linked_list<std::string> &l) {
	assert(l.length() == 4);
	assert(l.empty() == false);

	l.add_after("after_two", "two");
	assert(l.length() == 5);

	l.add_after("end", "four");
	assert(l.length() == 6);
	assert(l.back() == "end");
}

void test_add_before(tf::linked_list<std::string> &l) {
	assert(l.length() == 6);
	assert(l.empty() == false);

	l.add_before("before_three", "three");
	assert(l.length() == 7);

	l.add_before("start", "one");
	assert(l.length() == 8);
	assert(l.front() == "start");
}

void test_iterator(const tf::linked_list<std::string> &l) {
	assert(l.length() == 8);
	assert(l.empty() == false);

	int i = 0;
	for (auto it = l.begin(); it.condition(); ++it) {
		switch (i) {
		case 0: assert(*it == "start"); break;
		case 1: assert(*it == "one"); break;
		case 2: assert(*it == "two"); break;
		case 3: assert(*it == "after_two"); break;
		case 4: assert(*it == "before_three"); break;
		case 5: assert(*it == "three"); break;
		case 6: assert(*it == "four"); break;
		case 7: assert(*it == "end"); break;
		default: assert(false);
		}
		i++;
	}
	assert(i == 8);

	for (auto it = l.end(); it.condition(); --it) {
		i--;
		switch (i) {
		case 0: assert(*it == "start"); break;
		case 1: assert(*it == "one"); break;
		case 2: assert(*it == "two"); break;
		case 3: assert(*it == "after_two"); break;
		case 4: assert(*it == "before_three"); break;
		case 5: assert(*it == "three"); break;
		case 6: assert(*it == "four"); break;
		case 7: assert(*it == "end"); break;
		default: assert(false);
		}
	}
	assert(i == 0);
}

void test_remove(tf::linked_list<std::string> &l) {
	assert(l.length() == 8);
	assert(l.empty() == false);

	l.remove("before_three");
	assert(l.length() == 7);

	l.remove("end");
	assert(l.length() == 6);
	assert(l.back() == "four");

	l.remove("start");
	assert(l.length() == 5);
	assert(l.front() == "one");

	int i = 0;
	for (auto it = l.begin(); it.condition(); ++it) {
		switch (i) {
		case 0: assert(*it == "one"); break;
		case 1: assert(*it == "two"); break;
		case 2: assert(*it == "after_two"); break;
		case 3: assert(*it == "three"); break;
		case 4: assert(it.value() == "four"); break;
		default: assert(false);
		}
		i++;
	}
}

void test_pop_back(tf::linked_list<std::string> &l) {
	assert(l.length() == 5);
	assert(l.empty() == false);

	assert(l.pop_back() == "four");
	assert(l.length() == 4);

	int i = 0;
	for (auto it = l.begin(); it.condition(); ++it) {
		switch (i) {
		case 0: assert(*it == "one"); break;
		case 1: assert(*it == "two"); break;
		case 2: assert(*it == "after_two"); break;
		case 3: assert(it.value() == "three"); break;
		default: assert(false);
		}
		i++;
	}
}

void test_pop_front(tf::linked_list<std::string> &l) {
	assert(l.length() == 4);
	assert(l.empty() == false);

	assert(l.pop_front() == "one");
	assert(l.length() == 3);

	int i = 0;
	for (auto it = l.begin(); it.condition(); ++it) {
		switch (i) {
		case 0: assert(*it == "two"); break;
		case 1: assert(*it == "after_two"); break;
		case 2: assert(it.value() == "three"); break;
		default: assert(false);
		}
		i++;
	}
}

void test_contains(const tf::linked_list<std::string> &l) {
	assert(l.length() == 3);
	assert(l.empty() == false);

	assert(l.contains("one") == false);
	assert(l.contains("two") == true);

	for (int i = 0; i < l.length(); ++i) {
		switch (i) {
		case 0: assert(l.contains("two")); break;
		case 1: assert(l.contains("after_two")); break;
		case 2: assert(l.contains("three")); break;
		default: assert(false);
		}
	}
}

void test_copying(const tf::linked_list<std::string> &l) {
	assert(l.length() == 3);
	assert(l.empty() == false);
	assert(l.front() == "two");
	assert(l.back() == "three");

	const tf::linked_list<std::string> l2 = l;
	assert(l2.length() == 3);
	assert(l2.empty() == false);
	assert(l2.front() == "two");
	assert(l2.back() == "three");

	tf::linked_list<std::string> l3;
	l3 = l;
	assert(l3.length() == 3);
	assert(l3.empty() == false);
	assert(l3.front() == "two");
	assert(l3.back() == "three");

	tf::linked_list<std::string> l4(std::move(l3));
	assert(l4.length() == 3);
	assert(l4.empty() == false);
	assert(l4.front() == "two");
	assert(l4.back() == "three");

	assert(l3.length() == 0);
	assert(l3.empty() == true);
}

void test_clear(tf::linked_list<std::string> &l) {
	assert(l.length() == 3);
	assert(l.empty() == false);

	l.clear();
	assert(l.length() == 0);
	assert(l.empty() == true);
}

int main(int argc, char *argv[]) {
	try {
		tf::linked_list<std::string> result = test_construction();
		test_add_back_and_back(result);
		test_add_front_and_front(result);
		test_add_after(result);
		test_add_before(result);
		test_iterator(result);
		test_remove(result);
		test_pop_back(result);
		test_pop_front(result);
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
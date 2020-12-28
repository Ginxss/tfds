#include <cassert>
#include <iostream>
#include "../tfds/tf_linked_list.hpp"

// For manual testing
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

int main(int argc, char *argv[]) {
	try {
		// construction
		tf::linked_list<std::string> l;
		assert(l.empty() == true);
		assert(l.length() == 0);

		// add, front and back
		try {
			l.front();
			assert(false);
			l.back();
			assert(false);
		}
		catch (tf::exception &) {
			assert(true);
		}

		l.add_back("four");
		assert(l.length() == 1);
		assert(l.empty() == false);

		l.add_back("five");
		l.add_front("one");
		l.add_front("zero");
		l.add_after("two", "one");
		l.add_before("three", "four");
		assert(l.length() == 6);
		assert(l.front() == "zero");
		assert(l.back() == "five");

		// iteration
		auto it = l.begin();
		assert(it.condition() == true);
		assert(*it == "zero");
		++it;
		assert(it.condition() == true);
		assert(*it == "one");
		++it;
		assert(it.condition() == true);
		assert(*it == "two");
		++it;
		assert(it.condition() == true);
		assert(*it == "three");
		++it;
		assert(it.condition() == true);
		assert(*it == "four");
		++it;
		assert(it.condition() == true);
		assert(*it == "five");
		++it;
		assert(it.condition() == false);

		auto it2 = l.end();
		assert(it2.condition() == true);
		assert(*it2 == "five");
		--it2;
		assert(it2.condition() == true);
		assert(*it2 == "four");
		--it2;
		assert(it2.condition() == true);
		assert(*it2 == "three");
		--it2;
		assert(it2.condition() == true);
		assert(*it2 == "two");
		--it2;
		assert(it2.condition() == true);
		assert(*it2 == "one");
		--it2;
		assert(it2.condition() == true);
		assert(*it2 == "zero");
		--it2;
		assert(it2.condition() == false);

		// contains and remove
		assert(l.contains("two") == true);
		assert(l.contains("twee") == false);

		l.remove("two");
		assert(l.contains("two") == false);

		l.remove("zero");
		assert(l.front() == "one");

		l.remove("five");
		assert(l.back() == "four");
		assert(l.length() == 3);
		assert(l.empty() == false);

		auto it3 = l.begin();
		assert(it3.condition() == true);
		assert(*it3 == "one");
		++it3;
		assert(it3.condition() == true);
		assert(*it3 == "three");
		++it3;
		assert(it3.condition() == true);
		assert(*it3 == "four");
		++it3;
		assert(it3.condition() == false);

		// pop, front and back
		assert(l.pop_front() == "one");
		assert(l.pop_back() == "four");
		assert(l.front() == "three");
		assert(l.back() == "three");

		// clear
		assert(l.length() == 1);
		assert(l.empty() == false);

		l.clear();
		assert(l.length() == 0);
		assert(l.empty() == true);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
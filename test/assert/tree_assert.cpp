#include <cassert>
#include <iostream>
#include "../../tfds/tf_search_tree.hpp"

void print_tree_info(const tf::search_tree<int, std::string> &t, const std::string &message = "Search Tree info") {
	std::cout << message << ":" << std::endl << std::endl;
	std::cout << "Size: " << t.size() << std::endl;
	std::cout << "Height: " << t.height() << std::endl;
	std::cout << "Empty: " << t.empty() << std::endl;
	std::cout << "Allows duplicate keys: " << t.allows_duplicate_keys() << std::endl;
	
	// std::cout << "Content: ";
	// t.print();

	std::cout << std::endl << "----------------------------------------" << std::endl << std::endl;
}

void print_tree_assert() {
	try {
		// construction
		tf::search_tree<int, std::string> t;
		tf::search_tree<int, std::string> t2(true);
		
		assert(t.size() == 0);
		assert(t.height() == 0);
		assert(t.empty() == true);
		assert(t.allows_duplicate_keys() == false);
		assert(t2.size() == 0);
		assert(t2.height() == 0);
		assert(t2.empty() == true);
		assert(t2.allows_duplicate_keys() == true);

		// insert and get
		t2.insert(0, "zero");
		t2.insert(0, "zero2");
		t2.insert(0, "zero3");
		assert(t2.height() == 1);
		t2.insert(1, "one");
		t2.insert(1, "one2");
		t2.insert(10, "ten");
		t2.insert(10, "ten2");
		t2.insert(10, "ten3");
		assert(t2.size() == 8);
		assert(t2.height() == 2);

		t.insert(0, "zero");
		assert(t.size() == 1);
		assert(t.empty() == false);
		assert(t.get(0) == "zero" && t[0] == "zero");
		try {
			t.insert(0, "zero");
			assert(false);
		}
		catch (tf::exception &) {
			assert(true);
		}

		t.insert(1, "one");
		assert(t.get(0) == "zero" && t[0] == "zero");
		assert(t.get(1) == "one" && t[1] == "one");

		try {
			t.get(2);
			assert(false);
			t[2];
			assert(false);
		}
		catch (tf::exception &) {
			assert(true);
		}

		t.insert(2, "two");
		assert(t.get(0) == "zero" && t[0] == "zero");
		assert(t.get(1) == "one" && t[1] == "one");
		assert(t.get(2) == "two" && t[2] == "two");

		t.insert(3, "three");
		assert(t.get(0) == "zero" && t[0] == "zero");
		assert(t.get(1) == "one" && t[1] == "one");
		assert(t.get(2) == "two" && t[2] == "two");
		assert(t.get(3) == "three" && t[3] == "three");

		t.insert(-30, "-thirty");
		assert(t.get(0) == "zero" && t[0] == "zero");
		assert(t.get(1) == "one" && t[1] == "one");
		assert(t.get(2) == "two" && t[2] == "two");
		assert(t.get(3) == "three" && t[3] == "three");
		assert(t.get(-30) == "-thirty" && t[-30] == "-thirty");

		t.insert(101, "hundredone");
		assert(t.get(0) == "zero" && t[0] == "zero");
		assert(t.get(1) == "one" && t[1] == "one");
		assert(t.get(2) == "two" && t[2] == "two");
		assert(t.get(3) == "three" && t[3] == "three");
		assert(t.get(-30) == "-thirty" && t[-30] == "-thirty");
		assert(t.get(101) == "hundredone" && t[101] == "hundredone");

		assert(t.size() == 6);
		assert(t.height() == 3);

		// iteration
		auto it = t.begin();
		assert(it.condition() == true);
		assert(it.key() == -30 && it.value() == "-thirty");
		++it;
		assert(it.condition() == true);
		assert(it.key() == 0 && it.value() == "zero");
		++it;
		assert(it.condition() == true);
		assert(it.key() == 1 && it.value() == "one");
		++it;
		assert(it.condition() == true);
		assert(it.key() == 2 && it.value() == "two");
		++it;
		assert(it.condition() == true);
		assert(it.key() == 3 && it.value() == "three");
		++it;
		assert(it.condition() == true);
		assert(it.key() == 101 && it.value() == "hundredone");
		++it;
		assert(it.condition() == false);


		auto it2 = t2.begin();
		assert(it2.condition() == true);
		assert(it2.key() == 0 && (*it2 == "zero" || *it2 == "zero2" || *it2 == "zero3"));
		++it2;
		assert(it2.condition() == true);
		assert(it2.key() == 0 && (*it2 == "zero" || *it2 == "zero2" || *it2 == "zero3"));
		++it2;
		assert(it2.condition() == true);
		assert(it2.key() == 0 && (*it2 == "zero" || *it2 == "zero2" || *it2 == "zero3"));
		++it2;
		assert(it2.condition() == true);
		assert(it2.key() == 1 && (*it2 == "one" || *it2 == "one2"));
		++it2;
		assert(it2.condition() == true);
		assert(it2.key() == 1 && (*it2 == "one" || *it2 == "one2"));
		++it2;
		assert(it2.condition() == true);
		assert(it2.key() == 10 && (*it2 == "ten" || *it2 == "ten2" || *it2 == "ten3"));
		++it2;
		assert(it2.condition() == true);
		assert(it2.key() == 10 && (*it2 == "ten" || *it2 == "ten2" || *it2 == "ten3"));
		++it2;
		assert(it2.condition() == true);
		assert(it2.key() == 10 && (*it2 == "ten" || *it2 == "ten2" || *it2 == "ten3"));
		++it2;
		assert(it2.condition() == false);

		// contains and remove
		assert(t.contains(2) == true);
		assert(t.contains(12) == false);
		
		assert(t.remove(2) == "two");
		assert(t.contains(2) == false);

		assert(t2.remove_value(0, "zero2") == "zero2");
		std::string removed = t2.remove_all(10);
		assert(removed == "ten" || removed == "ten2" || removed == "ten3");
		std::string removed2 = t2.remove(1);
		assert(removed2 == "one" || removed2 == "one2");
		std::string removed3 = t2.remove(1);
		assert(removed3 == "one" || removed3 == "one2");
		try {
			std::string nothing = t2.remove(1);
			assert(false);
		}
		catch (tf::exception &) {
			assert(true);
		}

		// pop, min and max
		assert(t.min() == "-thirty");
		assert(t.max() == "hundredone");
		assert(t.min() == "-thirty");
		assert(t.max() == "hundredone");
		assert(t.pop_min() == "-thirty");
		assert(t.pop_max() == "hundredone");
		assert(t.min() == "zero");
		assert(t.max() == "three");
		assert(t.size() == 3);

		// clear
		assert(t.size() == 3);
		assert(t.height() == 2);
		assert(t.empty() == false);

		t.clear();
		assert(t.size() == 0);
		assert(t.height() == 0);
		assert(t.empty() == true);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "All tests successful." << std::endl;
}

/* int main(int argc, char *argv[]) {
	print_tree_assert();

	return 0;
} */
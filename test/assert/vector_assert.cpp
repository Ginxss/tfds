#include <cassert>
#include <iostream>
#include "../../tfds/tf_vector.hpp"

// For manual testing
void print_vector_info(const tf::vector<std::string> &v, const std::string &message = "Array info") {
	std::cout << message << ":" << std::endl << std::endl;
	std::cout << "Size: " << v.size() << std::endl;
	std::cout << "Empty: " << v.empty() << std::endl;
	std::cout << "Capacity: " << v.capacity() << std::endl;

	std::cout << "Content: ";
	for (int i = 0; i < v.size(); ++i) {
		std::cout << v.get(i) << " | ";
	}
	
	std::cout << std::endl << std::endl << "----------------------------------------" << std::endl << std::endl;
}

int main(int argc, char *argv[]) {
	try {
		// construction
		tf::vector<std::string> v;
		tf::vector<std::string> v2(1000);
		tf::vector<std::string> v3(0);

		assert(v.size() == 0);
		assert(v.empty() == true);
		assert(v.capacity() == 10);
		assert(v2.size() == 0);
		assert(v2.empty() == true);
		assert(v2.capacity() == 1000);
		assert(v3.size() == 0);
		assert(v3.empty() == true);
		assert(v3.capacity() == 1);

		// add and get
		v.add("zero");
		assert(v.size() == 1);
		assert(v.empty() == false);
		assert(v.get(0) == "zero" && v[0] == "zero");

		v.add("one");
		assert(v.get(0) == "zero" && v[0] == "zero");
		assert(v.get(1) == "one" && v[1] == "one");

		v.add("two");
		assert(v.size() == 3);
		assert(v.capacity() == 10);
		assert(v.get(0) == "zero" && v[0] == "zero");
		assert(v.get(1) == "one" && v[1] == "one");
		assert(v.get(2) == "two" && v[2] == "two");
		try {
			v.get(3);
			assert(false);
		}
		catch (tf::exception &) {
			assert(true);
		}

		// reallocation
		v.add("three");
		v.add("four");
		v.add("five");
		v.add("six");
		v.add("seven");
		v.add("eight");
		v.add("nine");
		assert(v.size() == 10);
		assert(v.capacity() == 10);

		v.add("ten");
		assert(v.size() == 11);
		assert(v.capacity() == 20);
		assert(v.get(0) == "zero" && v[0] == "zero");
		assert(v.get(1) == "one" && v[1] == "one");
		assert(v.get(2) == "two" && v[2] == "two");
		assert(v.get(10) == "ten" && v[10] == "ten");

		v.reallocate(10000);
		assert(v.size() == 11);
		assert(v.capacity() == 10000);
		assert(v.get(0) == "zero" && v[0] == "zero");
		assert(v.get(1) == "one" && v[1] == "one");
		assert(v.get(2) == "two" && v[2] == "two");
		assert(v.get(10) == "ten" && v[10] == "ten");

		v.reallocate(20);
		assert(v.size() == 11);
		assert(v.capacity() == 20);
		assert(v.get(0) == "zero" && v[0] == "zero");
		assert(v.get(1) == "one" && v[1] == "one");
		assert(v.get(2) == "two" && v[2] == "two");
		assert(v.get(10) == "ten" && v[10] == "ten");

		// contains and remove
		assert(v.contains("two") == true);
		assert(v.contains("twee") == false);
		assert(v.remove(2) == "two");
		assert(v.contains("two") == false);

		// set all
		v.set_all("blah");
		for (int i = 0; i < v.size(); ++i) {
			assert(v.get(i) == "blah" && v[i] == "blah");
		}

		// clear
		assert(v.size() == 10);
		assert(v.empty() == false);
		assert(v.capacity() == 20);
		
		v.clear();
		assert(v.size() == 0);
		assert(v.empty() == true);
		assert(v.capacity() == 20);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
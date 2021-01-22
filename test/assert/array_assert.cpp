#include <cassert>
#include <iostream>
#include "../../tfds/tf_array.hpp"

// For manual testing
void print_array_info(const tf::array<std::string> &a, const std::string &message = "Array info") {
	std::cout << message << ":" << std::endl << std::endl;
	std::cout << "Capacity: " << a.capacity() << std::endl;
	std::cout << "Auto-Reallocating: " << a.auto_reallocating() << std::endl;

	std::cout << "Content: ";
	for (int i = 0; i < a.capacity(); ++i) {
		std::cout << a.get(i) << " | ";
	}
	
	std::cout << std::endl << std::endl << "----------------------------------------" << std::endl << std::endl;
}

int main(int argc, char *argv[]) {
	try {
		// construction
		tf::array<std::string> a;
		tf::array<std::string> a2(1000);
		tf::array<std::string> a3(10000, false);
		tf::array<std::string> a4(0, false);

		assert(a.capacity() == 10);
		assert(a.auto_reallocating() == true);
		assert(a2.capacity() == 1000);
		assert(a2.auto_reallocating() == true);
		assert(a3.capacity() == 10000);
		assert(a3.auto_reallocating() == false);
		assert(a4.capacity() == 1);
		assert(a4.auto_reallocating() == false);

		// insert, get and automatic reallocation
		a.insert(0, "zero");
		assert(a.capacity() == 10);
		assert(a.get(0) == "zero" && a[0] == "zero");

		a.insert(1001, "thousandone");
		assert(a.capacity() == 1010);
		assert(a.get(1001) == "thousandone" && a[1001] == "thousandone");

		a[10] = "ten";
		assert(a.capacity() == 1010);
		assert(a.get(10) == "ten" && a[10] == "ten");

		a[1999] = "ninenine";
		assert(a.capacity() == 2020);
		assert(a.get(1999) == "ninenine" && a[1999] == "ninenine");

		// reallocate
		a.reallocate(10000);
		assert(a.capacity() == 10000);
		assert(a.get(0) == "zero" && a[0] == "zero");
		assert(a.get(10) == "ten" && a[10] == "ten");
		assert(a.get(1001) == "thousandone" && a[1001] == "thousandone");
		assert(a.get(1999) == "ninenine" && a[1999] == "ninenine");

		a.reallocate(100);
		assert(a.capacity() == 100);
		assert(a.get(0) == "zero" && a[0] == "zero");
		assert(a.get(10) == "ten" && a[10] == "ten");
		try {
			a.get(1001);
			assert(false);
			a.get(1999);
			assert(false);
		}
		catch (tf::exception &) {
			assert(true);
		}

		// set all
		a.set_all("blah");
		for (int i = 0; i < a.capacity(); ++i) {
			assert(a.get(i) == "blah" && a[i] == "blah");
		}
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
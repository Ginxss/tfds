#include <cassert>
#include <iostream>
#include "../tfds/tf_stack.hpp"

// Helper function for manual testing
void print_stack_info(const tf::stack<std::string> &s, const std::string &message = "Stack info") {
	std::cout << message << ":" << std::endl << std::endl;
	std::cout << "Size: " << s.size() << std::endl;
	std::cout << "Empty: " << s.empty() << std::endl;
	std::cout << "Capacity: " << s.current_capacity() << std::endl;

	tf::stack<std::string> s2(s);
	std::cout << "Content: ";
	while (!s2.empty()) {
		std::cout << s2.pop() << " | ";
	}

	std::cout << std::endl << std::endl << "----------------------------------------" << std::endl << std::endl;
}

tf::stack<std::string> test_construction() {
	tf::stack<std::string> s1;
	assert(s1.size() == 0);
	assert(s1.empty() == true);
	assert(s1.current_capacity() == 10);

	tf::stack<std::string> s2(2);
	assert(s2.size() == 0);
	assert(s2.empty() == true);
	assert(s2.current_capacity() == 2);

	return s2;
}

void test_put_peek_pop(tf::stack<std::string> &s) {
	assert(s.size() == 0);
	assert(s.empty() == true);
	assert(s.current_capacity() == 2);

	s.put("one");
	s.put("two");
	assert(s.size() == 2);
	assert(s.empty() == false);
	assert(s.current_capacity() == 2);

	assert(s.peek() == "two");
	assert(s.peek() == "two");

	assert(s.pop() == "two");
	assert(s.size() == 1);
	assert(s.empty() == false);
	assert(s.current_capacity() == 2);

	assert(s.pop() == "one");
	assert(s.size() == 0);
	assert(s.empty() == true);
	assert(s.current_capacity() == 2);
}

void test_reallocation(tf::stack<std::string> &s) {
	assert(s.size() == 0);
	assert(s.empty() == true);
	assert(s.current_capacity() == 2);

	s.put("one");
	s.put("two");
	s.put("three");
	assert(s.size() == 3);
	assert(s.empty() == false);
	assert(s.current_capacity() == 4);
	assert(s.peek() == "three");
}

void test_copying(const tf::stack<std::string> &s) {
	assert(s.size() == 3);
	assert(s.empty() == false);
	assert(s.current_capacity() == 4);
	assert(s.peek() == "three");

	const tf::stack<std::string> s2 = s;
	assert(s2.size() == 3);
	assert(s2.empty() == false);
	assert(s2.current_capacity() == 4);
	assert(s2.peek() == "three");

	tf::stack<std::string> s3;
	s3 = s;
	assert(s3.size() == 3);
	assert(s3.empty() == false);
	assert(s3.current_capacity() == 4);
	assert(s3.peek() == "three");

	const tf::stack<std::string> s4(std::move(s3));
	assert(s4.size() == 3);
	assert(s4.empty() == false);
	assert(s4.current_capacity() == 4);
	assert(s4.peek() == "three");

	assert(s3.size() == 0);
	assert(s3.empty() == true);
	assert(s3.current_capacity() == 1);
}

int main(int argc, char *argv[]) {
	try {
		tf::stack<std::string> result = test_construction();
		test_put_peek_pop(result);
		test_reallocation(result);
		test_copying(result);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
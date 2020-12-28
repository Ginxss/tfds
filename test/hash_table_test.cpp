#include <cassert>
#include <iostream>
#include "../tfds/tf_hash_table.hpp"

void print_hash_table_info(const tf::hash_table<std::string, int> &h, const std::string &message = "Hash Table info") {
	std::cout << message << ":" << std::endl << std::endl;
	std::cout << "Size: " << h.size() << std::endl;
	std::cout << "Empty: " << h.empty() << std::endl;
	std::cout << "Table Size: " << h.table_size() << std::endl;
	std::cout << "Checks duplicate keys: " << h.checks_duplicate_keys() << std::endl;
	
	std::cout << "Content: ";
	for (auto it = h.begin(); it.condition(); ++it) {
		// std::cout << "(Bucket #" << it.current_index << ") " << it.key() << "=" << it.value() << " | ";
		std::cout << it.key() << "=" << it.value() << " | ";
	}

	std::cout << std::endl << std::endl << "----------------------------------------" << std::endl << std::endl;
}

int main(int argc, char *argv[]) {
	try {
		// construction
		tf::hash_table<std::string, int> h;
		tf::hash_table<std::string, int> h2(1000);
		tf::hash_table<std::string, int> h3(100000, false);
		tf::hash_table<std::string, int> h4(0);

		assert(h.size() == 0);
		assert(h.table_size() == 100);
		assert(h.empty() == true);
		assert(h.checks_duplicate_keys() == true);
		assert(h2.size() == 0);
		assert(h2.table_size() == 1000);
		assert(h2.empty() == true);
		assert(h2.checks_duplicate_keys() == true);
		assert(h3.size() == 0);
		assert(h3.table_size() == 100000);
		assert(h3.empty() == true);
		assert(h3.checks_duplicate_keys() == false);
		assert(h4.size() == 0);
		assert(h4.table_size() == 1);
		assert(h4.empty() == true);
		assert(h4.checks_duplicate_keys() == true);

		// insert and get
		h3.insert("one", 1);
		h3.insert("one", 11);
		h3.insert("one", 111);

		h.insert("one", 1);
		assert(h.size() == 1);
		assert(h.empty() == false);
		assert(h.table_size() == 100);
		assert(h.get("one") == 1 && h["one"] == 1);
		try {
			h.insert("one", 11);
			assert(false);
		}
		catch (tf::exception &) {
			assert(true);
		}

		h.insert("two", 2);
		assert(h.get("one") == 1 && h["one"] == 1);
		assert(h.get("two") == 2 && h["two"] == 2);
		
		h.insert("three", 3);
		assert(h.get("one") == 1 && h["one"] == 1);
		assert(h.get("two") == 2 && h["two"] == 2);
		assert(h.get("three") == 3 && h["three"] == 3);

		h.insert("four", 4);
		assert(h.get("one") == 1 && h["one"] == 1);
		assert(h.get("two") == 2 && h["two"] == 2);
		assert(h.get("three") == 3 && h["three"] == 3);
		assert(h.get("four") == 4 && h["four"] == 4);

		h.insert("five", 5);
		assert(h.get("one") == 1 && h["one"] == 1);
		assert(h.get("two") == 2 && h["two"] == 2);
		assert(h.get("three") == 3 && h["three"] == 3);
		assert(h.get("four") == 4 && h["four"] == 4);
		assert(h.get("five") == 5 && h["five"] == 5);

		try {
			h.get("new");
			assert(false);
			h["new"];
			assert(false);
		}
		catch (tf::exception &e) {
			assert(true);
		}

		// iteration
		int i = 0;
		for (auto it = h.begin(); it.condition(); ++it) {
			assert((it.key() == "one" && it.value() == 1) ||
				   (it.key() == "two" && it.value() == 2) ||
				   (it.key() == "three" && it.value() == 3) ||
				   (it.key() == "four" && it.value() == 4) ||
				   (it.key() == "five" && it.value() == 5));
			i++;
		}
		assert(i == 5);

		// contains and remove
		assert(h.contains("one") == true);
		assert(h.contains("ene") == false);

		assert(h.remove("one") == 1);
		assert(h.contains("one") == false);
		
		int removed = h3.remove("one");
		assert(removed == 1 || removed == 11 || removed == 111);
		assert(h3.size() == 2);

		// clear
		assert(h.size() == 4);
		assert(h.empty() == false);

		h.clear();
		assert(h.size() == 0);
		assert(h.empty() == true);
	}
	catch (tf::exception &e) {
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "All tests successful." << std::endl;

	return 0;
}
#include "vector_performance.cpp"
#include "linked_list_performance.cpp"
#include "hash_table_performance.cpp"
#include "search_tree_performance.cpp"

// Naive tfds performance measure (mostly inserting and accessing of std::strings)
int main(int argc, char *argv[]) {
	int num_elements = 1000000;
	int runs = 1000;

	print_vector_performance(num_elements, runs);
	std::cout << "******************************" << std::endl << std::endl;

	print_list_performance(num_elements, runs);
	std::cout << "******************************" << std::endl << std::endl;

	print_table_performance(num_elements, runs);
	std::cout << "******************************" << std::endl << std::endl;

	print_tree_performance(num_elements, runs);

	return 0;
}

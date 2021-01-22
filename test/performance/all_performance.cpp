#include "vector_performance.cpp"
#include "list_performance.cpp"
#include "table_performance.cpp"
#include "tree_performance.cpp"

//Naive tfds performance measure (mostly inserting and accessing of std::strings)
int main(int argc, char *argv[]) {
	int num_elements = 1000000;
	int runs = 20;

	std::cout << "| META |" << std::endl << std::endl;
	std::cout << "runs each = " << runs << std::endl;
	std::cout << "number of elements each = " << num_elements << std::endl;
	std::cout << "type of elements each = std::string" << std::endl << std::endl;
	std::cout << "******************************" << std::endl << std::endl;

	std::cout << "| VECTOR |" << std::endl << std::endl;
	print_vector_performance(num_elements, runs);
	std::cout << "******************************" << std::endl << std::endl;

	std::cout << "| LINKED LIST |" << std::endl << std::endl;
	print_list_performance(num_elements, runs);
	std::cout << "******************************" << std::endl << std::endl;

	std::cout << "| HASH TABLE |" << std::endl << std::endl;
	print_table_performance(num_elements, runs);
	std::cout << "******************************" << std::endl << std::endl;

	std::cout << "| SEARCH TREE |" << std::endl << std::endl;
	print_tree_performance(num_elements, runs);

	return 0;
}
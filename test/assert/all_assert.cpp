#include "array_assert.cpp"
#include "vector_assert.cpp"
#include "list_assert.cpp"
#include "table_assert.cpp"
#include "tree_assert.cpp"

int main(int argc, char *argv[]) {
	print_array_assert();
	std::cout << std::endl << "******************************" << std::endl << std::endl;

	print_vector_assert();
	std::cout << std::endl << "******************************" << std::endl << std::endl;

	print_list_assert();
	std::cout << std::endl << "******************************" << std::endl << std::endl;

	print_table_assert();
	std::cout << std::endl << "******************************" << std::endl << std::endl;

	print_tree_assert();

	return 0;
}
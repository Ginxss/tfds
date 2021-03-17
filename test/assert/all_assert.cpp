#include "array_assert.cpp"
#include "vector_assert.cpp"
#include "linked_list_assert.cpp"
#include "hash_table_assert.cpp"
#include "search_tree_assert.cpp"

int main(int argc, char *argv[]) {
	test_array();
	test_vector();
	test_list();
	test_table();
	test_tree();

	return 0;
}
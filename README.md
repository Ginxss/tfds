# tfds
**A collection of data structures in C++:**
* [Array](#array)
* [Stack](#stack)
* [Linked List](#linked-list)
* [Hash Table](#hash-table)
* [Search Tree](#search-tree)
* [Multi Search Tree](#multi-search-tree)
* [FIFO Queue](#fifo-queue)
* [Priority Queue](#priority-queue)
* [Multi Priority Queue](#multi-priority-queue)

---

Why not just use the standard C++ data structures, you ask? Well, there's a couple of reasons:

First of all, every one of these data structures is faster than their C++ counterpart in almost all scenarios (The only exception being std::vector, which is slightly faster than tf::array because tf::array automatically allocates new space if accessed out of bounds, while std::vector just throws an exception. So these two structures actually behave quite differently and can't really be compared).

Second, i like to know what going on under the hood. Have you ever tried looking into an std::priority_queue (or any of the C++ data structures, for that matter) to see how it is implemented? If so, you will know that it's just not possible, and that these data structures are supposed to be black boxes to the programmer. With **tfds** (or any self-implemented data structure), you can just open the *.hpp and see exactly what happens if a certain function is called.

Third, because there are so many (poorly named) data structures available in C++, many programmers just resort to using std::vector for basically all tasks, because they don't want to spend time researching which one of these is the best data structure for the job. **tfds** is a small collection of the most basic and important data structures, which makes picking the right one a trivial task.

Note that all these data structures do is allocate and deallocate the memory needed for the elements. So if you are programming object oriented for example, destructors are not getting called.

---
---

## Array
Dynamic array that can reallocate with an appropriate size when accessed out of bounds. The reallocation size is the smallest multiple of the current size that can fit the new index (in most cases 2 * old index). Automatic reallocation can be turned off in the constructor.

The default initial capacity is 10. The more the initial capacity approaches the maximum index, the less reallocating has to be performed by the array.

This dynamic array is roughly half as fast as a native c array.

The array deallocates on destruction.

---

### Constructor
Default constructor with type `int`, initial capacity 10 and automatic reallocation turned on:
```
tf::array<int> array;
```
A custom initial capacity (in this case 100) and the reallocation behaviour can be set in the constructor:
```
tf::array<int> array(100, false);
```
If the array is accessed out of bounds and automatic reallocation is set to false, the array throws a tf::exception.

---

### insert(index, value)
*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if the index is negative, or if the index is bigger than the current capacity and automatic reallocation is turned off.

Inserts the value 3 at index 1:
```
array.insert(1, 3);
```

---

### get(index)
*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if the index is negative, or if the index is bigger than the current capacity and automatic reallocation is turned off.

Returns a constant reference to the value at index 1:
```
int value = array.get(1);
```

---

### [index]
*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if the index is negative, or if the index is bigger than the current capacity and automatic reallocation is turned off.

Returns a reference to the value at index 1:
```
array[1] = 3;
int value = array[1];
```

---

### capacity()
*Runtime:* **O(1)**

Returns the current capacity of the array:
```
int array_capacity = array.capacity();
```

---

### autom_reallocating()
*Runtime:* **O(1)**

Returns `true` if the array is reallocating when accessed out of bounds.
```
bool array_is_reallocating = array.autom_reallocating();
```

---

### reallocate(new capacity)
*Runtime:* **O(1)**

Reallocates the array with the new capacity 20:
```
array.reallocate(20);
```

---

### clear()
*Runtime:* **O(n)**

Sets the entire memory of the array to 0:
```
array.clear();
```

---
---

## Stack
A standard stack structure with put, peek and pop.

Internally works as an array with a top index, which reallocates with twice its size when the top is reached. This allows for extremely high speed, but at the cost of a memory overhead.

The default initial capacity is 10. The more the initial capacity approaches the maximum number of elements, the less reallocating has to be performed by the stack. If sparse memory is required, the use of [Linked List](#linked-list) is recommended.

The stack deallocates on destruction.

---

### Constructor
Default constructor with type `int` and initial capacity 10:
```
tf::stack<int> stack;
```
A custom initial capacity, in this case 100, can be set in the constructor:
```
tf::stack<int> stack(100);
```

---

### put(value)
*Runtime:* **O(1)** / O(n) on reallocation

Puts the value 3 on the stack:
```
stack.put(3);
```

---

### peek()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the stack is empty.

Returns a reference to the top value of the stack, which can be read of modified:
```
int top_value = stack.peek();
stack.peek() = 4;
```

---

### pop()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the stack is empty.

Removes and returns the top value from the stack:
```
int top_value = stack.pop();
```

---

### size()
*Runtime:* **O(1)**

Returns the number of elements on the stack:
```
int num_elements = stack.size();
```

---

### empty()
*Runtime:* **O(1)**

Returns `true` if the stack is empty:
```
bool stack_empty = stack.empty();
```

---

### clear()
*Runtime:* **O(1)**

Removes all elements from the stack:
```
stack.clear();
```
In reality, clear() just resets the internal top index, which means that no actual memory is deallocated and the stack is not reverted to its original capacity.

---
---

## Linked List
A doubly linked list.

Allows iteration both forwards and backwards.

Can also be used as a FIFO Queue, although the use of the wrapper [FIFO Queue](#fifo-queue) is recommended for that, because of its simpler interface.

Can also be used as a stack, but if memory overhead is of little to no concern, the use of [Stack](#stack) is recommended for that, because of its much higher speed.

The linked list deallocates on destruction.

---

### Constructor
Constructor with type `int`:
```
tf::linked_list<int> list;
```

---

### Iteration
Iterate over the list front to back and print the values:
```
for (auto it = list.begin(); it.condition(); ++it) {
    std::cout << *it << std::endl;
}
```
Iterate backwards and print the values:
```
for (auto it = list.end(); it.condition(); --it) {
    std::cout << it.value() << std::endl;
}
```
The iterator is of the type `tf::linked_list<int>::iterator`, but for simplicity,  the use of `auto` is recommended.

The value of the iterator can be accessed with either `*it` or the method `it.value()`. These two methods are identical and are interchangeable.
The methods `it.next_value()` and `it.prev_value()` return pointers to the following or previous value of the iterator if they exist, otherwise `nullptr`.
 
---

### add_back(value)
*Runtime:* **O(1)**

Adds the value 3 to the end of the list:
```
list.add_back(3);
```

---

### add_front(value)
*Runtime:* **O(1)**

Adds the value 3 to the start of the list:
```
list.add_front(3);
```

---

### add_after(new value, existing value)
*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the existing value does not exist.

Adds the value 3 right after the value 4:
```
list.add_after(3, 4);
```

---

### add_before(new value, existing value)
*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the existing value does not exist.

Adds the value 3 in front of the value 4:
```
list.add_before(3, 4);
```

---

### remove(value)
*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the value does not exist.

Removes the value 3 from the list:
```
list.remove(3);
```

---

### back()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Returns a reference to the last value in the list:
```
int last_value = list.back();
```

---

### front()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Returns a reference to the first value in the list:
```
int first_value = list.front();
```

---

### pop_back()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Removes and returns the last value from the list:
```
int last_value = list.pop_back();
```

---

### pop_front()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Removes and returns the first value from the list:
```
int first_value = list.pop_front();
```

---

### contains(value)
*Runtime:* **O(n)**

Returns `true` if the value 3 is found in the list:
```
bool contains_value = list.contains(3);
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the list:
```
int num_elements = list.length();
```

---

### empty()
*Runtime:* **O(1)**

Returns `true` if the list has no elements:
```
bool list_empty = list.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all stored values:
```
list.clear();
```

---
---

## Hash Table
An unordered map (Separate Chaining Hash Map).

Any type with non-changing memory can be used as the key, because the hash is built from the memory block of the key.
Strings can be used as keys as well, `std::string` and `const char *` with the same string produce the same hash.

The default table size is 100 (table size = number of buckets, bucket = linked list of entries). The table size is fixed and can only be modified on construction. For the best possible performance, the table size should be roughly the maximum number of elements. The average case performance of **O(1)** can only be ensured if the table is big enough.

The hash table deallocates on destruction.

---

### Constructor
Default constructor with `int` keys and `int` values and table size 100:
```
tf::hash_table<int, int> table;
```
A custom table size, in this case 10, can be set in the constructor:
```
tf::hash_table<int, int> table(10);
```

---

### Iteration
Iterate over every entry in the hash table and print the values:
```
for (auto it = table.begin(); it.condition(); ++it) {
    std::cout << *it << std::endl;
}
```
The iterator is of the type `tf::hash_table<int, int>::iterator`, but for simplicity,  the use of `auto` is recommended. Backward iteration is not supported, since the order of the entries is random and does not matter.

The value of the iterator can be accessed with either `*it` or the method `it.value()`. These two methods are identical and are interchangeable. The key of the iterator can be accessed with the method `it.key()`.

In general, the hash table is not a data structure made for efficient iteration. If consistent and fast iteration is required, the use of [Linked List](#linked-list) is recommended.

---

### insert(key, value)
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key already exists.

Inserts the value 3 with key 1 into the hash table:
```
table.insert(1, 3);
```

---

### remove(key)
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes the entry with key 1 and returns the value:
```
int value = table.remove(1);
```

---

### get(key)
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a constant reference to the value with key 1:
```
int value = table.get(1);
```

---

### [key]
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a reference to the value with key 1:
```
int value = table[1];
table[1] = 4;
```

---

### contains(key)
*Runtime:* average case: **O(1)** / worst case: O(n)

Returns `true` if an entry with key 3 is found in the hash table:
```
bool contains_value = table.contains(3);
```

---

### size()
*Runtime:* **O(1)**

Returns the number of entries in the hash table.
```
int num_entries = table.size();
```

---

### empty()
*Runtime:* **O(1)**

Returns `true` if the hash table has no entries:
```
bool table_empty = table.empty();
```

---

### table_size()
*Runtime:* **O(1)**

Returns the number of buckets in the hash table:
```
bool num_buckets = table.table_size();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all stored values:
```
table.clear();
```

---
---

## Search Tree
An ordered map (iterative AVL Tree).

The entries are sorted by the key. If only sorting by value is needed, the values should just have themselves as keys. If duplicate keys are expected, the use of [ Multi Search Tree](#multi-search-tree) is recommended.

The keys for the search tree have to be comparable with `operator<` and `operator>`.

Can also be used as a Priority Queue, although the use of the wrapper [Priority Queue](#priority-queue) is recommended for that, because of its simpler interface.

The search tree deallocates on destruction.

---

### Constructor
Constructor with `int` keys and `int` values:
```
tf::search_tree<int, int> tree;
```

---

### Iteration
Iterate over every entry in the search tree in ascending order and print the values:
```
for (auto it = tree.begin(); it.condition(); ++it) {
    std::cout << *it << std::endl;
}
```
Iterate in descending order:
```
for (auto it = tree.end(); it.condition(); --it) {
    std::cout << it.value() << std::endl;
}
```
The iterator is of the type `tf::search_tree<int, int>::iterator`, but for simplicity,  the use of `auto` is recommended.

The value of the iterator can be accessed with either `*it` or the method `it.value()`. These two methods are identical and are interchangeable. The key of the iterator can be accessed with the method `it.key()`.

---

### insert(key, value)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key already exists.

Inserts the value 3 with key 1 into the search tree:
```
tree.insert(1, 3);
```

---

### remove(key)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes the entry with key 1 and returns the value:
```
int value = tree.remove(1);
```

---

### get(key)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a constant reference to the value with key 1:
```
int value = tree.get(1);
```

---

### [key]
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a reference to the value with key 1:
```
int value = tree[1];
tree[1] = 4;
```

---

### pop_min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the tree is empty.

Removes and returns the value with the smallest key:
```
int min_value = tree.pop_min();
```

---

### pop_max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the search tree is empty.

Removes and returns the value with the biggest key:
```
int max_value = tree.pop_max();
```

---

### contains(key)
*Runtime:* **O(log(n))**

Returns `true` if the tree contains an entry with key 1:
```
bool key_present = tree.contains(1);
```

---

### height()
*Runtime:* **O(1)**

Returns the height of the search tree.
```
int tree_height = tree.height();
```

---

### size()
*Runtime:* **O(1)**

Returns the number of entries in the search tree:
```
int num_entries = tree.size();
```

---

### empty()
*Runtime:* **O(log(n))**

Returns `true` if the search tree has no entries:
```
bool tree_empty = tree.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all entries:
```
tree.clear();
```

---
---

## Multi Search Tree
Behaves just like the [Search Tree](#search-tree) while allowing duplicate keys.

The entries are sorted by the key. Values with the same key are not ordered in any particular order. If no duplicate keys are expected, the use of [Search Tree](#search-tree) is recommended, because of its slightly higher speed.

The keys for the multi search tree have to be comparable with `operator<` and `operator>` and the values must be comparable with `operator==`.

Can also be used as a Multi Priority Queue, although the use of the wrapper [Multi Priority Queue](#multi-priority-queue) is recommended for that, because of its simpler interface.

The multi search tree deallocates on destruction.

---

### Constructor
Constructor with `int` keys and `int` values:
```
tf::multi_search_tree<int, int> tree;
```

---

### Iteration
Iterate over every entry in the search tree in ascending order and print the values:
```
for (auto it = tree.begin(); it.condition(); ++it) {
    std::cout << *it << std::endl;
}
```
Iterate in descending order:
```
for (auto it = tree.end(); it.condition(); --it) {
    std::cout << it.value() << std::endl;
}
```
The iterator is of the type `tf::multi_search_tree<int, int>::iterator`, but for simplicity,  the use of `auto` is recommended.

The value of the iterator can be accessed with either `*it` or the method `it.value()`. These two methods are identical and are interchangeable. The key of the iterator can be accessed with the method `it.key()`.

---

### insert(key, value)
*Runtime:* **O(log(n))**

Inserts the value 3 with key 1 into the multi search tree:
```
tree.insert(1, 3);
```

---

### remove(key)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes all entries with key 1 and returns a value:
```
int value = tree.remove(1);
```

---

### remove_value(key, value)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if either the key or the value do not exist.

Removes the entries with key 1 and value 1 and returns the removed value:
```
int value = tree.remove(1, 1);
```
While it is redundant to return the same value that was just removed, it works like that for reasons of symmetry and compatibility with the [Search Tree](#search-tree).

---

### get(key)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a constant reference to a value with key 1:
```
int value = tree.get(1);
```

---

### [key]
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a reference to a value with key 1:
```
int value = tree[1];
tree[1] = 4;
```

---

### pop_min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the multi search tree is empty.

Removes and returns a value with the smallest key:
```
int min_value = tree.pop_min();
```

---

### pop_max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the multi search tree is empty.

Removes and returns a value with the biggest key:
```
int max_value = tree.pop_max();
```

---

### contains(key)
*Runtime:* **O(log(n))**

Returns `true` if the multi search tree contains an entry with key 1:
```
bool key_present = tree.contains(1);
```

---

### height()
*Runtime:* **O(1)**

Returns the height of the multi search tree.
```
int tree_height = tree.height();
```

---

### size()
*Runtime:* **O(1)**

Returns the number of entries in the multi search tree:
```
int num_entries = tree.size();
```

---

### empty()
*Runtime:* **O(log(n))**

Returns `true` if the multi search tree has no entries:
```
bool tree_empty = tree.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all entries:
```
tree.clear();
```

---
---

## FIFO Queue
This is just a wrapper for the [Linked List](#linked-list), which provides only the functionality of a FIFO queue.

The queue deallocates on destruction.

---

### Constructor
Constructor with type `int`:
```
tf::fifo_queue<int> queue;
```

---

### add(value)
*Runtime:* **O(1)**

Adds the value 3 to the FIFO queue:
```
queue.add(3);
```

---

### next()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the FIFO queue is empty.

Returns the next value in the FIFO queue:
```
int next_value = queue.next();
```

---

### contains(value)
*Runtime:* **O(n)**

Returns `true` if the FIFO queue contains the value 3:
```
bool contains_value = queue.contains(3);
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the FIFO queue:
```
int num_elements = queue.length();
```

---

### empty()
*Runtime:* **O(1)**

Returns `true` if the FIFO queue has no elements:
```
bool queue_empty = queue.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all elements:
```
queue.clear();
```

---
---

## Priority Queue
This is just a wrapper for the [Search Tree](#search-tree), which provides only the functionality of a priority queue.

The entries are sorted by the key. If only sorting by value is needed, the values should just have themselves as keys. If duplicate keys are expected, the use of [Multi Priority Queue](#multi-priority-queue) is recommended.

The priority queue deallocates on destruction.

---

### Constructor
Constructor with `int` keys and `int` values:
```
tf::prio_queue<int, int> queue;
```

---

### insert(key, value)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key already exists.

Adds the value 3 with the key 1 to the priority queue:
```
queue.insert(1, 3);
```

---

### next_min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the priority queue is empty.

Removes and returns the value with the smallest key:
```
int min_value = queue.next_min();
```

---

### next_max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the priority queue is empty.

Removes and returns the value with the biggest key:
```
int max_value = queue.next_max();
```

---

### contains(key)
*Runtime:* **O(log(n))**

Returns `true` if the priority queue contains a value with key 3:
```
bool contains_value = queue.contains(3);
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the priority queue.
```
int num_entries = queue.length();
```

---

### empty()
*Runtime:* **O(1)**

Returns `true` if the priority queue has no entries.
```
bool queue_empty = queue.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all entries:
```
queue.clear();
```

---
---

## Multi Priority Queue
This is just a wrapper for the [Multi Search Tree](#multi-search-tree), which provides only the functionality of a multi priority queue.

The entries are sorted by the key. Values with the same key are not ordered in any particular order. If no duplicate keys are expected, the use of [Priority Queue](#priority-queue) is recommended, because of its slightly higher speed.

The multi priority queue deallocates on destruction.

---

### Constructor
Constructor with `int` keys and `int` values:
```
tf::multi_prio_queue<int, int> queue;
```

---

### insert(key, value)
*Runtime:* **O(log(n))**

Adds the value 3 with the key 1 to the multi priority queue:
```
queue.insert(1, 3);
```

---

### next_min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the multi priority queue is empty.

Removes and returns a value with the smallest key:
```
int min_value = queue.next_min();
```

---

### next_max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the multi priority queue is empty.

Removes and returns a value with the biggest key:
```
int max_value = queue.next_max();
```

---

### contains(key)
*Runtime:* **O(log(n))**

Returns `true` if the multi priority queue contains a value with key 3:
```
bool contains_value = queue.contains(3);
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the multi priority queue.
```
int num_entries = queue.length();
```

---

### empty()
*Runtime:* **O(1)**

Returns `true` if the multi priority queue has no entries.
```
bool queue_empty = queue.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all entries:
```
queue.clear();
```

---
---
# tfds
**A collection of data structures in C++:**
* [Array](#array)
* [Vector](#vector)
* [Linked List](#linked-list)
* [Hash Table](#hash-table)
* [Search Tree](#search-tree)
* [Stack](#stack)
* [FIFO Queue](#fifo-queue)
* [Priority Queue](#priority-queue)

---

This is a collection of data structures in C++. In my (admittedly suboptimal) testing the **tfds**-classes were always faster or equally as fast as their STL counterpart.

Also, the `iterator`s that are used in **tfds**-classes behave differently than usual iterators, which is why you can't use range-based for loops (That's because these `iterator`s allow both forward and backward iteration with the same syntax).

---
---

## Array
Dynamic array that can reallocate with an appropriate size when accessed out of bounds. The reallocation size is the smallest multiple of the current size that can fit the new index (in most cases 2 * old index). Automatic reallocation can be turned off in the constructor.

Obviously this is **dangerous** and should not be used in any kind of production code. But i thought it could be useful for quickly testing something or setting up a quick prototype, as the array can be pretty much treated like pseudo-code.

---

### Constructor
Default constructor with type `std::string`, initial capacity 10 and automatic reallocation turned on:
```
tf::array<std::string> array;
```
A custom initial capacity (in this case 100) and the reallocation behaviour can be set in the constructor:
```
tf::array<std::string> array(100, false);
```
Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

---

### insert(index, value)
*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs (Automatic reallocation turned off: throws a tf::exception if the index is out of bounds).

Inserts the value "hello" at index 1:
```
array.insert(1, "hello");
```

---

### get(index)
*Runtime:* **O(1)**

*Exceptions:* throws a tf::exception if index out of bounds.

Returns a constant reference to the value at index 1:
```
std::string value = array.get(1);
```

---

### [index]
*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs (Automatic reallocation turned off: throws a tf::exception if the index is out of bounds).

Returns a reference to the value at index 1:
```
array[1] = "hello";
int value = array[1];
```
Setting a value "out of bounds":
```
array[25] = "new";
```

---

### set_all(value)
*Runtime:* **O(n)**

Sets every entry to the value "blah":
```
array.set_all("blah");
```

---

### capacity()
*Runtime:* **O(1)**

Returns the current capacity of the array:
```
size_t array_capacity = array.capacity();
```

---

### auto_reallocating()
*Runtime:* **O(1)**

Returns `true` if the array automatically reallocates when accessed out of bounds.
```
bool array_is_reallocating = array.auto_reallocating();
```

---

### reallocate(new capacity)
*Runtime:* **O(n)**

*Exceptions:* throws a tf::exception if new_capacity is zero or if it is too big and causes a bad_alloc.

Reallocates the array with the new capacity 20:
```
array.reallocate(20);
```
The content of the array gets copied into the new buffer. If new_capacity is smaller than the old capacity, only the elements up to that point get copied, the rest gets deleted.

---
---

## Vector
A dynamic array similar to std::vector.

Unlike `tf::array`, the `tf::vector` does not automatically reallocate when accessed out of bounds. Instead, accessing out of bounds always throws an exception and new data is inserted to the back of the buffer with `add(...)`, doubling the buffer size when the capacity is reached.

---

### Constructor
Default constructor with type `std::string` and initial capacity 10:
```
tf::vector<std::string> vector;
```
A custom initial capacity, in this case 100, can be set in the constructor:
```
tf::vector<std::string> vector(100);
```
Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

---

### add(value)
*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs.

Adds the value "hello" to the vector:
```
vector.add("hello");
```

---

### get(index)
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the index is larger than the size.

Returns a constant reference to the element at index 1:
```
std::string value = vector.get(1);
```

---

### [index]
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the index is larger than the size.

Returns a reference to the element at index 1:
```
std::string value = vector[1];
vector[1] = "world";
```

---

### set_all(value)
*Runtime:* **O(n)**

Sets every element to the value "blah":
```
vector.set_all("blah");
```

---

### remove(index)
*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the index is larger than the size.

Removes and returns the element at index 1:
```
std::string value = vector.remove(1);
```

---

### reallocate(new capacity)
*Runtime:* **O(n)**

*Exceptions:* throws a tf::exception if new_capacity is zero or if it is too big and causes a bad_alloc.

Reallocates the vector with the new capacity 20:
```
vector.reallocate(20);
```
The content of the array gets copied into the new buffer. If new_capacity is smaller than the old capacity, only the elements up to that point get copied, the rest gets deleted.

---

### contains(value)
*Runtime:* **O(n)**

Returns `true` if the value "hello" is in the vector:
```
bool contains_value = vector.contains("hello");
```

---

### size()
*Runtime:* **O(1)**

Returns the number of elements in the vector:
```
size_t num_elements = vector.size();
```

---

### capacity()
*Runtime:* **O(1)**

Returns the current size of the internal buffer:
```
size_t vector_capacity = vector.capacity();
```

---

### empty()
*Runtime:* **O(1)**

Returns `true` if the size of the vector is zero:
```
bool vector_empty = vector.empty();
```

---

### clear()
*Runtime:* **O(1)**

Clear the vector:
```
vector.clear();
```
In reality, clear() just resets the internal index, which means that no actual memory is deallocated and the vector is not reverted to its original capacity. Following calls to `add(...)` will overwrite the data in the buffer.

---
---

## Linked List
A doubly linked list.

Allows iteration both forwards and backwards.

---

### Constructor
Constructor with type `std::string`:
```
tf::linked_list<std::string> list;
```
Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

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

The value of the iterator can be accessed with either `*it` or the method `it.value()` (both methods are identical and interchangeable).
The methods `it.next_value()` and `it.prev_value()` return pointers to the following or previous value if they exist, otherwise `nullptr`.
 
---

### add_back(value)
*Runtime:* **O(1)**

Adds the value "hello" to the end of the list:
```
list.add_back("hello");
```

---

### add_front(value)
*Runtime:* **O(1)**

Adds the value "hello" to the start of the list:
```
list.add_front("hello");
```

---

### add_after(new value, existing value)
*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the existing value does not exist.

Adds the value "world" right after the value "hello":
```
list.add_after("world", "hello");
```

---

### add_before(new value, existing value)
*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the existing value does not exist.

Adds the value "hi" in front of the value "hello":
```
list.add_before("hi", "hello");
```

---

### remove(value)
*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the value does not exist.

Removes the value "hello" from the list:
```
list.remove("hello");
```

---

### back()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Returns a reference to the last value in the list:
```
std::string last_value = list.back();
list.back() = "back";
```

---

### front()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Returns a reference to the first value in the list:
```
std::string first_value = list.front();
list.front() = "front";
```

---

### pop_back()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Removes and returns the last value from the list:
```
std::string last_value = list.pop_back();
```

---

### pop_front()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Removes and returns the first value from the list:
```
std::string first_value = list.pop_front();
```

---

### contains(value)
*Runtime:* **O(n)**

Returns `true` if the value "hello" is found in the list:
```
bool contains_value = list.contains("hello");
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the list:
```
size_t num_elements = list.length();
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

Any type with non-changing memory can be used as the key, as the hash is built from the memory block of the key.
Strings can be used as keys as well (C++ strings and C strings with the same text will produce the same hash).

The default table size is 100 (table size = number of buckets, bucket = linked list of entries). The table size is fixed and can only be modified on construction. For the best possible performance, the table size should be roughly the maximum number of elements. The average case performance of **O(1)** can only be ensured if the table is big enough.

---

### Constructor
Default constructor with `std::string` keys and `int` values and table size 100:
```
tf::hash_table<std::string, int> table;
```
You can set a custom table size and turn off checking for duplicate keys in the constructor:
```
tf::hash_table<std::string, int> table(10, false);
```
In this case, the hash table will not check the existing entries for duplicate keys when inserting to improve performance. Note that this flag is mainly supposed to speed up insertion time if you KNOW that there can never be identical keys. If there are duplicate keys inside the table, functions like get(...) and remove(...) will only find one of the inserted pairs.

Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

---

### Iteration
Iterate over every entry in the hash table and print the values:
```
for (auto it = table.begin(); it.condition(); ++it) {
    std::cout << *it << std::endl;
}
```
Backward iteration is not supported, since the order of the entries is random and does not matter.

The value of the iterator can be accessed with either `*it` or the method `it.value()` (both methods are identical and interchangeable). The key of the iterator can be accessed with the method `it.key()`.

The hash table is not a data structure made for efficient iteration. If consistent and fast iteration is required, the use of [Linked List](#linked-list) is recommended.

---

### insert(key, value)
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Checks duplicate keys: throws a tf::exception if the key already exists.

Inserts the value 1 with key "hello" into the hash table:
```
table.insert("hello", 1);
```

---

### remove(key)
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes the entry with key "hello" and returns the value:
```
std::string value = table.remove("hello");
```

---

### get(key)
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a constant reference to the value with key "hello":
```
std::string value = table.get("hello");
```

---

### [key]
*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a reference to the value with key "hello":
```
std::string value = table["hello"];
table["hello"] = 2;
```

---

### contains(key)
*Runtime:* average case: **O(1)** / worst case: O(n)

Returns `true` if an entry with key "hello" is found in the hash table:
```
bool contains_value = table.contains("hello");
```

---

### size()
*Runtime:* **O(1)**

Returns the number of entries in the hash table:
```
size_t num_entries = table.size();
```

---

### checks_duplicate_keys()
*Runtime:* **O(1)**

Returns `true` if the hash table looks for duplicate keys when inserting:
```
bool table_checking_duplicate_keys = table.checks_duplicate_keys();
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
size_t num_buckets = table.table_size();
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

The entries are sorted by the key. If duplicate keys are allowed, entries with the same key are not ordered in any particular order.

The keys have to be comparable with either `operator==`, `operator<` and `operator>` or compare functions in the `tf`-namespace like in *tf_compare_functions.hpp*.

---

### Constructor
Constructor with `int` keys and `std::string` values:
```
tf::search_tree<int, std::string> tree;
```
Additionally, you can allow duplicate keys with:
```
tf::search_tree<int, std::string> tree(true);
```
Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

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
The value of the iterator can be accessed with either `*it` or the method `it.value()` (both methods are identical and interchangeable). The key of the iterator can be accessed with the method `it.key()`.

---

### insert(key, value)
*Runtime:* **O(log(n))**

*Exceptions:* Duplicate keys not allowed (default behaviour): throws a tf::exception if the key already exists.

Inserts the value "hello" with key 1 into the search tree:
```
tree.insert(1, "hello");
```

---

### remove(key)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes and returns one entry with key 1:
```
std::string value = tree.remove(1);
```

---

### remove_all(key)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes all entries with key 1 and returns one of them:
```
std::string value = tree.remove_all(1);
```

---

### remove_value(key, value)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key-value pair does not exist.

Removes the entry with key 1 and value "hello" and returns "hello" again:
```
std::string same_value = tree.remove_value(1, "hello");
```

---

### get(key)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a constant reference to one value with key 1:
```
std::string value = tree.get(1);
```

---

### [key]
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a reference to one value with key 1:
```
std::string value = tree[1];
tree[1] = "world";
```
Obviously this only makes sense if duplicate keys are not allowed, as you wouldn't otherwise know which entry you are currently editing. It's possible though.

---

### min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the tree is empty.

Returns a reference to one value with the smallest key:
```
int min_value = tree.min();
```

---

### max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the tree is empty.

Returns a reference to one value with the largest key:
```
int max_value = tree.max();
```

---

### pop_min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the tree is empty.

Removes and returns one value with the smallest key:
```
int min_value = tree.pop_min();
```

---

### pop_max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the search tree is empty.

Removes and returns one value with the biggest key:
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
size_t tree_height = tree.height();
```

---

### size()
*Runtime:* **O(1)**

Returns the number of entries in the search tree:
```
size_t num_entries = tree.size();
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

## Stack
This is just a wrapper for `tf::vector` which only provides the functionality of a stack.

The high performance comes at the cost of a memory overhead, as the vector always reallocates with twice its previous size. If sparse memory is required, the use of [Linked List](#linked-list) is recommended.

---

### Constructor
Default constructor with type `std::string` and initial capacity 10:
```
tf::stack<std::string> stack;
```
A custom initial capacity, in this case 100, can be set in the constructor:
```
tf::stack<std::string> stack(100);
```
Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

---

### put(value)
*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs.

Puts the value "hello" on the stack:
```
stack.put("hello");
```

---

### peek()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the stack is empty.

Returns a reference to the top value of the stack:
```
std::string top_value = stack.peek();
stack.peek() = "world";
```

---

### pop()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the stack is empty.

Removes and returns the top value from the stack:
```
std::string top_value = stack.pop();
```

---

### size()
*Runtime:* **O(1)**

Returns the number of elements on the stack:
```
size_t num_elements = stack.size();
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

## FIFO Queue
This is just a wrapper for the [Linked List](#linked-list) which provides only the functionality of a FIFO queue.

---

### Constructor
Constructor with type `std::string`:
```
tf::fifo_queue<std::string> queue;
```
Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

---

### add(value)
*Runtime:* **O(1)**

Adds the value "hello" to the FIFO queue:
```
queue.add("hello");
```

---

### next()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the FIFO queue is empty.

Returns the next value in the FIFO queue:
```
std::string next_value = queue.next();
```

---

### contains(value)
*Runtime:* **O(n)**

Returns `true` if the FIFO queue contains the value "hello":
```
bool contains_value = queue.contains("hello");
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the FIFO queue:
```
size_t num_elements = queue.length();
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
This is just a wrapper for the [Search Tree](#search-tree) which provides only the functionality of a priority queue.

---

### Constructor
Constructor with `int` keys and `std::string` values:
```
tf::prio_queue<int, std::string> queue;
```
Additionally, you can allow duplicate keys in the constructor:
```
tf::prio_queue<int, std::string> queue(true);
```
Copy constructor, copy assignment operator, move constructor and move assignment operator also exist.

---

### insert(key, value)
*Runtime:* **O(log(n))**

*Exceptions:* Duplicate keys not allowed (default behaviour): throws a tf::exception if the key already exists.

Adds the value "hello" with the key 1 to the priority queue:
```
queue.insert(1, "hello");
```

---

### next_min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the priority queue is empty.

Removes and returns one value with the smallest key:
```
std::string min_value = queue.next_min();
```

---

### next_max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the priority queue is empty.

Removes and returns one value with the biggest key:
```
std::string max_value = queue.next_max();
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
size_t num_entries = queue.length();
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
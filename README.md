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

This is a collection of data structures in C++.

The `iterator`s that are used in **tfds**-classes behave differently than usual iterators, which is why you can't use range-based for loops (That's because these `iterator`s allow both forward and backward iteration with the same syntax).

---
---

## Array

Dynamic array that can reallocate with an appropriate size when accessed out of bounds. The reallocation size is the smallest multiple of the current size that can fit the new index (in most cases 2 * old index). Automatic reallocation can be turned off in the constructor.

Obviously this is **dangerous** and should not be used in any kind of production code. But I thought it could be useful for testing something or setting up a quick prototype, as the array can pretty much be treated like pseudo-code.

---

### Array Constructor

Default constructor with type `std::string`, initial capacity 10 and automatic reallocation turned on:

```cpp
tf::array<std::string> array;
```

A custom initial capacity (in this case 100) and the reallocation behaviour can be set in the constructor:

```cpp
tf::array<std::string> array(100, false);
```

---

### array.insert(index, value)

*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs (If automatic reallocation turned off: throws a tf::exception if the index is out of bounds).

Inserts the value "hello" at index 1:

```cpp
array.insert(1, "hello");
```

---

### array.get(index)

*Runtime:* **O(1)**

*Exceptions:* throws a tf::exception if index out of bounds.

Returns a constant reference to the value at index 1:

```cpp
std::string value = array.get(1);
```

---

### array[index]

*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs (If automatic reallocation turned off: throws a tf::exception if the index is out of bounds).

Returns a reference to the value at index 1:

```cpp
array[1] = "hello";
std::string value = array[1];
```

Setting a value "out of bounds":

```cpp
array[25] = "new";
```

---

### array.set_all(value)

*Runtime:* **O(n)**

Sets every entry to the value "hello":

```cpp
array.set_all("hello");
```

---

### array.reallocate(new capacity)

*Runtime:* **O(n)**

*Exceptions:* throws a tf::exception if new_capacity is zero or if it is too big and causes a bad_alloc.

Reallocates the array with the new capacity 20:

```cpp
array.reallocate(20);
```

The content of the array gets copied into the new buffer. If new_capacity is smaller than the old capacity, only the elements up to that point get copied, the rest gets deleted.

---

### array.capacity()

*Runtime:* **O(1)**

Returns the current current size of the internal buffer:

```cpp
size_t array_capacity = array.capacity();
```

---

### array.auto_reallocating()

*Runtime:* **O(1)**

Returns `true` if the array automatically reallocates when accessed out of bounds.

```cpp
bool array_is_reallocating = array.auto_reallocating();
```

---
---

## Vector

A dynamic array similar to std::vector.

Unlike `tf::array`, the `tf::vector` does not automatically reallocate when accessed out of bounds. Instead, accessing out of bounds always throws an exception and new data is inserted to the back of the buffer with `add(...)`, doubling the buffer size when the capacity is reached.

---

### Vector Constructor

Default constructor with type `std::string` and initial capacity 10:

```cpp
tf::vector<std::string> vector;
```

The initial capacity can be set in the constructor:

```cpp
tf::vector<std::string> vector(100);
```

---

### vector.add(value)

*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs.

Adds the value "hello" to the vector:

```cpp
vector.add("hello");
```

---

### vector.get(index)

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the index is larger or equal to the size.

Returns a constant reference to the element at index 1:

```cpp
std::string value = vector.get(1);
```

---

### vector[index]

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the index is larger or equal to the size.

Returns a reference to the element at index 1:

```cpp
std::string value = vector[1];
vector[1] = "world";
```

---

### vector.set_all(value)

*Runtime:* **O(n)**

Sets every element to the value "hello":

```cpp
vector.set_all("hello");
```

---

### vector.remove(index)

*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the index is larger or equal to the size.

Removes and returns the element at index 1:

```cpp
std::string value = vector.remove(1);
```

---

### vector.contains(value)

*Runtime:* **O(n)**

Returns `true` if the value "hello" is in the vector:

```cpp
bool contains_value = vector.contains("hello");
```

---

### vector.reallocate(new capacity)

*Runtime:* **O(n)**

*Exceptions:* throws a tf::exception if new_capacity is zero or if it is too big and causes a bad_alloc.

Reallocates the vector with the new capacity 20:

```cpp
vector.reallocate(20);
```

The content of the vector gets copied into the new buffer. If new_capacity is smaller than the old capacity, only the elements up to that point get copied, the rest gets deleted.

---

### vector.clear()

*Runtime:* **O(1)**

Clear the vector:

```cpp
vector.clear();
```

In reality, clear() just resets the internal index, which means that no actual memory is deallocated and the vector is not reverted to its original capacity. Following calls to `add(...)` will overwrite the data in the buffer.

---

### vector.size()

*Runtime:* **O(1)**

Returns the number of elements in the vector:

```cpp
size_t num_elements = vector.size();
```

---

### vector.capacity()

*Runtime:* **O(1)**

Returns the current size of the internal buffer:

```cpp
size_t vector_capacity = vector.capacity();
```

---

### vector.empty()

*Runtime:* **O(1)**

Returns `true` if the size of the vector is zero:

```cpp
bool vector_empty = vector.empty();
```

---
---

## Linked List

A doubly linked list.

Allows iteration both forwards and backwards.

---

### List Constructor

Constructor with type `std::string`:

```cpp
tf::linked_list<std::string> list;
```

---

### List Iteration

Iterate over the list front to back and print the values:

```cpp
for (auto it = list.begin(); it.has_value(); ++it) {
    std::cout << *it << std::endl;
}
```

Iterate backwards and print the values:

```cpp
for (auto it = list.end(); it.has_value(); --it) {
    std::cout << it.value() << std::endl;
}
```

The value of the iterator can be accessed with either `*it` or the method `it.value()` (both methods are identical and interchangeable).
The methods `it.next_value()` and `it.prev_value()` return pointers to the following or previous value if they exist, otherwise `nullptr`.

---

### list.add_back(value)

*Runtime:* **O(1)**

Adds the value "hello" to the end of the list:

```cpp
list.add_back("hello");
```

---

### list.add_front(value)

*Runtime:* **O(1)**

Adds the value "hello" to the start of the list:

```cpp
list.add_front("hello");
```

---

### list.add_after(new value, existing value)

*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the existing value does not exist.

Adds the value "world" right after the value "hello":

```cpp
list.add_after("world", "hello");
```

---

### list.add_before(new value, existing value)

*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the existing value does not exist.

Adds the value "hi" in front of the value "hello":

```cpp
list.add_before("hi", "hello");
```

---

### list.add_back_all(other list)

*Runtime:* **O(other.n)**

Adds all values from `list2` to the back of `list`.

```cpp
list.add_back_all(list2);
```

`list2` is traversed front to back. Both lists have to share the same template type.

---

### list.back()

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Returns a reference to the last value in the list:

```cpp
std::string last_value = list.back();
list.back() = "back";
```

---

### list.front()

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Returns a reference to the first value in the list:

```cpp
std::string first_value = list.front();
list.front() = "front";
```

---

### list.pop_back()

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Removes and returns the last value from the list:

```cpp
std::string last_value = list.pop_back();
```

---

### list.pop_front()

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the list is empty.

Removes and returns the first value from the list:

```cpp
std::string first_value = list.pop_front();
```

---

### list.remove(value)

*Runtime:* **O(n)**

*Exceptions:* Throws a tf::exception if the value does not exist.

Removes the value "hello" from the list:

```cpp
list.remove("hello");
```

---

### list.contains(value)

*Runtime:* **O(n)**

Returns `true` if the value "hello" is found in the list:

```cpp
bool contains_value = list.contains("hello");
```

---

### list.clear()

*Runtime:* **O(n)**

Deallocates all stored values:

```cpp
list.clear();
```

---

### list.length()

*Runtime:* **O(1)**

Returns the number of elements in the list:

```cpp
size_t num_elements = list.length();
```

---

### list.empty()

*Runtime:* **O(1)**

Returns `true` if the list has no elements:

```cpp
bool list_empty = list.empty();
```

---
---

## Hash Table

An unordered map (Separate Chaining Hash Map).

Any type with non-changing memory can be used as the key, as the hash is built from the memory block of the key.
Strings can be used as keys as well (C++ strings and C strings with the same text will produce the same hash).

The default table size is 100 (table size = number of buckets, bucket = linked list of entries). The table size is fixed and can only be modified on construction. For the best performance, the table size should be roughly the maximum number of elements. The performance of **O(1)** can only be ensured if the table is big enough.

---

### Table Constructor

Default constructor with `std::string` keys and `int` values and table size 100:

```cpp
tf::hash_table<std::string, int> table;
```

You can set a custom table size and turn off checking for duplicate keys in the constructor:

```cpp
tf::hash_table<std::string, int> table(10, false);
```

In this case, the hash table will not check the existing entries for duplicate keys when inserting to improve performance. This flag is mainly supposed to speed up insertion time if you KNOW that there can never be identical keys. If there are duplicate keys inside the table, functions like get(...) and remove(...) will only find one of the inserted pairs.

---

### Table Iteration

Iterate over every entry in the hash table and print the values:

```cpp
for (auto it = table.begin(); it.has_value(); ++it) {
    std::cout << *it << std::endl;
}
```

Backward iteration is not supported, since the order of the entries is random and does not matter.

The value of the iterator can be accessed with either `*it` or the method `it.value()` (both methods are identical and interchangeable). The key of the iterator can be accessed with the method `it.key()`.

---

### table.insert(key, value)

*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Checks duplicate keys: throws a tf::exception if the key already exists.

Inserts the value 1 with key "hello" into the hash table:

```cpp
table.insert("hello", 1);
```

---

### table.get(key)

*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a constant reference to the value with key "hello":

```cpp
std::string value = table.get("hello");
```

---

### table[key]

*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a reference to the value with key "hello":

```cpp
std::string value = table["hello"];
table["hello"] = 2;
```

---

### table.remove(key)

*Runtime:* average case: **O(1)** / worst case: O(n)

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes the entry with key "hello" and returns the value:

```cpp
std::string value = table.remove("hello");
```

---

### table.contains(key)

*Runtime:* average case: **O(1)** / worst case: O(n)

Returns `true` if an entry with key "hello" is found in the hash table:

```cpp
bool contains_value = table.contains("hello");
```

---

### table.clear()

*Runtime:* **O(n)**

Deallocates all stored values:

```cpp
table.clear();
```

---

### table.size()

*Runtime:* **O(1)**

Returns the number of entries in the hash table:

```cpp
size_t num_entries = table.size();
```

---

### table.table_size()

*Runtime:* **O(1)**

Returns the number of buckets in the hash table:

```cpp
size_t num_buckets = table.table_size();
```

---

### table.empty()

*Runtime:* **O(1)**

Returns `true` if the hash table has no entries:

```cpp
bool table_empty = table.empty();
```

---

### table.checks_duplicate_keys()

*Runtime:* **O(1)**

Returns `true` if the hash table looks for duplicate keys when inserting:

```cpp
bool table_checking_duplicate_keys = table.checks_duplicate_keys();
```

---
---

## Search Tree

An ordered map (iterative AVL Tree).

The entries are sorted by the key. If duplicate keys are allowed, entries with the same key are not ordered in any particular order.

The keys have to be comparable with either `operator==`, `operator<` and `operator>` or compare functions in the `tf`-namespace like in *tf_compare_functions.hpp*.

---

### Tree Constructor

Constructor with `int` keys and `std::string` values:

```cpp
tf::search_tree<int, std::string> tree;
```

Additionally, you can allow duplicate keys with:

```cpp
tf::search_tree<int, std::string> tree(true);
```

---

### Tree Iteration

Iterate over every entry in the search tree in ascending order and print the values:

```cpp
for (auto it = tree.begin(); it.has_value(); ++it) {
    std::cout << *it << std::endl;
}
```

Iterate in descending order:

```cpp
for (auto it = tree.end(); it.has_value(); --it) {
    std::cout << it.value() << std::endl;
}
```

The value of the iterator can be accessed with either `*it` or the method `it.value()` (both methods are identical and interchangeable). The key of the iterator can be accessed with the method `it.key()`.

---

### tree.insert(key, value)

*Runtime:* **O(log(n))**

*Exceptions:* Duplicate keys not allowed: throws a tf::exception if the key already exists.

Inserts the value "hello" with key 1 into the search tree:

```cpp
tree.insert(1, "hello");
```

---

### tree.get(key)

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a constant reference to one value with key 1:

```cpp
std::string value = tree.get(1);
```

---

### tree[key]

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Returns a reference to one value with key 1:

```cpp
std::string value = tree[1];
tree[1] = "world";
```

---

### tree.min()

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the tree is empty.

Returns a reference to one value with the smallest key:

```cpp
std::string min_value = tree.min();
```

---

### tree.max()

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the tree is empty.

Returns a reference to one value with the largest key:

```cpp
std::string max_value = tree.max();
```

---

### tree.pop_min()

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the tree is empty.

Removes and returns one value with the smallest key:

```cpp
std::string min_value = tree.pop_min();
```

---

### tree.pop_max()

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the search tree is empty.

Removes and returns one value with the biggest key:

```cpp
std::string max_value = tree.pop_max();
```

---

### tree.remove(key)

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes and returns one entry with key 1:

```cpp
std::string value = tree.remove(1);
```

---

### tree.remove_all(key)

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key does not exist.

Removes all entries with key 1 and returns one of them:

```cpp
std::string value = tree.remove_all(1);
```

---

### tree.remove_value(key, value)

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key-value pair does not exist.

Removes the entry with key 1 and value "hello" and returns "hello" again:

```cpp
std::string same_value = tree.remove_value(1, "hello");
```

---

### tree.contains(key)

*Runtime:* **O(log(n))**

Returns `true` if the tree contains an entry with key 1:

```cpp
bool key_present = tree.contains(1);
```

---

### tree.contains_value(key, value)

*Runtime:* **O(log(n))**

Returns `true` if the tree contains an entry with key 1 and value "one":

```cpp
bool key_value_present = tree.contains(1, "one");
```

---

### tree.clear()

*Runtime:* **O(n)**

Deallocates all entries:

```cpp
tree.clear();
```

---

### tree.size()

*Runtime:* **O(1)**

Returns the number of entries in the search tree:

```cpp
size_t num_entries = tree.size();
```

---

### tree.height()

*Runtime:* **O(1)**

Returns the height of the search tree.

```cpp
size_t tree_height = tree.height();
```

---

### tree.empty()

*Runtime:* **O(log(n))**

Returns `true` if the search tree has no entries:

```cpp
bool tree_empty = tree.empty();
```

---

### tree.allows_duplicate_keys()

*Runtime:* **O(1)**

Returns `true` if the search tree allows multiple entries with the same key:

```cpp
bool is_multi_search_tree = tree.allows_duplicate_keys();
```

---
---

## Stack

This is just a wrapper for `tf::vector` which only provides the functionality of a stack.

The high performance comes at the cost of a memory overhead, as the vector always reallocates with twice its previous size. If sparse memory is required, use a [Linked List](#linked-list).

---

### Stack Constructor

Default constructor with type `std::string` and initial capacity 10:

```cpp
tf::stack<std::string> stack;
```

A custom initial capacity, in this case 100, can be set in the constructor:

```cpp
tf::stack<std::string> stack(100);
```

---

### stack.put(value)

*Runtime:* **O(1)** / O(n) on reallocation

*Exceptions:* Throws a tf::exception if a buffer overflow or bad_alloc occurs.

Puts the value "hello" on the stack:

```cpp
stack.put("hello");
```

---

### stack.peek()

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the stack is empty.

Returns a reference to the top value of the stack:

```cpp
std::string top_value = stack.peek();
stack.peek() = "world";
```

---

### stack.pop()

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the stack is empty.

Removes and returns the top value from the stack:

```cpp
std::string top_value = stack.pop();
```

---

### stack.clear()

*Runtime:* **O(1)**

Removes all elements from the stack:

```cpp
stack.clear();
```

In reality, clear() just resets the internal top index, which means that no actual memory is deallocated and the stack is not reverted to its original capacity.

---

### stack.size()

*Runtime:* **O(1)**

Returns the number of elements on the stack:

```cpp
size_t num_elements = stack.size();
```

---

### stack.empty()

*Runtime:* **O(1)**

Returns `true` if the stack is empty:

```cpp
bool stack_empty = stack.empty();
```

---
---

## FIFO Queue

This is just a wrapper for the [Linked List](#linked-list) which provides only the functionality of a FIFO queue.

---

### FIFO Queue Constructor

Constructor with type `std::string`:

```cpp
tf::fifo_queue<std::string> fifo_queue;
```

---

### fifo_queue.add(value)

*Runtime:* **O(1)**

Adds the value "hello" to the FIFO queue:

```cpp
fifo_queue.add("hello");
```

---

### fifo_queue.next()

*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the FIFO queue is empty.

Returns the next value in the FIFO queue:

```cpp
std::string next_value = fifo_queue.next();
```

---

### fifo_queue.contains(value)

*Runtime:* **O(n)**

Returns `true` if the FIFO queue contains the value "hello":

```cpp
bool contains_value = fifo_queue.contains("hello");
```

---

### fifo_queue.clear()

*Runtime:* **O(n)**

Deallocates all elements:

```cpp
fifo_queue.clear();
```

---

### fifo_queue.length()

*Runtime:* **O(1)**

Returns the number of elements in the FIFO queue:

```cpp
size_t num_elements = fifo_queue.length();
```

---

### fifo_queue.empty()

*Runtime:* **O(1)**

Returns `true` if the FIFO queue has no elements:

```cpp
bool queue_empty = fifo_queue.empty();
```

---
---

## Priority Queue

This is just a wrapper for the [Search Tree](#search-tree) which provides only the functionality of a priority queue.

---

### Priority Queue Constructor

Constructor with `int` keys and `std::string` values:

```cpp
tf::prio_queue<int, std::string> prio_queue;
```

Additionally, you can allow duplicate keys in the constructor:

```cpp
tf::prio_queue<int, std::string> queue(true);
```

---

### prio_queue.insert(key, value)

*Runtime:* **O(log(n))**

*Exceptions:* Duplicate keys not allowed (default behaviour): throws a tf::exception if the key already exists.

Adds the value "hello" with the key 1 to the priority queue:

```cpp
prio_queue.insert(1, "hello");
```

---

### prio_queue.next_min()

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the priority queue is empty.

Removes and returns one value with the smallest key:

```cpp
std::string min_value = prio_queue.next_min();
```

---

### prio_queue.next_max()

*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the priority queue is empty.

Removes and returns one value with the biggest key:

```cpp
std::string max_value = prio_queue.next_max();
```

---

### prio_queue.contains(key)

*Runtime:* **O(log(n))**

Returns `true` if the priority queue contains a value with key 3:

```cpp
bool contains_value = prio_queue.contains(3);
```

---

### prio_queue.clear()

*Runtime:* **O(n)**

Deallocates all entries:

```cpp
prio_queue.clear();
```

---

### prio_queue.length()

*Runtime:* **O(1)**

Returns the number of elements in the priority queue.

```cpp
size_t num_entries = prio_queue.length();
```

---

### prio_queue.empty()

*Runtime:* **O(1)**

Returns `true` if the priority queue has no entries.

```cpp
bool queue_empty = prio_queue.empty();
```

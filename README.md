# tfds
**A collection of data structures in C++**

## Array
Dynamic array that reallocates with an appropriate size when accessed out of bounds.

The new size is the smallest multiple of the current size that can fit the new index (in most cases 2 * old index).

This dynamic array is roughly half as fast as a native c array.

The array deallocates on destruction.

---

### Constructor
Default constructor with type `int` and initial capacitity 10:
```
tf::array<int> array;
```
A custom starting capacity can be assinged in the constructor, in the following case 100. The more the initial capacity approaches the maximum index, the less reallocating has to be performed by the array.
```
tf::array<int> array(100);
```

---

### insert(index, value)
*Runtime:* **O(1)** / O(n) on reallocation

Inserts the value 3 at index 1:
```
array.insert(1, 3);
```

---

### get(index)
*Runtime:* **O(1)** / O(n) on reallocation

Returns a constant reference to the value at index 1:
```
int value = array.get(1);
```

---

### [index]
*Runtime:* **O(1)** / O(n) on reallocation

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

Internally works as an array with a top index, which reallocates with twice its size when the top is reached. This allows for extremely high speed, but at the cost of a memory overhead. If sparse memory is required, the use of the Linked List is recommended.

The stack deallocates on destruction.

---

### Constructor
Default constructor with type `int` and initial capacitity 10:
```
tf::stack<int> stack;
```
A custom starting capacity can be assinged in the constructor, in the following case 100. The more the initial capacity approaches the maximum number of elements, the less reallocating has to be performed by the stack.
```
tf::stack<int> stack(100);
```

---

### put(value)
*Runtime:* **O(1)** / O(n) on reallocation

Puts the value 3 at the top of the stack:
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

Removes the top value from the stack and returns it:
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

Returns true if the stack is empty:
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
In reality, clear() just resets the internal top index, which means that no actual memory is deallocated and the stack is not being reverted to its original capacity.

---
---

## Linked List
A doubly linked list.

Allows iteration both forwards and backwards.

Can also be used as a FIFO Queue, although the use of the wrapper [FIFO Queue] is recommended for that.

Can also be used as a stack, but if memory overhead is of little to no concern, the use of [Stack] is recommended for that, because of its much higher speed.

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
    std::cout << *it << std::endl;
}
```

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

Returns true if the value 3 is found in the list:
```
bool contains_value = list.contains(3);
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements of the list:
```
int num_elements = list.length();
```

---

### empty()
*Runtime:* **O(1)**

Returns true if the list has no elements:
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

Any type with non-changing memory can be used as a key, since the hash is built from the memory block of the key.
Strings can be used as keys as well, `std::string` and `const char *` with the same string produce the same hash.

For the best possible performance, the table size should be roughly the maximum number of elements. The average case performance of **O(1)** can only be ensured if the table is big enough.

The hash table deallocates on destruction.

---

### Constructor
Default constructor with `int` keys and `int` values and table size 100:
```
tf::hash_table<int, int> table;
```
A custom table size can be assinged in the constructor, in the following case 10:
```
tf::hash_table<int, int> table(10);
```

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

### size()
*Runtime:* **O(1)**

Returns the number of elements in the hash table.
```
int num_elements = table.size();
```

---

### empty()
*Runtime:* **O(1)**

Returns true if the hash table has no elements:
```
bool table_empty = table.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deletes all stored values:
```
table.clear();
```

---
---

## Search Tree
An ordered map (iterative AVL Tree).

Can also be used as a Priority Queue, although the use of the wrapper [Priority Queue] is recommended for that.

The search tree deallocates on destruction.

---

### Constructor
Constructor with `int` keys and `int` values:
```
tf::search_tree<int, int> tree;
```

---

### insert(key, value)
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the key already exists.

Inserts the value 3 with key 1:
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

*Exceptions:* Throws a tf::exception if the tree is empty.

Removes and returns the value with the biggest key:
```
int max_value = tree.pop_max();
```

---

### contains(key)
*Runtime:* **O(log(n))**

Returns true if the tree contains an entry with key 1:
```
bool key_present = tree.contains(1);
```

---

### height()
*Runtime:* **O(1)**

Returns the height of the tree.
```
int tree_height = tree.height();
```

---

### size()
*Runtime:* **O(1)**

Returns the number of entries in the tree:
```
int num_entries = tree.size();
```

---

### empty()
*Runtime:* **O(log(n))**

Returns true if the tree has no entries.
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
This is just a wrapper for the [Linked List], which only supports the actions that make a FIFO Queue.

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

Adds the value 3 to the queue:
```
queue.add(3);
```

---

### next()
*Runtime:* **O(1)**

*Exceptions:* Throws a tf::exception if the queue is empty.

Returns the next value in the queue:
```
int next_value = queue.next();
```

---

### contains(value)
*Runtime:* **O(n)**

Returns true if the queue contains the value 3:
```
bool contains_value = queue.contains(3);
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the queue:
```
int num_elements = queue.length();
```

---

### empty()
*Runtime:* **O(1)**

Returns true if the queue has no elements:
```
bool queue_empty = queue.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all elements in the queue:
```
queue.clear();
```

---
---

## Priority Queue
This is just a wrapper for the [Search Tree], which only supports the actions that make a Priority Queue.

The Elements are sorted by the key. If only sorting by value is needed, the values should just have themselves as keys.

The queue deallocates on destruction.

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

Adds the value 3 with the key 1 to the queue:
```
queue.insert(1, 3);
```

---

### next_min()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the queue is empty.

Removes and returns the value with the smallest key:
```
int min_value = queue.next_min();
```

---

### next_max()
*Runtime:* **O(log(n))**

*Exceptions:* Throws a tf::exception if the queue is empty.

Removes and returns the value with the biggest key:
```
int max_value = queue.next_max();
```

---

### contains(key)
*Runtime:* **O(log(n))**

Returns true if the queue contains the value 3:
```
bool contains_value = queue.contains(3);
```

---

### length()
*Runtime:* **O(1)**

Returns the number of elements in the queue.
```
int num_entries = queue.length();
```

---

### empty()
*Runtime:* **O(1)**

Returns true if the queue has no entries.
```
bool queue_empty = queue.empty();
```

---

### clear()
*Runtime:* **O(n)**

Deallocates all entires in the queue:
```
queue.clear();
```

---
---
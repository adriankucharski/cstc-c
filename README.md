# Vector Library for C

A type-safe, flexible vector implementation for C that supports automatic memory management and custom element constructors/destructors.

## Features

- Type-safe implementation using C preprocessor macros
- Automatic memory management (both manual and automatic cleanup)
- Support for custom element constructors and destructors
- Simple API for common vector operations
- Memory optimization functions
- Support for primitive types and complex data structures

## Basic Usage

```c
#include "vector.h"
#include <stdio.h>

// Example of printing an integer
void print_int(int n) {
    printf("%d ", n);
}

int main() {
    // Create a new vector of integers
    vector_int *vec = new_vector_int();
    
    // Add elements
    vec->push(vec, 10);
    vec->push(vec, 20);
    vec->push(vec, 30);
    
    // Insert an element at index 1
    vec->insert(vec, 1, 15);
    
    // Access elements
    printf("First element: %d\n", vec->front(vec));  // 10
    printf("Last element: %d\n", vec->back(vec));    // 30
    printf("Element at index 1: %d\n", vec->at(vec, 1));  // 15
    
    // Replace an element
    vec->replace(vec, 2, 25);
    
    // Print all elements
    printf("All elements: ");
    vec->foreach(vec, print_int);  // 10 15 25 30
    printf("\n");
    
    // Check if vector is empty
    if (!vec->empty(vec)) {
        printf("Vector contains %zu elements\n", vec->size);
    }
    
    // Remove last element
    vec->pop(vec);
    
    // Clear the vector
    vec->clear(vec);
    
    // Free memory
    vec->free_memory(vec);
    
    return 0;
}
```

## Automatic Memory Management with Scoped Variables

```c
#include "vector.h"
#include <stdio.h>

void vector_demo() {
    // Vector will be automatically freed when it goes out of scope
    scoped vector_int *vec = new_vector_int();
    
    vec->push(vec, 10);
    vec->push(vec, 20);
    vec->push(vec, 30);
    
    // No need to call free_memory() - it will be called automatically
}

int main() {
    vector_demo();
    // At this point, vec has been freed automatically
    return 0;
}
```

## Working with Strings

```c
#include "vector.h"
#include <stdio.h>

void print_string(char *str) {
    printf("\"%s\" ", str);
}

int main() {
    // Create a vector of strings
    scoped vector_charp *names = new_vector_charp();
    
    // Add strings (they will be deep-copied)
    names->push(names, "Alice");
    names->push(names, "Bob");
    names->push(names, "Charlie");
    
    // Print all names
    printf("Names: ");
    names->foreach(names, print_string);
    printf("\n");
    
    // Replace a name
    names->replace(names, 1, "Benjamin");
    
    // Clone the vector
    scoped vector_charp *names_copy = names->clone(names);
    
    // Add another name to the copy
    names_copy->push(names_copy, "Diana");
    
    // Print the original and copy
    printf("Original names: ");
    names->foreach(names, print_string);
    printf("\n");
    
    printf("Copied names: ");
    names_copy->foreach(names_copy, print_string);
    printf("\n");
    
    // Both vectors will be automatically freed when they go out of scope
    return 0;
}
```

## Custom Types with Constructors and Destructors

```c
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A simple person structure
typedef struct {
    char *name;
    int age;
} Person;

// Constructor for Person
Person person_constructor(const Person p) {
    Person new_p;
    new_p.name = strdup(p.name);
    new_p.age = p.age;
    return new_p;
}

// Destructor for Person
void person_destructor(Person p) {
    free(p.name);
}

// Print a person
void print_person(Person p) {
    printf("%s (%d) ", p.name, p.age);
}

// Declare a vector of Person
VECTOR(Person, vector_person, person_constructor, person_destructor);

int main() {
    scoped vector_person *people = new_vector_person();
    
    // Create and add people
    Person p1 = {.name = "Alice", .age = 30};
    Person p2 = {.name = "Bob", .age = 25};
    
    people->push(people, p1);
    people->push(people, p2);
    
    // We can safely free these now, as the vector has deep-copied them
    person_destructor(p1);
    person_destructor(p2);
    
    // Print all people
    printf("People: ");
    people->foreach(people, print_person);
    printf("\n");
    
    // Create a new person and replace one in the vector
    Person p3 = {.name = "Charlie", .age = 35};
    people->replace(people, 0, p3);
    person_destructor(p3);
    
    // Print again
    printf("Updated people: ");
    people->foreach(people, print_person);
    printf("\n");
    
    return 0;
}
```

## Memory Optimization

```c
#include "vector.h"
#include <stdio.h>

int main() {
    scoped vector_int *vec = new_vector_int();
    
    // Add a lot of elements
    for (int i = 0; i < 100; i++) {
        vec->push(vec, i);
    }
    
    printf("Vector size: %zu, Max size: %zu\n", vec->size, vec->__max_size);
    
    // Remove most elements
    for (int i = 0; i < 90; i++) {
        vec->pop(vec);
    }
    
    printf("After removal - Vector size: %zu, Max size: %zu\n", vec->size, vec->__max_size);
    
    // Optimize memory (shrink to fit)
    vec->optimize_memory(vec);
    
    printf("After optimization - Vector size: %zu, Max size: %zu\n", vec->size, vec->__max_size);
    

    // Memory will be automatically freed when going out of scope
    return 0;
}
```

## API Reference

### Creation and Destruction
- `new_vector_TYPE()` - Create a new vector
- `sized_vector_TYPE(size_t initial_size)` - Create a new vector with specified initial capacity
- `vec->free_memory(vec)` - Free the vector and its elements
- `scoped` - Attribute for automatic cleanup when variable goes out of scope (GCC and Clang only)

### Element Access
- `vec->at(vec, index)` - Access element at index
- `vec->front(vec)` - Access first element
- `vec->back(vec)` - Access last element

### Modifiers
- `vec->push(vec, element)` - Add element to end
- `vec->pop(vec)` - Remove last element
- `vec->insert(vec, index, element)` - Insert element at index
- `vec->replace(vec, index, element)` - Replace element at index
- `vec->clear(vec)` - Remove all elements

### Capacity
- `vec->size` - Number of elements
- `vec->empty(vec)` - Check if vector is empty
- `vec->add_memory(vec)` - Add memory if needed (called automatically)
- `vec->optimize_memory(vec)` - Shrink capacity to fit current size

### Operations
- `vec->foreach(vec, function)` - Apply function to each element
- `vec->clone(vec)` - Create a deep copy of the vector

### Creating Custom Vector Types
```c
// VECTOR(type, name, constructor, destructor)
VECTOR(MyType, vector_mytype, mytype_constructor, mytype_destructor);
```


Note: README.md was autogenerated with Claude 3.7 Sonnet Thinking.
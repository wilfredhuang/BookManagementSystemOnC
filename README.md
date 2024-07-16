# Bookstore Management System on C

Bookstore Management System is a C-based application that uses a hash table to manage book records efficiently. 

It provides comprehensive functionality for creating, reading, editing, deleting, and filtering book records. The program ensures data persistence through file operations on a binary file and employs recursive techniques for memory management. 

This program is like a callback to my simple book management system that i coded in python back in my programming essential class in my first year, first semester but now in C form with improved features after my time in polytechnic.

## Overview
Menu Options

    Create Book: Add a new book to the system.
    Read Book Details: Retrieve details of books by ID, ISBN, or view all records.
    Edit Book Details: Modify details of an existing book.
    Delete Book: Remove a book from the system by ID.
    Filter Book Records: Filter books based on price range and stock quantity.
    Book Statistics: View statistics about the books in the system.
    Save and Exit: Save data to the file and exit the program.
    
## Key Features!

### Hash Table Implementation
- **Efficient Storage and Retrieval**: Uses a hash table to store and manage book records, ensuring fast search operations by book ID.
- **Collision Handling**: Linked list structure within each hash table bucket to manage collisions gracefully.

#### Hash Table Structure
The hash table is defined using an array of linked lists to handle collisions. Here’s how the structures are defined:

```c
#define TABLE_SIZE 10

typedef struct Book {
    int id;
    char title[50];
    int price;
    int stockQuantity;
    int isbn;
    struct Book* next;
} Book;

typedef struct {
    Book* table[TABLE_SIZE];
} HashTable;
```

Hash Function

The hash function calculates an index based on the book ID to place the book in the hash table:

```c
int hashFunction(int id) {
    return id % TABLE_SIZE;
}
```

Insert Book

To insert a book, I first compute the index using the hash function and then add the book to the linked list at that index:

```
void insertBook(HashTable* ht, Book* book) {
    int index = hashFunction(book->id);
    book->next = ht->table[index];
    ht->table[index] = book;
}
```
### Book Management
- **Create Book**: Add new books to the system with a unique ID, title, price, stock quantity, and ISBN.
- **Read Book Details**: Retrieve book information by ID, ISBN, or view all records.
- **Edit Book Details**: Update book information including title, price, stock quantity, and ISBN.
- **Delete Book**: Remove books from the system by ID or ISBN.

### Data Persistence
- **Save Data**: Persist book data to a binary file (`sample_data.bin`) ensuring data is not lost between sessions.
- **Load Data**: Load book data from the file upon startup to restore the previous state of the system.

### User Input Validation
- **Input Validation**: Ensures users provide valid integer and string inputs, enhancing user experience and preventing errors.

### Filtering and Statistics
- **Filter Books**: Retrieve books based on price range and stock quantity.
- **Statistics**: Generate useful statistics such as total unique books, total stock quantity, and the prices of the most and least expensive books.

### Dynamic Memory Management
- **Recursive Memory Deallocation**: Utilizes recursion in the `freeBooks` function to release memory for all book records in a linked list.

```c
// The freeBooks function uses recursion to traverse and free the memory of all books in a linked list:
// This ensures that all dynamically allocated memory is properly released, preventing memory leaks.

void freeBooks(Book* book) {
    if (book != NULL) {
        freeBooks(book->next);
        free(book);
    }
}

```

### Menu-Driven Interface
- **User-Friendly Navigation**: Provides a menu-driven interface for easy navigation and operation.

## Notes

Developed on visual studio 2022 with gcc compiler

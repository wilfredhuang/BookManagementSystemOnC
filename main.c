#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define FILENAME "sample_data.bin" // bookDB.bin OR sample_data.bin
#define TABLE_SIZE 100

// Book Struct
typedef struct {
    int id;
    char title[50];
    int price;
    int stockQuantity;  
    int isbn;           
    struct Book* next;
} Book;

// Hash Table
typedef struct {
    Book* table[TABLE_SIZE];
} HashTable;

// Function Prototypes
HashTable* createHashTable();
unsigned int hashFunction(int id);
void insertBook(HashTable* hashTable, Book book);
Book* searchBook(HashTable* hashTable, int id);
Book* searchBookByISBN(HashTable* hashTable, int isbn);
void deleteBook(HashTable* hashTable, int id);
void deleteBookByISBN(HashTable* hashTable, int isbn);
void freeHashTable(HashTable* hashTable);;
void createBook(HashTable* hashTable);
void readBookDetail(HashTable* hashTable);
void readBookByID(HashTable* hashTable);
void readBookByISBN(HashTable* hashTable);
void readAllBooks(HashTable* hashTable);
void editBookDetail(HashTable* hashTable);
void deleteBookById(HashTable* hashTable);
void deleteBookByISBNOption(HashTable* hashTable);
void filterBookRecords(HashTable* hashTable);
void showBookStatistics(HashTable* hashTable);
void saveApplication(HashTable* hashTable);
void loadApplication(HashTable* hashTable);
void printMenu();
int getValidatedIntegerInput(const char* prompt);
void getValidatedStringInput(const char* prompt, char* buffer, int bufferSize);

int bookCount = 0;
int maxID = 0; // Track the maximum ID used

int main() {
    HashTable* hashTable = malloc(sizeof(HashTable));
    if (!hashTable) {
        perror("Failed to create hash table");
        exit(EXIT_FAILURE);
    }
    memset(hashTable, 0, sizeof(HashTable));

    loadApplication(hashTable);

    int userChoice;
    // Menu Interface
    while (1) {
        printMenu();
        userChoice = getValidatedIntegerInput("Enter a integer (0-6) to proceed: ");

        switch (userChoice) {
        case 1:
            createBook(hashTable);
            break;
        case 2:
            readBookDetail(hashTable);
            break;
        case 3:
            editBookDetail(hashTable);
            break;
        case 4:
            deleteBookById(hashTable);
            break;
        case 5:
            filterBookRecords(hashTable);
            break;
        case 6:
            showBookStatistics(hashTable);
            break;
        case 0:
            printf("Saving data...\n");
            saveApplication(hashTable);
            printf("Data saved.\n");
            freeHashTable(hashTable);
            printf("Hash table freed.\n");
            printf("Closing program...\n");
            exit(EXIT_SUCCESS);
        default:
            printf("Choice is: %d \n", userChoice);
            printf("Invalid choice. Please try again.\n");
            break;
        }
    }
    return 0;
}

// Sub-program
HashTable* createHashTable() {
    HashTable* hashTable = malloc(sizeof(HashTable));
    if (!hashTable) {
        perror("Failed to create hash table");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

unsigned int hashFunction(int id) {
    return id % TABLE_SIZE;
}

void insertBook(HashTable* hashTable, Book book) {
    unsigned int index = hashFunction(book.id);
    Book* newBook = malloc(sizeof(Book));
    if (!newBook) {
        perror("Failed to allocate memory for new book");
        return;
    }
    *newBook = book;
    newBook->next = hashTable->table[index];
    hashTable->table[index] = newBook;
    if (book.id > maxID) {
        maxID = book.id; // Update maxID if the current book's ID is greater
    }
}

Book* searchBook(HashTable* hashTable, int id) {
    unsigned int index = hashFunction(id);
    Book* current = hashTable->table[index];
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Book* searchBookByISBN(HashTable* hashTable, int isbn) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable->table[i];
        while (current) {
            if (current->isbn == isbn) {
                return current;
            }
            current = current->next;
        }
    }
    return NULL;
}

void deleteBook(HashTable* hashTable, int id) {
    unsigned int index = hashFunction(id);
    Book* current = hashTable->table[index];
    Book* prev = NULL;
    while (current) {
        if (current->id == id) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                hashTable->table[index] = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void deleteBookByISBN(HashTable* hashTable, int isbn) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable->table[i];
        Book* prev = NULL;
        while (current) {
            if (current->isbn == isbn) {
                if (prev) {
                    prev->next = current->next;
                }
                else {
                    hashTable->table[i] = current->next;
                }
                free(current);
                return;
            }
            prev = current;
            current = current->next;
        }
    }
}

void freeHashTable(HashTable* hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable->table[i];
        while (current) {
            Book* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable);
}

// File Mgmt
void saveApplication(HashTable* hashTable) {
    FILE* file;
    errno_t err = fopen_s(&file, FILENAME, "wb");
    if (err != 0) {
        perror("Failed to open file for writing");
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable->table[i];
        while (current) {
            size_t written = fwrite(current, sizeof(Book), 1, file);
            if (written != 1) {
                perror("Failed to write book data to file");
                fclose(file);
                return;
            }
            current = current->next;
        }
    }

    fclose(file);
    printf("Data saved successfully.\n");
}

// File Mgmt
void loadApplication(HashTable* hashTable) {
    FILE* file;
    errno_t err = fopen_s(&file, FILENAME, "rb");
    if (err != 0) {
        perror("Failed to open file");
        return;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, file) == 1) {
        if (book.id > 0 && book.price > 0 && strlen(book.title) > 0) {
            insertBook(hashTable, book);
        }
    }

    fclose(file);
    printf("Data loaded successfully.\n");
}

void createBook(HashTable* hashTable) {
    Book book;
    // ALWAYS generate a unique ID
    do {
        book.id = maxID + 1;
        maxID++;
    } while (searchBook(hashTable, book.id) != NULL);

    getValidatedStringInput("Enter book title: ", book.title, sizeof(book.title));
    book.price = getValidatedIntegerInput("Enter book price: ");
    book.stockQuantity = getValidatedIntegerInput("Enter book stock quantity: ");
    book.isbn = getValidatedIntegerInput("Enter book ISBN: ");
    insertBook(hashTable, book);
    bookCount++;
    printf("Book data created successfully.\n");
}

void readBookDetail(HashTable* hashTable) {
    int userChoice;
    printf("Retrieve book records by:\n");
    printf("1. ID\n");
    printf("2. ISBN\n");
    printf("3. All Records\n");
    printf("Enter your choice (0-3): ");
    userChoice = getValidatedIntegerInput("");

    switch (userChoice) {
    case 1:
        readBookByID(hashTable);
        break;
    case 2:
        readBookByISBN(hashTable);
        break;
    case 3:
        readAllBooks(hashTable);
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        break;
    }
}

void readBookByID(HashTable* hashTable) {
    int id = getValidatedIntegerInput("Enter book ID to retrieve: ");
    Book* book = searchBook(hashTable, id);
    if (book) {
        printf("ID: %d, Title: %s, Price: %d, Stock Quantity: %d, ISBN: %d\n",
            book->id, book->title, book->price, book->stockQuantity, book->isbn);
    }
    else {
        printf("No book found with ID %d.\n", id);
    }
}

void readBookByISBN(HashTable* hashTable) {
    int isbn = getValidatedIntegerInput("Enter book ISBN to retrieve: ");
    Book* book = searchBookByISBN(hashTable, isbn);
    if (book) {
        printf("ID: %d, Title: %s, Price: %d, Stock Quantity: %d, ISBN: %d\n",
            book->id, book->title, book->price, book->stockQuantity, book->isbn);
    }
    else {
        printf("No book found with ISBN %d.\n", isbn);
    }
}

void readAllBooks(HashTable* hashTable) {
    int found = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable->table[i];
        while (current) {
            printf("ID: %d, Title: %s, Price: %d, Stock Quantity: %d, ISBN: %d\n",
                current->id, current->title, current->price, current->stockQuantity, current->isbn);
            current = current->next;
            found = 1;
        }
    }
    if (!found) {
        printf("No books in record.\n");
    }
}

void editBookDetail(HashTable* hashTable) {
    int id = getValidatedIntegerInput("Enter book ID to update: ");
    Book* book = searchBook(hashTable, id);
    if (!book) {
        printf("Book with ID %d not found.\n", id);
        return;
    }

    getValidatedStringInput("Enter new title: ", book->title, sizeof(book->title));
    book->price = getValidatedIntegerInput("Enter new price: ");
    book->stockQuantity = getValidatedIntegerInput("Enter new stock quantity: ");
    book->isbn = getValidatedIntegerInput("Enter new ISBN: ");
    printf("Book updated successfully.\n");
}

void deleteBookById(HashTable* hashTable) {
    int id = getValidatedIntegerInput("Enter book ID to delete: ");
    deleteBook(hashTable, id);
    printf("Book deleted successfully.\n");
}

void deleteBookByISBNOption(HashTable* hashTable) {
    int isbn = getValidatedIntegerInput("Enter book ISBN to delete: ");
    deleteBookByISBN(hashTable, isbn);
    printf("Book deleted successfully.\n");
}

// FILTER - feature
void filterBookRecords(HashTable* hashTable) {
    int userChoice;
    printf("Filter book records by:\n");
    printf("1. Price Range\n");
    printf("2. Stock Quantity Range\n");
    printf("Enter your choice: ");
    userChoice = getValidatedIntegerInput("");

    switch (userChoice) {
    case 1:
    {
        int minPrice = getValidatedIntegerInput("Enter minimum price: ");
        int maxPrice = getValidatedIntegerInput("Enter maximum price: ");
        printf("Books with price between %d and %d:\n", minPrice, maxPrice);
        for (int i = 0; i < TABLE_SIZE; i++) {
            Book* current = hashTable->table[i];
            while (current) {
                if (current->price >= minPrice && current->price <= maxPrice) {
                    printf("ID: %d, Title: %s, Price: %d, Stock Quantity: %d, ISBN: %d\n",
                        current->id, current->title, current->price, current->stockQuantity, current->isbn);
                }
                current = current->next;
            }
        }
    }
    break;
    case 2:
    {
        int minStock = getValidatedIntegerInput("Enter minimum stock quantity: ");
        int maxStock = getValidatedIntegerInput("Enter maximum stock quantity: ");
        printf("Books with stock quantity between %d and %d:\n", minStock, maxStock);
        for (int i = 0; i < TABLE_SIZE; i++) {
            Book* current = hashTable->table[i];
            while (current) {
                if (current->stockQuantity >= minStock && current->stockQuantity <= maxStock) {
                    printf("ID: %d, Title: %s, Price: %d, Stock Quantity: %d, ISBN: %d\n",
                        current->id, current->title, current->price, current->stockQuantity, current->isbn);
                }
                current = current->next;
            }
        }
    }
    break;
    default:
        printf("Invalid choice. Please try again.\n");
        break;
    }
}

void showBookStatistics(HashTable* hashTable) {
    int uniqueBooks = 0;
    int totalBooks = 0;
    int mostExpensive = 0;
    int leastExpensive = INT_MAX;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable->table[i];
        while (current) {
            totalBooks++;
            if (current->price > mostExpensive) {
                mostExpensive = current->price;
            }
            if (current->price < leastExpensive) {
                leastExpensive = current->price;
            }
            current = current->next;
            uniqueBooks++;
        }
    }

    if (leastExpensive == INT_MAX) {
        leastExpensive = 0;
    }

    printf("Total unique books: %d\n", uniqueBooks);
    printf("Total number of books: %d\n", totalBooks);
    printf("Most expensive book price: %d\n", mostExpensive);
    printf("Least expensive book price: %d\n", leastExpensive);
}

// Input validations
int getValidatedIntegerInput(const char* prompt) {
    int value;
    char buffer[10];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {
                break;
            }
        }
        printf("Invalid input. Please enter a valid integer.\n");
    }
    return value;
}

void getValidatedStringInput(const char* prompt, char* buffer, int bufferSize) {
    printf("%s", prompt);
    if (fgets(buffer, bufferSize, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';  // Remove the newline character
        }
    }
}

// Menu Interface
void printMenu() {
    printf("|----------------------------------------------------------|\n");
    printf("| Book Management System                                   |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| 1. Add a new Book                                        |\n");
    printf("| 2. Retrieve book records                                 |\n");
    printf("|    1. Retrieve by ID                                     |\n");
    printf("|    2. Retrieve by ISBN                                   |\n");
    printf("|    3. Retrieve all records                               |\n");
    printf("| 3. Update book detail                                    |\n");
    printf("| 4. Delete a Book                                         |\n");
    printf("|    1. Delete by ID                                       |\n");
    printf("|    2. Delete by ISBN                                     |\n");
    printf("| 5. Filter book records                                   |\n");
    printf("|    1. Price Range                                        |\n");
    printf("|    2. Stock Quantity Range                               |\n");
    printf("| 6. Book Statistics                                       |\n");
    printf("|----------------------------------------------------------|\n");
    printf("|                                   | (0) Save and Exit    |\n");
    printf("|----------------------------------------------------------|\n");
}


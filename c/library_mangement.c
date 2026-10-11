#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100

struct Book {
    int id;
    char title[100];
    char author[100];
    int issued;
};

struct Book books[MAX_BOOKS];
int count = 0;

void saveBooks(void) {
    FILE *fp = fopen("books.dat", "wb");

    if (fp == NULL) {
        printf("Error saving records!\n");
        return;
    }

    fwrite(&count, sizeof(int), 1, fp);
    fwrite(books, sizeof(struct Book), count, fp);
    fclose(fp);
}

void loadBooks(void) {
    FILE *fp = fopen("books.dat", "rb");

    if (fp != NULL) {
        if (fread(&count, sizeof(int), 1, fp) != 1 ||
            count < 0 || count > MAX_BOOKS ||
            fread(books, sizeof(struct Book), count, fp) !=
                (size_t)count) {
            count = 0;
        }
        fclose(fp);
    }
}

int findBook(int id) {
    for (int i = 0; i < count; i++) {
        if (books[i].id == id)
            return i;
    }
    return -1;
}

void addBook(void) {
    if (count >= MAX_BOOKS) {
        printf("Book storage is full!\n");
        return;
    }

    struct Book b;

    printf("Enter book ID: ");
    if (scanf("%d", &b.id) != 1) {
        printf("Invalid input!\n");
        return;
    }

    if (findBook(b.id) != -1) {
        printf("Book ID already exists!\n");
        return;
    }

    printf("Enter book title: ");
    scanf(" %99[^\n]", b.title);

    printf("Enter author name: ");
    scanf(" %99[^\n]", b.author);

    b.issued = 0;
    books[count++] = b;

    saveBooks();
    printf("Book added successfully!\n");
}

void displayBooks(void) {
    if (count == 0) {
        printf("No books available.\n");
        return;
    }

    printf("\n%-8s %-25s %-20s %-10s\n",
           "ID", "Title", "Author", "Status");

    for (int i = 0; i < count; i++) {
        printf("%-8d %-25s %-20s %-10s\n",
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].issued ? "Issued" : "Available");
    }
}

void searchBook(void) {
    int id;

    printf("Enter book ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        return;
    }

    int index = findBook(id);

    if (index == -1) {
        printf("Book not found.\n");
    } else {
        printf("Title: %s\n", books[index].title);
        printf("Author: %s\n", books[index].author);
        printf("Status: %s\n",
               books[index].issued ? "Issued" : "Available");
    }
}

void issueBook(void) {
    int id;

    printf("Enter book ID to issue: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        return;
    }

    int index = findBook(id);

    if (index == -1) {
        printf("Book not found.\n");
    } else if (books[index].issued) {
        printf("Book is already issued.\n");
    } else {
        books[index].issued = 1;
        saveBooks();
        printf("Book issued successfully!\n");
    }
}

void returnBook(void) {
    int id;

    printf("Enter book ID to return: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        return;
    }

    int index = findBook(id);

    if (index == -1) {
        printf("Book not found.\n");
    } else if (!books[index].issued) {
        printf("This book was not issued.\n");
    } else {
        books[index].issued = 0;
        saveBooks();
        printf("Book returned successfully!\n");
    }
}

int main(void) {
    int choice;
    loadBooks();

    do {
        printf("\n=== LIBRARY MANAGEMENT SYSTEM ===\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book\n");
        printf("4. Issue Book\n");
        printf("5. Return Book\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Restart the program.\n");
            return 1;
        }

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: printf("Goodbye!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);

    return 0;
}

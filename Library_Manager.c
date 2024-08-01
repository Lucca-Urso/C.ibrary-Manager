//Projeto: criar um gerenciador para uma biblioteca que irá organizar livros. Cada livro deve conter um nome, autor, ano de publicação, valor e se já está alugado.
//Os livros estarão nomeados em um arquivo, que deverá ser lido e as informações coletadas e separadas. Esse gerenciador deve conter as opções: 1 - Visualizar livros,
//2 - Organizar livro (deve organizar por nome, valor ou pelo ano de publicação), 3 - Alugar livro (Caso o livro já esteja alugado, a operação será inválida), 
//4 - Adicionar livro, 5 - Remover livro, 6 - Finalizar operação.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define max 150

typedef struct {
    char name[max];
    char author[max];
    int published;
    float price;
    bool taken;
} Book;

FILE* read(char file_name[max]);
FILE* rewrite(char file_name[max], Book books_array[], int size, char remove_book[]);
void append(char file_name[max], Book book);

Book* read_database();
void read_string(char s[]);

void print_books(Book books_array[max], int size);
int compare_by_name(const void* name1, const void* name2);
int compare_by_publish(const void* name1, const void* name2);
int compare_by_price(const void* name1, const void* name2);
void rent_book(Book books_array[], int size);
void add_book();
void remove_book(Book books_array[], int size);

int main (int argc, char *argv[]) {
    int operation, i = 0, n = 0, opt;
    char line[max];
    FILE* file = read("books.txt");
    Book books_array[max];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '-' && line[1] == '-') {
            continue;
        }
        
        if (line[0] == '|' && line[1] == '|') {
            break;
        }

        Book book;
        switch (i) {
            case 0:
                strcpy(book.name, strchr(line, ':') + 2);
                book.name[strcspn(book.name, "\n")] = '\0';
                break;
            case 1:
                strcpy(book.author, strchr(line, ':') + 2);
                book.author[strcspn(book.author, "\n")] = '\0';
                break;
            case 2:
                book.published = atoi(strchr(line, ':') + 2);
                break;
            case 3:
                book.price = atof(strchr(line, '$') + 1);
                break;
            case 4:
                if (strncmp(strchr(line, ':') + 2, "Yes", 3) == 0) {
                    book.taken = true;
                } else {
                    book.taken = false;
                }

                books_array[n] = book;
                n++;
                break; 
        }
        
        i++;
        if (i == 5) {
            i = 0;
        }
    }

    fclose(file); 

    while (true) {
        printf("\n====================|Library Manager|====================\n"
        "1 - View book list\n2 - Sort books\n3 - Rent book\n4 - Add book\n5 - Remove book\n6 - Exit\n=> Operation: ");
        scanf("%i", &operation);

        switch (operation) {
            case 1:
                print_books(books_array, n);
                break;

            case 2:
                printf("\n==========<Sort books>==========\n");
                printf("1 - Name\n2 - Publish Date\n3 - Price\n=> Option: ");
                scanf("%i", &opt);

                switch (opt) {
                    case 1:
                        qsort(books_array, n, sizeof(Book), compare_by_name);
                        break;
                    
                    case 2:
                        qsort(books_array, n, sizeof(Book), compare_by_publish);
                        break;

                    case 3:
                        qsort(books_array, n, sizeof(Book), compare_by_price);
                        break;
                    
                    default:
                        printf("Invalid Operation...Exiting");
                        exit(0);
                    }

                break;

            case 3:
                printf("\n==========<Rent books>==========\n");
                rent_book(books_array, n);
                break;

            case 4:
                printf("\n==========<Add books>==========\n");
                add_book();
                break;

            case 5:
                printf("\n==========<Remove books>==========\n");
                remove_book(books_array, n);
                break;

            case 6:
                printf("Exiting...");
                exit(0);
     
            default:
                printf("Invalid Operation...Exiting");
                exit(0);
        }
    }

    return 0;
}

void print_books(Book books_array[max], int size) {
    for (int i = 0; i < size; i++) {
        printf("===============<Book %i>===============\n", i);
        printf("Name: %s\nAuthor: %s\nPublished: %i\nPrice: %.2f\nAvaible: %s\n",
        books_array[i].name, books_array[i].author, books_array[i].published, books_array[i].price, (books_array[i].taken == true) ? "Yes" : "No");
    }
}

int compare_by_name(const void* name1, const void* name2) {
    return strcmp(((Book*) name1)->name, ((Book*) name2)->name);
}

int compare_by_publish(const void* name1, const void* name2) { 
    return ((Book*)name1)->published - ((Book*)name2)->published; 
}

int compare_by_price(const void* name1, const void* name2) { 
    return ((Book*)name1)->price - ((Book*)name2)->price; 
} 

void rent_book(Book books_array[], int size) {
    int n = 0;
    char opt[max];

    printf("=> Avaible books: \n");
    for (int i = 0; i < size; i++) {
        if (books_array[i].taken == false) {
            printf("%i - %s\n", n, books_array[i].name);
            n++;
        }
    }
    printf("=> Enter book name: ");
    read_string(opt);
    
    for (int i = 0; i < size; i++) {
        if (strcmp(books_array[i].name, opt) == 0) {
            books_array[i].taken = true;
        }
    }

    rewrite("books.txt", books_array, size, "");
}

void add_book() {
    Book book;
    char taken[max];

    printf("- Name: ");
    read_string(book.name);

    printf("- Author: ");
    read_string(book.author);

    printf("- Published: ");
    scanf("%i", &book.published);
    getchar();

    printf("- Price: ");
    scanf("%f", &book.price);
    getchar();

    printf("- Taken: ");
    read_string(taken);

    if (strcmp(taken, "Yes") == 0) {
        book.taken = true;
    }
    else {
        book.taken = false;
    }

    append("books.txt", book);
}

void remove_book(Book books_array[], int size) {
    char book[max];

    printf("=> Enter book name: ");
    read_string(book);

    rewrite("books.txt", books_array, size, book);
}

void read_string(char s[]) {
  setbuf(stdin,0);
  fgets(s, max, stdin);
  if(s[strlen(s) - 1] == '\n'){
    s[strlen(s) - 1] = '\0';
  }
}

void append(char file_name[max], Book book) {
    FILE* file = fopen(file_name, "a");

    if (file == NULL) {
        printf("Erro during file operation...");
        exit(0);
    }

    char content[] = "--------------------------\nNome: %s\nAutor: %s\n"
                     "Ano de Publicacao: %i\nValor: R$%.2f\nAlugado: %s\n";

    fprintf(file, content, book.name, book.author, book.published, book.price, (book.taken == true) ? "Yes" : "No");
    fclose(file);
}

FILE* rewrite(char file_name[max], Book books_array[], int size, char remove_book[]) {
    FILE* file = fopen(file_name, "w");

    if (file == NULL) {
        printf("Erro during file operation..."); 
        exit(0);
    }

    char content[] = "--------------------------\nNome: %s\nAutor: %s\n"
                     "Ano de Publicacao: %i\nValor: R$%.2f\nAlugado: %s\n";

    for (int i = 0; i < size; i++) {
        if (strcmp(books_array[i].name, remove_book) != 0) {
            fprintf(file, content, books_array[i].name, books_array[i].author, 
            books_array[i].published, books_array[i].price, (books_array[i].taken == true) ? "Yes" : "No");
        }
    }

    fclose(file);
}

FILE* read(char file_name[max]) {
    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        printf("Erro during file operation..."); 
        exit(0);
    }
    
    return file;
}



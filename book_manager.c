#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//
#define MAX_LINE 100
#define CSV_FILE "./books.csv"

struct Book
{
    int book_id;
    char isbn[100];
    char authors[100];
    int original_publication_year;
    char original_title[100];
    float average_rating;
};

int isValidInt(char *str)
{
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i]) && str[i] != '\n')
            return 0;
    return 1;
}

float isValidFloat(char *str)
{
    int dotCount = 0;
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '.')
        {
            dotCount++;
            if (dotCount > 1)
                return -1;
        }
        else if (!isdigit(str[i]) && str[i] != '\n')
            return -1;
    }
    return 1;
}

void readBooks()
{
    FILE *file = fopen(CSV_FILE, "r");
    if (!file)
    {
        perror("Could not open CSV file");
        return;
    }

    int searchChoice;
    char searchTerm[100];

    printf("\nSearch by:\n");
    printf("1. Author\n2. Title\n3. Publication Year\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &searchChoice);
    getchar(); // Clear newline

    printf("Enter search term: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0'; // Trim newline

    char line[MAX_LINE];
    struct Book b;

    // Skip header line
    fgets(line, sizeof(line), file);
    int found = 0;

    printf("\n--- Matching Books ---\n");

    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        if (!token)
            continue;
        b.book_id = atoi(token);

        // Skip 4 fields: goodreads_book_id, best_book_id, work_id, books_count
        for (int i = 0; i < 4; i++)
        {
            token = strtok(NULL, ",");
            if (!token)
                break;
        }

        token = strtok(NULL, ",");
        if (!token)
            continue;
        strncpy(b.isbn, token, sizeof(b.isbn) - 1);
        b.isbn[sizeof(b.isbn) - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token)
            continue;
        strncpy(b.authors, token, sizeof(b.authors) - 1);
        b.authors[sizeof(b.authors) - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token)
            continue;
        b.original_publication_year = atoi(token);

        token = strtok(NULL, ",");
        if (!token)
            continue;
        strncpy(b.original_title, token, sizeof(b.original_title) - 1);
        b.original_title[sizeof(b.original_title) - 1] = '\0';

        // Skip language_code
        token = strtok(NULL, ",");
        if (!token)
            continue;

        // Get average_rating
        token = strtok(NULL, ",");
        if (!token)
            continue;
        b.average_rating = atof(token);

        int match = 0;
        switch (searchChoice)
        {
        case 1:
            if (strstr(b.authors, searchTerm) != NULL)
                match = 1;
            break;
        case 2:
            if (strstr(b.original_title, searchTerm) != NULL)
                match = 1;
            break;
        case 3:
            if (atoi(searchTerm) == b.original_publication_year)
                match = 1;
            break;
        default:
            printf("Invalid choice.\n");
            fclose(file);
            return;
        }

        if (match)
        {
            found = 1;
            printf("---------------------");
            printf("\nBook ID: %d\n", b.book_id);
            printf("Title: %s\n", b.original_title);
            printf("Author(s): %s\n", b.authors);
        }
        if (match)
        {
            char yesorno;
            printf("\n\n\ndo you want to see a extended version? press y to continue, any other key to skip: ");
            scanf(" %c", &yesorno);
            if (yesorno == 'y' || yesorno == 'Y')
            {

                printf("\nBook ID: %d\n", b.book_id);
                printf("Title: %s\n", b.original_title);
                printf("Author(s): %s\n", b.authors);
                printf("Year: %d\n", b.original_publication_year);
                printf("ISBN: %s\n", b.isbn);
                printf("Rating: %.2f\n", b.average_rating);
            }
            else
            {
                printf("\nOkay! no worries\n");
            }
        }
        if (match)
        {
            char yesorno;
            printf("\n\n\n\n\n\ndo you want to see the overview version again? press y to continue, any other key to skip: ");
            scanf(" %c", &yesorno);
            if (yesorno == 'y' || yesorno == 'Y')
            {

                printf("\nBook ID: %d\n", b.book_id);
                printf("Title: %s\n", b.original_title);
                printf("Author(s): %s\n", b.authors);
            }
            else
            {
                printf("\nOkay! no worries\n");
            }
        }
    }

    if (!found)
    {
        printf("No matching entries found.\n");
    }

    fclose(file);
}

// Function to save a new book to CSV
void savingmode()
{
    struct Book b;
    char buffer[100];
    char choice;

    FILE *file = fopen(CSV_FILE, "a");
    if (!file)
    {
        perror("Could not open file for appending");
        return;
    }

    do
    {
        printf("\n--- Add a New Book ---\n");

        // Book ID
        printf("Enter Book ID: ");
        fgets(buffer, sizeof(buffer), stdin);
        while (!isValidInt(buffer))
        {
            printf("Invalid. Enter Book ID again: ");
            fgets(buffer, sizeof(buffer), stdin);
        }
        b.book_id = atoi(buffer);

        // ISBN
        printf("Enter ISBN: ");
        fgets(b.isbn, sizeof(b.isbn), stdin);
        b.isbn[strcspn(b.isbn, "\n")] = '\0'; // Trim newline

        // Author(s)
        printf("Enter Author(s): ");
        fgets(b.authors, sizeof(b.authors), stdin);
        b.authors[strcspn(b.authors, "\n")] = '\0';

        // Publication Year
        printf("Enter Publication Year: ");
        fgets(buffer, sizeof(buffer), stdin);
        while (!isValidInt(buffer))
        {
            printf("Invalid. Enter year again: ");
            fgets(buffer, sizeof(buffer), stdin);
        }
        b.original_publication_year = atoi(buffer);

        // Title
        printf("Enter Book Title: ");
        fgets(b.original_title, sizeof(b.original_title), stdin);
        b.original_title[strcspn(b.original_title, "\n")] = '\0';

        // Average Rating
        printf("Enter Average Rating: ");
        fgets(buffer, sizeof(buffer), stdin);
        while (isValidFloat(buffer) == -1)
        {
            printf("Invalid. Enter rating again: ");
            fgets(buffer, sizeof(buffer), stdin);
        }
        b.average_rating = atof(buffer);

        // Write to file
        fprintf(file, "%d,0,0,0,0,%s,%s,%d,%s,eng,%.2f,0\n",
                b.book_id, b.isbn, b.authors,
                b.original_publication_year, b.original_title,
                b.average_rating);

        printf("Book added successfully!\n");

        // Ask if user wants to add another
        printf("Would you like to add another book? (y/n): ");
        choice = getchar();
        while (getchar() != '\n')
            ; // clear any extra input from buffer

    } while (choice == 'y' || choice == 'Y');

    fclose(file);
}

// Main function (starts the program)
int main()
{
    int choice;
    while (1)
    {
        printf("\n--- Book CSV Manager ---\n");
        printf("1. Add new book\n");
        printf("2. View all books\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline left by scanf

        switch (choice)
        {
        case 1:
            savingmode();
            break;
        case 2:
            readBooks();
            break;
        case 3:
            printf("Exiting.\n");
            exit(0);
        default:
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

//to run the code input in the terminal:
// gcc book_manager.c -o book_manager
// ./book_manager

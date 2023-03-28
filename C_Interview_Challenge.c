#include <stdio.h>
#include <string.h>
#include <strings.h>

char* user_sentence_input();
void parse_user_sentence(char *user_sentence);
struct Word *sort_using_links(struct Word *start);
void search_for_word(struct Word *head);
struct Word *find_word_by_first_four_casesens(struct Word *head, char search_word[4]);
struct Word *find_word_by_first_four_incasesens(struct Word *head, char search_word[4]);
struct Word *ask_add_word(struct Word **head);
void add_word(struct Word **head, char* new_word);
void ask_delete_list(struct Word *head);
void delete_list(struct Word **head);

// Create struct to store word data
struct Word {
    char full_word[50];
    char first_four[4];
    int word_length;
    struct Word* next;
};
struct Word words[100];
int word_index = 0;

int main(void) {

    // Get user input.
    char user_sentence[500];
    user_sentence_input(user_sentence);

    // Set pointer called head, to address of first element in words array.
    struct Word *head = &words[0];

    // Parse sentence into words.
    parse_user_sentence(user_sentence);

    // Sort list alphabetically. Set pointer called new head to linked list returned by sortby function.
    struct Word *new_head = sort_using_links(head);

    // TEST: Print the words in the correct order using the new head pointer
    // struct Word *current = new_head;
    // while (current != NULL) {
    //     printf("\n Word: %s, %i, %c\n", current->full_word, current->word_length, current->first_four[0]);
    //     current = current->next;
    // }

    // Ask if want to search, if so, search through linked list looking for user-entered substring.
    search_for_word(new_head);

    // Ask if want to add a word to list, if so, add a word.
    ask_add_word(&new_head);

    // Ask if want to delete the list, if so, clear list.
    ask_delete_list(new_head);

  return 0;
}


// Get user input.
char* user_sentence_input(char* user_sentence) {
    do {
        printf("Enter a sentence(500 characters or less): ");
        fgets(user_sentence, 500, stdin);
        if (strlen(user_sentence) > 501) {
            printf("Sentence over 500 characters \n");
        }
    }
    while (strlen(user_sentence) > 501);
    return user_sentence;
}

void parse_user_sentence(char *user_sentence) {

    int four_letters_index = 0;
    int letter_index = 0;

    for (int i = 0; i < strlen(user_sentence); i++) {
        // If char is not a space, comma, or apostrophe
        if (user_sentence[i] != ' ' && user_sentence[i] != ',' && user_sentence[i] != '"' && user_sentence[i] != 39 && user_sentence[i] != '\n') {
            // Add to current word count
            words[word_index].word_length++;

            // If four letters not complete, add letter to first four
            if (four_letters_index < 4) {
                words[word_index].first_four[four_letters_index] = user_sentence[i];
                four_letters_index++;
            }

            // Add current letter to current words.full_word
            words[word_index].full_word[letter_index] = user_sentence[i];
            letter_index++;
        }
        else if (user_sentence[i] == '?' || user_sentence[i] == '.' || user_sentence[i] == '!' || user_sentence[i] == '\n') {
            word_index++;
            break;
        }
        else if (user_sentence[i] == ' ') {
            // Set current word's next pointer to address of next index.
            words[word_index].next = &words[word_index + 1];
            word_index++;
            letter_index = 0;
            four_letters_index = 0;
            // When word done, set next node to NULL.
            words[word_index].next = NULL;
        }
    }
    words[word_index].next = NULL;
}

struct Word *sort_using_links(struct Word *start) {
    struct Word *end, *r, *p, *q, *temp;
    // Outer loop runs through linked list until all nodes sorted.
    for(end = NULL; end != start->next; end = p) {
        // Inner loop compares and swaps nodes
        for(r = p = start; p->next != end; r = p, p = p->next) {
            q = p->next;
            if(strcmp(p->full_word, q->full_word) > 0)
            {
                // Current node's next pointer swapped with "next's next" pointer
                p->next = q->next;
                // "Next's next" pointer re-directed to current node.
                q->next = p;
                if (p != start){
                    r->next = q;
                }
                else
                    start = q;
                temp = p;
                p = q;
                q = temp;
            }
        }
    }
    return start;
}

struct Word *find_word_by_first_four_incasesens(struct Word *head, char search_word[4]) {
    struct Word *current = head;
    while (current != NULL) {
        if (strcasecmp(search_word, current->first_four) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // If no match found.
}

struct Word *find_word_by_first_four_casesens(struct Word *head, char search_word[4]) {
    struct Word *current = head;
    while (current != NULL) {
        if (strcmp(search_word, current->first_four) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // If no match found.
}

void search_for_word(struct Word *head) {

    char input;

    printf("Would you like to search for a word? y/n: ");
    scanf(" %c", &input);

    if (input == 'y') {

        char search_word[4];
        char casesens;
        printf("Enter up to four letters to search for a word: ");
        scanf("%s", search_word);
        printf("Would you like the search to be case-sensitive? y/n: ");
        scanf(" %c", &casesens);
        if (casesens == 'y') {
            struct Word *found_word = find_word_by_first_four_casesens(head, search_word);
            if (found_word != NULL) {
                printf("The word found is: %s \n", found_word->full_word);
                search_for_word(head);
            } else {
                printf("No word found with the given first four letters(or user entered more than 4 letters to search by). \n");
                search_for_word(head);
            }
        }
        else if (casesens == 'n') {
            struct Word *found_word = find_word_by_first_four_incasesens(head, search_word);
            if (found_word != NULL) {
                printf("The word found is: %s \n", found_word->full_word);
                search_for_word(head);
            } else {
                printf("No word found with the given first four letters(or user entered more than 4 letters to search by). \n");
                search_for_word(head);
            }
        }
    }
    else if (input == 'n') {
        ask_add_word(&head);
    }
}

struct Word *ask_add_word(struct Word **head) {
    // Ask to add a word
    char input;
    char word[100];

    printf("Would you like to enter a word to the list? y/n: ");
    scanf(" %c", &input);

    if (input == 'y') {
        printf("Please enter a word: ");
        scanf("%s", word);
        printf("Added %s to list. \n", word);

        add_word(head, word);
        struct Word *new_head = sort_using_links(*head);
      
        // TEST: Print the words in the correct order using the new head pointer
        // struct Word *current2 = new_head;
        // while (current2 != NULL) {
        //     printf("\n Word: %s, %i, %c\n", current2->full_word, current2->word_length, current2->first_four[0]);
        //     current2 = current2->next;
        // }
      
        search_for_word(new_head);
    }
    else {
        printf("Would you like to delete the list? y/n: ");
        scanf(" %c", &input);
        if (input == 'y') {
            delete_list(head);
            return *head;
        }
        else if (input == 'n') {
            search_for_word(*head);
            return *head;
        }
    }
    return *head;
}

void add_word(struct Word **head, char* new_word) {
    // int i = 1;
    // If head is NULL, it is an empty list
    if(*head == NULL)
        *head = &words[0];  // Point head back towards address of first words element.
    //Otherwise, find the last node and add the newNode
    else
    {
        struct Word *lastNode = *head;

        //last node's next address will be NULL. Find last node
        while(lastNode->next != NULL)
        {
            lastNode = lastNode->next;
            // i++;
        }

        //add the newNode at the end of the linked list
        lastNode->next = &words[word_index];
    }

    // Copy the new word into the full_word field
    strcpy(words[word_index].full_word, new_word);
    // Copy the first four characters of the new word into the first_four field
    strncpy(words[word_index].first_four, new_word, 4);
    // Set the word_length field
    words[word_index].word_length = strlen(new_word);
    // Set the next pointer to point to the next non-empty spot in the array
    words[word_index].next = NULL;
    word_index++;
}


void ask_delete_list(struct Word *head) {
    char input;
    printf("Would you like to delete the list? y/n: ");
    scanf(" %c", &input);

    if (input == 'y') {
        delete_list(&head);
    }
    else if (input == 'n') {
        search_for_word(head);
    }
}


void delete_list(struct Word **head) {
    struct Word *temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        temp->next = NULL;
        temp = NULL;
    }
    *head = NULL;
    word_index = 0;
    ask_add_word(head);
}


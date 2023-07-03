#include <stdio.h>
#include "document.h"
#include <string.h>
#include <ctype.h>
/*
Yanit Gebrewold
UID: 177866933
DID: yanitgeb
*/

/*This function will take in a Document struct and initialize it appropriately.
The document's name will be initialized using the name parameter in this
function. It will iterate through every paragraph and line and fill it null
bytes. If doc is null, name is null, or length of name is greater than
Max string size, do nothing and return failure */
int init_document(Document *doc, const char *name) {

    if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE) {
        return FAILURE;
    }
    else {
        int i, j, k;

        strcpy(doc->name, name); /* Copies doc name into variable inside doc */
        doc->number_of_paragraphs = 0;
        for (i = 0; i < MAX_PARAGRAPHS; i++) { 
            /* number of lines starts at 0 */
            doc->paragraphs[i].number_of_lines = 0;
            for (j = 0; j < MAX_PARAGRAPH_LINES; j++) {
                for (k = 0; k < MAX_STR_SIZE + 1; k++) {
                    doc->paragraphs[i].lines[j][k] = '\0'; /*initializes all 
                                                            lines with
                                                            null bytes */
                }
            }
        }
        return SUCCESS;
    }
}

/*This function will reset a document by simply changing the number of
paragraphs to equal zero */
int reset_document(Document *doc) {
    if (doc == NULL) {
        return FAILURE;
    }
    else {
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
}

/* This function will print the information inside the Document struct according
to the appropriate formatting of information. The name of the doc will be
printed along with how many paragraphs there are and each line within those
paragraphs */
int print_document(Document *doc) {
    if (doc == NULL) {
        return FAILURE;
    }
    else {
        int i, j;

        printf("Document name: \"%s\"\n", doc->name);
        printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
        for (i = 0; i < doc->number_of_paragraphs; i++) {
            for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
                printf("%s \n", doc->paragraphs[i].lines[j]);
            }
            /* Print new line after each paragraph until the ending paragraph */
            if (i < doc->number_of_paragraphs - 1) {
                printf("\n");
            }
        }
        return SUCCESS;
    }
}

/* This function will add a paragraph after the specified paragraph number
inside the parameter. If the paragraph_number is 0 or less than the current
amount of paragraphs, some shifting upwards will have to be done to allow for
some space for the new paragraph. Once the shifting is complete, the number of
lines in the paragraph will be zero and then the lines arrray in the paragraph
will be initialized with null bytes to avoid overwriting previous data*/
int add_paragraph_after(Document *doc, int paragraph_number) {
    int i, j, k;

    if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS ||
        paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    else {

        /* shifts existing paragraphs after paragraph number to the right */
        for (i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--) {
            doc->paragraphs[i + 1] = doc->paragraphs[i];
        }

        /* sets every line in the new paragraph with null bytes for later */
        for (j = 0; j < MAX_PARAGRAPH_LINES; j++) {
            for (k = 0; k < MAX_STR_SIZE + 1; k++) {
                doc->paragraphs[paragraph_number].lines[j][k] = '\0';
            }
        }

        doc->number_of_paragraphs++;
        doc->paragraphs[paragraph_number].number_of_lines = 0;

        return SUCCESS;
    }
}

/* This function will add a line after the specified line_number of a document
in the specified paragraph. If the line is to be added in the middle of existing
lines, then it will shift all of the other lines down to add the new line in. */
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {
    int i;

    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        doc->paragraphs[paragraph_number - 1].number_of_lines == 
        MAX_PARAGRAPH_LINES || 
        line_number > doc->paragraphs[paragraph_number - 1].number_of_lines ||
        new_line == NULL) {
        return FAILURE;
    }
    else {
        /* Shift the existing lines after line number down */
        for (i = doc->paragraphs[paragraph_number - 1].number_of_lines - 1;
             i >= line_number; i--) {
            strcpy(doc->paragraphs[paragraph_number - 1].lines[i + 1],
                   doc->paragraphs[paragraph_number - 1].lines[i]);
        }
        /* increase number of lines */
        doc->paragraphs[paragraph_number - 1].number_of_lines++;

        for (i = 0; i < MAX_STR_SIZE + 1; i++) {
            doc->paragraphs[paragraph_number - 1].lines[line_number][i] = '\0';
        }

        /* Copy the new line into the line_number position */
        strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], 
                new_line);
        return SUCCESS;
    }
}

/* This function will change the number of lines parameter to contain 
the value of number of lines in the specified paragraph number in the function 
parameter */
int get_number_lines_paragraph(Document *doc, int paragraph_number, 
                                int *number_of_lines) {
    int num_of_lines;

    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        number_of_lines == NULL) {
        return FAILURE;
    }

    num_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;

    *number_of_lines = num_of_lines;

    return SUCCESS;
}

/* This function will append a line to the specified paragraph. This means it 
will append itself to already existing lines if there are some. For the sake 
of efficiency and because of the way the paragraphs and lines are assigned into
the arrays, we can simply copy the new line into an index indicated by number
of lines. */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        doc->paragraphs->number_of_lines == MAX_PARAGRAPH_LINES ||
        new_line == NULL) {
        return FAILURE;
    }

    strcpy(doc->paragraphs[paragraph_number - 1].
            lines[doc->paragraphs[paragraph_number - 1].
            number_of_lines], new_line);

    doc->paragraphs[paragraph_number - 1].number_of_lines++;

    return SUCCESS;
}

/* This function will remove a line at the specified line number. It will move 
all remaining lines upwards to fill in the gap */
int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i, j;

    if (doc == NULL || paragraph_number > doc->number_of_paragraphs ||
        line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
        return FAILURE;
    }

    /* It will set the line at the position to all null bytes so that the new
    lines can be copied over */
    for (j = 0; j < MAX_STR_SIZE + 1; j++) {
        doc->paragraphs[paragraph_number - 1].lines[line_number - 1][j] = '\0';
    }

    /* The shifting of the lines will occur to fill in the gap */
    for (i = line_number - 1;
         i < doc->paragraphs[paragraph_number - 1].number_of_lines - 1; i++) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i],
               doc->paragraphs[paragraph_number - 1].lines[i + 1]);
    }

    /* Set the last index with null bytes to free up the data space and prevent
    overwriting with strcpy in the future */
    for (j = 0; j < MAX_STR_SIZE + 1; j++) {
        doc->paragraphs[paragraph_number - 1].lines[i][j] = '\0';
    }

    doc->paragraphs[paragraph_number - 1].number_of_lines--;

    return SUCCESS;
}

/* This funciton will load in paragraphs and lines from a 2 dimensional array
filled with lines. Each time there is an empty string, that indicates that there
should be a new paragraph. There is no guarantee the document will be loaded
in from the beginning of an empty document, so I will call add_paragraph and 
append_line to make sure it doesn't overwrite any existing important information
in the document */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
    int i;

    if (doc == NULL || data == NULL || data_lines == 0) {
        return FAILURE;
    }

    /* adds a new paragraph to open up space for loading the document */
    add_paragraph_after(doc, doc->number_of_paragraphs);

    /* adds a new paragraph every time an empty string is found in data and 
    begins inputting the lines from the new paragraph number*/
    for (i = 0; i < data_lines; i++) {
        if (strcmp(data[i], "") == 0) {
            add_paragraph_after(doc, doc->number_of_paragraphs);
        }
        else {
            append_line(doc, doc->number_of_paragraphs, data[i]);
        }
    }

    return SUCCESS;
}

/* This function will replace the target text with the replacement text 
everywhere it appears in the document. It will search through every paragraph 
and line to find the target text to replace it. At appropriate times, the arrays
will be reset to null bytes to ensure safety against writing over existing 
text. */

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j, s, l, o, counter, holder_value, len, k = 0, entry = 0;
    int word_num, letters, value, word_count = 0;
    char holder[MAX_STR_SIZE + 1] = "", string_cat[MAX_STR_SIZE + 1] = "";
    char temp[MAX_STR_SIZE + 1] = "";
    char *target_ptr, *found_ptr;
    char words[MAX_STR_SIZE + 1];

    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }

    /* initialize the arrays with null bytes and begin by creating an array that
    contains all the individual words in a sentence string to make it easier to
    work with them */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            for (l = 0; l < MAX_STR_SIZE + 1; l++) {
                holder[l] = '\0';
                temp[l] = '\0';
                words[l] = '\0';
            }

            strcpy(words, doc->paragraphs[i].lines[j]);

            /* reset the values */
            word_num = 0, word_count = 1;
            entry = 0;

            len = strlen(words);

            /* resets the array to null bytes to avoid overwriting */
            for (s = 0; s < MAX_STR_SIZE + 1; s++) {
                string_cat[s] = '\0';
            }

            /* This block of code will go through all the words and begin the 
            process of replacing each iteration of target within each word that 
            contains it */
            target_ptr = words;
            holder_value = 0;
            letters = 0;
            for (counter = 0; counter < len; counter++) {
                if (strstr(target_ptr, target) != NULL) {
                     /* update found_ptr */
                    found_ptr = strstr(target_ptr, target); 
                    while (target_ptr != found_ptr) {
                        /* add what's not considered as the target*/
                        holder[holder_value++] = words[letters++]; 
                        target_ptr++;
                    }
                        
                    /* jump over to the next position without target text */
                    target_ptr += strlen(target); 

                    letters += strlen(target);
                    strncpy(holder + holder_value, replacement, 
                            strlen(replacement));
                    holder_value += strlen(replacement);
                    
                }
                else {
                    strcpy(holder + holder_value, target_ptr);
                    break;
                }

            }

            for (s = 0; s < MAX_STR_SIZE + 1; s++) {
                words[s] = '\0';
            }

            /* copies whatever is in holder to words array combine all the words 
            and spaces appropriately */
            strcpy(words, holder);

            /* resets the arrays to be reused soon */
            for (s = 0; s < MAX_STR_SIZE + 1; s++) {
                temp[s] = '\0';
                holder[s] = '\0';
                doc->paragraphs[i].lines[j][s] = '\0';
            }

            /* copy everything in the line when the work is done */
            strcpy(doc->paragraphs[i].lines[j], words);
        }
    }

    return SUCCESS;
}

/* This function will highlight the specified target word wherever it is in the
lines in the document. It will call the replace_text function to help make
the highlighting easier by simply replacing the target word with the highlight
brackets concatenated to it. */

int highlight_text(Document *doc, const char *target) {
    char string_cat[MAX_STR_SIZE] = ""; /* CHANGE BACK TO HIGHLIGHT_START_STR */
    const char *replacement;

    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    /* Concatenate string_cat with the "[", target, and "]" to produce a
    "highlighted" text and use that as replacement for the target to make
    highlighting easier */
    strcpy(string_cat, HIGHLIGHT_START_STR);
    strcat(string_cat, target);
    strcat(string_cat, HIGHLIGHT_END_STR);

    replacement = string_cat;

    replace_text(doc, target, replacement);

    return SUCCESS;
}

/* This funciton will remove a particular text by replacing it with an empty
string. It will call the replace_text function to make things easier so that
we can simply replace the target with an empty string to act as though we 
removed the text */
int remove_text(Document *doc, const char *target) {
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }

    replace_text(doc, target, "");

    return SUCCESS;
}

/* This function is similar to load_document, but the data is loaded instead 
from an array. A blank line will begin a new paragraph. The functionr returns 
FAILURE if doc is null, filename is null, or the number of paragraphs would 
exceed MAX_PARAGRAPHS; otherwise the function returns SUCCESS. If the number of 
paragraphs would exceed MAX_PARAGRAPHS, the function adds paragraphs until the 
maximum limit is reached. */
int load_file(Document *doc, const char *filename) {
    FILE *stream;
    char buffer[MAX_STR_SIZE + 1] = "";
    char string[MAX_STR_SIZE + 1] = "";
    char data[MAX_STR_SIZE + 1][MAX_STR_SIZE + 1];
    char holder[MAX_STR_SIZE + 1] = "";
    int i, string_len, index = 0, num_of_data_lines = 0;
    int starting_paragraph = 0;

    if (doc == NULL || filename == NULL || 
        doc->number_of_paragraphs == MAX_PARAGRAPHS) {
        return FAILURE;
    }

    stream = fopen(filename, "r");

    /* if file does not exist */
    if (stream == NULL) {
        return FAILURE; 
    }

    /* adds a new paragraphs automatically */
    if (add_paragraph_after(doc, starting_paragraph++) == FAILURE) {
        return FAILURE;
    }

    /* reads through the whole file and places the information inside the 
    array of data */
    while (fgets(buffer, MAX_STR_SIZE + 1, stream) != NULL) {
        num_of_data_lines++;
        string_len = strlen(buffer);
        buffer[string_len - 1] = '\0';
        strcpy(data[index++], buffer);
    }

    /*adds a new paragraph every time an empty string is found in data and 
    begins inputting the lines from the new paragraph number. */
    for (i = 0; i < num_of_data_lines; i++) {
        if (strcmp(data[i], "") == 0 || strcmp(data[i], "\n") == 0 || 
            sscanf(data[i], " %s", holder) == EOF || isspace(data[i][0]) != 0){
            /* calls add_paragraph_after to add the data, but if it returns 
            FAILURE, the whole function load_file returns FAILURE*/
            if (add_paragraph_after(doc, starting_paragraph++) == FAILURE) {
                fclose(stream);
                return FAILURE;
            }
        }
        else {
            /* calls append_line to add the data, but if it returns FAILURE, the
            whole function load_file returns FAILURE*/
            if (append_line(doc, starting_paragraph, data[i]) == FAILURE) {
                fclose(stream);
                return FAILURE;
            }
        }
    }

    fclose(stream); /* closes the file stream */
    return SUCCESS;
}

/* This function prints the paragraphs associated with the specified document
to the specified file (overwriting the file). Each paragraphs is separated by a 
newline. The function returns FAILURE if doc is NULL, filename is NULL, or file
cannot be opened; otherwise the function returns SUCCESS. */
int save_document(Document *doc, const char *filename) {
    FILE *stream;
    int i, j;

    if (doc == NULL || filename == NULL) {
        return FAILURE;
    }

    stream = fopen(filename, "w");

    /* if the file does not exist, the function fails*/
    if (stream == NULL) {
        return FAILURE;
    }

    /* prints all data to the file that is named */
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            fprintf(stream, "%s \n", doc->paragraphs[i].lines[j]);
        }

        /* Print new line after each paragraph until the ending paragraph */
        if (i < doc->number_of_paragraphs - 1) {
            fprintf(stream, "\n");
        }
    }

    fclose(stream); /* closes the file stream */

    return SUCCESS;
}

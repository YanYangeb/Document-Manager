#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include "document.h"
#include <ctype.h>

#define MAX_COMMAND_LENGTH 1024

/*
Yanit Gebrewold
UID: 177866933
DID: yanitgeb
*/

/* In this function, we will process user input. Depending on the number of 
commands given, the process will occur a little bit differently. If there is 
just no input after the executable file call, then we will read from standard 
input and create a prompt with ">". If there an input after the executable, 
we will check if the input is a valid file and then read from it. */
int main(int argc, char *argv[]) {

    Document doc;
    const char *doc_name = "main_document";
    int counter = 0; 
    int string_len, function_len, i;

    char command[MAX_COMMAND_LENGTH + 1] = "";
    char function_name[MAX_STR_SIZE + 1] = "";
    

    FILE *stream;

    /* if there is one argument (executable), we read from standard input */
    if (argc == 1) {
        stream = stdin;
        printf("> ");

        /* process the first command given */
        if (fgets(command, MAX_COMMAND_LENGTH + 1, stream) == NULL) {
            exit(EXIT_SUCCESS);
        }

        /* removes the "/n" at the end of the command */
        string_len = strlen(command);
        command[string_len - 1] = '\0';
    }
    /* if there is one argument after the executable, we will try to open the
    file to read and if it cannot be opened, we create an error message */
    else if (argc == 2) {
        stream = fopen(argv[1], "r");
        if (stream == NULL) {
            fprintf(stderr, "%s cannot be opened. \n", argv[1]);
            exit(EX_OSERR);
        }

        /* process first command and checks if fgets returns NULL */
        if (fgets(command, MAX_COMMAND_LENGTH + 1, stream) == NULL) {
            exit(EXIT_SUCCESS);
        }

        /* removes the "/n" at the end of the command */
        string_len = strlen(command);
        command[string_len - 1] = '\0';
    }

    /* If there is more argument that asked for, it will print this error 
    message to standard error and exit with the exit command EX_USAGE */
    else {
        fprintf(stderr, "Usage: user_interface \n");
        fprintf(stderr, "Usage: user_interface <filename> \n"); 
        exit(EX_USAGE);
    }

    init_document(&doc, doc_name); /* intializes the document */

    /* while the command given is not quit or exit */
    while (strcmp(command, "quit") && strcmp(command, "exit")) {

        /* This will reset the function name to null to prevent overwriting */
        function_len = strlen(function_name);
        for (i = 0; i < function_len; i++) {
            function_name[i] = '\0';
        }

        /* if the command line is not empty */
        if (sscanf(command, " %s", function_name) != EOF 
            || strcmp(command, "\n") != 0 || strcmp(command, "") != 0) {
            
            
            if (strcmp(function_name, "add_paragraph_after") == 0) {
                add_paragraph_after_process(&doc, function_name, command);
            }
           
            else if (strcmp(function_name, "add_line_after") == 0) {
                add_line_after_process(&doc, function_name, command);
            }

            else if (strcmp(function_name, "append_line") == 0) {
                append_line_process(&doc, function_name, command);
            }

            else if (strcmp(function_name, "print_document") == 0) {
                print_document_process(&doc, function_name, command);
            }
            else if (strcmp(function_name, "quit") == 0) {
                char holder[MAX_STR_SIZE + 1] = "";
                counter = sscanf(command, " %s %s", function_name, holder);
                
                /* checks if there are more inputs than asked */
                if (counter != 1) {
                    printf("Invalid Command \n");
                }
                else {
                    /* exit the program */
                    exit(EXIT_SUCCESS); 
                    break;
                }
                
            }

            else if (strcmp(function_name, "exit") == 0)
            {
                char holder[MAX_STR_SIZE + 1] = "";
                counter = sscanf(command, " %s %s", function_name, holder);
                /* checks if there are more inputs than asked */
                if (counter != 1) {
                    printf("Invalid Command \n");
                }
                else {
                    /* exit the program */
                    exit(EXIT_SUCCESS); 
                    break;
                }
            }
            else if (strcmp(function_name, "remove_line") == 0) {
                remove_line_process(&doc, function_name, command);
            }
            else if (strcmp(function_name, "load_file") == 0) {
                load_file_process(&doc, function_name, command);
            }
            else if (strcmp(function_name, "replace_text") == 0) {
                replace_text_process(&doc, function_name, command);
            }
            else if (strcmp(function_name, "highlight_text") == 0) {
                highlight_text_process(&doc, command);
            }
            else if (strcmp(function_name, "remove_text") == 0) {
               remove_text_process(&doc, command);    
            }
            else if (strcmp(function_name, "save_document") == 0) {
                save_document_process(&doc, function_name, command);
            }
            else if (strcmp(function_name, "reset_document") == 0) {
                reset_document_process(&doc, function_name, command);
            }
            /* if it finds #, do nothing */
            else if (strcmp(function_name, "#") == 0) {
                ; /* do nothing */
            }
            else {
                if (sscanf(command, " %s", function_name) == EOF || 
                    strcmp(command, "") == 0 || strcmp(command, "\n") == 0) {
                    ; /* do nothing */
                }
                else {
                    /* checks for a pound key if it is concatenated to a word */
                    char pound_key;
                    sscanf(function_name, " %c", &pound_key);
                    if (pound_key != '#')
                    {
                        printf("Invalid Command \n");
                    }
                }
            }
        }

        /* if there is just one argument and we read from input */
        if (argc == 1) {
            printf("> ");
        }

        /* scans the next line */
        if (fgets(command, MAX_COMMAND_LENGTH + 1, stream) == NULL) {
            exit(EXIT_SUCCESS);
            break;
        }

        /* gets rid of the \n at the end of the command */
        string_len = strlen(command);
        if (command[string_len - 1] == '\n') {
            command[string_len - 1] = '\0';
        }
    }

    fclose(stream); /* close the stream */

    return 0;
}

/* This helper method will help process the information needed to call the 
add paragraph function using a command */
int add_paragraph_after_process(Document *doc, char *function_name, 
                                char *command) {
    int counter, paragraph_number;
    char holder[MAX_STR_SIZE + 1] = "";

    counter = sscanf(command, " %s %d %s", function_name, &paragraph_number, 
                    holder); 

    /* makes sure the user put the command correctly */
    if (counter != 2 || paragraph_number < 0) {
        printf("Invalid Command \n");
    }
    else if (add_paragraph_after(doc, paragraph_number)  == FAILURE) {
         printf("%s failed \n", function_name);
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
add line after function using a command */
int add_line_after_process(Document *doc, char *function_name, char *command) {
    char new_line[MAX_STR_SIZE + 1] = "";
    int counter, line_number, paragraph_number;
    char *line;

    counter = sscanf(command, " %s %d %d %s", function_name, 
                    &paragraph_number, &line_number, new_line);
    /* makes sure command argument count is 4 and paragraph number
    and line number are less than 0 (or equal for paragraph num)*/                
    if (counter != 4 || paragraph_number <= 0 || line_number < 0) {
        printf("Invalid Command \n");
    }
    else {
        /* finds the occurrence of an "*" */
        line = strstr(command, "*");
                    
        if (line != NULL) {
            line++; /* pass over the first quotation mark */
            strcpy(new_line, line);

            if (add_line_after(doc, paragraph_number, line_number, 
                                new_line) == FAILURE) {
                printf("%s failed \n", function_name);
            }
        }
        else {
            printf("Invalid Command \n");
        }
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
append line function using a command */
int append_line_process(Document *doc, char *function_name, char *command) {
    int counter, paragraph_number;
    char new_line[MAX_STR_SIZE + 1] = "";
    char *line;

    counter = sscanf(command, " %s %d %s", function_name, 
                    &paragraph_number, new_line);
    /* checks validity of command */
    if (counter != 3 || paragraph_number <= 0) {
        printf("Invalid Command \n");
    }
    else {
        /* finds occurence of "*" */
        line = strstr(command, "*");
        if (line != NULL) {
            line++;
            strcpy(new_line, line);

            /* calls the append_line function and printed message
            if the function fails */
            if (append_line(doc, paragraph_number, new_line) == FAILURE) {
                printf("%s failed \n", function_name);
            }
        }
        else {
            printf("Invalid Command \n");
        }
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
print document function using a command */
int print_document_process(Document *doc, char *function_name, char *command) {
    char holder[MAX_STR_SIZE + 1] = "";
    int counter;

    counter = sscanf(command, " %s %s", function_name, holder);

    /* if there is more input than just 1, print the message */
    if (counter != 1) {
        printf("Invalid Command \n");
    }
    else {
        print_document(doc); /* no failure statement */
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
remove line function using a command */
int remove_line_process(Document *doc, char *function_name, char *command) {
    char holder[MAX_STR_SIZE + 1] = "";
    int counter, line_number, paragraph_number;

    counter = sscanf(command, " %s %d %d %s", function_name, 
                    &paragraph_number, &line_number, holder);

    /* checks if command is valid */
    if (counter != 3 || paragraph_number <= 0 || line_number <= 0) {
        printf("Invalid Command \n");
    }
    else {
                    
        /* runs the function and checks if it failed */
        if (remove_line(doc, paragraph_number, line_number) == FAILURE) {
            printf("%s failed \n", function_name);
        }
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
load file function using a command */
int load_file_process(Document *doc, char *function_name, char *command) {
    char holder[MAX_STR_SIZE + 1] = "";
    char file_name[MAX_STR_SIZE + 1] = "";
    int counter; 

    counter = sscanf(command, " %s %s %s", function_name, file_name, holder);

    /* checks if command is valid */
    if (counter != 2) {
        printf("Invalid Command \n");
    }
    else {
        /* runs the function and checks if it failed */
        if (load_file(doc, file_name) == FAILURE) {
            printf("%s failed \n", function_name);
        }
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
replace text function using a command */
int replace_text_process(Document *doc, char *function_name, char *command) {
    char *first_quote, *second_quote, *replace_quote1, *replace_quote2;
    int len;
    char target_text[MAX_STR_SIZE + 1] = "";
    char replace_line[MAX_STR_SIZE + 1] = "";

    first_quote = strstr(command, "\""); /* finds first quotation*/
    if (first_quote != NULL) {
        first_quote++;
        /* find the second quotation */
        second_quote = strstr(first_quote, "\""); 
        if (second_quote != NULL) {
            len = second_quote - first_quote;
        }
        else {
            printf("Invalid Command \n"); 
        }
    }
    else {
        printf("Invalid Command \n");
    } 

    /* if "target" is found */
    if (first_quote != NULL && second_quote != NULL) {
                    
        strncpy(target_text, first_quote, len);
        target_text[len] = '\0';

        /* finds the second set of quotation marks */
        second_quote++;
        replace_quote1 = strstr(second_quote, "\"");
        if (replace_quote1 != NULL) {
            replace_quote1++;
            replace_quote2 = strstr(replace_quote1, "\"");
            if (replace_quote2 != NULL) {
                len = replace_quote2 - replace_quote1;
            }
            else {
                printf("Invalid Command \n");
            }
        }
        else {
            printf("Invalid Command \n");
        }
    }

    /* if both Target and Replacement are found in command */
    if (first_quote != NULL && second_quote != NULL && 
        replace_quote1 != NULL && replace_quote2 != NULL) {
                    
        strncpy(replace_line, replace_quote1, len); /* copy accordingly */
        replace_line[len] = '\0';

        if (replace_text(doc, target_text, replace_line)  == FAILURE) {
            printf("%s failed \n", function_name);
        }
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
highlight text function using a command */
int highlight_text_process(Document *doc, char *command) {
    char *first_quote, *second_quote;
    int len;
    char target_text[MAX_STR_SIZE + 1] = "";
               
    /* find first occurence of quotation mark */
    first_quote = strstr(command, "\"");
    if (first_quote != NULL) {
        first_quote++;
        second_quote = strstr(first_quote, "\"");
        if (second_quote != NULL) {
            len = second_quote - first_quote; /* finds distance from pointers */
        }
        else {
            printf("Invalid Command \n");
        }
    }
    else {
        printf("Invalid Command \n");
    }

    if (first_quote != NULL && second_quote != NULL) {
        strncpy(target_text, first_quote, len);

        target_text[len] = '\0';

        highlight_text(doc, target_text);
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
remove text function using a command */
int remove_text_process(Document *doc, char *command) {
    char *first_quote, *second_quote;
    int len;
    char target_text[MAX_STR_SIZE + 1] = "";

    /* finds first occurence of quotations mark */
    first_quote = strstr(command, "\"");
    if (first_quote != NULL) {
        first_quote++;
        second_quote = strstr(first_quote, "\"");
        if (second_quote != NULL) {
            len = second_quote - first_quote;
        }
        else {
            printf("Invalid Command \n");
        }
    }
    else {
        printf("Invalid Command \n");
    }

    /* if target is found */
    if (first_quote != NULL && second_quote != NULL) {
        strncpy(target_text, first_quote, len);

        target_text[len] = '\0';

        remove_text(doc, target_text);
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
save document function using a command */
int save_document_process(Document *doc, char *function_name, char *command) {
    char holder[MAX_STR_SIZE + 1] = "";
    char file_name[MAX_STR_SIZE + 1] = "";

    /* checks if command is valid */
    if (sscanf(command, " %s %s %s", function_name, file_name, holder) != 2) {
        printf("Invalid Command \n");
    }

    else {
        /* calls functions and checks if it fails */
        if (save_document(doc, file_name) == FAILURE) {
        printf("%s failed \n", function_name);
        }
    }

    return 1;
}

/* This helper method will help process the information needed to call the 
reset document function using a command */
int reset_document_process(Document *doc, char *function_name, char *command) {
    char holder[MAX_STR_SIZE + 1] = "";
                
    /* checks if the command for reset_document is valid */
    if (sscanf(command, " %s %s", function_name, holder) != 1) {
        printf("Invalid Command \n");
    }
    else {
        reset_document(doc); /* no failed message */
    }

    return 1;
}




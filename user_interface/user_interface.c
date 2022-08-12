#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"

#define MAX_LEN 1024
/* Author: Jingyun Li    */
/* UID number: 117242874 */
/* Directory ID: jingyli */

/* this is the main function that recieve and deal with the input */
int main(int argc, char **argv) {
  char param[16][MAX_LEN + 1];
  FILE *input;
  char line[MAX_LEN + 1];
  int i, j;
  int cnt = 0;
  int flag = 0;

  /* these variables are used to put data read for the commands */
  int n1, n2;
  int para_num, line_num;
  char filename[MAX_STR_SIZE + 1];
  Document doc;

  /* this if statement judges whether the input are standard input or not */
  if (argc == 1) {
    input = stdin;
    flag = 1;
  } else if (argc == 2) {
    input = fopen(argv[1], "r");
    if (input == NULL) {
      fprintf(stderr, "%s cannot be opened.", argv[1]);
      return EX_OSERR;
    }
  } else {
    fprintf(stderr, "Usage: user_interface\n");
    fprintf(stderr, "Usage: user_interface <filename>\n");
    return EX_USAGE;
  }

  /* before starting the commands, first initiate the document */
  init_document(&doc, "main_document");
  if (flag) {
    printf("> ");
  }

  /* this loop helps get the line input until the end of the file */
  fgets(line, MAX_LEN + 1, input);
  while (!feof(input)) {
    for (i = 0; line[i]; i++) {
      if (line[i] == '\n') {
	line[i] = '\0';
	break;
      }
    }
    /* this loop help put all the inputs in to a char */
    /* array and counts the lenth of the array */
    for (i = 0, cnt = 0;; cnt++) {
      while (isspace(line[i])) {
	i++;
      }
      if (line[i] == '\"') {
	for (j = i + 1; line[j] != '\"' && line[j]; j++) {
	  param[cnt][j - i - 1] = line[j];
	}
	param[cnt][j - i - 1] = '\0';
	j++;
      } else {
	for (j = i; !isspace(line[j]) && line[j]; j++) {
	  param[cnt][j - i] = line[j];
	}
	param[cnt][j - i] = '\0';
      }
      if (param[cnt][0] == '\0') {
	break;
      }
      i = j;
    }

    /* this command adds a paragraph to the document */
    if (strcmp(param[0], "add_paragraph_after") == 0) {
      if (cnt != 2) {
	printf("Invalid Command\n");
      } else {
	n1 = sscanf(param[1], "%d", &para_num);
	if (n1 != 1 || para_num < 0) {
	  printf("Invalid Command\n");
	} else if (add_paragraph_after(&doc, para_num) == FAILURE) {
	  printf("add_paragraph_after failed\n");
	}
      }

      /* this command adds a line after the line with a specific line number */
    } else if (strcmp(param[0], "add_line_after") == 0) {
      n1 = sscanf(param[1], "%d", &para_num);
      n2 = sscanf(param[2], "%d", &line_num);
      if (n1 != 1 || n2 != 1 || para_num <= 0 || line_num < 0
	  || param[3][0] != '*') {
	printf("Invalid Command\n");
      } else {
	for (i = 0; line[i] != '*'; i++);
	if (add_line_after(&doc, para_num, line_num, line + i + 1) == FAILURE) {
	  printf("add_line_after failed\n");
	}
      }

      /* this command prints the document information */
    } else if (strcmp(param[0], "print_document") == 0) {
      if (cnt != 1) {
	printf("Invalid Command\n");
      } else {
	print_document(&doc);
      }

      /* this command terminates the user interface */
    } else if (strcmp(param[0], "quit") == 0) {
      if (cnt != 1) {
	printf("Invalid Command\n");
      } else {
	return EXIT_SUCCESS;
      }

      /* this command terminates the user interface */
    } else if (strcmp(param[0], "exit") == 0) {
      if (cnt != 1) {
	printf("Invalid Command\n");
      } else {
	return EXIT_SUCCESS;
      }

      /* this command appends a line to the specified paragraph */
    } else if (strcmp(param[0], "append_line") == 0) {
      n1 = sscanf(param[1], "%d", &para_num);
      if (n1 != 1 || para_num <= 0 || param[2][0] != '*') {
	printf("Invalid Command\n");
      } else {
	for (i = 0; line[i] != '*'; i++);
	if (append_line(&doc, para_num, line + i + 1) == FAILURE) {
	  printf("append_line failed\n");
	}
      }

      /* this command removes the specified line from the paragraph */
    } else if (strcmp(param[0], "remove_line") == 0) {
      if (cnt != 3) {
	printf("Invalid Command\n");
      } else {
	n1 = sscanf(param[1], "%d", &para_num);
	n2 = sscanf(param[2], "%d", &line_num);
	if (n1 != 1 || n2 != 1 || para_num <= 0 || line_num <= 0) {
	  printf("Invalid Command\n");
	} else if (remove_line(&doc, para_num, line_num) == FAILURE) {
	  printf(" remove_line failed\n");
	}
      }

      /* this command loads the specified file into the current document */
    } else if (strcmp(param[0], "load_file") == 0) {
      if (cnt != 2) {
	printf("Invalid Command\n");
      } else if (load_file(&doc, param[1]) == FAILURE) {
	printf("load_file failed\n");
      }

      /* this command replaces the string ”TARGET” with ”REPLACEMENT” */
    } else if (strcmp(param[0], "replace_text") == 0) {
      if (cnt != 3) {
	printf("Invalid Command\n");
      } else if (replace_text(&doc, param[1], param[2]) == FAILURE) {
	printf("replace_text failed\n");
      }

      /* this command highlights the string ”TARGET” */
    } else if (strcmp(param[0], "highlight_text") == 0) {
      if (cnt != 2) {
	printf("Invalid Command\n");
      } else if (highlight_text(&doc, param[1]) == FAILURE) {
	printf("highlight_text failed\n");
      }

      /* this command removes the string ”TARGET” */
    } else if (strcmp(param[0], "remove_text") == 0) {
      if (cnt != 2) {
	printf("Invalid Command\n");
      } else if (remove_text(&doc, param[1]) == FAILURE) {
	printf("remove_text failed\n");
      }

      /* this command saves the curent document to the specified file */
    } else if (strcmp(param[0], "save_document") == 0) {
      if (cnt != 2) {
	printf("Invalid Command\n");
      } else {
	n1 = sscanf(param[1], "%s", filename);
	if (n1 != 1) {
	  printf("Invalid Command\n");
	} else if (save_document(&doc, filename) == FAILURE) {
	  printf("save_document failed\n");
	}
      }

      /* this command resets the curent document */
    } else if (strcmp(param[0], "reset_document") == 0) {
      if (cnt != 1) {
	printf("Invalid Command\n");
      } else {
	reset_document(&doc);
      }

      /* this is true when the input is not a comment nor a empty line */
    } else if (!(param[0][0] == '#' || param[0][0] == '\0')) {
      printf("Invalid Command\n");
    }
    if (flag) {
      printf("> ");
    }
    fgets(line, MAX_LEN + 1, input);
  }
  fclose(input);
  return EXIT_SUCCESS;
}


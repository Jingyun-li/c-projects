#include <stdio.h>
#include <string.h>
#include "document.h"

/* author: Jingyun Li    */
/* UID number: 117242874 */
/* Directory ID: jingyli */

/* This function initiate the document with */
/* 0 paragraphs and sets the document’s name */ 
int init_document(Document *doc, const char *name){
  size_t size;
  if (name == NULL) {
    return FAILURE;
  }
  size = strlen(name);
  if (doc == NULL || size < 1 || size > MAX_STR_SIZE) {
    return FAILURE;
  }
  strcpy(doc->name, name);
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/* This function sets the number of paragraphs to 0 */
int reset_document(Document *doc){
  if (doc == NULL) {
    return FAILURE;
  }
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/* This function prints the document’s name, number of */
/* paragraphs, and seperately the paragraphs of the document */
int print_document(Document *doc){
  int i, j;
  int para_num;
  int line_num;
  if (doc == NULL) {
    return FAILURE;
  }
  para_num = doc->number_of_paragraphs;
  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
  for (i = 0; i < para_num; i++) {
    line_num = doc->paragraphs[i].number_of_lines;
    for (j = 0; j < line_num; j++) {
      printf("%s\n", doc->paragraphs[i].lines[j]);
    }
    /* This if condition helps to print a empty line between paragraphs */ 
    if (i < para_num - 1) {
      printf("\n");
    }
  }
  return SUCCESS;
}

/* This function adds a paragraph after the specified paragraph number */
int add_paragraph_after(Document *doc, int paragraph_number){
  int i, j;
  int line_num;
  int para_num;
  if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS
      || paragraph_number + 1 > MAX_PARAGRAPHS || paragraph_number < 0) {
    return FAILURE;
  }
  para_num = doc->number_of_paragraphs;
  if (paragraph_number > para_num){
    return FAILURE;
  }
  doc->number_of_paragraphs++;
  /* This for loop helps to move all adjacent paragraphs foward */
  for (i = para_num; i >= paragraph_number; i--) {
    line_num = doc->paragraphs[i].number_of_lines;
    doc->paragraphs[i + 1].number_of_lines = doc->paragraphs[i].number_of_lines;
    for (j = 0; j < line_num; j++) {
      strcpy(doc->paragraphs[i + 1].lines[j], doc->paragraphs[i].lines[j]);
    }
  }
  return SUCCESS;
}

/* This function adds a new line after the line */
/* with the specified line number */
int add_line_after(Document *doc, int paragraph_number, int line_number,
		   const char *new_line){
  int j;
  int line_num;
  if (doc == NULL) {
    return FAILURE;
  }
  if (paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  line_num = doc->paragraphs[paragraph_number - 1].number_of_lines;
  if (line_number > line_num || line_number < 0 ||
      line_num + 1 > MAX_PARAGRAPH_LINES) {
    return FAILURE;
  }

  /* The function will dirst move the lines which were previously located in */
  /* the place where we want to add the new line foward, then add the line */
  for (j = line_num; j > line_number; j--) {
    strcpy(doc->paragraphs[paragraph_number - 1].lines[j],
	   doc->paragraphs[paragraph_number - 1].lines[j - 1]);
  }
  strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
  doc->paragraphs[paragraph_number - 1].number_of_lines++;
  return SUCCESS;
}

/* This function returns the number of lines in a paragraph */
int get_number_lines_paragraph(Document *doc, int paragraph_number, 
			       int *number_of_lines){
  if (doc == NULL || number_of_lines == NULL) {
    return FAILURE;
  }
  if (paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  *number_of_lines = doc->paragraphs[paragraph_number].number_of_lines;
  return SUCCESS;
}

/* This function appends a new line to the specified paragraph */
int append_line(Document *doc, int paragraph_number, const char *new_line){
  int line_num;
  if (doc == NULL) {
    return FAILURE;
  }
  if (paragraph_number < 0 || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  line_num = doc->paragraphs[paragraph_number - 1].number_of_lines;
  if (line_num > MAX_PARAGRAPH_LINES) {
    return FAILURE;
  }
  doc->paragraphs[paragraph_number - 1].number_of_lines++;
  strcpy(doc->paragraphs[paragraph_number - 1].lines[line_num], new_line);
  return SUCCESS;
}

/* This function removes the specified line from the paragraph.*/
int remove_line(Document *doc, int paragraph_number, int line_number){
  int j;
  int line_num;
  if (doc == NULL) {
    return FAILURE;
  }
  if (paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  if (line_number <= 0 ||
      line_number > doc->paragraphs[paragraph_number - 1].number_of_lines) {
    return FAILURE;
  }
  line_num = doc->paragraphs[paragraph_number - 1].number_of_lines;

  /* This for loop help to move lines that is after the removed line back */
  for (j = line_number - 1; j < line_num; j++) {
    strcpy(doc->paragraphs[paragraph_number - 1].lines[j], 
	   doc->paragraphs[paragraph_number - 1].lines[j + 1]);
  }
  doc->paragraphs[paragraph_number - 1].number_of_lines--;
  return SUCCESS;
}

/* This function adds the first data lines number of rows from */
/* the data array to the beginning of the document */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
  int i;
  int para_cnt;
  int para_idx;
  if (doc == NULL || data == NULL || data_lines == 0) {
    return FAILURE;
  }
  if (doc->number_of_paragraphs >= MAX_PARAGRAPHS) {
    return FAILURE;
  }
  if (data_lines >= MAX_PARAGRAPH_LINES) {
    return FAILURE;
  }
  /* initiate the variable para_cnt with 1*/
  para_cnt = 1;

  /* This loop counts the paragraphs */
  for (i = 0; i < data_lines; i++) {
    if (data[i][0] == '\0') {
      para_cnt++;
    }
  }
  if (doc->number_of_paragraphs + para_cnt > MAX_PARAGRAPHS) {
    return FAILURE;
  }

  /* This loop move things that was previously in */
  /* the beginning in the document forward */
  for (i = doc->number_of_paragraphs - 1; i >= 0; i--) {
    memcpy(&doc->paragraphs[i + para_cnt], &doc->paragraphs[i],
	   sizeof(doc->paragraphs[0]));
  }
  doc->number_of_paragraphs += para_cnt;
  para_idx = 0;
  doc->paragraphs[0].number_of_lines = 0;

  /* This loop put things in the array data in to the doc */
  for (i = 0; i < data_lines; i++) {
    if (data[i][0] == '\0') {
      doc->paragraphs[++para_idx].number_of_lines = 0;
    } else {
      strcpy(doc->paragraphs[para_idx].lines[doc->paragraphs[para_idx].
	     number_of_lines], data[i]);
      if (++doc->paragraphs[para_idx].number_of_lines >= MAX_PARAGRAPH_LINES) {
	return FAILURE;
      }
    }
  }
  return SUCCESS;
}

/* This function replaces every appearance of the text */
/* target in the document with the text replacement */
int replace_text(Document *doc, const char *target, const char *replacement){
  int i, j, k;
  int para_num, line_num;
  size_t tar_len, rep_len;
  char tmp[MAX_STR_SIZE + 1];
  if (doc == NULL || target == NULL || replacement == NULL) {
    return FAILURE;
  }

  /* initiate tar_len as the length of target, */
  /* rep_len as the length of replacement if they are valid */
  tar_len = strlen(target);
  rep_len = strlen(replacement);
  para_num = doc->number_of_paragraphs;

  /* This layer traverse through paragraphs and gets the number of lines */
  for (i = 0; i < para_num; i++) {
    line_num = doc->paragraphs[i].number_of_lines;

    /* This layer traverse through the lines */
    for (j = 0; j < line_num; j++) {
      k = 0;

      /* This layer traverse through all the texts */
      /* and replace it after finding it*/
      while (k < strlen(doc->paragraphs[i].lines[j]) - tar_len) {
	if (strncmp(doc->paragraphs[i].lines[j] + k, target, tar_len) == 0) {
	  if (rep_len != tar_len) {
	    strcpy(tmp, doc->paragraphs[i].lines[j] + k + tar_len);
	    strcpy(doc->paragraphs[i].lines[j] + k + rep_len, tmp);
	  }
	  strncpy(doc->paragraphs[i].lines[j] + k, replacement, rep_len);
	  k += rep_len;
	} else {
	  k++;
	}
      }
    }
  }
  return SUCCESS;
}

/* This function highlights every appearance of the text target in the */
/* document it is implemented through calling the replace_text function */
int highlight_text(Document *doc, const char *target){
  char replacement[MAX_STR_SIZE + 1];
  if (target == NULL) {
    return FAILURE;
  }
  /* Put the highlighted target into the varaible replacement */
  strcpy(replacement, HIGHLIGHT_START_STR);
  strcat(replacement, target);
  strcat(replacement, HIGHLIGHT_END_STR);
  return replace_text(doc, target, replacement);
}

/* This function removes every appearance of the text target in the document */
/* it is implemented by calling the replace_text function */
int remove_text(Document *doc, const char *target){
  return replace_text(doc, target, "");
}

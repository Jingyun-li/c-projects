#include <stdio.h>
#include <stdlib.h>
#include "event.h"
#include "calendar.h"
#include "my_memory_checker_216.h"

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
	return ((Event*) ptr1)->duration_minutes - ((Event*) ptr2)->duration_minutes;
}

/* This test checks the function init_calandar, */
/* print_calendar and distroy_calendar*/

static int test1() {
	int days = 7;
	Calendar *calendar;

	if (init_calendar("test1", days, comp_minutes, NULL, &calendar) == SUCCESS) {
		if (print_calendar(calendar, stdout, 1) == SUCCESS) {
			return destroy_calendar(calendar);
		}
	}

	return FAILURE;
}

/* This test check how the function works if the */
/* same event is add for multiple times*/
static int test2() {
	int days = 7;
	Calendar *calendar;

	if (init_calendar("test2", days, comp_minutes, NULL, &calendar) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 1) == SUCCESS) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	return destroy_calendar(calendar);
}

/* This test is used to see the result when the comp_func is null*/
static int test3() {
	int days = 7;
	Calendar *calendar;

	if (init_calendar("test3", days, NULL, NULL, &calendar) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "study", 900, 60, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	return destroy_calendar(calendar);
}

/* This test is used to remove a event that does not exist*/
static int test4() {
	int days = 7;
	Calendar *calendar;
	const char *name = "review meeting";

	if (remove_event(calendar, name) == FAILURE) {
		return SUCCESS;
	}
	return FAILURE;
}

/* This test is used to find a event that does not exist */
static int test5() {
	int days = 7;
	Calendar *calendar;
	Event *event;

	if (init_calendar("test5", days, comp_minutes, NULL, &calendar) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "group meeting", 500, 60, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "lunch", 1200, 45, NULL, 3) == FAILURE) {
		return FAILURE;
	}
	if (find_event(calendar, "review meeting", &event) == FAILURE) {
		return FAILURE;
	}
	if (find_event(calendar, "review1 meeting", &event) == SUCCESS) {
		return FAILURE;
	}

	return SUCCESS;
}

/* This test is used to remove a event that does not exist */
static int test6() {
	int days = 7;
	Calendar *calendar;

	if (init_calendar("test6", days, comp_minutes, NULL, &calendar) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "group meeting", 500, 60, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "lunch", 1200, 45, NULL, 3) == FAILURE) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	if (remove_event(calendar, "lunch") == FAILURE) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	if (remove_event(calendar, "lunch1") == SUCCESS) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}

	return SUCCESS;
}

/* This test is used to remove a day that does not exist*/
static int test7() {
	int days = 7;
	Calendar *calendar;

	if (init_calendar("test7", days, comp_minutes, NULL, &calendar) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "group meeting", 500, 60, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 2) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "group meeting", 500, 60, NULL, 2) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "lunch", 1200, 45, NULL, 3) == FAILURE) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	if (clear_day(calendar, 1) == FAILURE) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	if (clear_day(calendar, 1) == SUCCESS) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	return SUCCESS;
}

/* This test is used to remove a calendar that does not exist*/
static int test8() {
	int days = 7;
	Calendar *calendar;

	if (init_calendar("test7", days, comp_minutes, NULL, &calendar) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "group meeting", 500, 60, NULL, 1) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "review meeting", 800, 80, NULL, 2) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "group meeting", 500, 60, NULL, 2) == FAILURE) {
		return FAILURE;
	}
	if (add_event(calendar, "lunch", 1200, 45, NULL, 3) == FAILURE) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	if (clear_calendar(calendar) == FAILURE) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}
	if (clear_calendar(calendar) == SUCCESS) {
		return FAILURE;
	}
	if (print_calendar(calendar, stdout, 1) == FAILURE) {
		return FAILURE;
	}

	return SUCCESS;
}

int main() {
	int result = SUCCESS;

	/***** Starting memory checking *****/
	start_memory_check();
	/***** Starting memory checking *****/

	if (test1() == FAILURE)
		result = FAILURE;
	if (test2() == FAILURE)
		result = FAILURE;
	if (test3() == FAILURE)
		result = FAILURE;
	if (test4() == FAILURE)
		result = FAILURE;
	if (test5() == FAILURE)
		result = FAILURE;
	if (test6() == FAILURE)
		result = FAILURE;
	if (test7() == FAILURE)
		result = FAILURE;
	if (test8() == FAILURE)
		result = FAILURE;
	/****** Gathering memory checking info *****/
	stop_memory_check();
	/****** Gathering memory checking info *****/

	if (result == FAILURE) {
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}


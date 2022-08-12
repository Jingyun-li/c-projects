#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendar.h"

/* this is a helper function that helps us to clear a single event */
static void clear_event(Calendar *calendar, Event *curr) {
	if (curr->info != NULL && calendar->free_info_func != NULL) {
		calendar->free_info_func(curr->info);
	}
	free(curr->name);
	free(curr);
}

/* this function initialize calendar and dynamically*/
/* allocates memories associated with the struct */
int init_calendar(const char *name, int days,
		int (*comp_func)(const void *ptr1, const void *ptr2),
		void (*free_info_func)(void *ptr), Calendar **calendar) {
	size_t len;
	int i;
	if (calendar == NULL || name == NULL || days < 1) {
		return FAILURE;
	}
	*calendar = (Calendar*) malloc(sizeof(Calendar));
	if (*calendar == NULL) {
		return FAILURE;
	}
	len = strlen(name);
	(*calendar)->name = malloc(len + 1);
	if ((*calendar)->name == NULL) {
		return FAILURE;
	} else {
		strcpy((*calendar)->name, name);
	}
	(*calendar)->days = days;
	(*calendar)->total_events = 0;
	(*calendar)->events = (Event**) malloc(sizeof(Event*) * days);
	for (i = 0; i < days; i++) {
		(*calendar)->events[i] = NULL;
	}
	(*calendar)->comp_func = comp_func;
	(*calendar)->free_info_func = free_info_func;
	return SUCCESS;
}

/* this function prints all the all the data in the calendar */
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
	Event *curr;
	int i = 0;
	int j = 0;
	if (calendar == NULL || output_stream == NULL) {
		return FAILURE;
	}
	if (print_all) {
		printf("Calendar's Name: \"%s\"\n", calendar->name);
		printf("Days: %d\n", calendar->days);
		printf("Total Events: %d\n", calendar->total_events);
	}
	printf("**** Events ****\n");

	/* if the calendar contains no event, this function will */
	/* return directly without printing anything */
	for (i = 0; i < calendar->days; i++) {
		curr = calendar->events[i];
		if (curr != NULL) {
			j++;
			break;
		}
	}
	if (j == 0) {
		return SUCCESS;
	}

	/* this loop traverse through the  entire calendar */
	/* and print down the information */
	for (i = 0; i < calendar->days; i++) {
		printf("Day %d\n", i + 1);
		curr = calendar->events[i];
		while (curr != NULL) {
			printf("Event's Name: \"%s\", Start_time: %d, Duration: %d\n", curr->name,
					curr->start_time, curr->duration_minutes);
			curr = curr->next;
		}
	}
	return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time,
		int duration_minutes, void *info, int day) {
	size_t len;
	Event *curr = NULL;
	Event *prev = NULL;
	Event *event = NULL;
	int i;
	if (calendar == NULL || name == NULL || start_time < 0 || start_time > 2400
			|| duration_minutes <= 0) {
		return FAILURE;
	}
	if (day < 1 || day > calendar->days || calendar->comp_func == NULL) {
		return FAILURE;
	}
	/* this loop directly returns failure if the same event already exist */
	for (i = 0; i < calendar->days; i++) {
		curr = calendar->events[i];
		while (curr != NULL) {
			if (!strcmp(curr->name, name)) {
				return FAILURE;
			}
			curr = curr->next;
		}
	}

	/* this part of the function allocate the space for the new event */
	len = strlen(name);
	event = (Event*) malloc(sizeof(Event));
	if (event == NULL) {
		return FAILURE;
	}
	event->name = (char*) malloc(len + 1);
	if (event->name == NULL) {
		return FAILURE;
	}
	strcpy(event->name, name);
	event->start_time = start_time;
	event->duration_minutes = duration_minutes;
	event->info = info;
	event->next = NULL;

	/* this part of the function put the event into the linked list */
	curr = calendar->events[day - 1];
	if (curr != NULL) {
		while (curr != NULL) {
			if (calendar->comp_func(event, curr) <= 0) {
				if (prev == NULL) {
					calendar->events[day - 1] = event;
				} else {
					prev->next = event;
				}
				event->next = curr;
				break;
			}
			prev = curr;
			curr = curr->next;
		}
		if (curr == NULL) {
			prev->next = event;
		}
	} else {
		calendar->events[day - 1] = event;
	}
	calendar->total_events++;
	return SUCCESS;
}

/* This event is true if calendar has an event with the specified name */
int find_event(Calendar *calendar, const char *name, Event **event) {
	int i;
	Event *curr;
	if (calendar == NULL || name == NULL) {
		return FAILURE;
	}
	for (i = 0; i < calendar->days; i++) {
		curr = calendar->events[i];
		while (curr != NULL) {
			if (!strcmp(curr->name, name)) {
				*event = curr;
				return SUCCESS;
			}
			curr = curr->next;
		}
	}
	*event = NULL;
	return FAILURE;
}

/* This event is true if calendar has an event */
/* with the specified name in the specified day */
int find_event_in_day(Calendar *calendar, const char *name, int day,
		Event **event) {
	Event *curr;
	if (calendar == NULL || name == NULL || day < 1 || day > calendar->days) {
		return FAILURE;
	}
	curr = calendar->events[day - 1];
	while (curr != NULL) {
		if (!strcmp(curr->name, name)) {
			*event = curr;
			return SUCCESS;
		}
		curr = curr->next;
	}
	*event = NULL;
	return FAILURE;
}

/* This function removes the event from the calendar, updates the */
/* number of events, and frees any memory allocated for the event */
int remove_event(Calendar *calendar, const char *name) {
	int i;
	Event *curr;
	Event *prev;
	if (calendar == NULL || name == NULL) {
		return FAILURE;
	}
	for (i = 0; i < calendar->days; i++) {
		curr = calendar->events[i];
		prev = NULL;
		while (curr != NULL) {
			if (!strcmp(curr->name, name)) {
				if (prev == NULL) {
					calendar->events[i] = curr->next;
				} else {
					prev->next = curr->next;
				}
				clear_event(calendar, curr);
				calendar->total_events--;
				return SUCCESS;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return FAILURE;
}

/* This function returns the info pointer associated with the specified event */
void* get_event_info(Calendar *calendar, const char *name) {
	int i;
	Event *curr;
	for (i = 0; i < calendar->days; i++) {
		curr = calendar->events[i];
		while (curr != NULL) {
			if (!strcmp(curr->name, name)) {
				return curr->info;
			}
			curr = curr->next;
		}
	}
	return NULL;
}

/* This function removes all the events for the specified day */
int clear_day(Calendar *calendar, int day) {
	Event *curr;
	Event *event;
	if (calendar == NULL || day < 1 || day > calendar->days) {
		return FAILURE;
	}
	curr = calendar->events[day - 1];
	while (curr != NULL) {
		event = curr;
		curr = curr->next;
		clear_event(calendar, event);
		calendar->total_events--;
	}
	calendar->events[day - 1] = NULL;
	return SUCCESS;
}

/* this function removes every event and sets every event list to empty */
int clear_calendar(Calendar *calendar) {
	int i;
	if (calendar == NULL) {
		return FAILURE;
	}
	for (i = 0; i < calendar->days; i++) {
		clear_day(calendar, i + 1);
	}
	return SUCCESS;
}

/* This function removes every event */
int destroy_calendar(Calendar *calendar) {
	if (calendar == NULL) {
		return FAILURE;
	}
	clear_calendar(calendar);
	free(calendar->events);
	free(calendar->name);
	free(calendar);
	return SUCCESS;
}


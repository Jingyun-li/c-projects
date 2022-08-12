/* 117242874 */
#include <stdio.h>
#include "command.h"
#include "executor.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

static int proc_tree(struct tree *t, int *read_pipe, int *write_pipe, int *fork_cnt);
static int proc_none(struct tree *t, int *read_pipe, int *write_pipe, int *fork_cnt) {
	int pid;

	if (!strcmp("exit", t->argv[0])) {
		exit(0);
	}

	if (!strcmp("cd", t->argv[0])) {
		return chdir(t->argv[1]);
	}

	if (t->input && read_pipe) {
		printf("Ambiguous input redirect.\n");
		return -1;
	}

	if (t->output && write_pipe) {
		printf("Ambiguous output redirect.\n");
		return -1;
	}

	pid = fork();
	if (pid < 0) {
		return -1;
	}

	if (!pid) {
		if (t->input) {
			int fd = open(t->input, O_RDONLY);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}

		if (t->output) {
			int fd = open(t->output, O_CREAT | O_WRONLY);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}

		if (read_pipe) {
			dup2(read_pipe[0], STDIN_FILENO);
			close(read_pipe[0]);
			close(read_pipe[1]);
		}

		if (write_pipe) {
			dup2(write_pipe[1], STDOUT_FILENO);
			close(write_pipe[0]);
			close(write_pipe[1]);
		}

		execvp(t->argv[0], t->argv);
		exit(1);
	}

	(*fork_cnt)++;
	return 0;
}

static int proc_pipe(struct tree *t, int *read_pipe, int *write_pipe, int *fork_cnt) {
	int fd[2];
	int n;
	int cnt = 0;
	int *pcnt;
	int i;

	if (!read_pipe && !write_pipe) {
		pcnt = &cnt;
	} else {
		pcnt = fork_cnt;
	}

	pipe(fd);
	n = proc_tree(t->left, read_pipe, fd, pcnt);
	if (n) {
		return n;
	}

	n = proc_tree(t->right, fd, write_pipe, pcnt);
	if (n) {
		return n;
	}

	close(fd[0]);
	close(fd[1]);
	n = 0;
	for (i = 0; i < cnt; i++) {
		int status;
		wait(&status);
		if (status) {
			n = status;
		}
	}

	return n;
}

static int proc_and(struct tree *t, int *read_pipe, int *write_pipe, int *fork_cnt) {
	int n;
	int cnt = 0;
	int i;

	n = proc_tree(t->left, read_pipe, write_pipe, &cnt);
	if (n) {
		return n;
	}

	n = 0;
	for (i = 0; i < cnt; i++) {
		int status;
		wait(&status);
		if (status) {
			n = status;
		}
	}

	if (n) {
		return n;
	}

	return proc_tree(t->right, read_pipe, write_pipe, fork_cnt);
}

static int proc_subshell(struct tree *t, int *read_pipe, int *write_pipe, int *fork_cnt) {
	int pid;

	pid = fork();
	if (pid < 0) {
		return -1;
	}

	if (!pid) {
		proc_tree(t->left, read_pipe, write_pipe, fork_cnt);
		proc_tree(t->right, read_pipe, write_pipe, fork_cnt);
		exit(0);
	}

	(*fork_cnt)++;
	return 0;
}

static int proc_tree(struct tree *t, int *read_pipe, int *write_pipe, int *fork_cnt) {
	if (!t) {
		return 0;
	}

	switch (t->conjunction) {
	case NONE:
		return proc_none(t, read_pipe, write_pipe, fork_cnt);

	case AND:
		return proc_and(t, read_pipe, write_pipe, fork_cnt);

	case PIPE:
		return proc_pipe(t, read_pipe, write_pipe, fork_cnt);

	case SUBSHELL:
		return proc_subshell(t, read_pipe, write_pipe, fork_cnt);

	default:
		break;
	}

	return 0;
}

int execute(struct tree *t) {
	int fork_cnt = 0;
	int i;

/*	print_tree(t);*/

	proc_tree(t, NULL, NULL, &fork_cnt);
	for (i = 0; i < fork_cnt; i++) {
		wait(NULL);
	}

	return 0;
}

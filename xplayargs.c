#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

static const struct {
	const char *extension;
	const char *program;
} g_handlers[] = {
	{ "mp3",	"mpg123" },
	{ "ogg",	"ogg122" }
};

int main(int argc, char *argv[]) {
	/* an audio file */
	char path[PATH_MAX] = {'\0'};

	/* the path length */
	ssize_t length = 0;

	/* the process exit code */
	int exit_code = EXIT_FAILURE;

	/* a loop index */
	unsigned int i = 0;

	/* a child process exit status */
	int status = 0;

	/* a child process ID */
	pid_t pid = (-1);

	/* the base file name */
	char *base_name = NULL;

	/* the file name extension */
	char *extension = NULL;

	/* the player used to play an audio file */
	const char *program = NULL;

	do {
		/* read the path of an audio file */
		if (NULL == fgets(path, sizeof(path), stdin)) {
			break;
		}

		/* strip the trailing line break */
		length = strlen(path) - 1;
		if ('\n' == path[length]) {
			path[length] = '\0';
		}

		/* get the base file name */
		if ('\0' == path[0]) {
			continue;
		}
		base_name = basename(path);

		/* locate the file name extension */
		extension = strrchr(base_name, '.');
		if (NULL == extension) {
			continue;
		}
		++extension;

		/* choose the player */
		program = NULL;
		for (i = 0; (sizeof(g_handlers) / sizeof(g_handlers[0])) > i; ++i) {
			if (0 == strcmp(g_handlers[i].extension, extension)) {
				program = g_handlers[i].program;
				break;
			}
		}
		if (NULL == program) {
			continue;
		}

		/* run the player and wait for it to terminate */
		pid = fork();
		switch (pid) {
			case (-1):
				goto end;

			case 0:
				(void) execlp(program, program, path, (char *) NULL);
				exit(EXIT_FAILURE);

			default:
				if (pid != waitpid(pid, &status, 0)) {
					goto end;
				}

				/* make sure the child process exited cleanly */
				if (!(WIFEXITED(status))) {
					goto end;
				}

				/* if at least one audio file was played, report success */
				if (EXIT_SUCCESS == WEXITSTATUS(status)) {
					exit_code = EXIT_SUCCESS;
				}
		}
	} while (1);

end:
	return exit_code;
}
#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

#define BUFF_SIZE 1024
#define BUFF_FLUSH -1

#define CMD_NORM	0
#define OR		1
#define AND		2
#define PIPE	3

#define GET_LINE 0

#define HISTORY_FILE	".simple_shell_history"
#define MAX_HISTORY	4096

extern char **environ;


/**
 * struct LinkedList - singly linked list
 * @i: the index value
 * @str: a string
 * @next: points to the next node
 */
typedef struct LinkedList
{
	int i;
	char *str;
	struct LinkedList *next;
} list_t;

/**
 *struct ShellInfo - contains pseudo-args to pass into a function
 *@arg: a string generated from _getline with args
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the arg count
 *@errcounter: the error count
 *@errors: the error code for exit()s
 *@linecount_flag: count this line of input if its on
 *@fname: the program filename
 *@env: linkedlist local copy of environ
 *@environ: modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buff: address of pointer to cmd_buf, on if chaining args
 *@cmd_buff_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct ShellInfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int errcounter;
	int errors;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buff;
	int cmd_buff_type;
	int readfd;
	int histcount;
} shell_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct embedded_cmd - contains a embedded_cmd string and related function
 *@type: the embedded_cmd command flag
 *@func: the function
 */
typedef struct embedded_cmd
{
	char *type;
	int (*func)(shell_t *);
} embedded_cmd_tab;

int print_shell(shell_t *sh, char **av);
int find_embedded_cmd(shell_t *);
int find_embedded_cmd(shell_t *);
void parse_cmd(shell_t *);
void fork_pid(shell_t *);
int prints_cmd(shell_t *, char *);
char *char_double(char *, int, int);
char *find_path(shell_t *, char *, char *);
void _putts(char *);
int _puttchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);
int _strlen(char *);
int _strcmp(char *, char *);
char *_strstr(const char *, const char *);
char *_strcat(char *, char *);
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);
char **strtow(char *, char *);
char **strtow2(char *, char);
char *_memset(char *, char, unsigned int);
void free_str_arr(char **);
void *_realloc(void *, unsigned int, unsigned int);
int free_ptr(void **);
int interactive(shell_t *);
int _delim(char, char *);
int _atoi(char *);
void print_err(shell_t *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void comment_handler(char *);
int _exit_(shell_t *);
int _cd_(shell_t *);
int _help_(shell_t *);
int shell_hist(shell_t *);
int _alias(shell_t *);
ssize_t get_input(shell_t *);
int _getline(shell_t *, char **, size_t *);
void clear_sh(shell_t *);
void set_sh(shell_t *, char **);
void free_sh(shell_t *, int);
char *_getenv(shell_t *, const char *);
int _env_(shell_t *);
int _setenv_(shell_t *);
int _unsetenv_(shell_t *);
int env_list(shell_t *);
char **get_environ(shell_t *);
int _unsetenv(shell_t *, char *);
int _setenv(shell_t *, char *, char *);
char *hist_ret(shell_t *sh);
int write_hist(shell_t *sh);
int read_hist(shell_t *sh);
int compile_hist(shell_t *sh, char *buf, int linecount);
int update_hist(shell_t *sh);
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int del_node(list_t **, unsigned int);
void free_list(list_t **);
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);
int is_cmd_arg(shell_t *, char *, size_t *);
void chain_checker(shell_t *, char *, size_t *, size_t, size_t);
int change_alias(shell_t *);
int change_vars(shell_t *);
int change_string(char **, char *);

/* Signal Handling Prototype */
void sigintHandler(int);


#endif	/* SHELL_H */

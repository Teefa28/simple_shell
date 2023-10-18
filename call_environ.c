#include "mymain.h"

/**
 * _env_ - prints the current environment
 * @sh: ...
 * Return: Always 0
 */
int _env_(shell_t *sh)
{
	print_list_str(sh->env);
	return (0);
}

/**
 * _getenv - gets the value of an environ variable
 * @sh: ...
 * @name: env var name
 *
 * Return: Null
 */
char *_getenv(shell_t *sh, const char *name)
{
	list_t *node = sh->env;
	char *p;

	while (node)
	{
		p = _strstr(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _setenv_ - sets a new environment variable
 * @sh: ...
 *  Return: Always 0
 */
int _setenv_(shell_t *sh)
{
	if (sh->argc != 3)
	{
		_putts("Requires three arguments\n");
		return (1);
	}
	if (_setenv(sh, sh->argv[1], sh->argv[2]))
		return (0);
	return (1);
}

/**
 * _unsetenv_ - deselects an environment variable
 * @sh: input value
 *  Return: 1 on delete, 0 otherwise
 */
int _unsetenv_(shell_t *sh)
{
	int i;

	if (sh->argc == 1)
	{
		_putts("Needs more sauce\n");
		return (1);
	}
	for (i = 1; i <= sh->argc; i++)
		_unsetenv(sh, sh->argv[i]);

	return (0);
}

/**
 * env_list - compiles the env linked list
 * @sh: ....
 * Return: Always 0
 */
int env_list(shell_t *sh)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	sh->env = node;
	return (0);
}

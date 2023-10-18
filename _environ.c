#include "mymain.h"

/**
 * get_environ - function prints env string arrays
 * @sh: structuct
 * Return: Always 0
 */
char **get_environ(shell_t *sh)
{
	if (!sh->environ || sh->env_changed)
	{
		sh->environ = list_to_strings(sh->env);
		sh->env_changed = 0;
	}

	return (sh->environ);
}

/**
 * _unsetenv - deselects an environment variable
 * @sh: structut value
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenv(shell_t *sh, char *var)
{
	list_t *node = sh->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = _strstr(node->str, var);
		if (p && *p == '=')
		{
			sh->env_changed = del_node(&(sh->env), i);
			i = 0;
			node = sh->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (sh->env_changed);
}

/**
 * _setenv - sets a new environment variable
 * @sh: struct
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int _setenv(shell_t *sh, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = sh->env;
	while (node)
	{
		p = _strstr(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			sh->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(sh->env), buf, 0);
	free(buf);
	sh->env_changed = 1;
	return (0);
}

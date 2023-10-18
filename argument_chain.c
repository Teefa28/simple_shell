#include "mymain.h"

/**
 * is_cmd_arg - function checks for char value in buffer
 * @sh: structuct value
 * @buf: the char to check
 * @p: address of current position in buffer
 *
 * Return: 1 if chain is delimiter, else 0
 */
int is_cmd_arg(shell_t *sh, char *buf, size_t *p)
{
	size_t checker = *p;

	if (buf[checker] == '|' && buf[checker + 1] == '|')
	{
		buf[checker] = 0;
		checker++;
		sh->cmd_buff_type = OR;
	}
	else if (buf[checker] == '&' && buf[checker + 1] == '&')
	{
		buf[checker] = 0;
		checker++;
		sh->cmd_buff_type = AND;
	}
	else if (buf[checker] == ';')
	{
		buf[checker] = 0;
		sh->cmd_buff_type = PIPE;
	}
	else
		return (0);
	*p = checker;
	return (1);
}

/**
 * chain_checker - operates on previous sh
 * @sh: structuct
 * @buf: ...
 * @p: current
 * @i: initial
 * @len: buffer length
 */
void chain_checker(shell_t *sh, char *buf, size_t *p, size_t i, size_t len)
{
	size_t checker = *p;

	if (sh->cmd_buff_type == AND)
	{
		if (sh->status)
		{
			buf[i] = 0;
			checker = len;
		}
	}
	if (sh->cmd_buff_type == OR)
	{
		if (!sh->status)
		{
			buf[i] = 0;
			checker = len;
		}
	}

	*p = checker;
}

/**
 * change_alias - changes an aliases
 * @sh: struct
 *
 * Return: 1 if changed, Else 0
 */
int change_alias(shell_t *sh)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(sh->alias, sh->argv[0], '=');
		if (!node)
			return (0);
		free(sh->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		sh->argv[0] = p;
	}
	return (1);
}

/**
 * change_vars - changes vars in the tokenized string
 * @sh: structuctarameter struct
 *
 * Return: 1 if changed, 0 otherwise
 */
int change_vars(shell_t *sh)
{
	int i = 0;
	list_t *node;

	for (i = 0; sh->argv[i]; i++)
	{
		if (sh->argv[i][0] != '$' || !sh->argv[i][1])
			continue;

		if (!_strcmp(sh->argv[i], "$?"))
		{
			change_string(&(sh->argv[i]),
				_strdup(convert_number(sh->status, 10, 0)));
			continue;
		}
		if (!_strcmp(sh->argv[i], "$$"))
		{
			change_string(&(sh->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(sh->env, &sh->argv[i][1], '=');
		if (node)
		{
			change_string(&(sh->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		change_string(&sh->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * change_string - changes string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if changed, 0 otherwise
 */
int change_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

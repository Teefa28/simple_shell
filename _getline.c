#include "mymain.h"

/**
 * input_buf - buffers chained commands
 * @sh: struct value
 * @buf: arg
 * @len: arg
 *
 * Return: resulting bytes
 */
ssize_t input_buf(shell_t *sh, char **buf, size_t *len)
{
	ssize_t b = 0;
	size_t length = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		b = getline(buf, &length, stdin);
#else
		b = _getline(sh, buf, &length);
#endif
		if (b > 0)
		{
			if ((*buf)[b - 1] == '\n')
			{
				(*buf)[b - 1] = '\0';
				b--;
			}
			sh->linecount_flag = 1;
			comment_handler(*buf);

			compile_hist(sh, *buf, sh->histcount++);
			{
				*len = b;
				sh->cmd_buff = buf;
			}
		}
	}
	return (b);
}

/**
 * get_input - gets a line minus the newline
 * @sh: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(shell_t *sh)
{
	static char *buf;
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(sh->arg), *p;

	_putchar(BUFF_FLUSH);
	r = input_buf(sh, &buf, &len);
	if (r == -1)
		return (-1);
	if (len)
	{
		j = i;
		p = buf + i;

		chain_checker(sh, buf, &j, i, len);
		while (j < len)
		{
			if (is_cmd_arg(sh, buf, &j))
				break;
			j++;
		}

		i = j + 1;
		if (i >= len)
		{
			i = len = 0;
			sh->cmd_buff_type = CMD_NORM;
		}

		*buf_p = p;
		return (_strlen(p));
	}

	*buf_p = buf;
	return (r);
}

/**
 * read_buf - function reads a buffer
 * @sh: ...
 * @buf: buffer
 * @i: size
 *
 * Return: value of r
 */
ssize_t read_buf(shell_t *sh, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(sh->readfd, buf, BUFF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - prints the next line of input from STDIN
 * @sh: ...
 * @ptr: pointer value
 * @length: size
 *
 * Return: s
 */
int _getline(shell_t *sh, char **ptr, size_t *length)
{
	static char buf[BUFF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(sh, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p)
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - interrupts the ctrl-C function
 * @sig_num: the signal number
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUFF_FLUSH);
}

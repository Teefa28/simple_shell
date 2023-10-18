#include "mymain.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	shell_t sh[] = {INFO_INIT};
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				puts(av[0]);
				puts(": 0: Can't open ");
				puts(av[1]);
				putchar('\n');
				putchar(BUFF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		sh->readfd = fd;
	}
	env_list(sh);
	read_hist(sh);
	print_shell(sh, av);
	return (EXIT_SUCCESS);
}

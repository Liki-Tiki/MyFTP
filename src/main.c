/*
** EPITECH PROJECT, 2018
** main
** File description:
** main
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ftp.h"

void	print_help(int ac, char **av)
{
	if (ac == 2 && strcmp(av[1], "-help") == 0)
	{
		printf("USAGE: ./server port path\n");
		printf("\tport is the port number");
		printf("on wich the server socket listens\n");
		printf("\tpath is the path to the");
		printf("home directory for the Anonymous user\n");
	}
}


int	main(int ac, char **av)
{
	t_server srv;

	print_help(ac, av);
	srv.is_passive = 0;
	srand(time(NULL));
	if (ac == 3)
	{
		srv.port = atoi(av[1]);
		srv.path = av[2];
		if (check_args_server(&srv) == 84)
			return (84);
	}
	else
		return (84);
	return (0);
}

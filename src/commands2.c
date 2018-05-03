/*
** EPITECH PROJECT, 2018
** comd
** File description:
** comd
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <netdb.h>
#include <string.h>
#include <dirent.h>
#include "ftp.h"

bool help_command(t_server *srv)
{
	char *str = "214-The following commands are recognized.";
	char *str2 = " USER PASS CWD CDUP QUIT LIST DELE PWD HELP NOOP";

	if (srv->pass != 1)
		write_info(srv->client_fd, "Please login with USER and PASS."
								, "530");
	else
	{
		write_info(srv->client_fd, str, "");
		write_info(srv->client_fd, str2, NULL);
		write_info(srv->client_fd, "Help OK.", "214");
	}
	return (true);
}

bool quit_command(t_server *srv)
{
	write_info(srv->client_fd, "Goodbye.", "221");
	close(srv->client_fd);
	exit(EXIT_SUCCESS);
}

bool    noop_command(t_server *srv)
{
	if (srv->pass != 1)
		write_info(srv->client_fd, "Please login with USER and PASS."
								, "530");
	else
		write_info(srv->client_fd, "NOOP ok.", "200");
	return (true);
}

bool cdup_command(t_server *srv)
{
	if (srv->pass != 1)
		write_info(srv->client_fd, "Please login with USER and PASS."
								, "530");
	else
	{
		chdir("../");
		write_info(srv->client_fd, "Directory successfully changed."
								, "250");
	}
	return (true);
}

bool cwd_command(t_server *srv)
{
	if (srv->user == 0 && srv->pass == 0)
		write_info(srv->client_fd, "Please login with USER and PASS."
								, "530");
	else {
		if (access(srv->tab[1], R_OK & F_OK) == -1)
			write_info(srv->client_fd, "Failed to change directory."
							, "550");
		if (chdir(srv->tab[1]) != -1)
		{
			srv->pwd = strdup(srv->tab[1]);
			chdir(srv->pwd);
			write_info(srv->client_fd,
				"Directory successfully changed."
								, "250");
		}
	}
	return (true);
}

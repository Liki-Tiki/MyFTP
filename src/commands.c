/*
** EPITECH PROJECT, 2018
** cmd
** File description:
** cmd
*/

#include "ftp.h"
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

bool user_command(t_server *srv)
{
	if (!(srv->tab[1]) || strlen(srv->tab[1]) == 0)
		write_info(srv->client_fd, "Permission denied.", "530");

	else if (srv->user == 1 && srv->pass == 1)
		write_info(srv->client_fd, "Already logged in..", "230");

	else
	{
		srv->info = strdup(srv->tab[1]);
		write_info(srv->client_fd, "Please specify the password.",
			"331");
		if (strcasecmp(srv->info, "Anonymous") == 0)
			srv->user = 1;
	}
	return (true);
}

bool pass_command(t_server *srv)
{
	if (srv->pass == 1)
		write_info(srv->client_fd, "Can't change from guest user.",
			"230");
	else if (srv->user == 1 && srv->pass == 0)
	{
		srv->pass = 1;
		srv->user = 1;
		write_info(srv->client_fd, "Login successful.", "230");
	}
	else if (srv->user == 0 && srv->pass == 0) {
		write_info(srv->client_fd, "Login with USER first.", "530");
	}
	return (0);
}

bool pwd_command(t_server *srv)
{
	char str[4096] = {0};

	if (srv->pass != 1)
		write_info(srv->client_fd, "Please login with USER and PASS.",
			"530");
	else if (getcwd(str, sizeof(str)) != NULL)
	{
		write_info(srv->client_fd, str, "257");
		srv->pwd = strdup(str);
	}
	free(srv->pwd);
	return (true);
}

bool dele_command(t_server *srv)
{
	if (srv->user == 0 && srv->pass == 0)
		write_info(srv->client_fd, "Please login with USER and PASS."
			, "530");
	if (remove(srv->tab[1]) == -1)
		write_info(srv->client_fd, "No permission.", "550");
	else
		write_info(srv->client_fd, "File deleted.", "250");
	return (true);
}

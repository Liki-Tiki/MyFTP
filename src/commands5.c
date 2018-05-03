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
#include <string.h>
#include <string.h>
#include <dirent.h>

void     do_commands(t_server *srv, char *cmd)
{
	char *tab;
	char *stock;

	stock = strdup(cmd);
	if (stock[0] == ' ')
		write_info(srv->client_fd, "Unknown command.", "500");
	else {
		tab = strtok(cmd, " \n\r");
		if (tab == NULL)
			return;
		find_ftp_command(srv, tab, stock);
		free(stock);
	}
}

void    loop_exec(t_server *srv)
{
	pid_t   pid;

	srv->user = 0;
	srv->pass = 0;
	while (42)
	{
		if ((srv->client_fd = accept(srv->fd, (struct sockaddr *)
			&srv->s_in_client, &srv->s_in_size)) >= 0)
		{
			pid = fork();
			forking(srv, pid);
		}
	}
}

void    write_info(int fd, const char *str, const char *mdp)
{
	if (mdp != NULL)
	{
		write(fd, mdp, strlen(mdp));
		write(fd, " ", 1);
	}
	write(fd, str, strlen(str));
	write(fd, "\r\n", strlen("\r\n"));
}

int     set_addr(t_server *srv)
{
	srv->sock.sin_family = AF_INET;
	srv->sock.sin_addr.s_addr = INADDR_ANY;
	srv->sock.sin_port = htons(srv->port);
	if (bind(srv->fd, (struct sockaddr *)
		&srv->sock, sizeof(srv->sock)) < 0)
	{
		printf("Bad bind\n");
		return (84);
	}
	if (listen(srv->fd, 42) == -1)
	{
		if (close(srv->fd) == -1)
			return (84);
		return (84);
	}
	loop_exec(srv);
	return (0);
}

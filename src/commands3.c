/*
** EPITECH PROJECT, 2018
** ok
** File description:
** k
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

int create_passive_socket(t_server *srv)
{
	socklen_t addr_size = sizeof(srv->data_sock);

	srv->data_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (srv->data_fd == -1)
		return (-1);
	srv->data_sock.sin_family = AF_INET;
	srv->data_sock.sin_addr.s_addr = INADDR_ANY;
	srv->data_sock.sin_port = htons(0);
	if (bind(srv->data_fd, (struct sockaddr *)&srv->data_sock,
			sizeof(srv->data_sock)) == -1)
		return (-1);
	getsockname(srv->data_fd, (struct sockaddr *)&srv->data_sock
						, &addr_size);
	listen(srv->data_fd, 10);
	return (0);
}

bool command_pasv(t_server *srv)
{
	char msg[1024];
	struct in_addr ip_addr;
	socklen_t addr_size = sizeof(ip_addr);
	char *str = "Unable to enter in passive mode.";

	srv->datafd = create_passive_socket(srv);
	if (srv->datafd == -1)
		write_info(srv->client_fd, str, "550");
	ip_addr.s_addr = INADDR_ANY;
	getsockname(srv->fd, (struct sockaddr *)&ip_addr, &addr_size);
	sprintf(msg, "Entering passive mode (0,0,0,0,%d,%d).",
		ntohs(srv->data_sock.sin_port) / 256,
		ntohs(srv->data_sock.sin_port) % 256);
	srv->is_passive = 1;
	write_info(srv->client_fd, msg, "227");
	return (true);
}

int     check_stor(t_server *srv)
{
	if ((srv->lsss = open(srv->tab[1], O_WRONLY | O_CREAT, S_IRUSR |
			S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)) < 0)
		write_info(srv->client_fd, "Error in file", "550");
	else
	{
		write_info(srv->client_fd, "Success", "150");
		srv->buffer2[0] = 0;
		while ((srv->ret2 = read(srv->client_data_fd,
					srv->buffer2, 1)) > 0)
		{
			if (srv->buffer2[0] != '\r')
				write(srv->lsss, &srv->buffer2, 1);
		}
		write_info(srv->client_fd, "Finish", "226");
		close(srv->lsss);
	}
	return (0);
}

bool stor_command(t_server *srv)
{
	struct in_addr ip_addr;
	socklen_t addr_size = sizeof(ip_addr);

	if ((srv->client_data_fd = accept(srv->data_fd,
			(struct sockaddr *)&srv->data_socket,
					&addr_size)) == -1)
		write_info(srv->client_data_fd, "Unable to perform socket."
							, "550");
	if (srv->user == 0 && srv->user == 0)
		write_info(srv->client_fd, "Login with USER and PASS.", "530");
	else if (srv->is_passive == 0 && srv->user == 1 && srv->user == 1)
		write_info(srv->client_fd, "Permission denied.", "550");
	else {
		if (srv->tab[1] == NULL)
			close(srv->client_data_fd);
		else
			check_stor(srv);
	}
	close(srv->client_data_fd);
	return (true);
}

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

int     check_retr(t_server *srv)
{
	write_info(srv->client_fd, "Success", "150");
	srv->buffer[0] = 0;
	while ((srv->ret = read(srv->lss, srv->buffer, 1)) > 0) {
		if (srv->buffer[0] == '\n')
			write(srv->client_data_fd, "\r\n", 2);
		else
			write(srv->client_data_fd, srv->buffer, srv->ret);
	}
	write_info(srv->client_fd, "Finish", "226");
	close(srv->lss);
	return (0);
}

int	tablen(char **tab)
{
	int	i = 0;

	while (tab[i])
		i++;
	printf("value : %d\n", i);
	return (i);
}

bool    retr_command(t_server *srv)
{
	struct in_addr ip_addr;
	socklen_t addr_size = sizeof(ip_addr);

	if ((srv->client_data_fd = accept(srv->data_fd,
			(struct sockaddr *)&srv->data_socket,
					&addr_size)) == -1)
		write_info(srv->client_data_fd, "Unable to perform socket."
					, "550");
	if (srv->is_passive == 0 && srv->user == 1 && srv->pass == 1)
		write_info(srv->client_fd, "Use PORT or PASV first.", "425");
	else if (srv->user == 0 || srv->pass == 0)
		write_info(srv->client_fd, "Connect USER and PASS", "530");
	else if (access(srv->tab[1], R_OK | F_OK) == -1)
		write_info(srv->client_fd, "ERROR in file", "550");
	else if ((srv->lss = open(srv->tab[1], O_RDONLY)) == -1)
		write_info(srv->client_fd, "Error in file", "550");
	else
		check_retr(srv);
	close(srv->client_data_fd);
	return (true);
}

int     check_list(t_server *srv)
{
	if (srv->tab[1] == NULL)
		srv->tab[1] = strdup("./");
	srv->ls = popen("ls -la", "r");
	write_info(srv->client_fd, "List incoming !", "150");
	while (!feof(srv->ls) && (fread(srv->buff, 1, 1, srv->ls)) > 0) {
		if (srv->buff[0] == '\n' && srv->buff != false)
			write(srv->client_data_fd, "\r\n", 2);
		else if (srv->buff[0] != '\n' && srv->buff != false)
			write(srv->client_data_fd, &srv->buff, 1);
		memset(srv->buff, 0, 1024);
	}
	write_info(srv->client_fd, "End of listing", "226");
	pclose(srv->ls);
	return (0);
}

bool list_command(t_server *srv)
{
	struct in_addr ip_addr;
	socklen_t addr_size = sizeof(ip_addr);

	if ((srv->client_data_fd = accept(srv->data_fd,
		(struct sockaddr *)&srv->data_socket,
				&addr_size)) == -1)
		write_info(srv->client_data_fd, "Unable to perform socket.",
				"550");
	if (srv->is_passive == 0 && srv->user == 1 && srv->pass == 1)
		write_info(srv->client_fd, "Use PORT or PASV first.", "425");
	else if (srv->user == 0 || srv->pass == 0)
		write_info(srv->client_fd, "Please login with USER and PASS."
							, "530");
	else
		check_list(srv);
	close(srv->client_data_fd);
	return (true);
}

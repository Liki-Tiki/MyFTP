/*
** EPITECH PROJECT, 2018
** k
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
#include <string.h>
#include <dirent.h>

static t_server_commands g_commands[] =
{
	{"USER", &user_command},
	{"PASS", &pass_command},
	{"PWD", &pwd_command},
	{"CWD", &cwd_command},
	{"LIST", &list_command},
	{"CDUP", &cdup_command},
	{"RETR", &retr_command},
	{"PASV", &command_pasv},
	{"HELP", &help_command},
	{"NOOP", &noop_command},
	{"DELE", &dele_command},
	{"QUIT", &quit_command},
	{"STOR", &stor_command},
	{NULL, NULL}
};

void	my_str_to_wordtab(char ***tab, char *str, char a)
{
	int           i = 0;
	int           x = 0;
	int           y;

	if (((*tab) = malloc(sizeof(char *) * (strlen(str) + 1))) == NULL)
		return ;
	while (str[i] != '\0') {
		if (((*tab)[x] = malloc(strlen(str) *
			sizeof(char) + 1)) == NULL)
			return ;
		y = 0;
		while (str[i] != '\0' && (str[i] != a && str[i] != '\n'))
			(*tab)[x][y++] = str[i++];
		(*tab)[x][y] = '\0';
		x++;
		if (str[i] != '\0')
			i++;
	}
	(*tab)[x] = NULL;
}

int	check_args_server(t_server *srv)
{
	DIR *dir;

	if (!(srv->port >= 0 && srv->port <= 65535))
		return (84);
	dir = opendir(srv->path);
	if (dir == NULL)
		return (84);
	else {
		chdir(srv->path);
		getcwd(srv->path, 256);
		srv->fd = socket(AF_INET, SOCK_STREAM, 6);
		if (srv->fd == -1)
			return (84);
		if (setsockopt(srv->fd, SOL_SOCKET, SO_REUSEADDR,
		&(int){ 1 }, sizeof(int)) < 0)
			return (84);
		if (set_addr(srv) == 84)
			return (84);
	}
	return (0);
}

bool	find_ftp_command(t_server *srv, char *tab, char *cmd)
{
	int	count;

	srv->cmd = tab;
	srv->string = cmd;
	my_str_to_wordtab(&srv->tab, srv->string, ' ');
	for (count = 0; g_commands[count].fonc; ++count)
	{
		if (strcasecmp(g_commands[count].fonc, tab) == 0)
		{
			return (g_commands[count].func
				(srv, cmd, srv->client_fd));
		}
	}
	if (g_commands[count].fonc == NULL && srv->user == 1 && srv->pass == 1)
		write_info(srv->client_fd, "Unknown command.", "500");
	else
		write_info(srv->client_fd, "Please login first", "530");
	return (true);
}

void	forking(t_server *srv, pid_t pid)
{
	char *cmd;
	char buffer[4097];
	int re = 1;

	memset(buffer, 0, 4097);
	if (pid == 0)
	{
		write_info(srv->client_fd, "(vsFTPd 3.0.0)", "220");
		while (re)
		{
			re = read(srv->client_fd, buffer, 4096);
			if ((cmd = strtok(buffer, "\r\n")) != NULL)
				do_commands(srv, cmd);
			memset(buffer, 0, 4097);
			usleep(0);
		}
		exit(84);
	}
	else
		close(srv->client_fd);
}

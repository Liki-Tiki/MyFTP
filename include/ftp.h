/*
** EPITECH PROJECT, 2018
** ftp
** File description:
** ftp
*/

#ifndef _MY_FTP_H_
#define _MY_FTP_H_

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>

# define CM_USER	"USER"
# define CM_PASS	"PASS"

typedef struct s_server
{
	FILE    *ls;
	char	*message;
	char	**tab;
	char	*path;
	char	*pwd;
	char	*string;
	int	port;
	char	*cmd;
	int	is_passive;
	int	ip;
	int	datafd;
	int	data_socket;
	char	buffer[4096];
	char	buffer2[2];
	int	lss;
	int	lsss;
	ssize_t ret;
	ssize_t ret2;
	int	client_data_fd;
	int	data_fd;
	char	buff[1024];
	char	*info;
	int	fd;
	int	pass;
	int	user;
	int	client_fd;
	char	*client_ip;
	socklen_t s_in_size;
	struct sockaddr_in data_sock;
	struct sockaddr_in sock;
	struct sockaddr_in s_in_client;
}		t_server;

typedef struct timeval		t_time;
typedef struct  s_server_commands
{
	const char   *fonc;
	bool          (*func)(t_server *, char *, int);
}               t_server_commands;


int	check_args_server(t_server *srv);
int	set_addr(t_server *srv);
bool	user_command();
void	write_info(int, const char *, const char *);
void	my_str_to_wordtab(char ***, char *, char);
bool	command_pasv();
bool	pass_command();
bool	list_command();
bool	quit_command();
bool	help_command();
bool	cwd_command();
bool	command_list();
void	free_tab(char **);
bool	noop_command();
bool	dele_command();
bool	retr_command();
bool	stor_command();
int	check_stor(t_server *srv);
bool	find_ftp_command(t_server *srv, char *, char *);
void	do_commands(t_server *srv, char *cmd);
bool	cdup_command();
bool	cwd_command();
bool	command_pasv();
bool	pwd_command();
void	loop_exec(t_server *srv);
void	forking(t_server *srv, pid_t pid);

#endif /* _MY_FTP_H_ */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 02:41:58 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/24 17:09:39 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_command	get_command(char *arg, char *envp[]);
static char			**get_path_var(char *envp[]);
static char			*get_pathname(char **path, char *cmd);

t_command	*fetch_commands(char *argv[], char *envp[],	t_fd_pair iof)
{
	t_command	*cmd;
	int			status;

	cmd = malloc(2 * sizeof (t_command));
	cmd[0] = (t_command){.status = -1, .param = NULL, .pathname = NULL};
	cmd[1] = (t_command){.status = -1, .param = NULL, .pathname = NULL};
	if (iof.fd[0] != -1)
		cmd[0] = get_command(argv[2], envp);
	cmd[1] = get_command(argv[3], envp);
	if (cmd[1].status)
	{
		status = cmd[1].status;
		free_command_vector(2, &cmd);
		close_fd_pair(iof);
		exit(status);
	}
	return (cmd);
}

static t_command	get_command(char *arg, char *envp[])
{
	t_command	command;
	char		**paths;

	command.param = ft_partial_split(arg, ' ', '\'');
	if (command.param == NULL)
	{
		command.status = 1;
		print_coded_error("pipex", "get_command", EINVAL);
	}
	paths = get_path_var(envp);
	command.status = path_verifier(paths);
	if (command.status)
		return (command);
	command.pathname = get_pathname(paths, command.param[0]);
	ft_free_char_matrix(&paths);
	command.status = pathname_verifier(command);
	return (command);
}

static char	**get_path_var(char *envp[])
{
	char	**path;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], ENV_PATH_VAR, ft_strlen(ENV_PATH_VAR)) == 0)
		{
			path = ft_split(envp[i] + ft_strlen(ENV_PATH_VAR), ':');
			break ;
		}
		i++;
	}
	return (path);
}

static char	*get_pathname(char **path, char *cmd)
{
	char	*pathname;
	char	*temp;
	int		i;

	if (!path || !cmd)
		return (NULL);
	i = 0;
	while (path[i])
	{
		temp = ft_strjoin(path[i], "/");
		pathname = ft_strjoin(temp, cmd);
		ft_null_free((void *)(&temp));
		if (!access(pathname, X_OK))
			break ;
		ft_null_free((void *)(&pathname));
		i++;
	}
	return (pathname);
}

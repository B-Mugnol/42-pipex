/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 02:41:58 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/31 01:30:52 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_command	get_command(char *arg, char *envp[]);
static char			**get_path_var(char *envp[]);
static char			*get_pathname(char **path, char *cmd);

t_command	*fetch_commands(int cmd_count, char *argv[], char *envp[],
						t_fd_pair iof)
{
	t_command	*cmd;
	int			i;

	cmd = malloc((cmd_count) * sizeof (t_command));
	if (!cmd)
		print_error_terminate("pipex: malloc", iof);
	i = -1;
	while (++i < cmd_count)
		cmd[i] = (t_command){.status = EXIT_FAILURE, .param = NULL,
			.pathname = NULL};
	if (iof.fd[0] != -1)
		cmd[0] = get_command(argv[0], envp);
	i = 0;
	while (++i < cmd_count)
		cmd[i] = get_command(argv[i], envp);
	return (cmd);
}

static t_command	get_command(char *arg, char *envp[])
{
	t_command	command;
	char		**paths;

	command.param = ft_partial_split(arg, ' ', '\'');
	if (command.param == NULL)
	{
		command.status = EXIT_FAILURE;
		print_coded_error("pipex", "get_command", EINVAL);
	}
	paths = get_path_var(envp);
	command.status = path_verifier(paths);
	if (command.status == EXIT_FAILURE)
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

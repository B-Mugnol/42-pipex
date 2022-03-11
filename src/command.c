/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 02:41:58 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/11 19:27:27 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**get_path_var(char *envp[]);
static char	*get_pathname(char **path, char *cmd);
static void	clear_matrix(char ***m);

t_command	get_command(char *arg, char *envp[])
{
	t_command	command;
	char		**paths;
	int			i;

	command.param = ft_partial_split(arg, ' ', '\'');
	if (command.param == NULL)
	{
		command.param_count = 0;
		print_coded_error("pipex", "get_command", EINVAL);
	}
	i = 0;
	while (command.param[i])
		i++;
	command.param_count = i;
	paths = get_path_var(envp);
	if (path_verifier(&command, &paths))
		return (command);
	command.pathname = get_pathname(paths, command.param[0]);
	clear_matrix(&paths);
	if (pathname_verifier(&command))
		return (command);
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
		if (!access(pathname, F_OK))
			break ;
		ft_null_free((void *)(&pathname));
		i++;
	}
	return (pathname);
}

static void	clear_matrix(char ***m)
{
	size_t	i;

	if (!*m)
		return ;
	i = 0;
	while ((*m)[i])
		i++;
	ft_free_matrix((void *)(m), i);
}

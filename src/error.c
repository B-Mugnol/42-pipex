/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 17:19:40 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/17 20:46:28 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_coded_error(char *program, char *err_location, int err_code)
{
	if (program)
	{
		ft_putstr_fd(program, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err_location)
	{
		ft_putstr_fd(err_location, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(strerror(err_code), 2);
	ft_putstr_fd("\n", 2);
}

void	print_custom_error(char *program, char *err_location, char *err_msg)
{
	if (program)
	{
		ft_putstr_fd(program, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err_location)
	{
		ft_putstr_fd(err_location, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err_msg)
	{
		ft_putstr_fd(err_msg, 2);
	}
	ft_putstr_fd("\n", 2);
}

void	print_error_exit(char *err_location)
{
	perror(err_location);
	exit(EXIT_FAILURE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmugnol- <bmugnol-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/07 17:19:40 by bmugnol-          #+#    #+#             */
/*   Updated: 2022/03/10 20:53:33 by bmugnol-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_coded_error(char *program, char *err_location, int err_code)
{
	if (program)
	{
		ft_putstr_fd(program, 1);
		ft_putstr_fd(": ", 1);
	}
	if (err_location)
	{
		ft_putstr_fd(err_location, 1);
		ft_putstr_fd(": ", 1);
	}
	ft_putstr_fd(strerror(err_code), 1);
	ft_putstr_fd("\n", 1);
}

void	print_custom_error(char *program, char *err_location, char *err_msg)
{
	if (program)
	{
		ft_putstr_fd(program, 1);
		ft_putstr_fd(": ", 1);
	}
	if (err_location)
	{
		ft_putstr_fd(err_location, 1);
		ft_putstr_fd(": ", 1);
	}
	if (err_msg)
	{
		ft_putstr_fd(err_msg, 1);
	}
	ft_putstr_fd("\n", 1);
}

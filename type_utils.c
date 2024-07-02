/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:21:48 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 15:21:49 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redir(t_token_type type)
{
	if (type == redir_app || type == redir_in || type == redir_out || \
		type == heredoc)
		return (1);
	return (0);
}

int	is_command(t_token_type type)
{
	if (type == command || type == subshell)
		return (1);
	return (0);
}

int	is_operator(t_token_type type)
{
	return (type == and_op || type == or_op || type == pipe_op);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 17:07:52 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 17:07:53 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# define CWD_BUFF 256

typedef struct s_cwd
{
	char	*path;
	int		size;
}			t_cwd;

typedef struct s_env
{
	char	**env;
	char	**local;
}		t_env;

#endif

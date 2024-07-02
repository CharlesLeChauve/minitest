/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 15:25:04 by tgibert           #+#    #+#             */
/*   Updated: 2024/07/02 15:25:20 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_cpy_strtab(char **tab)
{
	int		i;
	char	**env_cpy;

	i = 0;
	while (tab[i])
		i++;
	env_cpy = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (tab[i])
	{
		env_cpy[i] = ft_strdup(tab[i]);
		i++;
	}
	env_cpy[i] = NULL;
	return (env_cpy);
}

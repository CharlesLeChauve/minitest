/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dirs_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 07:17:36 by tgibert           #+#    #+#             */
/*   Updated: 2024/06/27 07:50:09 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

char	*get_cwd(void)
{
	size_t	buffer_size;
	char	*path;

	buffer_size = CWD_BUFF;
	path = (char *)malloc(CWD_BUFF);
	while (!getcwd(path, buffer_size))
	{
		path = ft_realloc(path, buffer_size, buffer_size + buffer_size);
		buffer_size += buffer_size;
	}
	return (path);
}

void	pwd(void)
{
	char	*wd;

	wd = get_cwd();
	printf("%s\n", wd);
	free(wd);
}

void	act_env_pwd(char ***env)
{
	char	**pwds;
	char	*pwd_ptr;

	pwds = (char **)malloc(3 * sizeof(char *));
	if (!pwds)
		return ;
	pwds[0] = ft_strjoin_free("PWD=", get_cwd(), 1);
	pwd_ptr = get_env_var(*env, "PWD");
	if (pwd_ptr)
		pwds[1] = ft_strjoin_free("OLDPWD=", ft_strdup(pwd_ptr + 4), 1);
	else
		pwds[1] = NULL;
	pwds[2] = NULL;
	replace_existing_vars(&pwds, env);
	free(pwds);
}

int	change_directory(char *path, char ***env)
{
	DIR	*directory;

	if (!path)
		path = "/home";
	directory = opendir(path);
	if (!directory)
	{
		perror("tash: cd");
		return (1);
	}
	else
		closedir(directory);
	if (chdir(path))
	{
		perror("chdir() error\n");
		return (1);
	}
	else
		act_env_pwd(env);
	return (0);
}

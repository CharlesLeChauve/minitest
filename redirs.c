#include "minishell.h"

void    restore_stds_and_close_dup(int out_save, int in_save)
{
	if  (out_save != -1)
	{
		dup2(out_save, STDOUT_FILENO);
		close(out_save);
	}
	if (in_save != -1)
	{
		dup2(in_save, STDIN_FILENO);
		close(in_save);
	}
}

int    make_redir_in(t_token_lst *redir)
{
	int fd;

	fd = -1;
	if (redir->type == redir_in)
	{
		if (check_acces(redir->text, read_o))
		{
			fd = open_mode(redir->text, read_o);
			dup2(fd, STDIN_FILENO);
		}
		else
			return (1);
	}
	else if (redir->type == heredoc)
	{
		heredoc_handle(redir->text);
	}
	return (0);
}

int    make_redir_out(t_token_lst *redir)
{
	int fd;

	fd = -1;
	if (redir->type == redir_out)
	{
		fd = open_write(redir->text, truncate_o);
		dup2(fd, STDOUT_FILENO);
	}
	else if (redir->type == redir_app)
	{
		fd = open_write(redir->text, append_o);
		if (fd == -1)
		{
			perror("open_write");
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (1);
		}
		close(fd);
	}
	return (0);
}

void    handle_redirs(t_cmd_block *cmd_block)
{
	t_list  *redirs;

	if (cmd_block->redirs)
	{
		redirs = cmd_block->redirs;
		while (redirs)
		{
			//si l'une des redirs ne peut pas se produire
			if (((t_token_lst *)redirs->content)->type == redir_app || ((t_token_lst *)redirs->content)->type == redir_out)
				if (make_redir_out((t_token_lst *)redirs->content))
					exit(EXIT_FAILURE) ;
			if (((t_token_lst *)redirs->content)->type == redir_in || ((t_token_lst *)redirs->content)->type == heredoc)
				if (make_redir_in((t_token_lst *)redirs->content))
					exit (EXIT_FAILURE);
			redirs = redirs->next;
		}
		redirs = NULL;
	}
}

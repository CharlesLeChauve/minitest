# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include "minishell.h"

int exec_command_and_redirs(char **tokens)
{
    pid_t pid;
    int status;

    if ((pid = fork()) == -1) 
    {
        perror("fork");
        return -1;
    }
    if (pid == 0) 
    {
        execvp(tokens[0], tokens);
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    if (waitpid(pid, &status, 0) == -1) 
    {
        perror("waitpid");
        return -1;
    }
    if (WIFEXITED(status)) 
        return WEXITSTATUS(status);
    return -1;
}

int	exec_ast(t_ast_node *ast)
{
    if (ast->type == pipe_op ) // && pas de redirection de sortie a gauche
    {
        int pipe_fds[2];
        pid_t pid1, pid2;
        if (pipe(pipe_fds) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if ((pid1 = fork()) == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid1 == 0)
        {
            close(pipe_fds[0]);
            dup2(pipe_fds[1], STDOUT_FILENO);
            close(pipe_fds[1]);
            exit(exec_ast(ast->left));
        }
        if ((pid2 = fork()) == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid2 == 0)
        {
            close(pipe_fds[1]);
            dup2(pipe_fds[0], STDIN_FILENO);
            close(pipe_fds[0]);
            exit(exec_ast(ast->right));
        }
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        return 0;
    }
    else if (ast->type == and_op && !exec_ast(ast->left))
        return (exec_ast(ast->right));
    else if (ast->type == or_op && exec_ast(ast->left))
        return (exec_ast(ast->right));
    else
        return (exec_command_and_redirs(ast->tokens));
}

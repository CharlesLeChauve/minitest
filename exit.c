#include "minishell.h"

int ft_is_numeric(char *str)
{
    int i;
    
    i = 0;
    if (str == NULL || *str == '\0')
        return 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

// void    free_ressources()
// {
    
// }

void ft_exit(char **args)
{
    int exit_code;
    
    if (args[1] == NULL)
        exit(0);
    exit_code = ft_atoi(args[1]);
    if (ft_is_numeric(args[1]))
    {
        if (args[2] != NULL)
        {
            ft_putstr_fd("exit: too many arguments\n", 2);
            return ;
        }
        exit(exit_code);
    }
    else if (!ft_is_numeric(args[1]))
    {
        ft_putstr_fd("exit: numeric argument required\n", 2);
        exit(2);
    }
}
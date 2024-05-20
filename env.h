#ifndef ENV_H
# define ENV_H
# define CWD_BUFF 256

typedef struct s_cwd
{
    char    *path;
    int     size;
} t_cwd;

typedef struct s_env
{
    char **env;
    char **local;
}   t_env;

#endif
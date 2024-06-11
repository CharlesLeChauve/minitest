#include "minishell.h"

// Fonction pour calculer la taille de la liste chaînée
int list_size(t_list *list) {
    int size = 0;
    while (list) {
        size++;
        list = list->next;
    }
    return size;
}

// Fonction pour remplir exec_tab à partir de t_cmd_block
void create_exec_tab(t_cmd_block *cmd_block) {
    int arg_count = list_size(cmd_block->arg);
    int total_size = 1 + arg_count + 1; // command + options + arguments + NULL
    t_list *current;

    // Allouer de la mémoire pour exec_tab
    cmd_block->exec_tab = (char **)malloc(total_size * sizeof(char *));
    if (!cmd_block->exec_tab) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    // Ajouter la command
    cmd_block->exec_tab[index++] = cmd_block->command;
    // Ajouter les options
    // while (current) {
    //     cmd_block->exec_tab[index++] = (char *)current->content;
    //     current = current->next;
    // }
    // Ajouter les arguments
    current = cmd_block->arg;
    while (current) {
        cmd_block->exec_tab[index++] = ft_strdup((char *)current->content);
        current = current->next;
    }
    // Terminer le tableau avec NULL
    cmd_block->exec_tab[index] = NULL;
}

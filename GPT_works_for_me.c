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
    int option_count = list_size(cmd_block->option);
    int arg_count = list_size(cmd_block->arg);
    int total_size = 1 + option_count + arg_count + 1; // commande + options + arguments + NULL

    // Allouer de la mémoire pour exec_tab
    cmd_block->exec_tab = (char **)malloc(total_size * sizeof(char *));
    if (!cmd_block->exec_tab) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int index = 0;

    // Ajouter la commande
    cmd_block->exec_tab[index++] = cmd_block->commande;

    // Ajouter les options
    t_list *current = cmd_block->option;
    while (current) {
        cmd_block->exec_tab[index++] = (char *)current->content;
        current = current->next;
    }

    // Ajouter les arguments
    current = cmd_block->arg;
    while (current) {
        cmd_block->exec_tab[index++] = (char *)current->content;
        current = current->next;
    }

    // Terminer le tableau avec NULL
    cmd_block->exec_tab[index] = NULL;
}

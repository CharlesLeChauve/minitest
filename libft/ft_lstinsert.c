#include "libft.h"

void	ft_lst_insert(t_list *target_node, t_list *new)
{
    if (!new || !target_node)
        return ;
    new->next = target_node->next;
    target_node->next = new;
}

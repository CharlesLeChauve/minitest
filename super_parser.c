/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   super_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgibert <tgibert@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 07:45:00 by tgibert           #+#    #+#             */
/*   Updated: 2024/03/20 09:31:29 by tgibert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitests.h"

int	main(void)
{
	nextsym();
	block();
	expect(end_of_file);
}
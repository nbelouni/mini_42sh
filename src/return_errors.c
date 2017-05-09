/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogokar <dogokar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 21:13:35 by dogokar           #+#    #+#             */
/*   Updated: 2017/05/08 23:30:51 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

int		return_new_prompt(int ret)
{
	if (ret == ERR_EXIT)
		return (ret);
	else if (ret == ERR_NEW_PROMPT)
	{
		set_prompt(PROMPT2, ft_strlen(PROMPT2));
		return (ERR_NEW_PROMPT);
	}
	return (ret);
}

int		free_rg_and_return(char **rg, int ret)
{
	if (rg && rg[0])
	{
		ft_bzero(*rg, ft_strlen(*rg));
		ft_strdel(rg);
	}
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_quick_substitution.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogokar <dogokar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 21:13:30 by dogokar           #+#    #+#             */
/*   Updated: 2017/05/08 23:21:38 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

int		ft_quicksub_with_args2(t_elem *tail, char *tmp, char *p, char **s)
{
	char	*tmp2;
	int		ret;

	ret = 0;
	if ((tmp2 = ft_strsub(&(p[1]), 0, ft_get_index_of(&(p[1]), '^'))) == NULL)
		ret = ERR_EXIT;
	else
	{
		if ((*s = ft_strreplace(tail->name, tmp, tmp2)) == NULL)
			ret = ft_print_error("21sh", ERR_SUBSTITUTION, ERR_NEW_CMD);
		ft_strdel(&tmp2);
	}
	return (ret);
}

int		ft_quicksub_with_args(t_elem *tail, char **s, int *i)
{
	int		ret;
	char	*ptr;
	char	*tmp2;

	if (!(tmp2 = ft_strsub(&((*s)[*i]), 0, ft_get_index_of(&((*s)[*i]), '^'))))
		return (ERR_EXIT);
	ret = 0;
	ptr = ft_strchr(&((*s)[*i]), '^');
	if (ptr[1] == '^')
	{
		if ((*s = ft_strreplace(tail->name, tmp2, "")) == NULL)
			ret = ft_print_error("21sh", ERR_SUBSTITUTION, ERR_NEW_CMD);
	}
	else
		ret = ft_quicksub_with_args2(tail, tmp2, ptr, s);
	ft_strdel(&tmp2);
	return (ret);
}

int		ft_exec_quick_sub(t_elem *tail, char **s, int *i)
{
	int		ret;
	char	*tmp;

	*i += 1;
	ret = 0;
	if ((*s)[*i] == '^')
		return (ft_print_error("21sh", ERR_SUBSTITUTION, ERR_NEW_CMD));
	tmp = *s;
	if (ft_strchr(&((*s)[*i]), '^') == NULL)
	{
		if ((*s = ft_strreplace(tail->name, &(*s)[*i], "")) == NULL)
			ret = ft_print_error("21sh", ERR_SUBSTITUTION, ERR_NEW_CMD);
	}
	else
		ret = ft_quicksub_with_args(tail, s, i);
	ft_strdel(&tmp);
	return (ret);
}

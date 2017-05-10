/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_list_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogokar <dogokar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 21:13:35 by dogokar           #+#    #+#             */
/*   Updated: 2017/05/09 20:57:33 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

int		ft_find_fd(t_token *list)
{
	char	*s;

	s = NULL;
	if (list && list->word && !ft_atoi(list->word) &&
		ft_strcmp(list->word, "0") != 0)
	{
		free(list->word);
		list->word = NULL;
	}
	return (1);
}

/*
**Faut faire des check avant de trier la liste, pour l'instant on garde ca
*/

int		create_cmd(t_token **list)
{
	t_token		*tmp;
	int			ret;

	ret = 1;
	if ((*list)->prev && (PREVISCMD((*list)) || (*list)->prev->type == TARGET))
		return (1);
	tmp = ft_tokenew(CMD, NULL);
	if ((*list)->prev)
	{
		(*list)->prev->next = tmp;
		tmp->prev = (*list)->prev;
		ret = 0;
	}
	(*list)->prev = tmp;
	tmp->next = (*list);
	return (ret);
}

void	sort_list(t_token *elem)
{
	if ((ISREDIR(elem->type)) && elem->next && elem->next->type == CMD)
		elem->next->type = TARGET;
	if (elem->type == TARGET && elem->next && NEXTISCMD(elem))
		check_target_place(&elem);
	if (elem->type == CMD && elem->prev && PREVISCMD(elem))
		elem->type = ARG;
	if (elem->type == CMD)
		elem = is_local_var(elem);
}

int		sort_list_token(t_token **list, t_completion *completion, t_lst *hist)
{
	t_token		*elem;

	elem = *list;
	while (elem)
	{
		if (elem->type == FD_IN)
			(ft_swap_in(&elem) ? *list : (*list = elem));
		if (is_dir_type(elem->type) && ft_find_fd(elem))
			create_cmd(&elem) ? *list : (*list = elem);
		if (ISAMP(elem) && elem->next && check_error_out(elem->next))
			return (0);
		sort_list(elem);
		if (elem->type == DL_DIR && elem->next && elem->next->type == TARGET)
			if (here_doc(elem->next, completion, hist) == ERR_NEW_CMD)
				return (ERR_NEW_CMD);
		if (!elem->prev || !is_dir_type(elem->prev->type))
			expand_args(list, &elem);
		elem = elem->next;
	}
	while ((*list) && (*list)->prev)
		(*list) = (*list)->prev;
	return (0);
}

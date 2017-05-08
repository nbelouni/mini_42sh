/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   can_create_tree.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogokar <dogokar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 20:25:06 by dogokar           #+#    #+#             */
/*   Updated: 2017/05/08 23:21:38 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

int			print_err_message(char *s)
{
	ft_putstr_fd("21sh: syntax error near unexpected token `", 2);
	if (s)
		ft_putstr_fd(s, 2);
	ft_putstr_fd("'\n", 2);
	set_prompt(PROMPT1, ft_strlen(PROMPT1));
	return (ERR_NEW_CMD);
}

int			is_new_prompt(t_token *prev)
{
	if (prev)
	{
		if (!(prev = prev->next))
			return (TRUE);
		if (is_dir_type(prev->type))
			return (print_err_message("newline"));
		if ((is_separator_type(prev->type) && prev->type != DOT) ||
	(count_prev_char(prev->word, ft_strlen(prev->word), '\\') % 2 == 1))
		{
			set_prompt(PROMPT2, ft_strlen(PROMPT2));
			return (ERR_NEW_PROMPT);
		}
		set_prompt(PROMPT1, ft_strlen(PROMPT1));
	}
	return (TRUE);
}

int			is_parse_error(t_token *tmp, t_token *prev)
{
	if (prev && is_dir_type(prev->type) &&
	tmp && (is_dir_type(tmp->type) || tmp->type == TARGET))
	{
		if (ft_strlen(tmp->word) > 0 &&
		is_number(tmp->word, ft_strlen(tmp->word)) == TRUE)
			return (print_err_message(tmp->word));
	}
	if (is_separator_type(tmp->type) &&
	(!prev || is_separator_type(prev->type) || is_dir_type(prev->type)))
		return (print_err_message(tmp->word));
	if (is_dir_type(tmp->type) &&
	(prev && is_dir_type(prev->type)))
		return (print_err_message(tmp->word));
	return (0);
}

int			can_create_tree(t_token *lst)
{
	t_token	*tmp;
	t_token	*prev;
	int		ret;

	if (!lst)
		return (0);
	tmp = lst;
	prev = NULL;
	while (tmp)
	{
		prev = tmp->prev;
		if ((ret = is_parse_error(tmp, prev)))
			return (ret);
		tmp = tmp->next;
	}
	if (!prev &&
	(count_prev_char(lst->word, ft_strlen(lst->word) - 1, '\\') % 2 == 1))
	{
		set_prompt(PROMPT2, ft_strlen(PROMPT2));
		return (ERR_NEW_PROMPT);
	}
	return (is_new_prompt(prev));
}

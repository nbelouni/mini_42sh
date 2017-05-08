/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_buf_pars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogokar <dogokar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 21:13:34 by dogokar           #+#    #+#             */
/*   Updated: 2017/05/08 23:21:40 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void		reset_int_pt(t_pt *pt)
{
	pt->i = 0;
	pt->len = 0;
	pt->type = 0;
}

int			parse_list(t_token **list, char *s, t_pt *p)
{
	t_token	*new;

	new = NULL;
	if (!s)
		return (ft_print_error("21sh: ", ERR_MALLOC, ERR_EXIT));
	if (find_quote_end(s) == FALSE)
	{
		ft_strdel(&s);
		return (ERR_NEW_PROMPT);
	}
	if (!(new = ft_tokenew(p->type, s)))
		return (ft_print_error("21sh: ", ERR_MALLOC, ERR_EXIT));
	if (!(*list))
		*list = new;
	else
		ft_tokenpush(list, new);
	return (0);
}

int			check_fd_out(t_token **list, char *s, t_pt *p)
{
	int		ret;

	while (s[p->i + p->len] && ft_isalnum(s[p->i + p->len]) == 1)
		p->len++;
	if (p->len == 0 && s[p->i + p->len] == '-')
		p->len++;
	if (p->len != 0)
	{
		p->type = TARGET;
		if ((ret = parse_list(list, ft_strsub(s, p->i, p->len), p)))
			return (ret);
		p->i = p->i + p->len;
		p->len = 0;
	}
	return (0);
}

static int	check_fd_in(t_token **list, char *s, t_pt *p)
{
	int		j;
	int		ret;
	int		ret2;

	(void)list;
	j = 0;
	if (p->i + p->len < (int)ft_strlen(s))
	{
		ret = is_redir(s, p->i + p->len);
		ret2 = is_agreg(s, p->i + p->len);
	}
	else
	{
		ret = NO_TOKEN;
		ret2 = NO_TOKEN;
	}
	if (ret != SR_DIR && ret != SL_DIR && ret2 != DIR_L_AMP &&
		ret2 != DIR_R_AMP && ret != DR_DIR && ret != LR_DIR && ret != DL_DIR)
		return (0);
	while (p->len - j > 0 && ft_isdigit(s[p->i + p->len - j - 1]) == 1)
		j++;
	if ((p->i + p->len - j) != 0 && s[p->i + p->len - j - 1] != ' ')
		return (0);
	return (j);
}

int			cut_cmd(t_token **list, char *s, t_pt *p)
{
	int		j;
	int		ret;

	j = 0;
	ret = 0;
	p->i = cut_space(s, p->i);
	while (p->i + p->len < (int)ft_strlen(s) && s[p->i + p->len] &&
	check_tok(s, p->i + p->len) == NO_TOKEN)
	{
		cut_quote(s, p);
		p->len++;
	}
	if (check_fd_in(list, s, p) > 0)
		p->type = FD_IN;
	else
		p->type = CMD;
	if (p->len != 0)
	{
		if ((ret = parse_list(list, ft_strsub(s, p->i, p->len), p)))
			return (ret);
	}
	p->i = p->i + p->len;
	p->len = 0;
	return (0);
}

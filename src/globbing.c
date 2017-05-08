/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbelouni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 14:15:26 by nbelouni          #+#    #+#             */
/*   Updated: 2017/05/08 23:21:38 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

int		init_new_value(char *var_name, t_core *core, char **new_value)
{
	t_elem	*tmp;

	if (!var_name)
		return (ft_print_error("ft_21sh: ", ERR_MALLOC, ERR_EXIT));
	if (!(tmp = ft_find_elem(var_name, core->set)))
	{
		if (!(tmp = ft_find_elem(var_name, core->env)))
			tmp = ft_find_elem(var_name, core->exp);
	}
	*new_value = (tmp && tmp->value && tmp->value[0]) ? tmp->value : NULL;
	if (var_name)
		ft_strdel(&var_name);
	return (0);
}

int		replace_env_var(char **s, t_core *core)
{
	char	*new_val;
	char	*new_s;
	int		bg;
	int		end;
	int		tmp_len;

	if (init_begin_end(*s, &bg, &end) == TRUE)
		return (TRUE);
	if (init_new_value(ft_strsub(*s, bg, end), core, &new_val) == ERR_EXIT)
		return (ERR_EXIT);
	if (!(new_s = ft_strnew(ft_strlen(*s) - end + ft_strlen(new_val))))
		return (ft_print_error("ft_21sh: ", ERR_MALLOC, ERR_EXIT));
	ft_strncpy(new_s, *s, bg - 1);
	if (new_val)
		ft_strncpy(new_s + bg - 1, new_val, ft_strlen(new_val));
	tmp_len = ft_strlen(*s) - (bg + end);
	ft_strncpy(new_s + bg - 1 + ft_strlen(new_val), *s + bg + end, tmp_len);
	new_s[ft_strlen(*s) - (end) + ft_strlen(new_val)] = 0;
	if (*s)
	{
		ft_strdel(s);
		*s = new_s;
	}
	return (FALSE);
}

int		replace_tild(char **s, t_core *core)
{
	t_elem	*tmp;
	char	*new;

	if (!(tmp = ft_find_elem("HOME", core->env)))
		return (0);
	if (!tmp->value)
		return (0);
	if (!(new = ft_strnew(ft_strlen(*s) - 1 + ft_strlen(tmp->value))))
		return (ft_print_error("21sh: ", ERR_MALLOC, ERR_EXIT));
	ft_strncpy(new, tmp->value, ft_strlen(tmp->value));
	ft_strncpy(new + ft_strlen(tmp->value), *s + 1, ft_strlen(*s) - 1);
	ft_strdel(s);
	*s = new;
	return (0);
}
int		insert_new_args(char **s, t_reg_path **new_args, t_reg_path *tmp)
{
	t_reg_path	*tmp2;

	if (*new_args)
	{
		tmp2 = *new_args;
		while (tmp2->next)
			tmp2 = tmp2->next;
		tmp2->next = tmp;
		tmp->prev = tmp2;
	}
	else
		*new_args = tmp;
	ft_strdel(s);
	if (!(*s = ft_strdup("")))
		return (FALSE);
	tmp2 = tmp;
	while (tmp2)
	{
		supp_quotes(*s);
		tmp2 = tmp2->next;
	}
	return (TRUE);
}

int		globb(char **s, t_core *core, t_reg_path **new_args)
{
	int			i;
	int			ret;
	t_reg_path	*tmp;

	i = 0;
	ret = FALSE;
	if (*s[0] == '~')
	{
		if (replace_tild(s, core) == ERR_EXIT)
			return (ERR_EXIT);
	}
	while (ret == FALSE)
	{
		is_end(*s, &i, '\'');
		if ((ret = replace_env_var(s, core)) == ERR_EXIT)
			return (ERR_EXIT);
		i++;
	}
	if ((tmp = replace_regex(*s)))
	{
		if (insert_new_args(s, new_args, tmp) == FALSE)
			return (FALSE);
	}
	else
		supp_quotes(*s);
	return (TRUE);
}

int		args_len(char **args, t_reg_path *reg_args)
{
	int			len;
	int			i;
	t_reg_path	*tmp;

	i = -1;
	len = 0;
	while (args[++i])
	{
		if (args[i][0])
			len += 1;
	}
	tmp = reg_args;
	while (tmp)
	{
		len += 1;
		tmp = tmp->next;
	}
	return (len);
}

char	**add_in_args(char **args, t_reg_path *reg_args)
{
	t_reg_path	*tmp;
	char		**new;
	int			i;
	int			j;

	if (!reg_args)
		return (args);
	if (!(new = ft_memalloc(sizeof(char *) * (args_len(args, reg_args) + 1))))
		return (NULL);
	i = -1;
	j = -1;
	while (args[++i])
	{
		if (args[i][0])
		{	
			if (!(new[++j] = ft_strdup(args[i])))
				return (NULL);
		}
	}
	tmp = reg_args;
	while (tmp)
	{
		if (tmp->out[0])
		{
			if (!(new[++j] = ft_strdup(tmp->out)))
				return (NULL);
		}
		tmp = tmp->next;
	}
	new[j + 1] = NULL;
	return (new);
}

int		edit_cmd(char ***args, t_core *core)
{
	t_reg_path	*new_args;
	char		**tmp;
	int			i;

	new_args = NULL;
	i = -1;
	while ((*args)[++i])
	{
		if (globb(&(*args)[i], core, &new_args) == ERR_EXIT)
			return (ERR_EXIT);
	}
	if (new_args)
	{
		if (!(tmp = add_in_args(*args, new_args)))
			return (ERR_EXIT);
		*args = tmp;
		ft_reg_pathdestroy(&new_args);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogokar <dogokar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 21:13:34 by dogokar           #+#    #+#             */
/*   Updated: 2017/05/08 23:21:40 by nbelouni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"
#include "list.h"
#include "job.h"
#include "io.h"

/*
**Temporary code which join the redirection operator and arguments,
**--ABSOLUTLY NOT FINAL-- form of redirection. We are using this
**solution just to perform some check.
*/

static t_mode		(g_tab_mode[7]) = {
	[0] = { SL_DIR, O_RDONLY, OPEN | CLOSE},
	[1] = { SR_DIR, O_WRONLY | O_CREAT | O_TRUNC, OPEN | CLOSE},
	[2] = { DL_DIR, 0, CLOSE | WRITE},
	[3] = { DR_DIR, O_WRONLY | O_APPEND | O_CREAT, OPEN | CLOSE},
	[4] = { LR_DIR, O_RDWR | O_CREAT, OPEN | CLOSE},
	[5] = { DIR_L_AMP, O_RDWR, 0},
	[6] = { DIR_R_AMP, O_RDWR | O_TRUNC, 0},
};

static void		set_mode_redir(t_tree *node_redir, t_io *io, int left)
{
	int				i;

	i = 0;
	while (i < 7)
	{
		if (g_tab_mode[i].redir == TOKEN(node_redir))
		{
			io->flag |= g_tab_mode[i].flag;
			io->mode = g_tab_mode[i].mode;
			if ((i == 5 || i == 6) && (node_redir->right->cmd)[0][0] == '-')
			{
				io->flag = CLOSE;
				io->dup_src = left;
				io->dup_target = -2;
			}
			else if (i == 5 || i == 6)
			{
				io->dup_src = ft_atoi((node_redir->right->cmd)[0]);
			}
		}
		i++;
	}
}

int				list_iter_int(t_list *list, int (*f)(void *, int), int dofork)
{
	int		ret;

	while (list)
	{
		if ((ret = f(list->content, dofork)))
			return (ret);
		list = list->next;
	}
	return (0);
}

int				restore_fd(t_io *io, int dofork)
{
	if (!io)
		return (0);
	if (io->tab_fd[0] != -1)
	{
		dup2(io->tab_fd[0], io->dup_target);
		close(io->tab_fd[0]);
	}
	if (!dofork && io->dup_target > 2)
		close(io->dup_target);
	if (io->tab_fd[1] != -1)
	{
		dup2(io->tab_fd[1], io->dup_src);
		close(io->tab_fd[0]);
	}
	return (0);
}

int				save_fd(t_io *io, int type_redir, int dofork)
{
	if (io->dup_target >= 0 && !dofork)
		io->tab_fd[0] = dup(io->dup_target);
	if ((type_redir == DIR_L_AMP || type_redir == DIR_R_AMP) && !dofork)
		io->tab_fd[1] = dup(io->dup_src);
	return (0);
}

t_node_p		create_redir(t_tree *node_redir, t_node_p left_node)
{
	t_io			*io;
	int				left;
	t_process_p		process;

	io = new_io();
	io->str = (node_redir->right->cmd)[0];
	io->flag = DUP;
	if (!node_redir->cmd)
		left = ((TOKEN(node_redir) == SL_DIR || TOKEN(node_redir) == LR_DIR ||
		TOKEN(node_redir) == DIR_L_AMP || TOKEN(node_redir) == DL_DIR) ? 0 : 1);
	else
		left = ft_atoi((node_redir->cmd)[0]);
	io->dup_target = left;
	set_mode_redir(node_redir, io, left);
	process = ((t_list *)left_node->data)->content;
	insert_link_bottom(&(process->io_list), new_link(io, sizeof(*io)));
	return (left_node);
}

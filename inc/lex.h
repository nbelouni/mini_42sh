/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dogokar <dogokar@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/03 21:14:59 by dogokar           #+#    #+#             */
/*   Updated: 2017/05/03 21:14:59 by dogokar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEX_H
# define LEX_H

# include "ft_21sh.h"

# define ISAMP(x) (x->type == DIR_L_AMP || x->type == DIR_R_AMP)
# define NEXTISCMD(x) (x->next->type == CMD || x->next->type == ARG)
# define PREVISCMD(x) (x->prev->type == CMD || x->prev->type == ARG)
# define ISREDIR(x) ((x > START && x < OR) || (x > AND && x < DIR_L_AMP))

typedef struct		s_pt
{
	int				i;
	int				len;
	int				type;
}					t_pt;

typedef struct		s_tok
{
	int				tok;
}					t_tok;

typedef struct		s_tree
{
	int				token;
	char			**cmd;
	t_token			*token_or;
	struct s_tree	*father;
	struct s_tree	*left;
	struct s_tree	*right;
}					t_tree;

typedef struct		s_lib
{
	int				toke;
	int				priority;
}					t_lib;

int					is_char(char *s, int i, char c);
int					is_number(char *s, int len);
int					find_prev_char(char *s, int len, char c);
int					find_next_char(char *s, int len, char c);
int					count_prev_char(char *s, int i, char c);

int					is_separator(char *s, int i);
int					is_redirection(char *s, int i);
int					is_dquote(char *s, int i);
int					is_squote(char *s, int i);
int					is_any_quote(char *s, int i);
int					is_space(char *s, int i);
int					is_dot(char *s, int i);
int					is_redir(char *s, int i);
int					is_agreg(char *s, int i);
int					is_or_and(char *s, int i);
int					is_cmd(char *s, int i);

int					is_separator_type(int type);
int					is_dir_type(int type);
int					is_text_type(int type);

int					find_dquote_end(char *s, int i);
int					find_squote_end(char *s, int i);

int					parse_buf(t_token **l, char *s, t_completion *c, t_lst *h);
int					cut_space(char *s, int i);
void				reset_int_pt(t_pt *pt);
int					parse_list(t_token **list, char *s, t_pt *p);
int					cut_cmd(t_token **list, char *s, t_pt *p);
int					check_tok(char *s, int l);
int					cut_quote(char *s, t_pt *p);
int					check_fd_out(t_token **list, char *s, t_pt *p);

void				ft_tokenadd(t_token **alst, t_token *new);
void				ft_tokenclear(t_token **lst);
void				ft_tokendestroy(t_token **begin);
t_token				*ft_tokenew(int type, char *word);
void				ft_tokenpush(t_token **begin, t_token *new);
int					sort_list_token(t_token **list, t_completion *c, t_lst *h);
int					ft_swap_in(t_token **list);
int					check_error_out(t_token *elem);
void				check_target_place(t_token **list);

int					return_new_prompt(int ret);
int					here_doc(t_token *elem, t_completion *c, t_lst *hist);

t_tree				*creat_left(t_token *lst);
t_tree				*creat_right(t_token *lst);
void				ft_push_ast(t_token *list, t_tree **ast);
void				free_ast(t_tree *ast);
void				print_tab(char **tabol);
void				print_debug_ast(t_tree *node);
void				print_t(t_tree *tree);
t_list				*concate_argv(t_token *lst);

int					priority(t_token *node_lst, t_token *tmp);
char				**copy_fd(t_token *tmp);
char				**concate_cmd(t_token *lst);
t_token				*search_toke(t_token *lst);
t_token				*search_toke_prev(t_token *lst);
t_tree				*new_tree(t_token *lst);
t_tree				*add_tree(t_token *lst);

#endif

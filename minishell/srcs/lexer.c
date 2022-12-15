/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xbasabe- <xbasabe-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 10:47:09 by xbasabe-          #+#    #+#             */
/*   Updated: 2022/12/14 10:47:09 by xbasabe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_cmds(t_stack **node, char *token)
{
	char	**words;
	int		i;
	int		len;

	/*words = ft_split(token, ' ');
	len = ft_str2len(words);
	if (len > 1)
	{
		(*node)->pipe.arg = (char **)malloc(sizeof(char *) * len + 1);
		if (!((*node)->pipe.arg))
			return ;
	}
	*/
	(*node)->pipe.cmd = ft_strdup(words[0]);
	i = 1;
	while (words[i])
	{
		(*node)->pipe.arg[i - 1] = ft_strdup(words[i]);
		i++;
	}
	if (len > 1)
		(*node)->pipe.arg[i - 1] = NULL;
	cmd_path(node);
	clear(words);
}

void	cmd_path(t_stack **node)
{
	char	ruta[999];
	char	comand[999];
	int		i;
	int		c;

	i = ft_strlen((*node)->pipe.cmd);
	c = 0;
	while (--i >= 0)
	{
		if ((*node)->pipe.cmd[i] == '/')
			c++;
	}
	i = 0;
	while (c > 0)
	{
		if ((*node)->pipe.cmd[i] == '/')
			c--;
		ruta[i] = (*node)->pipe.cmd[i];
		i++;
	}
	ruta[i] = '\0';
	(*node)->pipe.ext_path = ruta;
	if (ruta[0] != '\0')
		relative_path(*node);
	c = 0;
	while ((*node)->pipe.cmd[i] != '\0')
		comand[c++] = (*node)->pipe.cmd[i++];
	comand[c] = '\0';
	ft_strcpy((*node)->pipe.cmd, comand);
}

void	relative_path(t_stack *node)
{
	if (node->pipe.ext_path[0] == '.' && node->pipe.ext_path[1] == '/')
		exp_act_path(node);
	if (node->pipe.ext_path[0] == '.' && node->pipe.ext_path[1] == '.'
		&& node->pipe.ext_path[2] == '/')
		exp_up_path(node);
}

void	exp_act_path(t_stack *node)
{
	char	*exp_dir;

	exp_dir = active_dir();
	node->pipe.ext_path++;
	exp_dir = stradd(exp_dir, node->pipe.ext_path);
	node->pipe.ext_path = exp_dir;
}

void	exp_up_path(t_stack *node)
{
	char	*exp_dir;
	char	*tmp;
	int		i;

	exp_dir = active_dir();
	i = ft_strlen(exp_dir);
	while (exp_dir[i] != '/')
		i--;
	tmp = (char *)malloc(sizeof(char) * (i + 2));
	tmp[i + 1] = '\0';
	while (i >= 0)
	{
		tmp[i] = exp_dir[i];
		i--;
	}
	node->pipe.ext_path++;
	node->pipe.ext_path++;
	node->pipe.ext_path++;
	exp_dir = tmp;
	exp_dir = stradd(exp_dir, node->pipe.ext_path);
	node->pipe.cmd = exp_dir;
	free(tmp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xbasabe- <xbasabe-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 10:40:03 by marvin            #+#    #+#             */
/*   Updated: 2022/12/09 19:14:27 by nlibano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
void	delete_quotes(char	**s)
{
	int	i;
	int	j;
//	int	count;

//	count = 0;
//	i = -1;
//	while ((*s)[++i])
//	{
//		if ((*s)[i] == 34)
//			count ++;
//	}
//	if (count % 2 != 0)
//		exit(0);
	i = -1;
	while ((*s)[++i])
	{
		if ((*s)[i] == 34)
		{
			j = i - 1;
			while ((*s)[++j])
			{
				(*s)[j] = (*s)[j + 1];
				if ((*s)[j + 1] == '\0')
					break ;
			}
		}
	}
}
*/
int	echo(t_stack *node, char *input)
{
	char	*output;
	int		i;
	char	*s;

output = input;

	if (node->pipe.arg[0] == NULL)
	{
		fd_putstr_out("\n", node);
		return (0);
	}
	i = -1;
	while (node->pipe.arg[++i])
	{
		if ((str_cmp(node->pipe.arg[0], "-n") == 0 && i > 1) || \
				(str_cmp(node->pipe.arg[0], "-n") != 0 && i > 0))
			fd_putstr_out(" ", node);
		if (str_cmp(node->pipe.arg[i], "-n") == 0)
			continue ;
		s = node->pipe.arg[i];
//		if (s[0] == '"' && s[ft_strlen(s) - 1] == '"')
//		{
			delete_quotes(&s);			
//			fd_putstr_out(s, node);
//		}
//		else if (s[0] == '$')
		if (s[0] == '$')
		{
			if (getenv(&s[1]))
				fd_putstr_out(getenv(&s[1]), node);
		}
		else
			fd_putstr_out(s, node);
	}
	if (str_cmp(node->pipe.arg[0], "-n") != 0)
		fd_putstr_out("\n", node);
	return (0);
}

void	env(t_stack *node)
{
	t_env	*env;

	env = g_shell.env;
	while (env)
	{
		fd_putstr_out(env->name, node);
		fd_putstr_out("=", node);
		fd_putstr_out(env->val, node);
		fd_putstr_out("\n", node);
		env = env->next;
	}
}

void	pwd(t_stack *node)
{
	t_env	*env;

	env = g_shell.env;
	while (env)
	{
		if (str_cmp(env->name, "PWD") == 0)
		{
			fd_putstr_out(env->val, node);
			fd_putstr_out("\n", node);
			break;
		}
		env = env->next;
	}
}

void	unset(char *input)
{
	t_env	*env;
	t_env	*tmp;
	char	**arguments;

	env = g_shell.env;
	arguments = ft_split(input, ' ');
	if (!arguments[1])
		return ;
	tmp = NULL;
	while (env)
	{
		if (str_cmp(env->name, arguments[1]) == 0)
		{
			if (tmp)
			{
				if(env->next)
					tmp->next = NULL;
				else
					tmp->next = env->next;
			}
			ft_lstdelone(env);
			clear(arguments);
			return ;
		}
		tmp = env;
		env = env->next;
	}
	clear(arguments);
	return ;
}

void	exit_kill(t_stack *node) 
{
	delete_all_nodes(node);
	exit(0);
}

int	exec_built_in(char *input, t_stack *node)
{
	if (str_cmp(node->pipe.cmd, "echo") == 0)
		echo(node, input);
	else if (str_cmp(node->pipe.cmd, "pwd") == 0)
		pwd(node);
	else if (str_cmp(node->pipe.cmd, "cd") == 0)
		cd(input, node);
	else if (str_cmp(node->pipe.cmd, "export") == 0)
		export(node);
	else if (str_cmp(node->pipe.cmd, "unset") == 0)
		unset(input);
	else if (str_cmp(node->pipe.cmd, "env") == 0)
		env(node);
	else if (str_cmp(node->pipe.cmd, "exit") == 0)
		exit_kill(node);
	g_shell.num_quit = 0;
	return (0);
}

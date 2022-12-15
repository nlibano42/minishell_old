/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xbasabe- <xbasabe-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 10:40:03 by marvin            #+#    #+#             */
/*   Updated: 2022/12/14 09:48:46 by xbasabe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	cwd[256];

	getcwd(cwd, sizeof(cwd));
	fd_putstr_out(cwd, node);
	fd_putstr_out("\n", node);
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
				if (!env->next)
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

int	exit_kill(t_stack *node)
{
	if (node->pipe.arg)
	{
		if (node->pipe.arg[1])
		{
			printf("-Minishell: exit: too many arguments\n");
			g_shell.num_quit = 1;
			delete_all_nodes(node);
			return (1);
		}
		printf("exit\n");
		g_shell.num_quit = ft_atoi(node->pipe.arg[0]);
	}
	delete_all_nodes(node);
	exit(g_shell.num_quit);
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
		return (exit_kill(node), 1);
	else if (str_cmp(node->pipe.cmd, "$?") == 0) 
	{
		fd_putstr_out(ft_itoa(g_shell.num_quit), node);
		fd_putstr_out("\n", node);
	}
	g_shell.num_quit = 0;
	return (0);
}

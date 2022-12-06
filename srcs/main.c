/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xbasabe- <xbasabe-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 11:33:47 by xbasabe-          #+#    #+#             */
/*   Updated: 2022/12/06 18:09:12 by nlibano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear(char **intro)
{
	int	i;

	i = 0;
	while (intro[i])
	{
		free(intro[i]);
		i++;
	}
	free (intro);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	char	**tokens;
	t_stack	*stack;

	(void)argc;
	(void)argv;
	stack = NULL;
	tokens = NULL;
	sig_handler(1);
	while (1)
	{
		input = readline("MiniShell $> ");
		add_history(input);
		if (!input)
			sig_handler(3);
		if (ft_strlen(input) > 0)
		{
			tokens = ft_split(input, ' ');
			if (str_cmp(tokens[0], "exit") == 0)
			{
				free_all_params(&stack, &input, &tokens);
			//	clear(tokens);
			//	free(input);
				break ;
			}
			stack = pipe_stack(input, env);
			exec_stack(stack);
		}
		free_all_params(&stack, &input, &tokens);
	}
	return (0);
}

void	free_all_params(t_stack **stack, char **input, char ***tokens)
{
	if (*stack)
		deleteAllNodes(*stack);
	if (*input)
		free(*input);
	if (*tokens)
		clear(*tokens);
}


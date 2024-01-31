/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ali <ali@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 18:16:40 by mel-akhd          #+#    #+#             */
/*   Updated: 2024/01/26 05:12:05 by ali              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal;

void	init_data(char **env, t_data *data)
{
	char **export;

	save_fds(data);
	data->cmds_pids = NULL;
	tcgetattr(STDIN_FILENO , &data->term_attr);
	data->env = env;
	export = ft_strsdup(env);
	data->env_list = get_env_list(env);
	data->export_list = get_export_list(export);
	free_strs(export);
	increase_shlvl(data->env_list);
	increase_shlvl(data->export_list);
	add_env(&data->env_list, "?", "0");
	handle_signals();
}


void minishell_cycle(t_data *data)
{
	char	*input;
	t_input	*input_list;
	
	input = readline(PROMPT);
	while (input != NULL)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &data->term_attr);
		data->syntax_error = 0;
		input_list = parser(input, data);
		if(input[0])
			add_history(input);
		free(input);
		if (input_list)
			execution(input_list, data);
		parse_free(input_list);
		input = readline(PROMPT);
	}
	clear_history();
	input_list = NULL;
	free(input);
	free_all(input_list, data);
	printf("\n");
}

int	main(int ac, char **av, char **env)
{
	t_data data;

	(void)av;
	(void)ac;
	init_data(env, &data);
	minishell_cycle(&data);
	return (0);
}

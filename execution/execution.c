#include "../minishell.h"

static void exec_cmd(t_input *input_list, t_data *data)
{
    int status;

    status = redir(input_list->redirect);
    if(status == 0)
    {
        if(is_builtin(input_list))
        {
            status = run_a_builtin(input_list, data);
            free_and_exit(status, data, input_list);
        }
        else
            run_cmd(input_list, data);
    }
    free_and_exit(1, data, input_list);
}

static void run_one_cmd(t_input *input_list, t_data *data)
{
    int status ;
    int pid;

    pid = 0;
    status = 0;
    if(!input_list->args[0])
        return ;
    status = (redir(input_list->redirect));
    if(status == 0)
    {
        if(is_cmd_ehco(input_list))
        {
            pid = fork();
            if(pid == 0)
            {
                status = run_a_builtin(input_list, data);
                free_and_exit(status, data, input_list);
            }
           waitpid(pid, &status, 0);
           set_exit_status(&data->env_list, exit_status(status));
        }
        else if(is_builtin(input_list))
        {
            status = run_a_builtin(input_list, data);
            set_exit_status(&data->env_list, status);
        }
        else
        {
            pid = fork();
            if(pid == 0)
                run_cmd(input_list, data);
            waitpid(pid, &status, 0);
            set_exit_status(&data->env_list, exit_status(status));
        }
    }
    else 
        set_exit_status(&data->env_list, 1);
}

 void run_all_cmds(t_input *input_list, t_data *data)
{
    int pipe_fd[2];
   int piped;
    int index;

   piped = 0;
    index = 0;
    while (input_list)
    {
        ft_pipe(pipe_fd, &piped, input_list);
        data->cmds_pids[index] = fork();
       // add_env(&data->env_list, "_", cmd_path); FIX THIS
        if(data->cmds_pids[index] == 0)
        {
            (close(pipe_fd[0]));
            exec_cmd(input_list, data);
        }
  
        index++; 
		reset_fds(data);
        input_list = input_list->next;
    }
    wait_pids(data, index);
}

int handle_execution(t_input *input_list, t_data *data)
{
    signal(SIGINT, dont_quit);
    if (input_list->next == NULL)
        run_one_cmd(input_list, data);
    else 
        run_all_cmds(input_list, data);
    reset_fds(data);
    handle_signals();
    return 0;
}


void execution(t_input *input_list, t_data *data)
{
    if(allocate_pids(input_list, data) != 0)
        return;
    if(run_herdocs(input_list, data) != 0)
        return;
    handle_execution(input_list, data);
    handle_signals();
}

#include "minishell.h"

void	report_error(char *filename, char *err_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (filename)
		ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
}

void	cd_report_error(char *cmd, char *filename, char *err_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
}

int		set_status_last_command_return(t_shell * shell, int i)
{
	shell->status_last_command = 1;
	return (i);
}

#include "minishell.h"

/*void	report_error2(const char *filename, const char *err_msg)
{
	char	buffer[512];
	int		len;
	char	*program;

	if (filename == NULL || err_msg == NULL)
		return;
	program = "minishell:";
	len = 0;
	while (*program)
		buffer[len++] = *program++;
	buffer[len++] = ' ';
	if(*filename == '\0')
	{
		buffer[len++] = '\'';
		buffer[len++] = '\'';
	}
	while (*filename)
		buffer[len++] = *filename++;
	buffer[len++] = ':';
	buffer[len++] = ' ';
	while (*err_msg)
		buffer[len++] = *err_msg++;
	//buffer[len++] = ' ';
	buffer[len++] = '\n';
	write(2, buffer, len);	
}*/


void report_error(char *filename, char *err_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
}

void cd_report_error(char *cmd, char *filename, char *err_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(filename, STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
}

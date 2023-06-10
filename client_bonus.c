/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudin <lgaudin@student.42malaga.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 15:02:14 by lgaudin           #+#    #+#             */
/*   Updated: 2023/06/10 14:30:55 by lgaudin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief    Sends 8 signals to the provided PID to transmit the
 * provided character bit by bit.
 * It starts from the MSB and progressively goes to the LSB.
 *
 * It sends SIGUSR1 if the bit is 1, SIGUSR2 if it is 0.
 *
 * @param    pid       server's PID
 * @param    character character to transmit
 */
void	send_signal(int pid, unsigned char character)
{
	int				i;
	unsigned char	temp_char;

	i = 8;
	temp_char = character;
	while (i > 0)
	{
		i--;
		temp_char = character >> i;
		if (temp_char % 2 == 0)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		usleep(420);
	}
}

/**
 * @brief    Handles the signals sent back by the server and prints
 * the corresponding information.
 *
 * @param    signal    The signal sent back by the server.
 */
void	handle_read_receipt(int signal)
{
	if (signal == SIGUSR1)
		ft_printf("Received bit 1\n");
	else if (signal == SIGUSR2)
		ft_printf("Received bit 0\n");
}

/**
 * @brief    Sends a message to the server character by character.
 *
 * @param    argc
 * @param    argv
 */
int	main(int argc, char *argv[])
{
	pid_t		server_pid;
	const char	*message;
	int			i;

	signal(SIGUSR1, handle_read_receipt);
	signal(SIGUSR2, handle_read_receipt);
	if (argc != 3)
	{
		ft_printf("Usage: %s <pid> <message>\n", argv[0]);
		exit(0);
	}
	server_pid = ft_atoi(argv[1]);
	message = argv[2];
	i = 0;
	while (message[i])
		send_signal(server_pid, message[i++]);
	send_signal(server_pid, '\0');
	return (0);
}

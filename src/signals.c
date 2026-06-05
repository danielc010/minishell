/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dachin <dachin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2026/05/22 14:32:00 by dachin        #+#    #+#                 */
/*   Updated: 2026/06/05 08:16:49 by dachin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Assigns sig to global signal variable. It only does a single atomic write
// - cannot be interrupted mid-way through and is hence async-signal-safe.
// Printf is not used here as it uses internal buffers and locks that might
// already be held when the signal interrupts the program, causing a deadlock.
static void	handle_signal(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
}

void	signals_waiting(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

// Configure the struct before registering it.
// sa.sa_handler = handle_signal — tells the OS which function to call when
// the signal arrives
// sigemptyset(&sa.sa_mask) — sets the "blocked signals during handler" mask to
// empty — no extra signals blocked while the handler runs
// sa.sa_flags = 0 — no special behaviour flags; use sa_handler
// (not sa_sigaction)
// Then sigaction(SIGINT, &sa, NULL) registers all of this with the OS
// for SIGINT.
// Together: "when SIGINT arrives, call handle_signal, block nothing extra,
// no special flags."
// sigaction defined by the OS in signal.h. Standard way to tell
//  the OS how to handle a signal.
void	signals_interactive(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

// sigaction: int sigaction(int signum, const struct sigaction *act, struct
//  sigaction *oldact);
// Restored default signal behaviour with SIG_DFL so child process can be
// killed by signals like SIGINT and SIGQUIT. The parent overrides default
// signal handling to do things like print a newline on Ctrl + C.
// The  sigaction()  system  call  is  used  to change the action taken by a
// process on receipt of a specific signal.
void	signals_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

#include "../include/minishell.h"

static void	handle_signal(int sig)
{
	g_signal = sig;
}

void	signals_interactive(void)
{
	struct	sigaction sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

// sigaction: int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
// The  sigaction()  system  call  is  used  to change the action taken by a process on receipt of a specific signal.


// TODO:
// For Phase 1 specifically you don't need the full implementation — just get the foundation in place:
// X Declare extern int g_signal in the header
// X Define int g_signal = 0 in main.c
// X Write a basic handler that sets it
// X Register it with sigaction in main
// X Check it in the readline loop

// void    handle_signal(int sig);
// void    signals_interactive(void);
// void    signals_child(void); // PHASE 7

#include "../include/minishell.h"

// Assigns sig to global signal variable. It only does a single atomic write - cannot be
// interrupted mid-way through and is hence async-signal-safe.
static void	handle_signal(int sig)
{
	g_signal = sig;
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Configure the struct before registering it.
// sa.sa_handler = handle_signal — tells the OS which function to call when the signal arrives
// sigemptyset(&sa.sa_mask) — sets the "blocked signals during handler" mask to empty — no extra signals blocked while the handler runs
// sa.sa_flags = 0 — no special behaviour flags; use sa_handler (not sa_sigaction)
// Then sigaction(SIGINT, &sa, NULL) registers all of this with the OS for SIGINT.

// Together: "when SIGINT arrives, call handle_signal, block nothing extra, no special flags."
void	signals_interactive(void)
{
	struct	sigaction sa; // defined by the OS in signal.h. Standard way to tell the OS how to handle a signal.

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

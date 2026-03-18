# minishell

_This project has been created as part of the 42 curriculum by dachin_

## Description

## Instructions
``` bash
# Compile the project
make

# Clean object files
make clean

# Full clean (remove executable)
make fclean

# Recompile everything
make re

# Running the program
## Mandatory - mormal pipe (equivalent to < file1 cmd1 | cmd2 > file2)
./pipex file1 cmd1 cmd2 file2

## Bonus - multiple pipe (equivalent to < file1 cmd1 | cmd2 ... | cmdn > file2)
./pipex file1 cmd1 cmd2 ... cmdn file2

## Bonus - heredoc pipe (equivalent to cmd1 << LIMITER | cmd2 >> file)
./pipex here_doc LIMITER cmd1 cmd2 ... cmdn file

# Useful valgrind check for open fds
valgrind --track-fds=yes

```

## Resources & Credits
Man Pages:
- man 2 pipe
- man 2 fork
- man 2 dup2
- man 2 execve
- man 2 waitpid
- man 3 access

Useful Links:
- Understanding fork: https://www.geeksforgeeks.org/fork-system-call/
- Understanding pipes: https://www.geeksforgeeks.org/pipe-system-call/
- File descriptors: https://www.bottomupcs.com/file_descriptors.xhtml
- Shell command language: https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html

AI was used to help understand the underlying concepts better and in creating tests to double check the correctness of the program.

1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

The implementation ensures that all child processes complete before the shell continues accepting user input by using waitpid(). If I forgot to call waitpid() on all child processes, the shell would accept commands without the child processes completing. This would lead to problems such as leaks.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() because the program would either continue waiting or fail to free data. If I were to leave pipes open, it could go wrong by causing problems such as incorrect behavior and leaks.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd is implemented as a built-in rather than an external command because it has to be able to change the current directory of the shell. If the cd was an external command, it would change the current directory of the child process and not the shell. The challenge that would arise if cd were implemented as an external process is the shell failing to change the current directory.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

I would modify my implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently by using dynamic memory allocation. This would ensure that the command list can be resized for each command that is added. The trade-offs that I would need to consider are potential memory leaks and possible errors.

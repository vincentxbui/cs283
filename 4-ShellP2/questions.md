1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  We use `fork/execvp` instead of just calling `execvp` directly to create a new child process. This ensures that the shell continues to run when the command is executed. With just calling `execvp` directly, the shell would not work.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails, -1 will be returned. This would mean that the system is unable to create a new child process. The implementation handles this scenario by returning an error.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  The execvp() finds the command to execute by searching through the directories in the system environment variable. The PATH system environment variable plays a role in this process by defining where execvp() should search.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of calling wait() in the parent process after forking is to wait for the child process to be done executing before continuing. If wait() is not called, there could be leaks and issues as a result.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  The WEXITSTATUS() provides the exit status of the child process. This is important because it lets the parent process know when the child process has successfully executed or failed.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  The implementation of build_cmd_buff() handles quoted arguments by viewing everything inside quotations as a single argument. This is necessary because it parses commands and passes them as one single argument.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The parsing logic in this assignment parses the input as one single command, while the previous assignment splits the commands with pipes. An unexpected challenge in refactoring the old code was handling quotations since it has to know when a quote starts and ends.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  The purpose of signals in a Linux system is to notify processes of events that occur. They differ since signals are notifications, while other forms of interprocess communication (IPC) deal with data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  The SIGKILL signal terminates a process when used. The typical use case is when a process has to be stopped. The SIGTERM signal waits to terminate so it can do any cleaning beforehand. The typical use case is shutting down a system. The SIGINT signal interrupts a program when CTRL + C is pressed. The typical use case is interrupting a system.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, the process is paused. It cannot be caught or ignored like SIGINT because it is meant to stop a process no matter what.

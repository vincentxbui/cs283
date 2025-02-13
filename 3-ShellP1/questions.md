1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  A good choice for this application was `fgets()` since it can get user input while preventing buffer overflow. Not only does it successfully read user input, but it knows when a line is done being read. This makes `fgets()` a good choice since it is both efficient and useful for this application.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Memory was allocated with `malloc()` for `cmd_buff` in `dsh_cli.c` since the size of user input can vary. If a fixed-size array were to be allocated instead, it would put limitations on the amount of user input. With the use of `malloc()`, the buffer size can be changed if needed.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  In `dshlib.c`, it was necessary for the function `build_cmd_list()` to trim leading and trailing spaces to prevent errors. If there were any invalid spaces, they could be viewed as a command and this would cause problems.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  The first example of redirection that we should implement in our custom shell is input. It has to do with STDIN and is used with the symbol < to get input from a file. A challenge we might have while implementing this is checking if the file we want to read exists. If it does not exist, there should be an error. The second example of redirection that we should implement in our custom shell is output. It has to do with STDOUT and is used with the symbol > to control where output goes. A challenge we might have while implementing this is checking the permissions of a file. The third example of redirection that we should implement in our custom shell is error. It has to do with STDERR and displays errors. A challenge we might have while implementing this is knowing what would be considered an output and what would be considered an error.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  The key difference between redirection and piping is that redirection relates to the input and output of a file, while piping gets the output of a command and connects it to the input of another command. Redirection has to do with files and piping has to do with commands.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep STDERR and STDOUT separate in a shell since they have different purposes. Since STDERR is for error messages and STDOUT is for regular output, it makes it easier to differentiate the two. If an error were to exist, it would be easier to find since STDERR is specifically for errors.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our custom shell should handle errors from commands that fail by only displaying the specific errors that occur. There should not be a way to merge STDOUT and STDERR since output and errors should be their own thing. With the two not merged, it makes them easier to differentiate since STDERR would only display the error and not any extra output. With this, it would make it easier to find and fix errors.

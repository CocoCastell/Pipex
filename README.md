# About Pipex

This project is about handling pipes, child and parent processes, redirections  
and getting a better understanding of how UNIX/Linux handle processus.  

# How to run

First, compile in the directory with one of the following commands
```
make
```
```
make bonus
```
Then, run the programm with one argument, or more for the bonus
```
./pipex echo hey | cat > outfile
```

# Implementation

- Using fork() function to create child processes.
- Using dup2() to handle redirections with the STDIN, STDOUT and other file descriptors.
- For bonus, I use recursion to create a child process for each command and use STDIN
  to get the user input for the heredoc (<<).

# Grade

✅ 125/100  

// compile with "gcc forking_example -o forking_example
// then run "./forking_example" from working directory
// NOTE THAT YOU MAY WANT TO PREPARE YOURSELF TO KILL THIS PROGRAM AFTER A FEW SECONDS (control-C), there is no check written here for termination and ./forking example will run forever


/** This is a short program to illustrate the features of the fork() system call. Used for the creation of a new (child) process, fork is an essential tool for working with process creation/scheduling on Unix operation systems.

This program, forking_example.c, really won't do anything useful for you. A parent process here is spawning two child processes, and each of the three processes will be printing out and incrementing a seperate counter onto the terminal.

An important thing to look at here is the nature of scheduling (CPU runtime) for each process. Parent, child1, and child2 are all competing for CPU and this evident from execution.
**/

#include <stdio.h>
#include <sys/types.h>

void Child1();
void Child2();
void Parent();

int main() {
  pid_t pid;
  pid = fork();
  if (pid == 0) {
    Child1();
  } else {
    pid = fork();
    if (pid == 0){
      Child2();
    } else {
      Parent();
    }
  }
}

void Child1() {
  int a;
  for (a = 1; a>0; a++) {
    printf("Child1 %d\n", a);
  }
}

void Child2() {
  int b;
  for (b = 2; b>0; b++) {
    printf("Child2 %d\n", b);
  }
}

void Parent() {
  int c;
  for (c = 3;c>0;c++) {
    printf("Parent %d\n", c);
  }
}

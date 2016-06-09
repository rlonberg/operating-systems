/* Compile with: "gcc upper.c -o upper -lreadline" */

/*

This example utilizes the fork system call and then sets up two pipes for the child and parent to communicate with eachother (2 are necessary because pipes are unidirectional).

User inputs a string and the program prints that back out, one character at a time, in upper case form

*/









#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER_SIZE 200
#define READ_END 0
#define WRITE_END 1

pid_t Fork(void);
int Pipe(int pipefd[2]);
int Read(int fd, void *buf, size_t count);
int Write(int fd, const void *buf, size_t count);



int main(void){
  //pre while vars
  int status;

  while (1) {
    char * user = readline("Enter: ");
    pid_t pid = Fork();
    
    if (pid==0) {
      //spawn
      if (0==strcmp(user,"Who are you?")) {
	printf("I'm Simon. You created me on this machine, and I've been here since\n");
      } else {
	printf("Access to this path returned %d\n",access(user,F_OK));
	execlp("/bin/ls", "/bin/ls",user,NULL);
      }
      return 0;
    } else {
      //parent
      wait(&status);
      printf("Is there a specific file you were interested in?\n");
      }
  }

  return 0;
}

// wrapper for pipe() syscall
int Pipe(int pipefd[2]) {
  int p = pipe(pipefd);
  if (-1==p) {
    perror("pipe error: ");
    exit(-1);
  } else {
    return p;
  }
}

// wrapper for fork() syscall
pid_t Fork(void) {
  pid_t pd = fork();
  if (-1==pd) {
    perror("fork error: ");
    exit(-1);
  } 
  return pd;
}

// wrapper for write() syscall
int Write(int fd, const void *buf, size_t count) {
  int rt = write(fd,buf,count);
  if (-1==rt) {
    perror("write fail: ");
    exit(-1);
  } else {
    return rt;
  }
}

// wrapper for read() syscall
int Read(int fd, void *buf, size_t count) {
  int r = read(fd, buf, count);
  if (-1==r) {
    perror("read fail: ");
    exit(-1);
  } else {
    return r;
  }
}

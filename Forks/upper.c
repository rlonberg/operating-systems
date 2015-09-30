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

  // readline from console                                                                           
  char *stdinput = readline("please input a string to place through upper: \n");
  int bytes_read = strlen(stdinput);

  //char read_msg[1];
  char read_msg[BUFFER_SIZE];
  int fd[2];
  int fdBack[2];
  pid_t pid;

  Pipe(fd);
  Pipe(fdBack);

  pid = Fork();
  if(pid == 0){
    //child 
    close(fd[WRITE_END]);
    close(fdBack[READ_END]);

    while ((Read(fd[READ_END], read_msg, 1)!=0)) {
      //printf("%s\n", read_msg);
      char u = read_msg[0];
      u = (char) toupper(u);
      Write(fdBack[WRITE_END], &u, 1);
    }
    //close
    char letter = '\0';
    Write(fdBack[WRITE_END], &letter, 1);
    close(fd[READ_END]);
    close(fdBack[WRITE_END]);
  
  } else{
    //parent
    close(fd[READ_END]);
    close(fdBack[WRITE_END]);
    char letter;
    int w;
    for (w=0; w < bytes_read; w++) {
      // write
      letter = stdinput[w];
      Write(fd[WRITE_END], &letter, 1);
    }
    //close
    letter = '\0';
    Write(fd[WRITE_END], &letter, 1);
    close(fd[WRITE_END]);
    while ((Read(fdBack[READ_END], read_msg, 1))!=0) {
      printf("%s\n", read_msg);
    }
    //close
    close(fdBack[READ_END]);
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

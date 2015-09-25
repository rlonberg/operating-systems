// compile with the line: "gcc tokens.c -o tokens"
// and then run with "./tokens" from working directory

/**

This is a quick example illustrating the pipe(2) system call and how it works. The pipe mechanism is one way to handle interprocess communication between a parent process and a child process. One thing to remember here is that pipes are unidirectional. When we work with a pipe, it is necessary to set the file descriptor up as an array of multiple file integers because we need to specify the read end as well as the write end.

This program, tokens.c, initiates a parent process on an infinite loop. That parent process reads standard input from the user, prompting the user to enter a string for "tokenization." The user hands the parent a string, and the parent forks a child process to go ahead and tokenize that string before returning it back to the parent.

For instance, if you enter the line:
"Too    many     spaces   between     words"

You should receive a tokenized version of your input:
"Too many spaces between words"

One bug that I am currently working on here is fixing the output so that the parent doesn't dump additional 'garbage' characters into the stdout after reprompting.

 **/









#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER_SIZE 50
#define READ_END 0
#define WRITE_END 1

pid_t Fork(void);
int Pipe(int pipefd[2]);
int Read(int fd, void *buf, size_t count);
int Write(int fd, const void *buf, size_t count);

int main(void){

  while(1) {
    //    setbuf( stdout, NULL);
  
    int fd[2];
    int fdBack[2];
    pid_t pid;

    // open pipes
    Pipe(fd);
    Pipe(fdBack);

    pid = Fork();
    if(pid == 0){    //CHILD
    
      // establish pipes
      close(fd[WRITE_END]);
      close(fdBack[READ_END]);

      // read from parent (1) int (2) then string, ...then close
      int size_read;
      Read(fd[READ_END], &size_read, sizeof(size_read));

      char parentInput[size_read];
      Read(fd[READ_END], parentInput, size_read);
      close(fd[READ_END]);
    
      // fix input from parent using strtok and strcat
      char parentOutput[BUFFER_SIZE] = "";
      const char s[2] = " ";
      char *token;
      token = strtok(parentInput,s);
      while (token!=NULL) {
	strcat(parentOutput,token);
	strcat(parentOutput,s);
	token = strtok(NULL,s);
      }

      // write back to parent then close
      int new_size = strlen(parentOutput);
      Write(fdBack[WRITE_END], &new_size, sizeof(new_size));
      Write(fdBack[WRITE_END], parentOutput, new_size);
      char letter = '\0';
      Write(fdBack[WRITE_END], &letter, 1);
      close(fdBack[WRITE_END]);
      exit(0);

    } else{ //parent

      // readline from console
      char *stdinput = readline("please input a string to tokenize: \n");
      int bytes_read = strlen(stdinput);

      // establish pipes
      close(fd[READ_END]);
      close(fdBack[WRITE_END]);
    
      // write to child (1) int (2) then string
      Write(fd[WRITE_END], &bytes_read, sizeof(bytes_read));       
      Write(fd[WRITE_END], stdinput, bytes_read);

      // close writeToChild
      char letter = '\0';
      Write(fd[WRITE_END], &letter, 1);
      close(fd[WRITE_END]);

      // read back from child
      int size_read;
      Read(fdBack[READ_END],&size_read, sizeof(size_read));

      char childFix[size_read+1];
      Read(fdBack[READ_END],childFix,size_read); 

      // print fix!
      childFix[size_read] = '\0';
      printf("%s\n",childFix);
      //fflush(stdout);

      // close readFromChild
      close(fdBack[READ_END]);

    
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

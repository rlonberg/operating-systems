/* COMPILE WITH: gcc summation.c -lpthread -o summation */
/* and then run with: "./summation" from working directory */


/**

This program, summation.c, offers a simple example of working with threads in C. Threads are a great tool for concurrent programmers, and many modern applications are multithreaded. One specific advantage that threads have over pipes is the overhead of creation. The overhead associated with thread creation is not nearly as time consuming or costly as that of process creation. Multithreading can simplify code and increase efficiency. OS Kernels today are generally multithreaded.

Summation.c is fairly self-explanatory, it gives you the summation of your input! The program receives an integer number N from the command line as a parameter and then uses a summation function to create TWO threads that compute the summation from 0 to N. This is how it works:

Thread1: computes summation from 0 to (N/2)-1
Thread2: computes summation from (N/2 - 1) to N

When each thread terminates, the main function adds the output of both threads and returns your total summation.


NOTE: my program is not yet finished. It compiles and works to specification, however I am still dealing with compilation warnings / trying to improve this code.

 **/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// structure for thread arguments                                                                                                                                     
struct thread_args {
  int i;
  int j;
  int name;
};


int summation(int i, int j);
void* f(void* v);

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("usage: activity1 [number to calculate summation of]\n");
    exit(-1);
  }

  int input = atoi(argv[1]);
  printf("calculating summation of %d\n", input);

  int sum = summation(1,input);

  //print
  printf("and the summation for %d = %d\n",input,sum);
  


  return (0);
}


int summation(int i, int j) {
  pthread_t first_thread;
  pthread_t second_thread;

  struct thread_args *v = malloc(sizeof(struct thread_args));
  v->i=i;
  v->j=j/2 - 1;
  v->name=1;
  //printf("naming half number %d\n",v->name);
  //printf("from %d to %d\n", v->i, v->j);
  //create                                                                                                                                                            
  pthread_create(&first_thread, NULL, f,(void*)v);
  struct thread_args *v_two = malloc(sizeof(struct thread_args));
  v_two->i = j/2;
  v_two->j = j;
  v_two->name = 2;
  //printf(" and naming half number %d\n", v_two->name);
  //printf(" and then from %d to %d\n", v_two->i, v_two->j);
  pthread_create(&second_thread, NULL, f, (void*)v_two);


  //join first result                                                                                                                                                          
  void* result_one;// = malloc(sizeof(int));
  pthread_join(first_thread, &result_one);
  int a = (int)result_one;
  //join second result
  int *result_two;// = malloc(sizeof(int));
  pthread_join(second_thread, (void**)&result_two);
  int b = (int)result_two;

  //printf("result1=%d\n",a);
  //printf("result2=%d\n",result_two);
  
  return a+b;
  
}

void* f(void* v) {
  struct thread_args *ta = (struct thread_args*) v;

  //printf("invoking f on half number %d\n",ta->name);
  int sum = 0;
  int i = ta->i;
  int j = ta->j;
  //printf("starting calculation from ...%d to %d...\n",i,j);
  //printf("entering for loop in f, sum=%d\n", sum);
  for (i;i<=j;i++) {
    //printf("HALF%d: sum is %d and iterator is %d\n",ta->name,sum,i);
    sum+=i;
  }
  int *retval=&sum;
  //printf("\nand the summation for half number %d, is %d\n",ta->name,*retval);

  //pthread_exit((void*)retval);
  return (void*)sum;

}

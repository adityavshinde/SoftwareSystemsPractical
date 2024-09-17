#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

unsigned long long rdtsc(){
  unsigned long long dst;
  __asm__ __volatile__ ("rdtsc":"=A"(dst));
  return dst;
}

int main(){
  long long int start, end;
  start = rdtsc();
  int i;
  for( i = 0; i < 100; i++ ){
    getppid();
  }
  end = rdtsc();
  printf("Time required (in cycles) to execute 100 getppid() is: %llu\n", end - start );
  return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./4_out
Time required (in cycles) to execute 100 getppid() is: 58443
---------------------------------------------------------------------
*/

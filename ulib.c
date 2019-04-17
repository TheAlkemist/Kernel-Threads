#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

//struct lock_t{
//  int ticknum;
//  int turn;
//}

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

int thread_create(void (*start_routine)(void*, void*), void* arg1, void* arg2)
{
  lock_t l;
  lock_init(&l);
  lock_acquire(&l);
  void *stack = malloc(4096 * 2);
  lock_release(&l);
  stack = stack + (4096 - (uint)stack % 4096);
  int id = clone(start_routine, arg1, arg2, stack);
  return id;
}
int thread_join()
{
 lock_t l;
 void *stack = malloc(sizeof(void*));
 int val;
 val = join(&stack);

 lock_init(&l);
 lock_acquire(&l);
 free(stack);
 lock_release(&l);

 return val;
}

void lock_init(lock_t* lock)
{
  lock->ticknum = 0;
  lock->turn = 0;
}

void lock_acquire(lock_t* lock)
{
  int myturn = fetch_and_add(&lock->ticknum,1); 
  while(lock->turn != myturn){
    
  } 
}

void lock_release(lock_t* lock)
{
  fetch_and_add(&lock->turn,1);
}

int fetch_and_add(int* loc, int inc){
  int val = *loc;
  *loc = val + inc;
  return val;
}

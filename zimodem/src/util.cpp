#include "util.h"


void freeCharArray(char **arr)
{
  if(*arr == NULL)
    return;
  free(*arr);
  *arr = NULL;
}

void setCharArray(char **target, const char *src)
{
  if(src == NULL)
    return;
  if(*target != NULL)
    free(*target);
  *target = (char *)malloc(strlen(src)+1);
  strcpy(*target,src);
}

char lc(char c)
{
  if((c>=65) && (c<=90))
    return c+32;
  if((c>=193) && (c<=218))
    return c-96;
  return c;
}
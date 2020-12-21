#include "input.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#define DEFAULT_BUFFER_SIZE 10

char *read_line()
{
  int size = DEFAULT_BUFFER_SIZE;
  char *str;
  str = (char *)calloc(size, sizeof(char));
  char c;
  c = getchar();
  int i = 0;
  while (c != '\n')
  {
    if (i >= size)
    {
      size = size * 2;
      str = (char *)realloc(str, size);
    }
    str[i] = c;
    c = getchar();
    ++i;
  }
  str[i] = '\0';
  return str;
}

/*
 * char **parse_args(char *line, char* to_split);
 * *line -> Reference to the string containing the entire line.
 * *to_split -> The delimiter to split the line by
 * Returns:
 * **args -> A list of the split strings
*/
char **parse_args(char *line, const char *to_split)
{
  //
  /* Count the number of resulting strings after the split */
  char *p = line;
  int count = count_characters(line,to_split)+1;
  char **args = calloc(count + 1, sizeof(char *));
  args[count] = NULL;
  for (int i = 0; p; i++)
  {
    args[i] = trim(strsep(&p, to_split));
  }
  return args;
}

int exec(char **args)
{
  int c, status;

  c = fork();
  if (!c)
  {
    int rc = execvp(args[0], args);
    if (rc == -1)
    {
      printf("Unknown command\n");
      exit(0);
    }
  }
  else
  {
    wait(&status);
  }
  return 0;

}

void redir(char **args, char *file)
{
  int fd1;
  // int out_backup;
  //int f, status;

  //out_backup = dup(STDOUT_FILENO); //dupes stdout
  fd1 = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);

  dup2(fd1, STDOUT_FILENO); //redirects fd1 to stdout
  close(fd1);

  char *arg = args[0];
  execlp(arg, arg, NULL);

  //char *temp[2];
  //strcpy(temp[0], args[0]);
  //exec(temp);
  /*
  f = fork();
  if (!f)
  {
    execlp(arg, arg, NULL);
  }
  else
  {
    wait(&status);
    printf("should be in terminal\n");
  }
  */
  //return 0;
  
}
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include "ls_helper.h"

#define MAX_SIZE 256

typedef int (*SortFunc)(const void *, const void *);

int main(int argc, char **argv)
{
  bool output_long = false;
  bool reverse_sort = false;
  bool show_size = false;
  bool show_hidden = false;
  
  char *dir_path[MAX_SIZE];
  int path_count = 0;

  SortFunc sort_function = sort_by_name;
  
  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
    {
      dir_path[path_count++] = argv[i];
    }
    else
    {
      switch (argv[i][1])
      {
      case 'l':
        output_long = true;
        break;
      case 't':
        sort_function = sort_by_time;
        break;
      case 'S':
        sort_function = sort_by_size;
        break;
      case 's':
        show_size = true;
        break;
      case 'r':
        reverse_sort = true;
        break;
      case 'a':
        show_hidden = true;
        break;
      default:
        fprintf(stderr, "Error: Unknown Flag '%s'\nAllowed Flags 'l', 't', 'S', 's', 'r', 'a'\n", argv[i]);
        exit(1);
      }
    }
  }

  if (reverse_sort)
  {
    if (sort_function == sort_by_time)
      sort_function = rev_sort_by_time;
    else if (sort_function == sort_by_size)
      sort_function = rev_sort_by_size;
    else
      sort_function = rev_sort_by_name;
  }

  if (path_count == 0)
    dir_path[path_count++] = ".";

  for (int i = 0; i < path_count; i++)
  {
    char *path = dir_path[i];
    struct stat stats;
    int err = stat(path, &stats);
    if (err)
    {
      printf("Error from stat('%s'): %d\n", path, errno);
      continue;
    }
    
    if (S_ISDIR(stats.st_mode))
    {
      DIR *dp;
      struct dirent *dir;
      dp = opendir(path);
      
      Directory dirs[MAX_SIZE];
      int dir_count = 0;
      size_t block_count = 0;
      
      while ((dir = readdir(dp)) != NULL)
      {
        if (show_hidden || dir->d_name[0] != '.')
        {
          char buf[PATH_MAX + 1];

          // Append path
          sprintf(buf, "%s/%s", path, dir->d_name);

          // Save stats
          stat(buf, &dirs[dir_count].stats);

          // Save directory entry
          dirs[dir_count].entry = *dir;

          // increment block count
          block_count += dirs[dir_count++].stats.st_blocks;
        }
      }
      
      closedir(dp);
      
      if (path_count)
        printf("Path: %s:\n", path);
      
      if (show_size || output_long)
        printf("Total: %ld\n", block_count);

      qsort(dirs, dir_count, sizeof *dirs, sort_function);
      print_details(dirs, output_long, dir_count, show_size);
    }
  }
}

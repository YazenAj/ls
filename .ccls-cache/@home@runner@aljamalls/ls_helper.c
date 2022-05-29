#include <stdio.h>>
#include <string.h>
#include "ls_helper.h"
#include <grp.h>
#include <pwd.h>
#include <time.h>

int sort_by_name(const void *d1, const void *d2)
{
    return strcasecmp(((Directory *)d1)->entry.d_name, ((Directory *)d2)->entry.d_name);  
}

int sort_by_size(const void *d1, const void *d2)
{
  return ((Directory *)d2)->stats.st_size - ((Directory *)d1)->stats.st_size;  
}

int sort_by_time(const void *d1, const void *d2)
{
    return ((Directory *)d2)->stats.st_ctime - ((Directory *)d1)->stats.st_ctime;  
}

int rev_sort_by_name(const void *d1, const void *d2)
{
    return sort_by_name(d2, d1);
}

int rev_sort_by_size(const void *d1, const void *d2)
{
  return sort_by_size(d2, d1);  
}

int rev_sort_by_time(const void *d1, const void *d2)
{
  return sort_by_time(d2, d1);
}

void print_details(Directory *dirs, bool output_long, size_t size, bool show_size)
{
  for (int i = 0; i < size; i++)
  {
    Directory dir = dirs[i];
    
    if (show_size)
      printf("%ld ", dir.stats.st_blocks);
    
    if (output_long)
    {
      // Print Permissions
      printf((S_ISDIR(dir.stats.st_mode))  ? "d" : "-");
      printf((dir.stats.st_mode & S_IRUSR) ? "r" : "-");
      printf((dir.stats.st_mode & S_IWUSR) ? "w" : "-");
      printf((dir.stats.st_mode & S_IXUSR) ? "x" : "-");
      printf((dir.stats.st_mode & S_IRGRP) ? "r" : "-");
      printf((dir.stats.st_mode & S_IWGRP) ? "w" : "-");
      printf((dir.stats.st_mode & S_IXGRP) ? "x" : "-");
      printf((dir.stats.st_mode & S_IROTH) ? "r" : "-");
      printf((dir.stats.st_mode & S_IWOTH) ? "w" : "-");
      printf((dir.stats.st_mode & S_IXOTH) ? "x" : "-");

      // Get user and group
      struct passwd *pws;
      struct group *grp;
      pws = getpwuid(dir.stats.st_uid);
      grp = getgrgid(dir.stats.st_gid);

      printf(" %ld %s %s %ld %s %s",
        dir.stats.st_nlink,
        pws->pw_name,
        grp->gr_name,
        dir.stats.st_size,
        ctime(&dir.stats.st_mtime),       
        dir.entry.d_name);
    }
    else
    {
      printf("%s  ", dir.entry.d_name);
    }
  } 
  printf("\n");
}
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

typedef struct directory
{
  struct dirent entry;
  struct stat stats;
} Directory;

int sort_by_name(const void *d1, const void *d2);
int sort_by_size(const void *d1, const void *d2);
int sort_by_time(const void *d1, const void *d2);
int rev_sort_by_name(const void *d1, const void *d2);
int rev_sort_by_size(const void *d1, const void *d2);
int rev_sort_by_time(const void *d1, const void *d2);
void print_details(Directory *dirs, bool output_long, size_t size, bool show_size);

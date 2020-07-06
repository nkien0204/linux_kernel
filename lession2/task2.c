#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

int main () {
	DIR * dir = opendir ("/home/ubuntu/cracking_linux_kernel/lession2");
	struct stat buffer;
	int status;
	if (dir == NULL) {
		printf ("Open directory failed\n");
		return -1;
	}
	
	struct dirent * entry;
	while ((entry = readdir (dir)) != NULL) {
		status = stat (entry->d_name, &buffer);
		printf ("%s   %ld   %s\n", entry->d_name, buffer.st_size, ctime(&buffer.st_mtime));
	}
	closedir(dir);
	return 0;
}

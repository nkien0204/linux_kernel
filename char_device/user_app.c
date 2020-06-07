#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/fakedevice"

int main() {
  int i, file_description;
  char choice, write_buff[100], read_buff[100];
  file_description = open(DEVICE, O_RDWR);

  if (file_description == -1) {
    printf ("file %s does not exist or has been locked by another process\n", DEVICE);
    exit(-1);
  }
  printf ("r = read from device\nw = write to device\nEnter command: ");
  scanf ("%c", &choice);

  switch (choice) {
    case 'r':
      read(file_description, read_buff, sizeof(read_buff));
      printf("device: %s\n", read_buff);
      break;
    case 'w':
      printf("Enter data: ");
      scanf(" %[^\n]", write_buff);
      write(file_description, write_buff, sizeof(write_buff));
      break;
    default:
      printf("command not correct");
      break;
  }
  return 0;
}
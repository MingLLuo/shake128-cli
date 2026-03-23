#include "lib/shake128.h"

int absorb(uint64_t* state) {
  unsigned char* state_bytes = (unsigned char*)state;
  unsigned char block[SHAKE128_RATE];
  ssize_t bytes_read;

  while ((bytes_read = read(STDIN_FILENO, block, SHAKE128_RATE)) > 0) {
    for (ssize_t i = 0; i < bytes_read; i++) {
      state_bytes[i] ^= block[i];
    }
    if (bytes_read < SHAKE128_RATE) {
      break;
    }
    Keccak_StatePermute(state);
  }

  if (bytes_read < 0) {
    perror("read");
    return -1;
  }
  padding(state_bytes, (size_t)bytes_read);
  Keccak_StatePermute(state);
  return 0;
}

void squeeze(uint64_t* state, int output_len) {
  unsigned char* state_bytes = (unsigned char*)state;
  int remaining = output_len;

  while (remaining > 0) {
    int chunk = (remaining < SHAKE128_RATE) ? remaining : SHAKE128_RATE;

    for (int i = 0; i < chunk; i++) {
      printf("%02x", state_bytes[i]);
    }

    remaining -= chunk;

    if (remaining > 0) {
      Keccak_StatePermute(state);
    }
  }
  printf("\n");
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <output_bytes>\n", argv[0]);
    return 1;
  }
  int output_len = atoi(argv[1]);
  if (output_len <= 0) {
    fprintf(stderr, "Error: output length must be positive\n");
    return 1;
  }

  uint64_t state[25] = {0};

  if (absorb(state) < 0) {
    return 1;
  }
  squeeze(state, output_len);
  return 0;
}
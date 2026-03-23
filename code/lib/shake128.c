#include "shake128.h"
#define ROL64(x, n) (((x) << (n)) | ((x) >> (64 - (n))))
#define INDEX(x, y) ((y) * 5 + (x))

void theta(uint64_t* A) {
  uint64_t C[5] = {0}, D[5];
  for (int x = 0; x < 5; x++)
    for (int y = 0; y < 5; y++) C[x] ^= A[INDEX(x, y)];

  for (int x = 0; x < 5; x++) D[x] = C[(x + 4) % 5] ^ ROL64(C[(x + 1) % 5], 1);

  for (int x = 0; x < 5; x++)
    for (int y = 0; y < 5; y++) A[INDEX(x, y)] ^= D[x];
}

void rho(uint64_t* A) {
  int x = 1, y = 0;
  for (int t = 0; t < 24; t++) {
    int offset = ((t + 1) * (t + 2) / 2) % 64;
    A[INDEX(x, y)] = ROL64(A[INDEX(x, y)], offset);
    int tmp = x;
    x = y;
    y = (2 * tmp + 3 * y) % 5;
  }
}

void pi(uint64_t* A) {
  uint64_t tmp[5 * 5];
  memcpy(tmp, A, 25 * sizeof(uint64_t));
  for (int x = 0; x < 5; x++)
    for (int y = 0; y < 5; y++) A[INDEX(x, y)] = tmp[INDEX((3 * y + x) % 5, x)];
}

void chi(uint64_t* A) {
  uint64_t C[5];
  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 5; x++) C[x] = A[INDEX(x, y)];
    for (int x = 0; x < 5; x++) {
      A[INDEX(x, y)] = C[x] ^ ((~(C[(x + 1) % 5])) & C[(x + 2) % 5]);
    }
  }
}

// Table from test vectors
static const uint64_t RC[24] = {
    0x0000000000000001ULL, 0x0000000000008082ULL, 0x800000000000808aULL,
    0x8000000080008000ULL, 0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL, 0x000000000000008aULL,
    0x0000000000000088ULL, 0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL, 0x8000000000008089ULL,
    0x8000000000008003ULL, 0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL, 0x8000000080008081ULL,
    0x8000000000008080ULL, 0x0000000080000001ULL, 0x8000000080008008ULL};

void iota(uint64_t* A, int round) { A[0] ^= RC[round]; }

void Keccak_StatePermute(uint64_t* state) {
  for (int round = 0; round < 24; round++) {
    theta(state);
    rho(state);
    pi(state);
    chi(state);
    iota(state, round);
  }
}

void Print_State(uint64_t* state) {
  unsigned char* state_bytes = (unsigned char*)state;
  for (int i = 0; i < 200; i++) {
    printf("%02x", state_bytes[i]);
  }
  printf("\n");
}

void padding(uint8_t* message, int message_len) {
  if (message_len < SHAKE128_RATE) {
    message[message_len] ^= 0x1F;
    message[SHAKE128_RATE - 1] ^= 0x80;
  }
}
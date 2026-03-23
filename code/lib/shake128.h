#ifndef SHAKE128_H
#define SHAKE128_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void theta(uint64_t* A);
void rho(uint64_t* A);
void pi(uint64_t* A);
void chi(uint64_t* A);
void iota(uint64_t* A, int round);

void Keccak_StatePermute(uint64_t* state);

void Print_State(uint64_t* state);
void padding(uint8_t* message, int message_len);
#define SHAKE128_RATE 168
#endif
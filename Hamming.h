/*
 * Hamming.h
 *
 *  Created on: Nov 16, 2016
 *      Author: tommy
 */

#ifndef HAMMING_H_
#define HAMMING_H_
#include <stdbool.h>
#include <time.h>

#define DATA_SIZE 4
#define BLOCK_SIZE 7
typedef enum {
	NO_ERROR, SINGLE_ERROR, DOUBLE_ERROR, PARITY_ERROR
} RESULT;

// k is indexed from Lowest to Highest, n-1..0
void set(int k, int* data);
void setTo(int k, int* data, int bit);
// k is indexed from Lowest to Highest, n-1..0
void unset(int k, int* data);
bool isSet(int k, int data);
void toggle(int k, int* data);
void hammingEncode(int dataSize, int blockSize, int* data);
int hammingDecode(int dataSize, int blockSize, int* data);
void hammingEncodeExtra(int dataSize, int blockSize, int* data);
RESULT hammingDecodeExtra(int dataSize, int blockSize, int* data);

//convert L to R index (1 to n) to L to H index (n-1..0)
int toLH(int i, int n);

//convert L to H index to L to R
int toLR(int i, int n);

bool isPowerOfTwo(int x);
char * int2bin(int i, int length);
void randomToggle(int blockSize, int* data);
void randomToggleArray(char* data, int length, int blockSize);
void split(char* source, char* dest, int sourceLength);
void join(char* source, char* dest, int sourceLength);
void encode(char* data, int length, int dataSize, int blockSize);
void decode(char* data, int length, int dataSize, int blockSize);
void printArray(char* data, int dataLength, int bitLength);
#endif /* HAMMING_H_ */

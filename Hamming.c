/*
 ============================================================================
 Name        : Hamming.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 Ton trong quyen tac gia:Luan
 ============================================================================
 */

#include "Hamming.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void set(int k, int* data) {
	*data |= 1 << k;
}
void unset(int k, int *data) {
	*data &= ~(1 << k);
}
bool isSet(int k, int data) {
	return data & 1 << k;
}
void setTo(int k, int* data, int bit) {
	if (bit == 0) {
		unset(k, data);
	} else {
		set(k, data);
	}
}
void toggle(int k, int* data) {
	if (isSet(k, *data)) {
		unset(k, data);
	} else {
		set(k, data);
	}
}

int hammingParity(int blockSize, int* data, int parityIndex) {
	int parity = 0;
	for (int i = parityIndex; i <= blockSize; i += 2 * parityIndex) {
		for (int k = i; k <= parityIndex + i - 1 && k <= blockSize; k++) {
			parity ^= isSet(toLH(k, blockSize), *data);

		}

	}
	return parity;
}
int overallParity(int blockSize, int *data) {
	int parity = 0;
	for (int i = 1; i <= blockSize; i++)
		parity ^= isSet(toLH(i, blockSize), *data);
	return parity;
}
void hammingEncode(int dataSize, int blockSize, int* data) {
	int newdata = 0;
	int dataIndex = 1;
	for (int i = 1; i <= blockSize; i++) {
		if (isPowerOfTwo(i)) {

		} else {
			if (isSet(toLH(dataIndex, dataSize), *data)) {
				set(toLH(i, blockSize), &newdata);
			} else {
				unset(toLH(i, blockSize), &newdata);
			}
			dataIndex++;
		}
	}
	for (int i = 1; i <= blockSize; i++) {
		if (isPowerOfTwo(i)) {
			setTo(toLH(i, blockSize), &newdata,
					hammingParity(blockSize, &newdata, i));
		}
	}
	*data = newdata;
}
void hammingEncodeExtra(int dataSize, int blockSize, int* data) {
	hammingEncode(dataSize, blockSize, data);
	int parity = overallParity(blockSize, data);
	*data = *data << 1 | parity;

}

int hammingDecode(int dataSize, int blockSize, int* data) {
	int C = 0;
	int index = 0;
	int newdata = 0;
	for (int i = 1; i <= blockSize; i++) {
		if (isPowerOfTwo(i)) {
			int parity = hammingParity(blockSize, data, i);
			C = C | parity << index++;
		} else {

		}
	}
	if (C != 0 && C <= blockSize) {
		toggle(toLH(C, blockSize), data);
	}

	for (int i = 1; i <= blockSize; i++) {
		if (isPowerOfTwo(i)) {
		} else {
			newdata <<= 1;
			newdata |= isSet(toLH(i, blockSize), *data);
		}
	}
	*data = newdata;
	return C;
}
RESULT hammingDecodeExtra(int dataSize, int blockSize, int* data) {
	int P = overallParity(blockSize + 1, data);
	*data >>= 1;

	int C = hammingDecode(dataSize, blockSize, data);
	if (C == 0 && P == 0) {
		return NO_ERROR;
	} else if (C != 0 && P == 1) {
		return SINGLE_ERROR;
	} else if (C != 0 && P == 0) {
		return DOUBLE_ERROR;
	} else if (C == 0 && P == 1) {
		return PARITY_ERROR;
	}
	return 0;
}

int toLH(int i, int n) {
	return n - i;
}

int toLR(int k, int n) {
	return n - k;
}

bool isPowerOfTwo(int x) {
	if (x == 0)
		return false;
	else {
		while (x % 2 == 0)
			x /= 2;
		return (x == 1);
	}
}

char * int2bin(int i, int length) {
	size_t bits = length;

	char * str = malloc(bits + 1);
	if (!str)
		return NULL;
	str[bits] = 0;

// type punning because signed shift is implementation-defined

	unsigned u = *(unsigned *) &i;

	for (; bits--; u >>= 1) //u = u tai vi tri bit ben phai cua u; (Tuong tu u+=1)
				//Bit chay tu dau mang den cuoi mang thi con tro cung di theo

		str[bits] = u & 1 ? '1' : '0'; //Neu dia chi cua con tro tai vi tri u 
	//nay la 1 thi se tra ve la 1 neu khong se tra ve 0; Day la toan tu AND

	return str;
}

void randomToggle(int blockSize, int* data) {
	for (int i = 0; i < 2; i++)
		if (rand() % 2) {
			toggle(rand() % blockSize, data);
		}

}

void randomToggleArray(char* data, int length, int blockSize) {
	for (int i = 0; i < length; i++) {
		int aData = (int) data[i];
		randomToggle(blockSize, &aData);
		data[i] = (char) aData;
	}
}

void split(char* source, char* dest, int sourceLength) {
	for (int i = 0; i < sourceLength * 2; i += 2) {
		dest[i] = (0xf0 & source[i / 2]) >> 4;
		dest[i + 1] = (0x0f & source[i / 2]);
	}
}

void join(char* source, char* dest, int sourceLength) {
	for (int i = 0; i < sourceLength / 2; i++) {
		dest[i] = source[i * 2] << 4 | source[i * 2 + 1];
	}
}

void printArray(char* data, int dataLength, int bitLength) {
	for (int i = 0; i < dataLength; i++) {
		char* binary = int2bin(data[i], bitLength);
		printf("%s ", binary);
		free(binary);
	}
	printf("\n");
}

void encode(char* data, int length, int dataSize, int blockSize) {
	printf("Encoding data	: ");
	printArray(data, length, dataSize);
	for (int i = 0; i < length; i++) {
		int aData = data[i];
		hammingEncodeExtra(dataSize, blockSize, &aData);
		printf("\t%s -> %s\n", int2bin(data[i], dataSize),
				int2bin(aData, blockSize));
		data[i] = (char) aData;
	}
	printf("Encoded data	: ");
	printArray(data, length, blockSize);
}

void decode(char* data, int length, int dataSize, int blockSize) {
	printf("Decoding data	: ");
	printArray(data, length, blockSize);

	for (int i = 0; i < length; i++) {
		int aData = data[i];
		RESULT result = hammingDecodeExtra(dataSize, blockSize, &aData);
		printf("\t%s -> %s", int2bin(data[i], blockSize),
				int2bin(aData, dataSize));
		switch (result) {
		case NO_ERROR:
			printf(" - No error");
			break;
		case SINGLE_ERROR:
			printf(" - Single error, corrected");
			break;
		case DOUBLE_ERROR:
			printf(" - Double error, not corrected");
			break;
		case PARITY_ERROR:
			printf(" - Parity bit error");
			break;
		}
		printf("\n");
		data[i] = (char) aData;

	}
	printf("Decoded data	: ");
	printArray(data, length, dataSize);

}

int main(void) {
	srand(time(NULL));
	char str[2] = "HW";
	char data[4];
	split(str, data, 2);
	//printArray(data, 4, 4);
	encode(data, 4, DATA_SIZE, BLOCK_SIZE);
	randomToggleArray(data, 4, 8);
	decode(data, 4, DATA_SIZE, BLOCK_SIZE);

	join(data, str, 4);
	puts(str);
	puts(int2bin(4, 4));
	return EXIT_SUCCESS;
}

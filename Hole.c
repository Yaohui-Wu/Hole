// Usage (encryption): Hole plaintext.file ciphertext.file password
// Usage (decryption): Hole ciphertext.file plaintext.file password
// Compiled on MacOS, Linux and *BSD in x86_64 platform.
// Talk is SO EASY, show you my GOD.
// Simple is beautiful.

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
// the secret keys table
    unsigned char aucKeysTable[256] = {
        0x01, 0x2F, 0x45, 0x4B, 0x25, 0x85, 0xb2, 0x2D, 0x9A, 0xbB, 0x5C, 0x8B, 0x5E, 0x0C, 0x80, 0xd5,
        0xb7, 0x12, 0xd7, 0xcF, 0xfC, 0xa3, 0x48, 0x17, 0x42, 0xb0, 0x84, 0x16, 0x1D, 0x96, 0x2B, 0x0D,
        0x76, 0xb6, 0x1C, 0xc5, 0x88, 0xbD, 0xa5, 0x6E, 0x9D, 0x9C, 0x29, 0xaC, 0x15, 0x03, 0x6F, 0x7C,
        0xd4, 0x54, 0x05, 0x5B, 0x3F, 0x65, 0xcB, 0x91, 0xe2, 0x67, 0xd0, 0x00, 0x41, 0xe9, 0xf1, 0x8E,
        0x66, 0xc6, 0xb4, 0xb9, 0x93, 0x08, 0x6A, 0x09, 0xa7, 0x43, 0xd8, 0x58, 0xe4, 0x73, 0x35, 0xbC,
        0x86, 0x07, 0xdB, 0x4F, 0x78, 0xc0, 0xf8, 0x18, 0x7F, 0x11, 0xb3, 0xc8, 0x1F, 0x3C, 0xf7, 0xa9,
        0x20, 0x63, 0xaE, 0x52, 0x49, 0xc2, 0x5F, 0x46, 0xeB, 0x74, 0x51, 0xbA, 0x89, 0xc1, 0xf4, 0x3B,
        0x62, 0x60, 0xf6, 0x0B, 0x26, 0xc7, 0x57, 0x37, 0xf3, 0x64, 0x0A, 0x8A, 0x77, 0x22, 0x87, 0x92,
        0x19, 0xfA, 0xb1, 0x32, 0xd2, 0xc9, 0xeD, 0xbF, 0xfF, 0xe0, 0x9E, 0xfD, 0xc4, 0x70, 0x2A, 0xcA,
        0xe1, 0x06, 0x5D, 0xb5, 0xd9, 0xa2, 0x34, 0xdC, 0x5A, 0xc3, 0x30, 0x6B, 0xf0, 0x3D, 0xdD, 0x4D,
        0x55, 0xe3, 0x90, 0x10, 0xdF, 0x7B, 0xdA, 0xfE, 0x33, 0x79, 0xf5, 0x7D, 0x99, 0xcE, 0xd6, 0x2C,
        0x98, 0x95, 0x71, 0xeF, 0xaD, 0x75, 0xa0, 0x27, 0x8D, 0x3E, 0xcD, 0x3A, 0x44, 0x36, 0x0E, 0x81,
        0xaF, 0x94, 0x23, 0x8C, 0xa6, 0x83, 0x7E, 0xbE, 0xaA, 0xeA, 0x82, 0xeE, 0x1B, 0x7A, 0x4A, 0x56,
        0x40, 0x39, 0x53, 0x1A, 0x6C, 0x0F, 0xe5, 0xf2, 0x02, 0xe6, 0x6D, 0x47, 0xfB, 0x13, 0x28, 0xf9,
        0x24, 0x14, 0xeC, 0x50, 0x8F, 0xcC, 0xe8, 0x59, 0xb8, 0x9B, 0x1E, 0xa8, 0xa1, 0xd3, 0x2E, 0xd1,
        0xa4, 0x69, 0xe7, 0x21, 0xdE, 0x38, 0xaB, 0x97, 0x68, 0x4C, 0x4E, 0x04, 0x9F, 0x61, 0x31, 0x72};

// any password length
    unsigned long ulPasswordLength = -1;

// get password length
    while(argv[3][++ulPasswordLength]);

    struct stat statFileSize;

    stat(argv[1], &statFileSize);

// get plaintext / ciphertext file size
    unsigned long ulFileSize = statFileSize.st_size;

// allocate storage space
    unsigned char *pucPlaintextOrCiphertext = (unsigned char*)malloc(ulFileSize);

// open plaintext / ciphertext file
    int iPlaintextOrCiphertext = open(argv[1], O_RDONLY, S_IREAD | S_IWRITE);

// read data from plaintext / ciphertext file
    read(iPlaintextOrCiphertext, pucPlaintextOrCiphertext, ulFileSize);

    close(iPlaintextOrCiphertext);

// initialize random number seed
    srand(ulFileSize % RAND_MAX);

// process plaintext / ciphertext data
    for(unsigned long i = 0; i < ulFileSize; i += 8)
    {
// keys table convert 8 * 32 = 256 bytes of data at a time in order to generate the random number of "JunTai" distribution
        for(unsigned long j = 0; j < 32; ++j)
        {
            unsigned long ulKeyIndex, ulKeyTemp, *pulKeySwap1 = (unsigned long*)aucKeysTable, *pulKeySwap2 = (unsigned long*)aucKeysTable;

            ulKeyIndex = (unsigned char)argv[3][j % ulPasswordLength] % 32;

            ulKeyTemp = pulKeySwap1[j];

            pulKeySwap1[j] = pulKeySwap2[ulKeyIndex];

            pulKeySwap2[ulKeyIndex] = ulKeyTemp;
        }

// use XOR to process the 8 bytes of plaintext / ciphertext data at a time
        *(unsigned long*)(pucPlaintextOrCiphertext + i) ^= *(unsigned long*)(aucKeysTable + rand() % 249);

// use keys table's value to change the password
        for(unsigned long k = 0; k < ulPasswordLength; ++k)
        {
            argv[3][k] = aucKeysTable[(unsigned char)argv[3][k]];
        }
    }

// open ciphertext / plaintext file
    iPlaintextOrCiphertext = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

// write data to ciphertext / plaintext file
    write(iPlaintextOrCiphertext, pucPlaintextOrCiphertext, ulFileSize);

    close(iPlaintextOrCiphertext);

    free(pucPlaintextOrCiphertext);

    return 0;
}

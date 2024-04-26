#include <stdio.h>
#include <string.h>

// Function to extract bytes from hex string based on offsets
void extractBytes(const char* hexInput, int start, int length, char* result) {
    strncpy_s(result, length * 2 + 1, hexInput + (start * 2), length * 2); // Corrected line
    result[length * 2] = '\0'; // Null-terminate the string
}

void reversetxid(char* txid) {
    // reverse txid then print txid(reversed) 
    //after that reverse each byte one by one i.e First byte is 6a51 then order should be 516a 
    int length = strlen(txid);
    for (int i = 0; i < length / 2; ++i) {
        char temp = txid[i];
        txid[i] = txid[length - i - 1];
        txid[length - i - 1] = temp;
    }
    for (int i = 0; i < length; i += 2) {
        char temp = txid[i];
        txid[i] = txid[i + 1];
        txid[i + 1] = temp;
    }
}

void printLittleEndian(const char* hexInput, int start, int length) {
    char littleEndian[9]; // Assuming 4 bytes + null terminator
    // Extract the bytes in little-endian format
    extractBytes(hexInput, start, length, littleEndian);
    // Reverse the bytes to get little-endian order
    reversetxid(littleEndian);
    // Convert little-endian hex string to integer
    unsigned int value = 0;
    sscanf_s(littleEndian, "%x", &value);
    printf("Previous output index: %u\n", value);
}

int main() {
    const char* hexInput = "020000000001056a51edb2fdd26ff90ef8a086fd1ec0c63210d0edfa67397f0dbeeeb621364e330100000000";

    // Define offsets for different parts
    int firstFourBytesOffset = 0;
    int flagOffset = 4; // Offset in bytes
    int flagLength = 3; // Length in bytes
    int txidOffset = 7; // Offset in bytes
    int txidLength = 32; // Length in bytes

    // Buffers to store extracted parts
    char version[9]; // 4 bytes + null terminator
    char flag[7]; // 3 bytes + null terminator
    char txid[65]; // 32 bytes + null terminator

    extractBytes(hexInput, firstFourBytesOffset, 4, version);

    unsigned int firstFourBytesInt;

    extractBytes(hexInput, flagOffset, flagLength, flag);

    extractBytes(hexInput, txidOffset, txidLength, txid);

    // Outputting the results
    printf("First 4 bytes: %s\n", version);
    printf("Flag: %s\n", flag);

    // Print the txid
    printf("txid (reversed): %s\n", txid);

    // Reverse each byte in the txid
    reversetxid(txid);

    // Outputting the reversed txid
    printf("txid (reversed each byte): %s\n", txid);

    // Print the next 4 bytes in little-endian format as the previous output index
    printLittleEndian(hexInput, txidOffset + txidLength, 4);

    return 0;
}
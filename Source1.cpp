#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 64

// Define structs tx, tx_in, and tx_out here (or include from a header)

// Function prototypes
tx* deserialize_tx(uint8_t* buffer, size_t* tx_size);
void free_tx(tx* transaction);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <hex-encoded-transaction>\n", argv[0]);
        return 1;
    }

    // ... (code to decode hex-encoded transaction into a buffer)

    size_t tx_size;
    tx* transaction = deserialize_tx(buffer, &tx_size);

    if (transaction == NULL) {
        printf("Error deserializing transaction\n");
        return 1;
    }

    // ... (code to print or process transaction data)

    free_tx(transaction);
    return 0;
}

tx* deserialize_tx(uint8_t* buffer, size_t* tx_size) {
    tx* transaction = malloc(sizeof(tx));
    if (transaction == NULL) {
        return NULL;
    }

    // Read version
    memcpy(&transaction->version, buffer, sizeof(uint32_t));
    buffer += sizeof(uint32_t);

    // Read number of inputs
    memcpy(&transaction->vin_sz, buffer, sizeof(uint32_t));
    buffer += sizeof(uint32_t);

    // Read number of outputs
    memcpy(&transaction->vout_sz, buffer, sizeof(uint32_t));
    buffer += sizeof(uint32_t);

    // Allocate memory for inputs and outputs
    transaction->vin = malloc(sizeof(tx_in) * transaction->vin_sz);
    if (transaction->vin == NULL) {
        free(transaction);
        return NULL;
    }
    transaction->vout = malloc(sizeof(tx_out) * transaction->vout_sz);
    if (transaction->vout == NULL) {
        free(transaction->vin);
        free(transaction);
        return NULL;
    }

    // Deserialize each input
    for (int i = 0; i < transaction->vin_sz; i++) {
        if (!deserialize_tx_in(buffer, &transaction->vin[i])) {
            free_tx(transaction);
            return NULL;
        }
        buffer += get_tx_in_size(&(transaction->vin[i]));
    }

    // Deserialize each output
    for (int i = 0; i < transaction->vout_sz; i++) {
        if (!deserialize_tx_out(buffer, &transaction->vout[i])) {
            free_tx(transaction->vin);  // Fix: Free previously allocated memory
            free_tx(transaction);
            return NULL;
        }
        buffer += get_tx_out_size(&(transaction->vout[i]));
    }

    // ... (code to deserialize other transaction data fields)

    *tx_size = buffer - (uint8_t*)transaction;
    return transaction;
}

// Implement similar functions for deserialize_tx_in, deserialize_tx_out,

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_HEX_LENGTH 65
#define MAX_OUTPUT_LENGTH 17
#define MAX_INPUTS 255
#define MAX_OUTPUTS 255

int scriptsigsize;
int numInputs;
int outscriptsigsize;

struct VersionFlag {
    char version[9];
    char flag[5];
    char txinputs[3];
};

struct TxIn {
    char txid[MAX_HEX_LENGTH];
    char reversetxid[MAX_HEX_LENGTH];
    char previousOutputIndex[10];
    char sequence[9];
    char scriptSigLength[3];
    char scriptsig[MAX_HEX_LENGTH];

    int scriptsigflag;
};

struct TxOut {
    char txout[2];
    char output_value[17];
    char outscriptSigLength[3];
    char scriptsig[MAX_HEX_LENGTH];
};
struct Txwitness {
    char witnesscomponent[3];
    char witnessdata[100];
};

void extractBytes(const char* hexInput, int start, int length, char* result) {
    memcpy(result, hexInput + (start * 2), length * 2);
    result[length * 2] = '\0';
}

char* reverseHexString(char* hexString) {
    int length = strlen(hexString);
    for (int i = 0; i < length / 2; ++i) {
        char temp = hexString[i];
        hexString[i] = hexString[length - i - 1];
        hexString[length - i - 1] = temp;
    }
    for (int i = 0; i < length; i += 2) {
        char temp = hexString[i];
        hexString[i] = hexString[i + 1];
        hexString[i + 1] = temp;
    }
    return hexString;
}

int hexToInt(const char* hexString) {
    int result = 0;
    for (int i = 0; hexString[i] != '\0'; ++i) {
        int digit;
        if (hexString[i] >= '0' && hexString[i] <= '9') {
            digit = hexString[i] - '0';
        }
        else if (hexString[i] >= 'a' && hexString[i] <= 'f') {
            digit = hexString[i] - 'a' + 10;
        }
        else if (hexString[i] >= 'A' && hexString[i] <= 'F') {
            digit = hexString[i] - 'A' + 10;
        }
        else {
            printf("Invalid hex character: %c\n", hexString[i]);
            return -1;
        }
        result = result * 16 + digit;
    }
    return result;
}

void printLittleEndian(const char* hexInput) {
    unsigned int value = 0;
    int hexLength = strlen(hexInput) / 2;
    for (int i = hexLength - 1; i >= 0; i--) {
        if (hexInput[i * 2] >= '0' && hexInput[i * 2] <= '9') {
            value = (value << 4) + (hexInput[i * 2] - '0');
        }
        else {
            value = (value << 4) + (hexInput[i * 2] - 'a' + 10);
        }
        if (hexInput[i * 2 + 1] >= '0' && hexInput[i * 2 + 1] <= '9') {
            value = (value << 4) + (hexInput[i * 2 + 1] - '0');
        }
        else {
            value = (value << 4) + (hexInput[i * 2 + 1] - 'a' + 10);
        }
    }
    printf(" %d\n", value);
}

int main() {
    const char* hexInput = "020000000001056a51edb2fdd26ff90ef8a086fd1ec0c63210d0edfa67397f0dbeeeb621364e330100000000feffffff2577513282bb52facf186cf6f8893d69aae1cdb2b0eb0317fc8794f4819af2b5010000006a47304402200607b736df8fe395d861a0754c77eb71ead51eb4a5078d93f83861e163519798022059a8c9d34af3b9632b119c68dc007dcdac3001eb358925a1db68eeac370d3d4b01210233541665c6963ea1b867f8b0076b241e87f7d48f3afa0566f4f22b753d36da32fefffffffd6b5eafd8070281dd36c4981000aac89bab0a54af7a1ca052ff9e895f7443d30100000000feffffff0c8e8cbad2a4feab6875454f1ece3fd33afb333d1624350f2636098cc3ccf3680100000000feffffffc64d43e3bb402aeba7cd58ba084140d87f8c8062ec1c5e668d0695533cd776d4000000006a473044022070dea539e1f01589cb87926ef8646540e453a5c90b80907680fefcbd513767d4022035ebc67a4360d62b9d1bdb2d89f210c78d7bc4316eacad1a4fd89427fe001ece0121024e61652d99350a41b7395a12263bdb1e350aca7f6e884733d8ef5260e833ef49feffffff0226d6fd000000000017a914978e3e09c10b72077c2102d1853883a5a4f0b61b87b74b150000000000160014f7533a613e95c63be15a519b8327e7f33ac126a60247304402206ecad418dbf2ab3dc6903ee44e3077e058df9968241fdf931093fbaf9a51e11b02201494d62531e6773220072856fc9726bc1e5c93767ea98da4b75d51411187cb280121039350003ee10d39a0811696f7deccf4c31c924ce55c2506191e819833f6c6baa30002473044022021962ffa5db42745d5a9a851fc508fab603ca690d17ae962a3aa9a6f0ff9d9ed02204b377666ab296cef8ff5abe9bb35eabd714f8845dbac9f352bcf0af89a8c220a012102a3338fc7a90f8e3802d61f1ea73d77edd257ba97848e3d3a97d790eb3105845c0247304402200de7b1f320d0498f2053e7c46bad62bf5b58b87f5b1f96d64ffa007afa8c0ba6022056669ccfaf4772934cd264755edb94228387802ca038e238977ca5a4dea6d4260121026491015716a9c02db53743dcfa33bd01a45fb287e092f09e5d33756f4c9b77040086c70c00";

    const int firstFourBytesOffset = 0;
    const int flagOffset = 4;
    const int flagLength = 2;

    struct VersionFlag vf;
    struct TxIn txInputs[MAX_INPUTS];
    struct TxIn txin;
    struct Txwitness txwitness;
    extractBytes(hexInput, firstFourBytesOffset, 4, vf.version);
    extractBytes(hexInput, flagOffset, flagLength, vf.flag);
    extractBytes(hexInput, flagOffset + flagLength, 1, vf.txinputs);
    printf("First 4 bytes: %s\n", vf.version);
    printf("Flag: %s\n", vf.flag);
    printf("======vin=======\n");

    numInputs = hexToInt(vf.txinputs);
    printf("Tx Inputs: %x\n", numInputs);
    int offset = flagOffset + flagLength + 1;
    for (int i = 0; i < numInputs && i < MAX_INPUTS; ++i) {
        printf("TxIn %d:\n", i + 1);

        // Create a new TxIn object for each iteration


        extractBytes(hexInput, offset, 32, txin.txid);
        extractBytes(hexInput, offset + 32, 4, txin.previousOutputIndex);

        int scriptSigLengthOffset = offset + 32 + 4;
        extractBytes(hexInput, scriptSigLengthOffset, 1, txin.scriptSigLength);

        if (txin.scriptSigLength[0] == '0') {
            extractBytes(hexInput, offset + 32 + 4 + 1, 4, txin.sequence);
            reverseHexString(txin.sequence);
            offset += 32 + 5 + 4;
        }
        else {
            printf("ScriptSig present\n");
            txin.scriptsigflag = 1;
            scriptsigsize = hexToInt(txin.scriptSigLength);
            extractBytes(hexInput, offset + 32 + 4 + 1, scriptsigsize, txin.scriptsig);
            extractBytes(hexInput, offset + 32 + 4 + 1 + scriptsigsize, 4, txin.sequence);
            reverseHexString(txin.sequence);
            offset = offset + 32 + 4 + 1 + scriptsigsize + 4;
        }

        // Store the current TxIn object in the array
        txInputs[i] = txin;

        // Print information
        printf("  txid : %s\n", txin.txid);
        reverseHexString(txin.txid);
        printf("  txid (reversed each byte): %s\n", txin.txid);
        printf("  Previous output index: ");
        printLittleEndian(txin.previousOutputIndex);
        if (scriptsigsize != 0) {
            printf("  Scriptsig Length : %x\n", scriptsigsize);
            printf("  ScriptSig(hex); %s\n", txin.scriptsig);
        }
        printf("  Sequence: %s\n", txin.sequence);
    }

    struct TxOut txOutputs[MAX_OUTPUTS];
    int numOutputs = 2;  
    int outscriptsigsize;

    struct TxOut txOut;

    extractBytes(hexInput, offset, 1, txOut.txout);
    printf("  Number of outputs: %s\n", txOut.txout);

    for (int i = 0; i < numOutputs && i < MAX_OUTPUTS; ++i) {
        printf("Txout %d:\n", i + 1);



        extractBytes(hexInput, offset + 1, 8, txOut.output_value);
        extractBytes(hexInput, offset + 1 + 8, 1, txOut.outscriptSigLength);
        outscriptsigsize = hexToInt(txOut.outscriptSigLength);
        printf("  Scriptsiglength: %s\n", txOut.outscriptSigLength);
        extractBytes(hexInput, offset + 1 + 8 + 1, outscriptsigsize, txOut.scriptsig);
        printf("  Scriptsig: %s\n", txOut.scriptsig);
        reverseHexString(txOut.output_value);
        printf("  Output Value: %s\n", txOut.output_value);

        txOutputs[i] = txOut;

        offset += 9 + outscriptsigsize;
    }
    // for (int i = 0; i < numInputs; i++) {
    //     txin = txInputs[i];
    //     if (txin.scriptsig == '0') {
    //         extractBytes(hexInput, offset, 1, txwitness.witnesscomponent);
    //         printf("Witnesscomponent: %s\n", txwitness.witnesscomponent);
    //         int witnesssize = hexToInt(txwitness.witnesscomponent);
    //         extractBytes(hexInput, offset, witnesssize, txwitness.witnessdata);
    //         printf("Witnessdata: %s\n", txwitness.witnessdata);
    //         offset += witnesssize;

    //     }
    // }
    return 0;
}

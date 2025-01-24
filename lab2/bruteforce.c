#include <stdio.h>
#include <stdlib.h>

void expandKey(int masterKey[], int roundKey1[], int roundKey2[]) {
    int permutedKey[10] = {masterKey[2], masterKey[4], masterKey[1], masterKey[6], masterKey[3], masterKey[9], masterKey[0], masterKey[8], masterKey[7], masterKey[5]};
//5 bit shift  
    int shiftedKey[10] = {permutedKey[1], permutedKey[2], permutedKey[3], permutedKey[4], permutedKey[0], permutedKey[6], permutedKey[7], permutedKey[8], permutedKey[9], permutedKey[5]};

    roundKey1[0] = shiftedKey[5];
    roundKey1[1] = shiftedKey[2];
    roundKey1[2] = shiftedKey[6];
    roundKey1[3] = shiftedKey[3];
    roundKey1[4] = shiftedKey[7];
    roundKey1[5] = shiftedKey[4];
    roundKey1[6] = shiftedKey[9];
    roundKey1[7] = shiftedKey[8];

    int furtherShiftedKey[10] = {permutedKey[2], permutedKey[3], permutedKey[4], permutedKey[0], permutedKey[1], permutedKey[7], permutedKey[8], permutedKey[9], permutedKey[5], permutedKey[6]};

    roundKey2[0] = furtherShiftedKey[5];
    roundKey2[1] = furtherShiftedKey[2];
    roundKey2[2] = furtherShiftedKey[6];
    roundKey2[3] = furtherShiftedKey[3];
    roundKey2[4] = furtherShiftedKey[7];
    roundKey2[5] = furtherShiftedKey[4];
    roundKey2[6] = furtherShiftedKey[9];
    roundKey2[7] = furtherShiftedKey[8];//again shifting
}

void inverseInitialPermutation(int perm[], int inversePerm[]) {
    for (int i = 0; i < 8; i++) {
        inversePerm[i] = 0;
    }
    for (int i = 0; i < 8; i++) {
        inversePerm[perm[i]] = i;
    }
}
//4 to 8
void expansionPermutation(int rightHalf[], int expandedRight[]) {
    expandedRight[0] = rightHalf[3];
    expandedRight[1] = rightHalf[0];
    expandedRight[2] = rightHalf[1];
    expandedRight[3] = rightHalf[2];
    expandedRight[4] = rightHalf[1];
    expandedRight[5] = rightHalf[2];
    expandedRight[6] = rightHalf[3];
    expandedRight[7] = rightHalf[0];
}

void roundFunction(int data[], int subkey[], int sbox0[4][4], int sbox1[4][4], int result[]) {
    int leftPart[4] = {data[0], data[1], data[2], data[3]};
    int rightPart[4] = {data[4], data[5], data[6], data[7]};
    //4 to 8
    int expandedRight[8];
    expansionPermutation(rightPart, expandedRight);

    int xorResult[8];
    for (int i = 0; i < 8; i++) {
        xorResult[i] = expandedRight[i] ^ subkey[i];
    }
//after xor split into 4
//.left shiftmsb
    int row1 = (xorResult[0] << 1) + xorResult[3];
    int col1 = (xorResult[1] << 1) + xorResult[2];
    int sbox0Value = sbox0[row1][col1];

    int row2 = (xorResult[4] << 1) + xorResult[7];
    int col2 = (xorResult[5] << 1) + xorResult[6];
    int sbox1Value = sbox1[row2][col2];

    int permutedP4[4] = {(sbox0Value & 2) >> 1, sbox0Value & 1, (sbox1Value & 2) >> 1, sbox1Value & 1};
//xor again
    for (int i = 0; i < 4; i++) {
        result[i] = leftPart[i] ^ permutedP4[i];
    }

    for (int i = 0; i < 4; i++) {
        result[i + 4] = rightPart[i];
    }
}

void decryptCipher(int cipher[], int roundKey1[], int roundKey2[], int sbox0[4][4], int sbox1[4][4], int perm[], int inversePerm[], int decryptedData[]) {
    int afterInitialPermutation[8];
    for (int i = 0; i < 8; i++) {
        afterInitialPermutation[i] = cipher[perm[i]];
    }

    int round1Output[8];
    roundFunction(afterInitialPermutation, roundKey2, sbox0, sbox1, round1Output);

    int swappedHalves[8] = {round1Output[4], round1Output[5], round1Output[6], round1Output[7], round1Output[0], round1Output[1], round1Output[2], round1Output[3]};
    int round2Output[8];
    roundFunction(swappedHalves, roundKey1, sbox0, sbox1, round2Output);

    for (int i = 0; i < 8; i++) {
        decryptedData[i] = round2Output[inversePerm[i]];
    }
}

int main() {
    int plainText[8] = {1, 0, 0, 1, 0, 1, 1, 1};
    int masterKey[10] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 1};
    int initialPerm[8] = {1, 5, 2, 0, 3, 7, 4, 6};
    int inversePerm[8];
    inverseInitialPermutation(initialPerm, inversePerm);

    int roundKey1[8];
    int roundKey2[8];
    expandKey(masterKey, roundKey1, roundKey2);

    int sbox0[4][4] = {{1, 0, 3, 2}, {3, 2, 1, 0}, {0, 2, 1, 3}, {3, 1, 3, 2}};
    int sbox1[4][4] = {{0, 1, 2, 3}, {2, 0, 1, 3}, {3, 0, 1, 0}, {2, 1, 0, 3}};
    //encrypt
    int afterInitialPerm[8];
    for (int i = 0; i < 8; i++) {
        afterInitialPerm[i] = plainText[initialPerm[i]];
    }

    int intermediateResult1[8];
    roundFunction(afterInitialPerm, roundKey1, sbox0, sbox1, intermediateResult1);

    int swappedIntermediate[8] = {intermediateResult1[4], intermediateResult1[5], intermediateResult1[6], intermediateResult1[7], intermediateResult1[0], intermediateResult1[1], intermediateResult1[2], intermediateResult1[3]};
    int finalRoundResult[8];
    roundFunction(swappedIntermediate, roundKey2, sbox0, sbox1, finalRoundResult);

    int cipherText[8];
    for (int i = 0; i < 8; i++) {
        cipherText[i] = finalRoundResult[inversePerm[i]];
    }

    printf("Plaintext: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", plainText[i]);
    }
    printf("\n");

    printf("Ciphertext: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", cipherText[i]);
    }
    printf("\n");

    // Brute Force Attack
    printf("Brute Force Decryption:\n");
    int decryptedText[8];
    for (int keyCombination = 0; keyCombination < 1024; keyCombination++) {
        int tempMasterKey[10];
        int tempKey = keyCombination;
        for (int i = 0; i < 10; i++) {
            tempMasterKey[9 - i] = tempKey % 2;
            tempKey /= 2;
        }

        int tempKey1[8], tempKey2[8];
        expandKey(tempMasterKey, tempKey1, tempKey2);

        decryptCipher(cipherText, tempKey1, tempKey2, sbox0, sbox1, initialPerm, inversePerm, decryptedText);

        int isMatch = 1;
        for (int i = 0; i < 8; i++) {
            if (decryptedText[i] != plainText[i]) {
                isMatch = 0;
                break;
            }
        }

        if (isMatch) {
            printf("Found matching key: %d\n", keyCombination);
            printf("Decrypted Text: ");
            for (int i = 0; i < 8; i++) {
                printf("%d ", decryptedText[i]);
            }
            printf("\n");
            return 0;
        }
    }

    printf("No matching key found after brute force.\n");
    return 0;
}

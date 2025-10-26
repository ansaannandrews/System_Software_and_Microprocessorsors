#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *intermediate, *optab, *symtab, *prgmlength, *objectcode, *output;

    // Open all required files
    optab = fopen("optab.txt", "r");
    intermediate = fopen("intermediate.txt", "r");
    symtab = fopen("symtab.txt", "r");
    prgmlength = fopen("prgmlength.txt", "r");
    objectcode = fopen("objectcode.txt", "w");
    output = fopen("output.txt", "w");

    if (!optab || !intermediate || !symtab || !prgmlength || !objectcode || !output) {
        fprintf(stderr, "Error: Unable to open one or more input files.\n");
        exit(EXIT_FAILURE);
    }

    char label[20], opcode[20], operand[20];
    char optab_mnemonic[20], optab_code[10];
    char sym_label[20], sym_address[20];
    char objectCode[60], textRecord[1000];

    int locctr, startAddr = 0, pgmlength = 0;
    int textStartAddr = 0, textLength = 0;

    // ---- READ FIRST LINE ----
    if (fscanf(intermediate, "%s %s %s", label, opcode, operand) != 3) {
        fprintf(stderr, "Error reading START record from intermediate file.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(output, "  %-10s %-10s %-10s\n", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        // START operand (usually hex address)
        startAddr = (int)strtol(operand, NULL, 16);

        // Read program length
        if (fscanf(prgmlength, "%X", &pgmlength) != 1) {
            fprintf(stderr, "Error: Invalid program length file.\n");
            exit(EXIT_FAILURE);
        }

        // Header record
        fprintf(objectcode, "H^%-6s^%06X^%06X\n", label, startAddr, pgmlength);

        // Initialize text start address for first record
        textStartAddr = startAddr;
    }

    // ---- MAIN LOOP ----
    while (fscanf(intermediate, "%X %s %s %s", &locctr, label, opcode, operand) == 4) {

        if (strcmp(opcode, "END") == 0)
            break;

        strcpy(objectCode, "");
        fprintf(output, "%04X\t%-10s %-10s %-10s", locctr, label, opcode, operand);

        // ---- BYTE ----
        if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C' && operand[1] == '\'') {
                for (int i = 2; i < strlen(operand) - 1; i++) {
                    sprintf(objectCode + strlen(objectCode), "%02X", operand[i]);
                }
            } else if (operand[0] == 'X' && operand[1] == '\'') {
                strncpy(objectCode, operand + 2, strlen(operand) - 3);
                objectCode[strlen(operand) - 3] = '\0';
            }
        }

        // ---- WORD ----
        else if (strcmp(opcode, "WORD") == 0) {
            sprintf(objectCode, "%06X", atoi(operand));
        }

        // ---- RESW / RESB ----
        else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0) {
            // End the current text record before skipping
            if (textLength > 0) {
                fprintf(objectcode, "T^%06X^%02X^%s\n", textStartAddr, textLength / 2, textRecord);
                textLength = 0;
            }
            fprintf(output, "\n");
            continue;
        }

        // ---- OPCODE FROM OPTAB ----
        else {
            int foundOp = 0;
            rewind(optab);
            while (fscanf(optab, "%s %s", optab_mnemonic, optab_code) != EOF) {
                if (strcmp(opcode, optab_mnemonic) == 0) {
                    strcpy(objectCode, optab_code);
                    foundOp = 1;
                    break;
                }
            }

            int foundSym = 0;
            rewind(symtab);
            while (fscanf(symtab, "%s %s", sym_label, sym_address) != EOF) {
                if (strcmp(operand, sym_label) == 0) {
                    sprintf(objectCode + strlen(objectCode), "%04X", (int)strtol(sym_address, NULL, 16));
                    foundSym = 1;
                    break;
                }
            }

            if (foundOp && !foundSym) {
                strcat(objectCode, "0000"); // Operand not found
            }
        }

        // ---- PRINT OBJECT CODE ----
        if (strlen(objectCode) > 0) {
            fprintf(output, "\t%s", objectCode);

            // Build text record
            if ((textLength + strlen(objectCode)) > 60 || textLength == 0) {
                // Write previous text record if full
                if (textLength > 0)
                    fprintf(objectcode, "T^%06X^%02X^%s\n", textStartAddr, textLength / 2, textRecord);

                textStartAddr = locctr;
                strcpy(textRecord, objectCode);
                textLength = strlen(objectCode);
            } else {
                strcat(textRecord, "^");
                strcat(textRecord, objectCode);
                textLength += strlen(objectCode);
            }
        }

        fprintf(output, "\n");
    }

    // ---- FINAL TEXT RECORD ----
    if (textLength > 0)
        fprintf(objectcode, "T^%06X^%02X^%s\n", textStartAddr, textLength / 2, textRecord);

    // ---- END RECORD ----
    fprintf(objectcode, "E^%06X\n", startAddr);

    fclose(optab);
    fclose(intermediate);
    fclose(symtab);
    fclose(prgmlength);
    fclose(objectcode);
    fclose(output);

    printf("PASS 2 completed successfully. Object and Output files generated.\n");
    return 0;
}


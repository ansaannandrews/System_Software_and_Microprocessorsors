#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE 100
#define MAX_TOKEN 20

int main() {
    FILE *source, *optab, *intermediate, *symtab, *length;
    char label[MAX_TOKEN], opcode[MAX_TOKEN], operand[MAX_TOKEN];
    char symtab_data[MAX_TOKEN], optab_data[MAX_TOKEN];
    int locctr, starting_address, pgm_length;
    bool symbol_found, opcode_found;

    // Open files with error checking
    source = fopen("sourcecode.txt", "r");
    optab = fopen("opcodetable.txt", "r");
    intermediate = fopen("intermediatefile.txt", "w");
    symtab = fopen("symboltable.txt", "w+");
    length = fopen("length.txt", "w");

    if (source == NULL || optab == NULL || intermediate == NULL || 
        symtab == NULL || length == NULL) {
        printf("Error: Unable to open one or more files\n");
        return 1;
    }

    // Read first line
    if (fscanf(source, "%s %s %s", label, opcode, operand) != 3) {
        printf("Error: Invalid source file format\n");
        return 1;
    }

    // Handle START directive
    if (strcmp(opcode, "START") == 0) {
        if (sscanf(operand, "%x", &starting_address) != 1) {
            printf("Error: Invalid starting address\n");
            return 1;
        }
        locctr = starting_address;
        fprintf(intermediate, "\t\t%s\t%s\t%s\n", label, opcode, operand);
    } else {
        locctr = 0;
    }

    // Main processing loop
    while (fscanf(source, "%s %s %s", label, opcode, operand) == 3 && 
           strcmp(opcode, "END") != 0) {
        // Handle labels
        if (strcmp(label, "**") != 0) {  // Check for non-empty label
            symbol_found = false;
            rewind(symtab);
            while (fscanf(symtab, "%s", symtab_data) == 1) {
                if (strcmp(symtab_data, label) == 0) {
                    printf("Error: Duplicate label %s\n", label);
                    symbol_found = true;
                    break;
                }
                fscanf(symtab, "%*[^\n]"); // Skip rest of line
            }
            if (!symbol_found) {
                fprintf(symtab, "%s\t%d\n", label, locctr);
            }
        }

        // Write location counter to intermediate file
        fprintf(intermediate, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        // Check opcode
        opcode_found = false;
        rewind(optab);
        while (fscanf(optab, "%s", optab_data) == 1) {
            if (strcmp(optab_data, opcode) == 0) {
                opcode_found = true;
                locctr += 3; // Standard instruction length
                break;
            }
        }

        // Handle directives
        if (!opcode_found) {
            if (strcmp(opcode, "WORD") == 0) {
                locctr += 3;
            } else if (strcmp(opcode, "RESW") == 0) {
                int words;
                if (sscanf(operand, "%d", &words) != 1) {
                    printf("Error: Invalid operand for RESW\n");
                    return 1;
                }
                locctr += 3 * words;
            } else if (strcmp(opcode, "RESB") == 0) {
                int bytes;
                if (sscanf(operand, "%d", &bytes) != 1) {
                    printf("Error: Invalid operand for RESB\n");
                    return 1;
                }
                locctr += bytes;
            } else if (strcmp(opcode, "BYTE") == 0) {
                if (operand[0] == 'C') {
                    locctr += strlen(operand) - 3; // Character constant
                } else if (operand[0] == 'X') {
                    locctr += (strlen(operand) - 3) / 2; // Hex constant
                } else {
                    printf("Error: Invalid BYTE constant\n");
                    return 1;
                }
            } else {
                printf("Error: Invalid opcode %s\n", opcode);
                return 1;
            }
        }
    }

    // Write END record
    fprintf(intermediate, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    // Calculate and save program length
    pgm_length = locctr - starting_address;
    printf("Program length: %d\n", pgm_length);
    fprintf(length, "%d", pgm_length);

    // Clean up
    fclose(sourcecode);
    fclose(opcodetable);
    fclose(intermediatefile);
    fclose(symboltable);
    fclose(length);

    return 0;
}

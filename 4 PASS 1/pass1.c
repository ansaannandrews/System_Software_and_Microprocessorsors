#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    FILE *source, *optab, *intermediate, *symtab, *prgmlength;
    source = fopen("source.txt", "r");
    optab = fopen("optab.txt", "r");
    intermediate = fopen("intermediate.txt", "w");
    symtab = fopen("symtab.txt", "w+");
    prgmlength = fopen("prgmlength.txt", "w");

    if (!source || !optab || !intermediate || !symtab || !prgmlength) {
        printf("Error: one or more files could not be opened.\n");
        return 1;
    }

    char label[20], opcode[20], operand[20];
    char symtab_data[20], optab_data[20];
    int locctr = 0, starting_address = 0, pgm_length = 0;
    bool symbol_found, opcode_found;

    // Read first line
    if (fscanf(source, "%s%s%s", label, opcode, operand) != 3) {
        printf("Error: invalid or empty source file.\n");
        return 1;
    }

    // START directive
    if (strcmp(opcode, "START") == 0) {
        starting_address = (int)strtol(operand, NULL, 16);  // allow hex start
        locctr = starting_address;
        fprintf(intermediate, "    %s\t%s\t%s\n", label, opcode, operand);
        fscanf(source, "%s%s%s", label, opcode, operand);
    } else {
        locctr = 0;
    }

    // Process lines until END
    while (strcmp(opcode, "END") != 0) {
        // --- SYMBOL TABLE HANDLING ---
        if (strcmp(label, "**") != 0) {
            symbol_found = false;
            rewind(symtab);
            while (fscanf(symtab, "%s", symtab_data) == 1) {
                if (strcmp(symtab_data, label) == 0) {
                    printf("Error: duplicate symbol %s\n", label);
                    symbol_found = true;
                    break;
                }
            }

            if (!symbol_found) {
                fprintf(symtab, "%s\t%04X\n", label, locctr);
            }
        }

        // --- OPCODE CHECK ---
        opcode_found = false;
        rewind(optab);
        while (fscanf(optab, "%s", optab_data) == 1) {
            if (strcmp(optab_data, opcode) == 0) {
                opcode_found = true;
                break;
            }
        }

        // --- WRITE TO INTERMEDIATE ---
        fprintf(intermediate, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        // --- UPDATE LOCCTR ---
        if (opcode_found) {
            locctr += 3;
        } else if (strcmp(opcode, "WORD") == 0) {
            locctr += 3;
        } else if (strcmp(opcode, "RESW") == 0) {
            locctr += 3 * atoi(operand);
        } else if (strcmp(opcode, "RESB") == 0) {
            locctr += atoi(operand);
        } else if (strcmp(opcode, "BYTE") == 0) {
            if (operand[0] == 'C')
                locctr += strlen(operand) - 3; // e.g. C'EOF' = 3 bytes
            else if (operand[0] == 'X')
                locctr += (strlen(operand) - 3) / 2; // e.g. X'F1' = 1 byte
        } else {
            printf("Warning: invalid opcode %s (ignored)\n", opcode);
        }

        // Read next line
        if (fscanf(source, "%s%s%s", label, opcode, operand) != 3) {
            break;
        }
    }

    // Write last END line
    fprintf(intermediate, "%04X\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    pgm_length = locctr - starting_address;
    printf("\nProgram length = %04X (%d)\n", pgm_length, pgm_length);
    fprintf(prgmlength, "%d", pgm_length);

    fclose(source);
    fclose(optab);
    fclose(intermediate);
    fclose(symtab);
    fclose(prgmlength);

    return 0;
}

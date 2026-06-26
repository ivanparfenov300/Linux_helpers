#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cc20.h"

#define BUFFER_SIZE 128

int main() {

    // Keys are not randomly generated
    unsigned int keys[16] = {840269343, 582321049, 789265432, 675839987,
                             873467890, 749258335, 888975733, 645104954,
                             142371434, 123652743, 583087523, 421957481,
                             909834567, 432487654, 239414874, 348973472};
    
    unsigned char message[BUFFER_SIZE] = {0};  // Always 128 bytes, rest are zeros

    printf("1 - Encrypt\n"
           "2 - Decrypt\n"
           "Enter your choice: ");
    int choice = 0;
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        FILE *file;

        printf("Enter message: ");
        fgets((char*)message, BUFFER_SIZE, stdin); 
        message[strlen((char*)message) - 1] = '\0';

        // Encrypt the entire fixed-size buffer
        cc(keys, message, message + BUFFER_SIZE);
    
        printf("\nEncrypted %d bytes: ", BUFFER_SIZE);
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            printf("%02x", message[i]);
            if ((i + 1) % 16 == 0) printf("\n                        "); // Formatting
        }

        // File name will be fixed for now
        file = fopen("noSecret.txt", "w");
        if (file == NULL) {
            perror("Error opening file.\n");
            return EXIT_FAILURE;
        }
        fprintf(file, "%s", message);
        fclose(file);
    }
    else if (choice == 2) {
        FILE *file;

        file = fopen("noSecret.txt", "r");
        if (file == NULL) {
            perror("Error opening file.\n");
            return EXIT_FAILURE;
        }

        fgets((char*)message, sizeof(message), file);
        fclose(file);

        // Decrypt the entire fixed-size buffer
        cc(keys, message, message + BUFFER_SIZE);
        printf("\nDecrypted message:\n%s\n", message);
    }
    else {
        printf("Invalid choice.\n");
    }    

    return EXIT_SUCCESS;
}

// Standard C libraries for input/output operations and string handling
#include <stdio.h>
#include <string.h>

// Custom header declaring the check_credentials function prototype
#include "auth.h"

// Verifies if the supplied username and password match any entry in users.txt
int check_credentials(const char *username, const char *password)
{
    // STEP 1: Attempt to open "users.txt" in read-only mode
    FILE *file = fopen("users.txt", "r");
    
    // If the file cannot be opened (e.g., file missing or permission error), fail authentication safely
    if (!file) {
        return 0;
    }

    // Buffers to store each raw line and extracted credentials from the file
    char line[256];
    char file_user[128];
    char file_pass[128];

    // STEP 2: Read users.txt line-by-line until reaching the End Of File (EOF)
    while (fgets(line, sizeof(line), file)) {
        
        // Parse the line into two separate strings: username and password
        // %127s prevents buffer overflow by limiting each string to 127 characters
        if (sscanf(line, "%127s %127s", file_user, file_pass) == 2) {
            
            // STEP 3: Compare input credentials against the line extracted from the file
            if (strcmp(username, file_user) == 0 && strcmp(password, file_pass) == 0) {
                
                // Match found! Close the open file pointer and return success (1)
                fclose(file);
                return 1; 
            }
        }
    }

    // STEP 4: If the entire file was scanned without a match, close the file and return failure (0)
    fclose(file);
    return 0; 
}
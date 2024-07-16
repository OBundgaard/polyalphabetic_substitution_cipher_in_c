#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


bool IsValidParameter(char *param, bool checkAlpha)
{
    // If null, then obviously it's not valid
    if (param == NULL)
    {
        return false;
    }

    // Must meet the two character minimum length
    int paramLength = strlen(param);
    if (paramLength < 2)
    {
        return false;
    }

    // Check if all characters are alphabetic
    if (checkAlpha)
    {
        for (int i = 0; i < paramLength; i++)
        {
            if (!isalpha(param[i]))
            {
                return false;
            }
        }
    }

    return true;
}


int IndexOf(char *param, char character)
{
    char *c;
    int index;

    c = strchr(param, character);
    index = (int)(c - param);

    return index;
}


void CreateAlphabet(char *alphabet, char *keyword)
{

    // Parameter checks for keyword
    if (!IsValidParameter(keyword, true))
    {
        strcpy(alphabet, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        return;
    }

    // Uppercase the keyword
    strupr(keyword);

    // Allocate memory for temporary alphabet
    char *tempAlphabet = malloc((27 + strlen(keyword)) * sizeof(char));

    // Combine keyword and ASCII letters within temporary alphabet
    strcpy(tempAlphabet, keyword);
    strcat(tempAlphabet, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    // Main loop
    int alphabetIndex = 0; // To ensure we correctly place characters
    for (int i = 0; tempAlphabet[i] != '\0'; i++)
    {
        // Check if the character is in the alphabet already
        if (strchr(alphabet, tempAlphabet[i]) == NULL)
        {
            // Add the character
            alphabet[alphabetIndex] = tempAlphabet[i];
            alphabetIndex++;
        }
    }
    alphabet[26] = '\0'; // Finalize the alphabet with the null terminator

    // Deallocate memory for temporary alphabet
    free(tempAlphabet);
}


const char *ShiftAlphabet(char *alphabet, int shifts)
{
    // Iterations must be positive
    if (shifts < 1)
    {
        return alphabet;
    }

    // Allocate memory and copy the given alphabet
    char *shiftedAlphabet = malloc(27 * sizeof(char));
    strcpy(shiftedAlphabet, alphabet);

    // Shift loop
    for (int i = 0; i != shifts; i++)
    {
        // Main loop
        char firstCharacter = shiftedAlphabet[0]; // Store first character
        for (int j = 1; shiftedAlphabet[j] != '\0'; j++)
        {
            shiftedAlphabet[j-1] = shiftedAlphabet[j]; // Move character back once
        }
        shiftedAlphabet[25] = firstCharacter; // Place the stored character at the end
    }

    return shiftedAlphabet;
}


void PrepareMessage(char *message)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        if (!isalpha(message[i]))
        {
            message[i] = 'X';
            continue;
        }
        message[i] = toupper(message[i]);
    }
}


void Encrypt(char *message, char *password, char *keyword)
{
    // Validate password
    if (!IsValidParameter(password, true))
    {
        return;
    }
    strupr(password); // Password needs to be uppercase too

    // Allocate memory for alphabet and create it
    char *alphabet = malloc(27 * sizeof(char));
    CreateAlphabet(alphabet, keyword);

    // Prepare the message directly
    PrepareMessage(message);

    // Main loop
    for (int i = 0; message[i] != '\0'; i++)
    {
        // Find X and Y with given password and message
        int x = IndexOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ", password[i % strlen(password)]);
        int y = IndexOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ", message[i]);

        // Shift alphabet
        message[i] = ShiftAlphabet(alphabet, y)[x];
    }

    // Deallocate all variables
    free(alphabet);
}


void Decrypt(char *message, char *password, char *keyword)
{
    // Validate password
    if (!IsValidParameter(password, true))
    {
        return;
    }
    strupr(password); // Password needs to be uppercase too

    // Allocate memory for alphabet and create it
    char *alphabet = malloc(27 * sizeof(char));
    CreateAlphabet(alphabet, keyword);

    // Main loop
    for (int i = 0; message[i] != '\0'; i++)
    {
        // Find X and Y with given password and message
        int x = IndexOf("ABCDEFGHIJKLMNOPQRSTUVWXYZ", password[i % strlen(password)]);

        for (int y = 0; y != 26; y++)
        {
            if (ShiftAlphabet(alphabet, y)[x] == message[i])
            {
                message[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[y];
                break;
            }
        }
    }

    // Deallocate all variables
    free(alphabet);
}


int main()
{
    char message[] = "Hello friend, let's meet at eight.thirty pm! - Me";
    char password[] = "stone";
    char keyword[] = "silver";

    Encrypt(message, password, keyword); // ZXZYPMYRUCRWGFMWHGRIEXPAIPHGOHYSBFXZIRALMCSFIMNSO
    printf("%s\n", message);

    Decrypt(message, password, keyword); // HELLOXFRIENDXXLETXSXMEETXATXEIGHTXTHIRTYXPMXXXXME - special characters and numbers will be replaced by X
    printf("%s\n", message);

    return 0;
}

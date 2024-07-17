#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


char LETTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int ALPHABET_LENGTH = 27;


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


int IndexOfLetter(char character)
{
    char *c;
    int index;

    c = strchr(LETTERS, character);
    index = (int)(c - LETTERS);

    return index;
}


void CreateAlphabet(char *alphabet, char *keyword)
{

    // Parameter checks for keyword
    if (!IsValidParameter(keyword, true))
    {
        strcpy(alphabet, LETTERS);
        return;
    }

    // Uppercase the keyword
    strupr(keyword);

    // Allocate memory for temporary alphabet
    char *tempAlphabet = (char *) calloc(ALPHABET_LENGTH + strlen(keyword), sizeof(char));

    // Combine keyword and ASCII letters within temporary alphabet
    strcpy(tempAlphabet, keyword);
    strcat(tempAlphabet, LETTERS);

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
    alphabet[ALPHABET_LENGTH - 1] = '\0'; // Finalize the alphabet with the null terminator

    // Deallocate memory for temporary alphabet
    free(tempAlphabet);
}


char *ShiftAlphabet(char *alphabet, int shifts)
{
    // Iterations must be positive
    if (shifts < 1)
    {
        return alphabet;
    }

    // Allocate memory and copy the given alphabet
    char *shiftedAlphabet = (char *) calloc(ALPHABET_LENGTH, sizeof(char));
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
        shiftedAlphabet[ALPHABET_LENGTH - 2] = firstCharacter; // Place the stored character at the end
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
    char *alphabet = (char *) calloc(ALPHABET_LENGTH, sizeof(char));
    CreateAlphabet(alphabet, keyword);

    // Prepare the message directly
    PrepareMessage(message);

    // Main loop
    char *shiftedAlphabet;
    for (int i = 0; message[i] != '\0'; i++)
    {
        // Find X and Y with given password and message
        int x = IndexOfLetter(password[i % strlen(password)]);
        int y = IndexOfLetter(message[i]);

        // Shift alphabet
        shiftedAlphabet = ShiftAlphabet(alphabet, y);
        message[i] = shiftedAlphabet[x];
    }

    // Deallocate all variables
    free(alphabet);
    free(shiftedAlphabet);
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
    char *alphabet = (char *) calloc(ALPHABET_LENGTH, sizeof(char));
    CreateAlphabet(alphabet, keyword);

    // Main loop
    char *shiftedAlphabet;
    for (int i = 0; message[i] != '\0'; i++)
    {
        // Find X and Y with given password and message
        int x = IndexOfLetter(password[i % strlen(password)]);

        for (int y = 0; y != ALPHABET_LENGTH - 1; y++)
        {
            shiftedAlphabet = ShiftAlphabet(alphabet, y);
            if (shiftedAlphabet[x] == message[i])
            {
                message[i] = LETTERS[y];
                break;
            }
        }
    }

    // Deallocate all variables
    free(alphabet);
    free(shiftedAlphabet);
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

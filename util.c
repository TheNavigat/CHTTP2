#include <stdio.h>

void append_to_string(char* destination, char* source)
{
    // Detect end of destination
    int end_of_destination = 0;

    while(destination[end_of_destination] != '\0')
        end_of_destination++;

    printf("\n\nEnd of destination: %d\n\n", end_of_destination);

    int i;

    for(i = 0; source[i] != '\0'; i++)
    {
        destination[end_of_destination + i] = source[i];
    }

    destination[end_of_destination + i + 1] = '\0';
}

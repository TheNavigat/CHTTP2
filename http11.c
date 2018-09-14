#include <stdio.h> // This is used for testing, we'll need to remove it later
#include <string.h>

int detect_crlf(char* buffer, int position) {
    for(int i = 0; i < 3; i+= 2)
        if(buffer[position + i] != '\r' || buffer[position + i + 1] != '\n')
            return(0);

    printf("DETECTED");
    return(1);
}

int process_headers(char* buffer) {
    int buffer_length = strlen(buffer);

    if(buffer_length < 4) return(0); // We need to detect 2 CLRFs

    int i = 0;

    int detected = 0;

    while(i < buffer_length - 4) // 0 is '\0'
    {
        if(detect_crlf(buffer, i)) {
            detected = 1;
            break;
        }

        i++;
    }

    if(detected) {
        printf("%.*s\n", i, buffer);
    }

    // Break down into lines
    char* lines[20];

    // Point to the first character
    lines[0] = buffer;

    int k = 1;

    for(int j = 0; j < i; j++)
    {
        if(buffer[j] == '\r')
            buffer[j] = '\0';
        else if(buffer[j] == '\n')
        {
            buffer[j] = '\0';
            lines[k++] = &buffer[j + 1];
            printf("\nNEXT\n");
        }
    }

    buffer[i] = '\0';

    // Parse lines
    for(int j = 0; j < k; j++)
    {
        printf("%s\n", lines[j]);
    }

    return(0);
}

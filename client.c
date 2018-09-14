#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "http11.h"
#include "util.h"

#define PORT 80

int domain_to_in_addr(char* domain, struct in_addr* address)
{
    struct hostent *lh = gethostbyname(domain);

    char ipv4_address[16];

    if (lh) {
        // h_addr_list is a char** but it's formatted... weirdly...
        // This is the way to print it correctly
        sprintf(
            ipv4_address, "%d.%d.%d.%d",
            (unsigned char)(lh->h_addr_list[0][0]),
            (unsigned char)(lh->h_addr_list[0][1]),
            (unsigned char)(lh->h_addr_list[0][2]),
            (unsigned char)(lh->h_addr_list[0][3])
        );

        printf("%s\n", ipv4_address);
    }
    else
    {
        herror("gethostbyname");
        return(-1);
    }

    struct in_addr return_value;

    if(inet_pton(AF_INET, ipv4_address, address) <= 0)
    {
        herror("Invalid address/address not supported.\n");
        return(-1);
    }

    return(0);
}

int main() {
    int sock_fd;

    struct sockaddr_in address;

    char *hello = "GET / HTTP/1.1\r\n\r\n";
    char buffer[1024] = {0};
    char in_buffer[50000] = {0};

    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error.\n");
        return(-1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    int res = domain_to_in_addr("www.yelp.com", &address.sin_addr);

    if(res < 0) {
        return(-1);
    }

    printf("BLA");
    printf("\n%d\n", address.sin_addr.s_addr);

    if(connect(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Connection failed.\n");
        return(-1);
    }

    send(sock_fd, hello, strlen(hello), 0);
    printf("Sent.\n");

    // https://stackoverflow.com/questions/1790750/what-is-the-difference-between-read-and-recv-and-between-send-and-write

    int count = 0;
    do {
        count = read(sock_fd, buffer, 1020);
        buffer[count] = '\0';

        append_to_string(in_buffer, buffer);

        // printf("%s\n", buffer);
        // printf("In buffer:");
        // printf("\n\n%s\n\n\n", in_buffer);
        // printf("DONE\n\n");

        process_headers(in_buffer);

        if (count < 0)
            printf("ERROR reading response from socket");
        if (count == 0)
            break;
    } while(1);

    return 0;
}

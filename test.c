#include <stdio.h>
#include <netdb.h>


int main(int argc, char *argv[])
{
    struct hostent *lh = gethostbyname("localhost");

    if (lh) {
        // h_addr_list is a char** but it's formatted... weirdly...
        // This is the way to print it correctly
        printf("%d.%d.%d.%d\n", (unsigned char)(lh->h_addr_list[0][0]),
                        (unsigned char)(lh->h_addr_list[0][1]),
                        (unsigned char)(lh->h_addr_list[0][2]),
                        (unsigned char)(lh->h_addr_list[0][3]));
    }
    else
        herror("gethostbyname");

    return 0;
}

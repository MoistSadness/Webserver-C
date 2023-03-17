#include "stdio.h"
#include "string.h"
#include "errno.h" // errno.h provides an error number for whatever has gone wrong with the application
#include "sys/socket.h"
#include "arpa/inet.h"
#include <fcntl.h> // for open
#include <unistd.h> // for close


#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    char buffer[BUFFER_SIZE];

    char response[] = "HTTP/1.0 200 OK\r\n"
        "Server: webserver-c\r\n"
        "Content-type: text/html\r\n\r\n"
        "<html>hello, world</html>\r\n";

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Throw error if the socket creation fails
    if (sockfd == -1){ 
        perror("Webserver (socket)");
        return 1;
    }
    printf("Socket created successfuly!\n");

    // Creating the address to bind the socket to 
    struct sockaddr_in host_addr;                           // Creating structure for local address
    int host_addrlen = sizeof(host_addr);
                                                            // *** Setting the address variables ***
    host_addr.sin_family = AF_INET;                         // AF_INET refers to IP/TCP
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);          // htonl converts the local IP address into network byte order, which allows the value to be read regardless of CPU architecture
                                                            // INADDR_ANY refers to the local IP address (0.0.0.0)
    if( bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen ) != 0){
        perror("Webserver (bind)");
        return 1;
    }
    //printf("%d, %d, %d\n", host_addr.sin_family, host_addr.sin_port, host_addr.sin_addr.s_addr);
    printf("Socket bound successfully!\n");

    // Listen for incoming connections, socket is set to passive mode
    if( listen(sockfd, SOMAXCONN) != 0){                    // SOMAXCONN is a constant value set to 128.
        perror("Webserver (listen)");
        return 1;
    }
    printf("Listening for connections on port %d\n", PORT);

    // Set up an infinite loop to accept requests
    for( ; ; ){
        // Accept incoming transmissions
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr,(socklen_t *)&host_addrlen);
        if (newsockfd < 0 ){
            perror("Webserver (accept)");
            continue;
        }
        printf("Connection accepted\n");

        // Reading from socket
        int socketRead = read(newsockfd, buffer, BUFFER_SIZE);
        if(socketRead < 0){
            perror("Webserver (read)");
            continue;
        }

        // Write to socket
        int socketWrite = write(newsockfd, response, strlen(response));
        if(socketWrite < 0){
            perror("Webserver (write)");
            continue;
        }

        close(newsockfd);
    }

    return 0;
}
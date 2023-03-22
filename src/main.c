#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "errno.h" // errno.h provides an error number for whatever has gone wrong with the application
#include "sys/socket.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "arpa/inet.h"
#include "fcntl.h" // for open
#include "unistd.h" // for close

#define PORT 8080
#define BUFFER_SIZE 1024

/*** Read an HTML file into memory
 * Takes a path to the desired file as input
 * Return a file pointer
 * 
*/
char* readHtmlFile(char* pagesRoot){
    //printf("Reading file\n");

    char testfile[] = "./testfile.html";
    FILE* fp;

    struct stat st;     // Store file data here
    if (stat(pagesRoot, &st) ==0) printf("File size is %d\n", st.st_size);      // Get the size of the file in bytes

    // Dynamically allocate memory for file, read it into buffer and print it
    char* fileBuffer = malloc(st.st_size * sizeof(char));
    

    // Free memory



    return fileBuffer;
}

/*** Free dynamically allocated memory
*/
void closeHtmlFile(char* file){
    free(file);
    if(file){       // If the pointer is still allocated, send an error message
        perror("Webserver (free)");
    }
}

/*** The request needs to be handled
 * Run some code based on the method here, GET, POST, etc... Basic CRUD functionality will be handled for now.
 * 
*/
void handle_http_request(int newsockfd, char* method, char* uri, char* version){
    char response[] = "HTTP/1.0 200 OK\r\n"
        "Server: webserver-c\r\n"
        "Content-type: text/html\r\n\r\n"
        "<html>hello, world</html>\r\n";

    // This is the root directory for the HTML page router 
    char pagesRoot[] = "../pages/index.html";

    // A switch statement cannot be used with strings, so an if/else block will have to be set up with strcmp.
    if (strcmp(method, "GET") == 0){                                            // READ
        printf("Get request!\n");
        // Serve html files based on what the URI is
        //router(uri);
    } 
    else if (strcmp(method, "POST") == 0){                                      // CREATE
        printf("Post request!\n");
    } 
    else if (strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0){        // UPDATE
        printf("Update request!\n");
    } 
    else if (strcmp(method, "DELETE") == 0){                                    // DELETE
        printf("Delete request!\n");
    } 
    else {
        printf("Invalid request!\n");
    }

    fflush(stdout);


    // Read HTML file
    char* file = readHtmlFile(pagesRoot);


   
    int socketWrite = write(newsockfd, response, strlen(response));
    if(socketWrite < 0){
        perror("Webserver (write)");
        return;
        //continue;
    }

    // Close HTML file and free the used memory
    closeHtmlFile(file);
}

/*** Route a GET reqest based on the URI. Use a filesystem based routing system like in Next.js
 * Use the URI to generate a path to the desired HTML file, load it into memory and return a pointer to the string 
 where it can be sent to the client or modified or whatver needs to be done
 *      If file does't exists, send 404
 * index.html will correspond to the root file in a directory
*/
char* router(char * uri){
    printf("Routing!");
    return uri;
}


/*** Set up sockets, addresses and application loop
 * 
 * 
*/
int main(){
    char buffer[BUFFER_SIZE];
    
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // Throw error if the socket creation fails
    if (sockfd == -1){ 
        perror("Webserver (socket)");
        return 1;
    }
    printf("Socket created successfully!\n");

    // Creating the address to bind the socket to 
    struct sockaddr_in host_addr;                           // Creating structure for local address
    int host_addrlen = sizeof(host_addr);

    // Creating client address
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);
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

        // Get client address
        int clientSocket = getsockname(newsockfd, (struct sockaddr *)&client_addr,(socklen_t *)&client_addrlen);
        if(clientSocket < 0){
            perror("Webserver (getsockname)");
            continue;
        }

        // Reading from socket. The entire HTTP request will be stored inside buffer
        int socketRead = read(newsockfd, buffer, BUFFER_SIZE);
        if(socketRead < 0){
            perror("Webserver (read)");
            continue;
        }

        // The requesst has the following structure: <method> <path> <version> - creating variables to store them
        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);       // Parse the request header

        /*** Display HTTP request information and the client IP address from which it has arived ***
            inet_ntoa() converts network byte order to a IPV4 string 
            ntohl() converts integer from network byte order to host byte order. 
            This distinction has to be made to account for different CPU architectures. Some are in big endian, others are in little endian and are not compatible with each other.*/
        printf("%s %s %s \t from \t [%s:%u]\n",method, uri, version, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));  

        // Handle the request now that it has been confirmed and printed to screen
        handle_http_request(newsockfd, method, uri, version);

        close(newsockfd);
    }

    return 0;
}
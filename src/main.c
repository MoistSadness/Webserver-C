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

/* Takes in a file pointer to the response, the location of the target file, and the current size of the response
  * Get the file size of the target string
  * Allocate memory for current string size + target string size
  * Copy contents of original string to the new buffer
  * Open target file
  * Append contents of target file to the new buffer 
  * Update current size of response by reference
  * Print the new file size
  * Return the char* for the new buffer
*/
char* openFileAndAddToString(char* response, char* targetFile, int* responseSize){
    // Declaring variables
    char* buffer;
    int bufferSize;

    // Get the file size of the target string
    struct stat st;
    if (stat(targetFile, &st) == 0) {
        bufferSize = st.st_size + (*responseSize);
        printf("Target file size is %lld\n", st.st_size);      // Get the size of the file in bytes
        } else return "Failed";

    //Allocate memory for current string size + target string size
    printf("Allocating %d + %lld = %d for new string\n", 
        *responseSize, 
        st.st_size, 
        bufferSize
    ); 
    buffer = (char*)malloc(bufferSize);
    strcpy(buffer, response);       // Copy contents of original string to the new buffer
    char* current = buffer + (*responseSize) - 1;     // subtract 1 from the position to remove the '/0' string terminator otherwise the string won't be read

    int bytesRead, chunk = 20;
    char temparr[chunk];
    int totalBytesRead = 0;
    
    FILE *file = fopen(targetFile, "r");
    if (file){
        do {
            bytesRead = fread(temparr, sizeof(char), chunk, file);
            //printf("%s\n", temparr);
            memcpy(current, temparr, chunk);
            current += bytesRead;
            totalBytesRead += bytesRead;
        } while (bytesRead == chunk);

        // Close file
        fclose(file);
        // Ternimate the buffer so string functions can work on it
        *current = '\0';
        printf("%d bytes have been read\n\n", totalBytesRead);
    } else perror("Webserver (fopen)");

    // Updating response size with the size of the new response string
    *responseSize = bufferSize;

    free(response);     // Free the original string
    return buffer;
}

    /***    BUILDING HTML FILE
     * Get file size for response_headers, head, navbar, target html amd head_close and add them +1 to get the total size of the response
     * Allocate memory of response size for string and copy headers into it
     * Open head file and append it to allocated string
     * Repeat for navbar, then target html, then footer, then head_close
     * Write response to socket
     * Free memory
    */
char * buildHtmlFile(char* pages){
    printf("Building HTML response\n");
    char response_headers[] = "HTTP/1.0 200 OK\r\n"
                                "Server: webserver-c\r\n"
                                "Content-type: text/html\r\n\r\n";
    
    char headPath[] = "../boilerplate/head.html";               // Path to head html
    /**/
    char navbarPath[] = "../boilerplate/navbar.html";           // Path to navbar html
    char footerPath[] = "../boilerplate/footer.html";           // Path to footer html
    char headclosePath[] = "../boilerplate/head_close.html";    // Path to head_close html
    

    // Creating the base response that contains only the response headers
    int responseSize = sizeof(response_headers) / sizeof(char);       // Add 1 extra byte to add string terminator
    printf("Allocating %d bytes for response headers\n", responseSize); 
    char* response = (char*)malloc(responseSize);
    strcpy(response, response_headers);       // Copy response headers into allocated memory

    // Add the head to the html
    response = openFileAndAddToString(response, headPath, &responseSize);

    // Add the navbar to the html
    response = openFileAndAddToString(response, navbarPath, &responseSize);

    // Add the target to the html
    response = openFileAndAddToString(response, pages, &responseSize);

    // Add the footer to the html
    response = openFileAndAddToString(response, footerPath, &responseSize);

    // Add the head_close to the html
    response = openFileAndAddToString(response, headclosePath, &responseSize);

    return response;
}



/*** The request needs to be handled
 * Run some code based on the method here, GET, POST, etc... Basic CRUD functionality will be handled for now.
 * 
*/
void handle_http_request(int newsockfd, char* method, char* uri, char* version){
    char pages[] = "../pages";                   // This is the root directory for the HTML page router 

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

    /*** Router
     * 
    

    // Build path to the target html file. Add the uri to the base path for the pages to get a base path
    printf("%d %d \n", strlen(pages), strlen(uri));
    char* pagepath = (char*)malloc(strlen(pages) + strlen(uri) + 1);
    memcpy(pagepath, pages, strlen(pages));
    strncat(pagepath, uri, strlen(uri));

    // Check the to see if it ends with a  '/' character. If it does, add 'index.html' so it sends the root file to the client
    // Check to see if there is an -/index.html file to see if the root of that directory exists. Otherwise send a 404


    printf("The file is located at %s\n", pagepath);

    */

    char pagepath[] = "../pages/index.html";  
    char* response = buildHtmlFile(pagepath);



    fflush(stdout);
    int socketWrite = write(newsockfd, response, strlen(response));
    if(socketWrite < 0){
        perror("Webserver (write)");
        return;
        //continue;
    }

    // Free memory
    //free(pagepath);
    free(response);
    //printf("Freeing Memory\n\n");

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

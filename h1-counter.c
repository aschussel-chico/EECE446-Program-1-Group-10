#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER "www.ecst.csuchico.edu"
#define PORT "80"
#define REQUEST "GET /~kkredo/file.html HTTP/1.0\r\n\r\n"

int main(int argc, char *argv[]) {
    int chunk_size;
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char *buffer;
    int total_bytes = 0;
    int total_h1_count = 0;
    ssize_t bytes_received;
    const char *request = REQUEST;
    size_t request_len = strlen(request);
    ssize_t bytes_sent;
    size_t total_sent = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <chunk_size>\n", argv[0]);
        fprintf(stderr, "chunk_size must be between 5 and 1000\n");
        return 1;
    }

    chunk_size = atoi(argv[1]);
    if (chunk_size < 5 || chunk_size > 1000) {
        fprintf(stderr, "Error: chunk_size must be between 5 and 1000\n");
        return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(SERVER, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "Failed to connect\n");
        freeaddrinfo(servinfo);
        return 1;
    }

    freeaddrinfo(servinfo);

    while (total_sent < request_len) {
        bytes_sent = send(sockfd, request + total_sent, request_len - total_sent, 0);
        if (bytes_sent == -1) {
            perror("send");
            close(sockfd);
            return 1;
        }
        total_sent += bytes_sent;
    }

    buffer = (char *)malloc(chunk_size);
    if (buffer == NULL) {
        perror("malloc");
        close(sockfd);
        return 1;
    }

    while ((bytes_received = recv(sockfd, buffer, chunk_size, 0)) > 0) {
        total_bytes += bytes_received;
        
        char *pos = buffer;
        char *end = buffer + bytes_received;
        while ((pos = memmem(pos, end - pos, "<h1>", 4)) != NULL) {
            total_h1_count++;
            pos += 4;
        }
    }

    if (bytes_received == -1) {
        perror("recv");
        free(buffer);
        close(sockfd);
        return 1;
    }

    free(buffer);
    close(sockfd);

    printf("%d %d\n", total_h1_count, total_bytes);

    return 0;
}

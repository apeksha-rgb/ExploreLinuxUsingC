#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <process.h>
    #pragma comment(lib, "ws2_32.lib")

    #define CLOSESOCK closesocket
    #define socklen_t int
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>

    #define CLOSESOCK close
#endif

#define PORT 5555
#define BUF 1024

// ---------------------------------------
// Initialize Winsock (Windows only)
// ---------------------------------------
void init_sockets() {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif
}

// ---------------------------------------
// Cleanup Winsock
// ---------------------------------------
void cleanup_sockets() {
#ifdef _WIN32
    WSACleanup();
#endif
}

// ---------------------------------------
// Send a message
// ---------------------------------------
void send_message(int sock) {
    char msg[BUF];

    printf("You: ");
    fgets(msg, BUF, stdin);

    send(sock, msg, strlen(msg), 0);

    // Exit if user typed "exit"
    if (strncmp(msg, "exit", 4) == 0)
        exit(0);
}

// ---------------------------------------
// Receive message
// ---------------------------------------
void receive_message(int sock) {
    char buf[BUF];
    int n = recv(sock, buf, BUF - 1, 0);

    if (n <= 0) {
#ifdef _WIN32
        printf("Connection closed. Error: %d\n", WSAGetLastError());
#else
        perror("recv");
#endif
        exit(0);
    }

    buf[n] = '\0';   // Null-terminate safely
    printf("Peer: %s\n", buf);

    // Exact match for exit
    if (strcmp(buf, "exit") == 0 || strcmp(buf, "exit\n") == 0)
        exit(0);
}

// ---------------------------------------
// MAIN
// ---------------------------------------
int main() {
    init_sockets();

    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

#ifdef SERVER
    // ------------------ SERVER ------------------
    printf("Starting server...\n");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("bind failed");
        exit(1);
    };
    listen(sock, 1);

    printf("Waiting for client...\n");
    
    struct sockaddr_in clientAddr;

    int clientSock;
    socklen_t len = sizeof(clientAddr);
    clientSock = accept(sock, (struct sockaddr*)&clientAddr, &len);

    if(clientSock < 0){
        perror("accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    // Chat loop
    while (1) {
        receive_message(clientSock);
        send_message(clientSock);
    }

#else
    // ------------------ CLIENT ------------------
    printf("Client: Enter server IP: ");
    char ip[32];
    scanf("%31s", ip);

    int c;
    while ((c=getchar()) !='\n' && c!= EOF); // clear newline

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    printf("Connecting...\n");
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    printf("Connected to server.\n");

    // Chat loop
    while (1) {
        send_message(sock);
        receive_message(sock);
    }

#endif

    CLOSESOCK(sock);
    cleanup_sockets();
    return 0;
}

#include "socket_util.h"

void listenAndPrint(int socketFD)
{
    char buffer[1024];

    while (true)
    {
        ssize_t amountReceived = recv(socketFD, buffer, 1024, 0);

        if (amountReceived > 0)
        {
            buffer[amountReceived] = 0;
            printf("%s", buffer);
        }
        else if (!amountReceived)
            break;
    }

    close(socketFD);
}

void startListeningAndPrintMessagesOnNewThread(int socketFD)
{
    pthread_t id;
    pthread_create(&id, NULL, listenAndPrint, socketFD);
}

void readConsoleEntriesAndSendToServer(int socketFD)
{
    char *name = NULL;
    size_t nameSize = 0;
    printf("Please enter your name: ");
    ssize_t nameCount = getline(&name, &nameSize, stdin);
    if (nameCount > 0)
        name[nameCount - 1] = 0;

    char *line = NULL;
    size_t lineSize = 0;
    printf("Type your message (type exit to exit)\n");

    char buffer[1024];

    while (true)
    {
        ssize_t charCount = getline(&line, &lineSize, stdin);
        line[charCount - 1] = 0;

        snprintf(buffer, sizeof buffer, "%s: %s", name, line);

        if (charCount > 0)
        {
            if (!strncmp(line, "exit", charCount))
                break;

            ssize_t amountSent = send(socketFD, buffer, strlen(buffer), 0);
        }
    }

    free(line);
    free(name);
}

int main()
{
    int socketFD = createTCPIPv4Socket();
    struct sockaddr_in *address = createIPv4Address("127.0.0.1", 2000);

    int result = connect(socketFD, address, sizeof *address);

    if (!result)
        printf("Connection was successful!\n");

    startListeningAndPrintMessagesOnNewThread(socketFD);

    readConsoleEntriesAndSendToServer(socketFD);

    free(address);
    close(socketFD);

    return 0;
}

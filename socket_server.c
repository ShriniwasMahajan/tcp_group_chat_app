#include "socket_util.h"

struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

int acceptedSocketsCount = 0;
struct AcceptedSocket acceptedSockets[10];

struct AcceptedSocket *acceptIncomingConnections(int serverSocketFD)
{
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, &clientAddress, &clientAddressSize);

    struct AcceptedSocket *acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;

    if (!acceptedSocket->acceptedSuccessfully)
        acceptedSocket->error = clientSocketFD;

    return acceptedSocket;
}

void sendReceivedMessageToTheOtherClients(char *buffer, int socketFD)
{
    for (int i = 0; i < acceptedSocketsCount; i++)
        if (acceptedSockets[i].acceptedSocketFD != socketFD)
            send(acceptedSockets[i].acceptedSocketFD, buffer, strlen(buffer), 0);
}

void receiveAndPrintIncomingData(int socketFD)
{
    char buffer[1024];

    while (true)
    {
        ssize_t amountReceived = recv(socketFD, buffer, 1024, 0);

        if (amountReceived > 0)
        {
            buffer[amountReceived] = 0;
            printf("%s\n", buffer);

            sendReceivedMessageToTheOtherClients(buffer, socketFD);
        }
        else if (!amountReceived)
            break;
    }

    close(socketFD);
}

void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket *pSocket)
{
    pthread_t id;
    pthread_create(&id, NULL, receiveAndPrintIncomingData, pSocket->acceptedSocketFD);
    pthread_detach(id);
}

void startAcceptingIncomingConnections(int serverSocketFD)
{
    while (true)
    {
        struct AcceptedSocket *clientSocket = acceptIncomingConnections(serverSocketFD);
        if (acceptedSocketsCount < 10)
            acceptedSockets[acceptedSocketsCount++] = *clientSocket;
        else
            close(clientSocket->acceptedSocketFD);

        receiveAndPrintIncomingDataOnSeparateThread(clientSocket);

        free(clientSocket);
    }
}

int main()
{
    int serverSocketFD = createTCPIPv4Socket();
    struct sockaddr_in *serverAddress = createIPv4Address("", 2000);

    int result = bind(serverSocketFD, serverAddress, sizeof *serverAddress);
    if (!result)
        printf("Socket was bound successfully!\n");
    else
        perror("Bind failed");

    int listenResult = listen(serverSocketFD, 10);

    startAcceptingIncomingConnections(serverSocketFD);

    free(serverAddress);
    shutdown(serverSocketFD, SHUT_RDWR);

    return 0;
}

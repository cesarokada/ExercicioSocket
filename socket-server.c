#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Lê texto do socket e imrpime. Continua até que o socket feche.
Retorna não-zero se o cliente enviou um “quit” ou zero caso
contrário. */

int server (int client_socket)
{
    while (1) {
        int length;
        char* text;
        /* Primeiro, lê o tamanho de uma mensagem de texto do socket.
        Se a leitura retorna zero, o cliente fechou a conexão. */
        if (read (client_socket, &length, sizeof (length)) == 0)
            return 0;
        /* Aloca o buffer para armazenar o texto. */
        text = (char*) malloc (length);
        /* Lê o texto, e o imprime. */
        read (client_socket, text, length);
        printf ("%s\n", text);
        /* Se o cliente enviou a mensagem "quit", terminamos. */
        if (!strcmp (text, "quit")){
            free (text); /* Libera o buffer. */
            return 1;
        }
        else free (text); /* Libera o buffer. */
    }
}


int main (int argc, char* const argv[])
{
    const int port = atoi(argv[1]);
    int socket_fd;
    struct sockaddr_in name;
    int client_sent_quit_message;

    /* Cria o socket. */
    socket_fd = socket (PF_INET, SOCK_STREAM, 0);
    /* Indica que este é um servidor. */
    name.sin_family = AF_INET;
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    name.sin_port = htons(port);
    bind (socket_fd, (struct sockaddr *) &name, sizeof(struct sockaddr_in));
    /* Escuta por conexões. */
    listen (socket_fd, 5);
    /* Repetidamente aceita conexões, e cria um novo socket para lidar
    com cada cliente. Continua até um cliente enviar a mensagem “quit”. */
    do {
        struct sockaddr_in client_name;
        socklen_t client_name_len;
        int client_socket_fd;
        /* Aceita uma conexão. */
        client_name_len = sizeof(client_name);
        client_socket_fd = accept (socket_fd, (struct sockaddr *)
        &client_name, &client_name_len);
        /* Manipula a conexão. */
        client_sent_quit_message = server (client_socket_fd);
        /* Fecha nossa ponta da conexão. */
        close (client_socket_fd);
    }while (!client_sent_quit_message);
    /* Remove o arquivo de socket. */
    close (socket_fd);
    return 0;
}

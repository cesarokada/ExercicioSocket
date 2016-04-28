
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

/* Imprime o contéudo da home page do servidor ao qual o socket.
está conectado. Returna uma indicação de sucesso. */
void write_text (int socket_fd, const char* text)
{
/* Escreve o número de bytes na string, incluindo o terminador nulo */
int length = strlen (text) + 1;
write (socket_fd, &length, sizeof (length));
/* Escreve a string. */
write (socket_fd, text, length);
}

int main (int argc, char* const argv[])
{
    int socket_fd;
    struct sockaddr_in name;
    struct hostent* hostinfo;
    /* Cria o socket. */
    socket_fd = socket (PF_INET, SOCK_STREAM, 0);
    /* Armazena o nome do servidor no endereço do socket. */
    name.sin_family = AF_INET;
    /* Converte de strings para números. */
    hostinfo = gethostbyname (argv[1]);
    if (hostinfo == NULL)
        return 1;
    else
        name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
    /* Servidores Web usam a porta 80. */
    name.sin_port = htons (atoi(argv[2]));
    /* Conecta ao servidor web */
    if (connect (socket_fd, (struct sockaddr *) &name, sizeof(struct sockaddr_in)) == -1) {
        perror ("connect");
        return 1;
    }
    write_text(socket_fd,argv[3]);
    return 0;
}

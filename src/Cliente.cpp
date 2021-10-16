#include <iostream>
#include <winsock2.h>

using namespace std;

class Cliente
{
public:
    //atributos
    WSADATA wsaData;
    SOCKET server;//creo socket server
    SOCKADDR_IN addr;
    char buffer[1024];
    char ip[20];
    int puerto;

    Cliente(int puerto, char ip[20])
    {
        WSAStartup(MAKEWORD(2,0), &wsaData);
        server = socket(AF_INET, SOCK_STREAM, 0);

        addr.sin_addr.s_addr = inet_addr(ip);// ile digo que escuche en una ip especifica para conectarme
        addr.sin_family = AF_INET;//asigna la ip del cliente
        addr.sin_port = htons(puerto); //puerto al cual conectarme

        //conectarse al servidor
        connect(server, (SOCKADDR *)&addr, sizeof(addr));
        cout<<"Conectado al servidor"<<endl;
    }
    // metodos
    void enviar()
    {
        cout<<"Escribe el mensaje a enviar: "<<endl;
        cin>>this->buffer;
        send(server, buffer, sizeof(buffer), 0);
        cout<<"Mensaje enviado"<<endl;
        memset(buffer, 0, sizeof(buffer));
    }

    void recibir()
    {
        recv(server, buffer, sizeof(buffer),0);
        cout<<"El servidor dice: "<<buffer<<endl;
        memset(buffer, 0, sizeof(buffer));
    }

    void cerrarSocket()
    {
        closesocket(server);
        WSACleanup();
        cout<<"Socket cerrado"<<endl;
    }

};

#include <iostream>
#include <winsock2.h>

using namespace std;

class Cliente
{
public:
    //atributos
    WSADATA wsaData;
    SOCKET server;//creo socket server
    SOCKADDR_IN direccion;
    char buffer[1024];
    char ip[20];
    int puerto;

    Cliente(int puerto, char ip[20])
    {
        WSAStartup(MAKEWORD(2,0), &wsaData);
        server = socket(AF_INET, SOCK_STREAM, 0);

        direccion.sin_addr.s_addr = inet_addr(ip);// ile digo que escuche en una ip especifica para conectarme (ip del server)
        direccion.sin_family = AF_INET;//asigna la ip del cliente
        direccion.sin_port = htons(puerto); //puerto al cual conectarme (puerto del server)

        //conectarse al servidor
        if( (connect(server, (SOCKADDR *)&direccion, sizeof(direccion))) != 0 )
        {
            cout<<("No se pudo conectar o el servidor esta ocupado")<<endl;
        }
        else
        {
            cout<<"Conectado"<<endl;
        }

    }
    // metodos
    void enviar()
    {
        cout<<"Escribe el mensaje a enviar: "<<endl;
        cin>>this->buffer;
        send(server, buffer, sizeof(buffer), 0);
        cout<<"Mensaje enviado"<<endl;
        memset(buffer, 0, sizeof(buffer)); //resetear el buffer
    }

    void enviar(const char *mensaje)
    {
        strcpy(buffer, mensaje);

        send(server, buffer, sizeof(buffer), 0);
        cout<<"solicitud enviada"<<endl;
        memset(buffer, 0, sizeof(buffer)); //resetear el buffer
    }



    char* recibir()
    {
        recv(server, buffer, sizeof(buffer),0);

        return buffer;
        memset(buffer, 0, sizeof(buffer));
    }

    void cerrarSocket()
    {
        closesocket(server);
        WSACleanup();
        cout<<"Socket cerrado"<<endl;
    }

};

#include <iostream>
#include <winsock2.h>

using namespace std;

class Cliente
{
public:
    //atributos
    WSADATA wsaData;
    SOCKET servidor;//creo socket server
    SOCKADDR_IN direccion;
    char buffer[1024];
    char ip[20];
    int puerto;

    Cliente(int puerto, char ip[20])
    {
        WSAStartup(MAKEWORD(2,0), &wsaData);
        servidor = socket(AF_INET, SOCK_STREAM, 0);

        direccion.sin_addr.s_addr = inet_addr(ip);// ile digo que escuche en una ip especifica para conectarme (ip del server)
        direccion.sin_family = AF_INET;//asigna la ip del cliente
        direccion.sin_port = htons(puerto); //puerto al cual conectarme (puerto del server)



    }
    // metodos

    /*void enviar()
    {

        cout<<"Escribe el mensaje a enviar: "<<endl;
        cin>>this->buffer;
        send(servidor, buffer, sizeof(buffer), 0);
        cout<<"Mensaje enviado"<<endl;
        memset(buffer, '\0', strlen(buffer)); //resetear el buffer
    }
    */

    void conectarServidor()
    {
        //conectarse al servidor
        if( connect(servidor, (SOCKADDR *)&direccion, sizeof(direccion)) != INVALID_SOCKET )
        {
            cout<<"Conectado al servidor!"<<endl;

        }
        else
        {
            cout<<("Solo puede haber un cliente conectado a la vez, por favor intentelo mas tarde")<<endl;
        }
    }

    void enviar(const char *mensaje)
    {
        strcpy(buffer, mensaje);
        cout<<buffer<<endl;
        send(servidor, buffer, sizeof(buffer), 0);
        cout<<"solicitud enviada"<<endl;
        memset(buffer,'\0', strlen(buffer)); //resetear el buffer
    }



    string recibir()
    {
        int bytesRecibidos = recv(servidor, buffer, sizeof(buffer),0);
        if(bytesRecibidos == SOCKET_ERROR)
        {
            cout<<"El servidor se desconecto"<<endl;
        }
        string recibido = buffer;
        memset(buffer,'\0', strlen(buffer));
        return recibido;

    }

    void cerrarSocket()
    {
        closesocket(servidor);
        WSACleanup();
        cout<<"Socket cerrado"<<endl;
    }

};

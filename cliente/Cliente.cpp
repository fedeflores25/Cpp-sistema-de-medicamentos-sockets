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
    /*
    void enviar()
    {

        cout<<"Escribe el mensaje a enviar: "<<endl;
        cin>>this->buffer;
        send(server, buffer, sizeof(buffer), 0);
        cout<<"Mensaje enviado"<<endl;
        memset(buffer, '\0', strlen(buffer)); //resetear el buffer
    }*/
    void conectarServidor()
    {
        //conectarse al servidor
        if( (connect(servidor, (SOCKADDR *)&direccion, sizeof(direccion))) == 0 )
        {
            cout<<"Conectado al servidor!"<<endl;

        }
        else
        {
            cout<<("No se pudo conectar o el servidor esta ocupado")<<endl;
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



    const char *recibir()
    {
        recv(servidor, buffer, sizeof(buffer),0);

        return buffer;
        memset(buffer,'\0', strlen(buffer));

    }

    void cerrarSocket()
    {
        closesocket(servidor);
        WSACleanup();
        cout<<"Socket cerrado"<<endl;
    }

};

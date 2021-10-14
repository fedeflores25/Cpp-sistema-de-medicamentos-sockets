#include <iostream>
#include <winsock2.h>

using namespace std;

class Servidor
{
public:
    //Atributos
    WSADATA wsaData;
    SOCKET server, cliente;//creo sockets server y cliente
    SOCKADDR_IN serverAddr, clienteAddr;
    char buffer[1024];
    Server()
    {
        WSAStartup(MAKEWORD(2,0),&wsaData);
        server = socket(AF_INET,SOCK_STREAM,0);

        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5555);

        bind(server,(SOCKADDR *)&serverAddr,sizeof(serverAddr));//asigna ip y numero de puerto
        listen(server, 0); //asigna el socket como servidor
        cout<<"Escuchando conexiones entrantes..."<<endl;
        int clientAddrSize = sizeof(clienteAddr);

        //aceptar conexion
        if(accept(server,(SOCKADDR *)&clienteAddr,&clientAddrSize) != INVALID_SOCKET)
        {
            cout<<"Cliente conectado"<<endl;
        }
    }//fin constructor del server
    //METODOS
    void recibir()
    {
        recv(cliente, buffer,sizeof(buffer),0);
        cout<<"El cliente dice: "<<buffer<<endl;
        memset(buffer,0,sizeof(buffer));
    }
    void enviar()
    {
        cout<<"Escribe el mensaje a enviar: "<<endl;
        cin>>this->buffer;
        send(server,buffer,sizeof(buffer),0);
        memset(buffer, 0, sizeof(buffer));
    }
    void cerrarSocket()
    {
        closesocket(cliente);
        cout<<"Cliente desconectado, socket cerrado."<<endl;
    }


};//fin clase server

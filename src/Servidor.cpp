#include <iostream>
#include <winsock2.h>

using namespace std;

class Server
{
public:
    //Atributos
    WSADATA wsaData;
    SOCKET server, cliente;
    SOCKADDR_IN serverAddr, clienteAddr;
    char buffer[1024];
    Server()
    {
        WSAStartup(MAKEWORD(2,0),&wsaData);
        server = socket(AF_INET,SOCK_STREAM,0);
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(5555);
        bind(server,(SOCKADDR *)&serverAddr,sizeof(serverAddr));
        listen(server, 0);
        cout<<"Escuchando conexiones entrantes..."<<endl;
        int clientAddrSize = sizeof(clienteAddr);
        cliente = accept(server,(SOCKADDR *)&clienteAddr,&clientAddrSize);
        if(cliente != INVALID_SOCKET)
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
        cout<<"Escribe el mensaje a enviar: ";
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

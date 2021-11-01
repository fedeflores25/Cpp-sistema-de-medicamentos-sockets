#include <iostream>
#include <winsock2.h>
#include <string>
#include <time.h>
#include <fstream>

using namespace std;

class Servidor
{
public:
    //Atributos
    WSADATA wsaData;
    SOCKET servidor, cliente;//creo sockets servidor y cliente
    SOCKADDR_IN servidorDireccion, clienteDireccion; //obtengo direccion para el servidor y el cliente. SOCKADDR significa Socket Address

    char buffer[1024];
    int puerto;

    Servidor(int puerto)
    {
        //CREACION DEL SERVIDOR
        WSAStartup(MAKEWORD(2,0),&wsaData);// Me parece que es para compatibilizar con la lib winsock2.h

        servidor = socket(AF_INET,SOCK_STREAM,0);//estos argumentos que le paso quieren decir que va a usar el protocolo tcp/ip

        servidorDireccion.sin_addr.s_addr = INADDR_ANY;//Le digo al servidor que escuche en cualquiera de las interfaces del sistema operativo
        servidorDireccion.sin_family = AF_INET; //asigna ip
        servidorDireccion.sin_port = htons(puerto);//le asigno el numero de puerto al servidor

        //Ahora tengo que asociar A (direccion ip y el puerto del servidor)
        //con B (el Socket que creamos con el protocolo que voy a usar TCP/IP)


        //cuando se cierra mal el servidor se puede producir un problema que cuelga el ip o el puerto
        //setea un flag que le dice al sist operativo para que le permita reutilizar la conexion
        int activado = 1;
        setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR,(const char *)&activado, sizeof(activado));

        int tiempoLimite = 120000;
        if ( setsockopt(servidor, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tiempoLimite,sizeof(tiempoLimite) ) == SOCKET_ERROR )
        {
            cout << "Error al querer usar setsockpot" << endl;

            closesocket(servidor);
            WSACleanup();
        }


        //la funcion bind pide estructura (servidor, SOCKADDR, tamaño) y como usamos SOCKADDR _IN que es mas especifica la vamos a castear
        //la funcion bind puede fallar(si otro proceso ya esta usando el mismo puerto), por eso hay que validarla
        if (bind(servidor,(SOCKADDR *)&servidorDireccion,sizeof(servidorDireccion))!= 0)//asigna ip y numero de puerto con el socket servidor
        {
            cout<<("Fallo el bind, no se pudo levantar el servidor")<<endl;
        }
        else
        {
            string auxPuerto = to_string(puerto);

            listen(servidor, 0); //asigna el socket como servidor y maximo de conexiones acumulables
            editarServerLog(fechaYHora()+": =============================");
            editarServerLog(fechaYHora()+": =======Inicia Servidor=======");
            editarServerLog(fechaYHora()+": =============================");
            editarServerLog(fechaYHora()+": Socket creado. Puerto de escucha: "+auxPuerto);
        }
    }//fin constructor del servidor

    //METODOS

    void aceptar()
    {
        //ESCUCHAR CONEXIONES Y ACEPTAR*****************************

        cout<<"Escuchando conexiones entrantes..."<<endl;
        int clienteDireccionLongitud = sizeof(clienteDireccion);

        //es importante asignar al cliente la aceptacion de conexion, si no estuviera seria lo mismo que no hacer el accept
        if( (cliente = accept(servidor,(SOCKADDR *)&clienteDireccion,&clienteDireccionLongitud)) != INVALID_SOCKET)
        {
            cout<<"Cliente conectado"<<endl;
            editarServerLog(fechaYHora()+": =============================");
            editarServerLog(fechaYHora()+": =======Cliente conectado=======");
            editarServerLog(fechaYHora()+": =============================");
        }
        else
        {
            cout<<"No se pudo conectar el cliente"<<endl;


        }
    }

    string recibir()
    {
        int bytesRecibidos = recv(cliente, buffer,sizeof(buffer),0);
        //en caso de error agrego este if
        if(bytesRecibidos == SOCKET_ERROR)
        {
            if(WSAETIMEDOUT == WSAGetLastError())
            {
                cout << "La conexion expirado." << endl;
                editarServerLog(fechaYHora()+": =============================");
                editarServerLog(fechaYHora()+": =======La conexion expiro=======");
                editarServerLog(fechaYHora()+": =============================");

            }
            cout<<("El cliente se desconecto")<<endl;
            editarServerLog(fechaYHora()+": =============================");
            editarServerLog(fechaYHora()+": =======El cliente se desconecto=======");
            editarServerLog(fechaYHora()+": =============================");
            cerrarSocket();
        }
        string recibido = buffer;
        memset(buffer,'\0',strlen(buffer));
        return recibido;

    }

    /*void enviar()
    {
        cout<<"Escribe el mensaje a enviar: "<<endl;
        cin>>this->buffer;
         fflush(stdin);
        send(servidor,buffer,sizeof(buffer),0);
        memset(buffer,'\0',strlen(buffer));
    }*/

    void enviar(const char *mensaje)
    {
        strcpy(buffer, mensaje);
        cout<<buffer<<endl;
        send(cliente,buffer,sizeof(buffer),0);
        memset( buffer,'\0',strlen(buffer));
        cout<<"Mensaje enviado"<<endl;
    }

    void cerrarSocket()
    {
        closesocket(cliente);
        WSACleanup();
        cout<<"Socket cerrado."<<endl;
        editarServerLog(fechaYHora()+": =============================");
        editarServerLog(fechaYHora()+": =======Socket cerrado.=======");
        editarServerLog(fechaYHora()+": =============================");
    }

    string fechaYHora()
    {
        time_t ahora = time(0);
        struct tm tstruct;
        char fechaYHoraFinal[80];
        tstruct = *localtime(&ahora);

        strftime(fechaYHoraFinal, sizeof(fechaYHoraFinal), "%Y-%m-%d | %X", &tstruct);

        return fechaYHoraFinal;
    }

    string mostrarServerLog()
    {
        string linea,texto;

        ifstream archivo("server.log.txt");

        //recorre el archivo y guarda cada linea en la variable texto
        while (getline(archivo, linea))
        {
            texto=texto+linea+"\n";
        }
        archivo.close();
        return texto;
    }

    void editarServerLog(string linea)
    {
        string texto = mostrarServerLog();

        ofstream archivo("server.log.txt");
        archivo<<texto<<linea;
    }

    bool esCorrectoUsuarioYClave(string usuario, string clave)
    {
        string linea,texto;
        string datos[2];
        int contador=0;
        size_t posicion;
        bool bandera=false;

        fstream archivo("usuarios.txt");

        //recorre el archivo y guarda cada linea en la variable texto
        while (getline(archivo, linea))
        {

            while( (posicion=linea.find(";")) != string::npos  )
            {
                cout<<"segundo while"<<endl;

                datos[contador]=linea.substr(0,posicion);
                datos[contador+1]=linea.substr(posicion+1,linea.length());

                linea.erase(0,linea.length());
            }
            if(datos[contador]==usuario)
            {
                break;
            }
        }

        if((datos[contador]==usuario) && (datos[contador+1]==clave))
        {
            bandera=true;
        }

        archivo.close();
        return bandera;
    }

    bool login(Servidor *servidor)
    {
        int contador = 0;
        bool bandera = false;

        while(contador<=3)
        {

            //codigo l | es una L en minuscula, con este codigo se puede proceder a realizar el login
            if(  servidor->recibir() == "l")
            {
                servidor->enviar("s");

                //usuario
                string usuario = servidor->recibir();
                cout<<"Usuario: "<<usuario<<endl;
                //contrase;a
                string clave = servidor->recibir();
                cout<<"Clave: "<<clave<<endl;

                if(usuario.length()<=12)
                {
                    if(servidor->esCorrectoUsuarioYClave(usuario,clave) == true)
                    {
                        bandera = true;

                    }
                    else
                    {
                        contador++;
                        //el codigo n | significa que el usuario y la contrase;a ingresados no son correctos
                        servidor->enviar("n");
                    }
                }
                else
                {
                    contador++;
                    //el codigo x | significa que el usuario ingresado tiene mas de 12 caracteres
                    servidor->enviar("x");
                }//fin if caracteres menores a 12

            }// fin if codigo de login l

        }//fin while login


        return bandera;
    }//

    void menuServidor(Servidor *servidor) {



    }

};//fin clase servidor

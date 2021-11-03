#include <iostream>
#include <winsock2.h>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <algorithm>

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

    //DESTRUCTOR
    ~Servidor() {}

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
        cout<<"Socket cliente cerrado."<<endl;
        editarServerLog(fechaYHora()+": =============================");
        editarServerLog(fechaYHora()+": =======Socket cliente cerrado.=======");
        editarServerLog(fechaYHora()+": =============================");
    }

    void cerrarSocketServidor()
    {
        closesocket(servidor);
        WSACleanup();
        cout<<"Socket servidor cerrado."<<endl;
        editarServerLog(fechaYHora()+": =============================");
        editarServerLog(fechaYHora()+": =======Socket servidor cerrado.=======");
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

    string convertirAMayusculas(string cadena)
    {
        transform(cadena.begin(), cadena.end(), cadena.begin(), [](unsigned char cadena)
        {
            return toupper(cadena);
        });
        return cadena;
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

    void menuServidor(Servidor *servidor)
    {
        int varSubMenu;
        string submenu = servidor->recibir();
        //convierto la variable a string
        stringstream geek(submenu);
        //transfiero el valor de geek a una variable de tipo int
        geek >> varSubMenu;

        switch(varSubMenu)
        {
        case 1:// CREAR TIPO DE MEDICAMENTO
        {
            string denominacion = servidor->recibir();

            denominacion = convertirAMayusculas(denominacion);


            if(existeDenominacion(denominacion) == false)
            {

                cout<<"El cliente ingreso una denominacion que no existe"<<endl;
                cout<<"Se crea el tipo de medicamento"<<endl;

                // ****> CREANDOTIPOMEDICAMENTO() ************************
                //guardar en binario

                //el codigo p, le confirma al cliente que el tipo de medicamento fue creado
                servidor->enviar("p");
                servidor->enviar("id");

            }
            else if(existeDenominacion(denominacion) == true)
            {
                cout<<"Error: El cliente ingreso una denominacion que ya existe"<<endl;
                //codigo z indica al cliente que la denominacion esta repetida

                servidor->enviar("z");
                string cadena1 = "El tipo de medicamento "+denominacion+" ya existe, por favor ingrese otra denominación";
                char mensaj[100];
                strcpy(mensaj,cadena1.c_str());
                servidor->enviar(mensaj);
            }
            else
            {
                cout<<"Error: resultado inesperado en la creacion de tipo medicamento"<<endl;
                servidor->enviar("z");
                servidor->enviar("opcion no valida en creacion tipo de medicamento");
            }


        };
        break;
        case 2:// ADMINISTRAR TIPO DE MEDICAMENTO
        {
            string id = servidor->recibir();
            string denominacion = servidor->recibir();
            string estado = servidor->recibir();

            if(encontrado(id,denominacion,estado) == true) //filtro para traer tipos de medicamentos
            {
                servidor->enviar("y");
                //le envia los datos encontrados con esos criterios de busqueda

                char mensa[1024];
                strcpy(mensa,traerDatos(id,denominacion,estado).c_str());
                servidor->enviar(mensa);

                //recibe la id y comprueba que exista
                if(existeiD(servidor->recibir()) == false)
                {
                    string cadena = "El tipo de medicamento que quiere modificar no se encuentra en esta busqueda";
                    char mensaj[100];
                    strcpy(mensaj,cadena.c_str());
                    servidor->enviar(mensaj);

                }
                else if(existeiD(servidor->recibir()) == true)//caso que exista
                {
                    //el codigo j, le confirma al cliente que el id se encontro
                    servidor->enviar("j");

                    denominacion = servidor->recibir();
                    estado = servidor->recibir();

                    if(existeDenominacion(denominacion) == false)
                    {

                        cout<<"El cliente ingreso una denominacion que no existe"<<endl;
                        cout<<"Se modifica el tipo de medicamento"<<endl;

                        // ****> CREANDOTIPOMEDICAMENTO() ************************
                        //guardar en binario

                        //envia esto para confirmar la creacion
                        servidor->enviar("tipo de medicamento creado (datos del medicamento)");

                    }
                    else if(existeDenominacion(denominacion) == true)
                    {
                        cout<<"Error: El cliente ingreso una denominacion que ya existe"<<endl;
                        //codigo z indica al cliente que la denominacion esta repetida

                        servidor->enviar("v");
                        char mensaj[100];
                        strcpy(mensaj,("El tipo de medicamento "+denominacion+" ya existe, por favor ingrese otra denominación").c_str());
                        servidor->enviar(mensaj);
                    }
                    else
                    {
                        cout<<"Error: resultado inesperado en la modificacion de tipo medicamento"<<endl;
                        servidor->enviar("v");
                        servidor->enviar("opcion no valida en modificacion tipo de medicamento");
                    }

                }
                else//caso inesperado
                {

                    cout<<"Error: resultado inesperado en la creacion de tipo medicamento"<<endl;
                    servidor->enviar("p");
                    servidor->enviar("opcion no valida en seleccion del id de tipo de medicamento");

                }

            }
            else if(encontrado(id,denominacion,estado) == false) // sino se encuentra devuelve el error
            {
                servidor->enviar("v");

            }
            else//comprobar lo inesperado
            {

                cout<<"Error: resultado inesperado en la busqueda de tipo medicamento"<<endl;
                servidor->enviar("u");
                servidor->enviar("opcion no valida en la busqueda tipo de medicamento");
            }

        };
        break;//CREAR MEDICAMENTO
        case 3:
        {
            string codigoProducto = servidor->recibir();
            string nombreComercial = servidor->recibir();
            nombreComercial = convertirAMayusculas(nombreComercial);
            string nombreDescripcionDroga = servidor->recibir();
            string tipoMedicamento = servidor->recibir();



            if( (esValido(nombreComercial, tipoMedicamento) == true) && (esCodigoProducto(codigoProducto) == true) )
            {

                //ARCHIVO BINARIO CREAR
                servidor->enviar("b");
                servidor->enviar("id del medicamento creado");
            }
            else if( esValido(nombreComercial, tipoMedicamento) == false )
            {
                servidor->enviar("w");
                string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente";
                char mensaje1[100];
                strcpy(mensaje1,cadena.c_str());
                servidor->enviar(mensaje1);
            }
            else if(esCodigoProducto(codigoProducto) == false)
            {
                servidor->enviar("w");
                string cadena = "El codigo de producto "+codigoProducto+" es incorrecto";
                char mensaje1[100];
                strcpy(mensaje1,cadena.c_str());
                servidor->enviar(mensaje1);


            }
            else if((esValido(nombreComercial, tipoMedicamento) == false) && (esCodigoProducto(codigoProducto) == false) && (existeTipoMedicamento(tipoMedicamento)==true))
            {
                servidor->enviar("w");
                string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente \n y el codigo de producto "+codigoProducto+" es incorrecto";
                char mensaje1[100];
                strcpy(mensaje1,cadena.c_str());
                servidor->enviar(mensaje1);


            }
            else if(existeTipoMedicamento(tipoMedicamento)==false)
            {
                servidor->enviar("w");
                string cadena = "El tipo de medicamento ingresado: "+tipoMedicamento+" no existe, por favor ingrese un tipo existente";
                char mensaje1[100];
                strcpy(mensaje1,cadena.c_str());
                servidor->enviar(mensaje1);
            }
            else
            {
                servidor->enviar("w");
                cout<<"ERROR: ocurrio una combinacion inesperada en las validaciones";
                servidor->enviar("ERROR: ocurrio una combinacion inesperada en las validaciones");
            }
            //validar que se cumpla
        };
        break;
        case 4://ADMINISTRAR MEDICAMENTO
        {
            string nombreComercial = servidor->recibir();
            string tipoMedicamento = servidor->recibir();

            if(encontrado(nombreComercial,tipoMedicamento) == true)//filtro para traer medicamentos
            {
                // ARCHIVO BINARIO TRAER DATOS CON ESOS FILTROS Y ENVIARLOS CON EL METODO DE ABAJO
                servidor->enviar("datos");

                if ( existeIdMedicamento(servidor->recibir()) == true )
                {
                    servidor->enviar("q");

                    if(servidor->recibir() == "ab")
                    {
                        servidor->enviar("Se elimino correctamente el registro");
                    }
                    else if(servidor->recibir() == "aa")
                    {
                        bool bandera5=true;
                        while(bandera5)
                        {
                            string codigoProducto;
                            string droga;
                            //modificar
                            nombreComercial = servidor->recibir();
                            codigoProducto = servidor->recibir();
                            droga = servidor->recibir();
                            tipoMedicamento = servidor->recibir();

                            if( (esValido(nombreComercial, tipoMedicamento) == true) && (esCodigoProducto(codigoProducto) == true) )
                            {
                                servidor->enviar("ac");

                                servidor->enviar("datos del medicamento creado");


                            }
                            else if( esValido(nombreComercial, tipoMedicamento) == false )
                            {
                                servidor->enviar("ad");
                                string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente";
                                char mensaje1[100];
                                strcpy(mensaje1,cadena.c_str());
                                servidor->enviar(mensaje1);
                            }
                            else if(esCodigoProducto(codigoProducto) == false)
                            {
                                servidor->enviar("ad");
                                string cadena = "El codigo de producto "+codigoProducto+" es incorrecto";
                                char mensaje1[100];
                                strcpy(mensaje1,cadena.c_str());
                                servidor->enviar(mensaje1);

                            }
                            else if((esValido(nombreComercial, tipoMedicamento) == false) && (esCodigoProducto(codigoProducto) == false) && (existeTipoMedicamento(tipoMedicamento)==true))
                            {
                                servidor->enviar("ad");
                                string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente \n y el codigo de producto "+codigoProducto+" es incorrecto";
                                char mensaje1[100];
                                strcpy(mensaje1,cadena.c_str());
                                servidor->enviar(mensaje1);
                            }
                            else if(existeTipoMedicamento(tipoMedicamento)==false)
                            {
                                servidor->enviar("ad");
                                string cadena = "El tipo de medicamento ingresado: "+tipoMedicamento+" no existe, por favor ingrese un tipo existente";
                                char mensaje1[100];
                                strcpy(mensaje1,cadena.c_str());
                                servidor->enviar(mensaje1);
                            }
                            else
                            {
                                servidor->enviar("ad");
                                cout<<"ERROR: ocurrio una combinacion inesperada en las validaciones";
                                servidor->enviar("ERROR: ocurrio una combinacion inesperada en las validaciones");
                            }


                        }//fin while



                    }
                    else
                    {
                        cout<<"Error: combinacion de teclas inesperadas"<<endl;
                    }


                }
                else if( existeIdMedicamento(servidor->recibir()) == false )
                {
                    servidor->enviar("r");
                    servidor->enviar("No ingresaste un id valido");
                }
                else
                {
                    servidor->enviar("r");
                    servidor->enviar("ERROR: opcion inesperada");
                }


            }
            else if(encontrado(nombreComercial,tipoMedicamento) == false)
            {
                servidor->enviar("h");
                servidor->enviar("No se encontró ningún resultado para los criterios seleccionados. Presione ENTER para continuar");
            }
            else
            {
                cout<<"Error: resultado inesperado en la busqueda de tipo medicamento"<<endl;
                servidor->enviar("h");
                servidor->enviar("ERROR: opcion no valida en la busqueda tipo de medicamento");
            }

        };
        break;
        case 5://VER REGISTRO
        {

        };
        break;
        case 6://SALIR
        {
            servidor->cerrarSocket();

        };
        break;
        default:
            cout<<"Cliente ingreso opcion no valida, se le permite volver a intentarlo"<<endl;

        }//fin switch
    }// fin metodo menuServidor


    //METODOS INCOMPLETOS TIPO MEDICAMENTO
    bool existeDenominacion(string denominacion)
    {
        //busca en el archivo binario que haya un tip de medicamento con esa denominacion
        return false;
    }//fin metodo existeDenominacion

    bool existeiD(string id)
    {
        //busca en el archivo binario que haya un tip de medicamento con esa denominacion
        return false;
    }//fin metodo existeDenominacion

    string traerDatos(string id,string denominacion,string estado)
    {
        //metodo binario para traer datos por estos filtros
        return "sssssss";
    }

    bool encontrado(string id,string denominacion,string estado)
    {
        //// ****> BUSCANDOTIPOMEDICAMENTO(string id,string denominacion,string estado) ************************
        //guardar en binario
        return true;
    }


    //METODOS INCOMPLETOS MEDICAMENTO
    bool esValido(string nombreComercial, string tipoMedicamento)
    {
        return true;
    }

    bool esCodigoProducto(string codigoProducto)
    {
        return true;
    }

    bool existeTipoMedicamento(string tipoMedicamento)
    {
        return true;
    }

    bool encontrado(string nombreComercial,string tipoMedicamento)
    {

        return true;
    }

    bool existeIdMedicamento(string id)
    {
        //busca en el archivo binario que haya un tip de medicamento con esa denominacion
        return false;
    }//fin metodo existeDenominacion
};//fin clase servidor

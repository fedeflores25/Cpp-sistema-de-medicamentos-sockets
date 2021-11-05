#include <iostream>
#include <winsock2.h>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <iterator>

using namespace std;

struct TipoMedicamento
{
    int id;
    char denominacion[20];
    bool estado;
};

struct Medicamento
{
    int id;
    char codigoProducto[50];
    char nombreComercial[50];
    char nombreDescripcionDroga[50];
    int idTipoMedicamento;
};


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

    void editarServerLogUsuario(string linea, string usuario)
    {
        string texto = mostrarServerLog();

        ofstream archivo(usuario+".log.txt");
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

    bool login(Servidor *servidor, string *user)
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
                user = &usuario;
                cout<<"Usuario: "<<user<<endl;
                //contrase;a
                string clave = servidor->recibir();
                cout<<"Clave: "<<clave<<endl;

                if(usuario.length()<=12)
                {
                    if(servidor->esCorrectoUsuarioYClave(usuario,clave) == true)
                    {
                        bandera = true;
                        editarServerLogUsuario((fechaYHora()+": ============================="), usuario);
                        editarServerLogUsuario((fechaYHora()+": =======Cliente conectado======="), usuario);
                        editarServerLogUsuario((fechaYHora()+": ============================="), usuario);
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

    void menuServidor(Servidor *servidor, string *usuario)
    {
        int varSubMenu;

        bool bandera = true;

        while(bandera)
        {
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

                if(servidor->existeDenominacion(denominacion) == false)
                {

                    cout<<"El cliente ingreso una denominacion que no existe"<<endl;
                    cout<<"Se crea el tipo de medicamento"<<endl;

                    // ****> CREANDOTIPOMEDICAMENTO() ***********************

                    int id = servidor->agregarArchivoTipoMedicamento(denominacion);
                    string aux = ""+id;
                    servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);
                    servidor->editarServerLogUsuario((fechaYHora()+": =======ID:"+aux+"- Alta Tipo Medicamento======="), *usuario);
                    servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);


                    //el codigo p, le confirma al cliente que el tipo de medicamento fue creado
                    servidor->enviar("p");
                    servidor->enviar("id");

                }
                else if(servidor->existeDenominacion(denominacion) == true)
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

                string denominacion = servidor->recibir();
                string estado = servidor->recibir();

                if(servidor->encontradoTipoMedicamento(denominacion,estado) == true) //filtro para traer tipos de medicamentos
                {
                    servidor->enviar("y");
                    //le envia los datos encontrados con esos criterios de busqueda

                    char mensa[1024];
                    strcpy(mensa,servidor->traerDatosTipoMedicamento(denominacion,estado).c_str());
                    servidor->enviar(mensa);

                    //recibe la id y comprueba que exista
                    int idEntero=0;
                    string idString=servidor->recibir();

                    stringstream geek(idString);
                    // lo convierto a int

                    geek >> idEntero;


                    if(servidor->existeiD(idEntero) == false)
                    {
                        string cadena = "El tipo de medicamento que quiere modificar no se encuentra en esta busqueda";
                        char mensaj[100];
                        strcpy(mensaj,cadena.c_str());
                        servidor->enviar(mensaj);

                    }
                    else if(servidor->existeiD(idEntero) == true)//caso que exista
                    {
                        //el codigo j, le confirma al cliente que el id se encontro
                        servidor->enviar("j");

                        //recibo denominacion y estado del cliente
                        denominacion = servidor->recibir();
                        estado = servidor->recibir();

                        //compruebo que no exista la denominacion
                        if(servidor->existeDenominacion(denominacion) == false)
                        {
                            bool estadoB=false;
                            //en el caso de no existir tengo que considerar las opciones con posible @
                            TipoMedicamento tipo =  (servidor->buscarArchivoTipoMedicamentoId(idEntero));

                            if(denominacion=="@")
                            {
                                denominacion = tipo.denominacion;

                            }
                            else if(estado=="@")
                            {
                                estado = tipo.estado;

                            }

                            if(estado=="s")
                            {
                                estadoB=true;
                            }

                            cout<<"El cliente ingreso una denominacion que no existe"<<endl;
                            cout<<"Se modifica el tipo de medicamento"<<endl;

                            // ****> CREANDOTIPOMEDICAMENTO() ************************
                            //guardar en binario
                            servidor->modificarTipoMedicamento(idEntero, denominacion, estadoB);
                            string aux = ""+idEntero;
                            servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);
                            servidor->editarServerLogUsuario((fechaYHora()+": =======ID:"+aux+"- Modificacion Tipo Medicamento======="), *usuario);
                            servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);


                            //envia esto para confirmar la creacion
                            servidor->enviar("tipo de medicamento creado (datos del medicamento)");

                        }
                        else if(servidor->existeDenominacion(denominacion) == true)
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
                else if(servidor->encontradoTipoMedicamento(denominacion,estado) == false) // sino se encuentra devuelve el error
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


                int idTipoMedicamento;

                //convierto la variable a string
                stringstream geek(tipoMedicamento);
                //transfiero el valor de geek a una variable de tipo int
                geek >> idTipoMedicamento;

                if( (servidor->esValido(nombreComercial, tipoMedicamento) == true) && (servidor->esCodigoProducto(codigoProducto) == true) && (servidor->existeTipoMedicamento(tipoMedicamento)==true) )
                {

                    int id = servidor->agregarArchivoMedicamento(codigoProducto,nombreComercial, nombreDescripcionDroga, idTipoMedicamento );
                    string aux = ""+id;
                    servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);
                    servidor->editarServerLogUsuario((fechaYHora()+": =======ID:"+aux+"- Creacion Medicamento======="), *usuario);
                    servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);

                    servidor->enviar("b");
                    servidor->enviar("id del medicamento creado");
                }
                else if(servidor->esValido(nombreComercial, tipoMedicamento) == false )
                {
                    servidor->enviar("w");
                    string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente";
                    char mensaje1[100];
                    strcpy(mensaje1,cadena.c_str());
                    servidor->enviar(mensaje1);
                }
                else if(servidor->esCodigoProducto(codigoProducto) == false)
                {
                    servidor->enviar("w");
                    string cadena = "El codigo de producto "+codigoProducto+" es incorrecto";
                    char mensaje1[100];
                    strcpy(mensaje1,cadena.c_str());
                    servidor->enviar(mensaje1);


                }
                else if( (servidor->esValido(nombreComercial, tipoMedicamento) == false) && (servidor->esCodigoProducto(codigoProducto) == false) && (servidor->existeTipoMedicamento(tipoMedicamento)==true))
                {
                    servidor->enviar("w");
                    string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente \n y el codigo de producto "+codigoProducto+" es incorrecto";
                    char mensaje1[100];
                    strcpy(mensaje1,cadena.c_str());
                    servidor->enviar(mensaje1);


                }
                else if( servidor->existeTipoMedicamento(tipoMedicamento) == false)
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

                if(servidor->encontradoMedicamento(nombreComercial,tipoMedicamento) == true)//filtro para traer medicamentos
                {
                    // ARCHIVO BINARIO TRAER DATOS CON ESOS FILTROS Y ENVIARLOS CON EL METODO DE ABAJO
                    char mensa[1024];
                    strcpy(mensa,servidor->traerDatosMedicamento(nombreComercial,tipoMedicamento).c_str());
                    servidor->enviar(mensa);


                    int idMedicamento = 0;
                    string idString = servidor->recibir();;
                    //convierto la variable a string
                    stringstream geek(idString);
                    //transfiero el valor de geek a una variable de tipo int
                    geek >> idMedicamento;

                    if ( servidor->existeIdMedicamento(idMedicamento) == true )
                    {
                        servidor->enviar("q");

                        if(servidor->recibir() == "ab")//ELIMINAR
                        {
                            string aux = ""+idMedicamento;
                            servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);
                            servidor->editarServerLogUsuario((fechaYHora()+": =======ID:"+aux+"- Eliminacion Medicamento======="), *usuario);
                            servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);
                            Medicamento medicamento =  servidor->buscarArchivoMedicamentoId(idMedicamento);



                            string one = ""+medicamento.id;
                            string two = medicamento.codigoProducto;
                            string thr = medicamento.nombreComercial;
                            string four = medicamento.nombreDescripcionDroga;
                            string five = ""+medicamento.idTipoMedicamento;


                            string auxiliar = one+" "+two+" "+thr+" "+four+" "+five;
                            char eliminado[1024];
                            strcpy(eliminado, auxiliar.c_str());
                            servidor->enviar(eliminado);
                            servidor->eliminarMedicamento(idMedicamento);



                        }
                        else if(servidor->recibir() == "aa") //MODIFICAR
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

                                int idTipoMedicamento = 0;

                                //convierto la variable a string
                                stringstream geek(tipoMedicamento);
                                //transfiero el valor d
                                geek >> idTipoMedicamento;

                                //combinacion correcta
                                if( (servidor->esValido(nombreComercial, tipoMedicamento) == true) && (servidor->esCodigoProducto(codigoProducto) == true) )
                                {

                                    Medicamento medicamento = servidor->buscarArchivoMedicamentoId(idMedicamento);

                                    if(nombreComercial=="@")
                                    {
                                        nombreComercial = medicamento.nombreComercial;

                                    }
                                    else if(codigoProducto=="@")
                                    {
                                        codigoProducto = medicamento.codigoProducto;

                                    }else if(droga=="@")
                                    {
                                        droga = medicamento.nombreDescripcionDroga;

                                    }else if(tipoMedicamento=="@")
                                    {
                                        idTipoMedicamento = medicamento.idTipoMedicamento;
                                    }

                                    servidor->modificarMedicamento(idMedicamento,codigoProducto, nombreComercial, droga,idTipoMedicamento );
                                    servidor->enviar("ac");

                                    string one = ""+medicamento.id;
                                    string two = medicamento.codigoProducto;
                                    string thr = medicamento.nombreComercial;
                                    string four = medicamento.nombreDescripcionDroga;
                                    string five = ""+medicamento.idTipoMedicamento;


                                    string auxiliar = one+" "+two+" "+thr+" "+four+" "+five;
                                    char modificado[1024];
                                    strcpy(modificado, auxiliar.c_str());
                                    servidor->enviar(modificado);

                                     string aux = ""+idMedicamento;
                                    servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);
                                    servidor->editarServerLogUsuario((fechaYHora()+": =======ID:"+aux+"- Modificado Medicamento======="), *usuario);
                                    servidor->editarServerLogUsuario((fechaYHora()+": ============================="), *usuario);



                                }
                                else if( servidor->esValido(nombreComercial, tipoMedicamento) == false )
                                {

                                    servidor->enviar("ad");
                                    string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente";
                                    char mensaje1[100];
                                    strcpy(mensaje1,cadena.c_str());
                                    servidor->enviar(mensaje1);
                                }
                                else if(servidor->esCodigoProducto(codigoProducto) == false)
                                {
                                    servidor->enviar("ad");
                                    string cadena = "El codigo de producto "+codigoProducto+" es incorrecto";
                                    char mensaje1[100];
                                    strcpy(mensaje1,cadena.c_str());
                                    servidor->enviar(mensaje1);

                                }
                                else if((servidor->esValido(nombreComercial, tipoMedicamento) == false) && (servidor->esCodigoProducto(codigoProducto) == false) && (servidor->existeTipoMedicamento(tipoMedicamento)==true))
                                {
                                    servidor->enviar("ad");
                                    string cadena = "El medicamento "+nombreComercial+"-"+tipoMedicamento+" ya existe, por favor ingrese un Nombre Comercial y/o Tipo de medicamento diferente \n y el codigo de producto "+codigoProducto+" es incorrecto";
                                    char mensaje1[100];
                                    strcpy(mensaje1,cadena.c_str());
                                    servidor->enviar(mensaje1);
                                }
                                else if(servidor->existeTipoMedicamento(tipoMedicamento)==false)
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
                    else if( servidor->existeIdMedicamento(idMedicamento) == false )
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
                else if(servidor->encontradoMedicamento(nombreComercial,tipoMedicamento) == false)
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
                bandera = false;
            };
            break;
            default:
                cout<<"Cliente ingreso opcion no valida, se le permite volver a intentarlo"<<endl;
            }//fin switch
        }//fin while

    }// fin metodo menuServidor


    //METODOS INCOMPLETOS TIPO MEDICAMENTO
    bool existeDenominacion(string denominacion)
    {
        return comprobarArchivoTipoMedicamentoDenominacion(denominacion);
    }//fin metodo existeDenominacion

    bool existeiD(int id)
    {
       bool bandera = false;
        if(buscarArchivoTipoMedicamentoId(id).id == 0){
            bandera = true;
        }

        return bandera;
    }//fin metodo existeDenominacion

    string traerDatosTipoMedicamento(string denominacion,string estado)
    {
        bool est= false;
        if(estado=="s") est=true;

        //capturo el puntero
        struct TipoMedicamento *tipos = buscarArchivoTipoMedicamento(denominacion,est);

        int cantidad = cantidadTipoMedicamento();

        struct TipoMedicamento tipo;
        string mensaje;
        string auxDeno;
        string auxEstado;
        string auxId;
            for(int i=0; i<cantidad; i++)
            {
                //capturo cada posicion del puntero y lo convierto al tipo correcto
                tipo = *((tipos)+i);
                auxDeno = tipo.denominacion;
                auxId= "" +tipo.id;

                if(tipo.estado=true)
                {
                  auxEstado = "activo";
                }
                else
                {
                    auxEstado = "Baja";
                }

                mensaje = mensaje +"/n"+auxId+" "+auxDeno+" "+auxEstado;



            }

        return "mensaje";
    }
    string traerDatosTipoMedicamento()
    {
        bool est= false;


        //capturo el puntero
        struct TipoMedicamento *tipos = traerArchivoTipoMedicamento();

        int cantidad = cantidadTipoMedicamento();

        struct TipoMedicamento tipo;
        string mensaje;
        string auxDeno;
        string auxEstado;
        string auxId;
            for(int i=0; i<cantidad; i++)
            {
                //capturo cada posicion del puntero y lo convierto al tipo correcto
                tipo = *((tipos)+i);
                auxDeno = tipo.denominacion;
                auxId= "" +tipo.id;

                if(tipo.estado=true)
                {
                  auxEstado = "activo";
                }
                else
                {
                    auxEstado = "Baja";
                }

                mensaje = mensaje +"/n"+auxId+" "+auxDeno+" "+auxEstado;
                }

                return mensaje;
    }

    bool encontradoTipoMedicamento(string denominacion,string estado)
    {
        //// ****> BUSCANDOTIPOMEDICAMENTO(string denominacion,string estado) ************************
        //guardar en binario

        bool bandera = false;
        if(estado == "s") bandera =true;


        return comprobarArchivoTipoMedicamento(denominacion,bandera);
    }


    //METODOS INCOMPLETOS MEDICAMENTO
    bool esValido(string nombreComercial, string tipoMedicamento)
    {

        return comprobarArchivoMedicamento(nombreComercial,tipoMedicamento);
    }

    string traerDatosMedicamento(string nombreComercial,string tipoMedicamento)
    {

                //capturo el puntero
        struct Medicamento *medicamentos = buscarArchivoMedicamento(nombreComercial,tipoMedicamento);

        int cantidad = cantidadMedicamento();

        struct Medicamento medicamento;
        string mensaje;
        string idString;
        string auxNombre;
        string auxCod;
        string auxDroga;
        string auxIdTipo;
            for(int i=0; i<cantidad; i++)
            {
                //capturo cada posicion del puntero y lo convierto al tipo correcto
                medicamento = *((medicamentos)+i);
                auxNombre = medicamento.nombreComercial;
                auxCod = medicamento.codigoProducto;
                idString= "" +medicamento.id;
                auxDroga = medicamento.nombreDescripcionDroga;
                auxIdTipo = ""+medicamento.idTipoMedicamento;


                mensaje = mensaje +"/n"+idString+" "+auxNombre+" "+auxCod+" "+auxDroga+" "+ auxIdTipo;


            }

        return "mensaje";
    }
    bool esCodigoProducto(string codigoProducto)
    {
        bool bandera =false;
        int suma=0;
        int verificador=0;
        int one,two,thr,four,five;
        string n1,n2,n3,n4,n5,n6;
        size_t p1=4,p2=5,p3=6,p4=7,p5=9,p6=11;



        n1=codigoProducto.substr(3, p1);
        stringstream geek1(n1);
        geek1>>one;

        n2=codigoProducto.substr(4, p2);
        stringstream geek2(n2);
        geek2>>two;

        n3=codigoProducto.substr(5, p3);
        stringstream geek3(n3);
        geek3>>thr;

        n4=codigoProducto.substr(6, p4);
        stringstream geek4(n4);
        geek4>>four;

        n5=codigoProducto.substr(8, p5);
        stringstream geek5(n5);
        geek5>>five;

        suma = one+two+thr+four+five;

        n6=codigoProducto.substr(10, p6);
        stringstream geek6(n6);
        geek6>>verificador;

        if(suma == verificador) bandera =true;



        return bandera;
    }

    bool existeTipoMedicamento(string tipoMedicamento)
    {
                int idTipoMedicamento;
                bool bandera = true;
                //convierto la variable a string
                stringstream geek(tipoMedicamento);
                //transfiero el valor de geek a una variable de tipo int
                geek >> idTipoMedicamento;
                TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(idTipoMedicamento);
                if( tipo.id == 0 ) bandera =false;

        return bandera;
    }

    bool encontradoMedicamento(string nombreComercial,string tipoMedicamento)
    {

        return comprobarArchivoMedicamento(nombreComercial, tipoMedicamento);
    }

    bool existeIdMedicamento(int id)
    {
       bool bandera = true;
        //busca en el archivo binario que haya un tip de medicamento con esa denominacion
        Medicamento med = buscarArchivoMedicamentoId(id);

        if(med.id==0) bandera=false;

        return bandera;
    }//fin metodo existeDenominacion



//Funciones
//TIPO MEDICAMENTO
    int escribirTipoMedicamento(int id, string denominacion)
    {

        //convierto el string a char
        char denomin[20];
        strcpy(denomin,denominacion.c_str());


        ofstream archivoBinario;
        archivoBinario.open("TipoMedicamento.dat", ios::out | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
        }

        TipoMedicamento tipo;

        tipo.id = id;
        strcpy(tipo.denominacion, denomin);
        //tipo.denominacion = denomin;
        tipo.estado = true;

        archivoBinario.write((char *)&tipo, sizeof(TipoMedicamento));

        archivoBinario.close();

        return tipo.id;

    }//fin funcion  escribir tipo medicamento

    int cantidadTipoMedicamento()
    {
        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        TipoMedicamento tipo;
        int contador = 0;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipo, sizeof(TipoMedicamento));

            if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
            {
                contador++;
            }



        }//fin while

        archivoBinario.close();

        return contador;
    }

    int agregarArchivoTipoMedicamento(string denominacion)
    {
        //convierto el string a char
        char denomin[20];
        strcpy(denomin,denominacion.c_str());

        ofstream archivoBinario;
        archivoBinario.open("TipoMedicamento.dat", ios::app | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
        }

        TipoMedicamento tipo;

        tipo.id = cantidadTipoMedicamento()+1;
        strcpy(tipo.denominacion, denomin);
        //tipo.denominacion = denomin;
        tipo.estado = true;

        archivoBinario.write((char *)&tipo, sizeof(TipoMedicamento));

        archivoBinario.close();

        return tipo.id;

    }

    void leerArchivoTipoMedicamento()
    {
        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        TipoMedicamento tipo;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipo, sizeof(TipoMedicamento));

            if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
            {
                cout<<"ID: "<<tipo.id<<endl;
                cout<<"Denominacion: "<<tipo.denominacion<<endl;
                if(tipo.estado == 1)
                {
                    cout<<"Estado: activado"<<endl<<endl;
                }
                else
                {
                    cout<<"Estado: desactivado"<<endl<<endl;
                }
            }



        }//fin while

        archivoBinario.close();
    }

    struct TipoMedicamento * traerArchivoTipoMedicamento()
    {
        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static TipoMedicamento tipos[20];
        int contador = 0;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

            if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
            {
                contador++;

            }



        }//fin while

        archivoBinario.close();

        return tipos;
    }

    struct TipoMedicamento * buscarArchivoTipoMedicamento(string denominacion, bool estado)
    {

        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static TipoMedicamento tipos[20];
        static TipoMedicamento aux[20];
        int contador = 0;

        string auxDenominacion;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

            auxDenominacion = tipos[contador].denominacion;
            if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)) && (tipos[contador].estado==estado) )//if para que no me repita el ultimo archivo
            {
                aux[contador].id = tipos[contador].id;
                strcpy(aux[contador].denominacion, tipos[contador].denominacion);
                aux[contador].estado = estado;
            }
            contador++;


        }//fin while

        archivoBinario.close();

        return aux;


    }


   bool comprobarArchivoTipoMedicamento(string denominacion, bool estado)
    {
        bool bandera = false;
        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static TipoMedicamento tipos[20];
        static TipoMedicamento aux[20];
        int contador = 0;

        string auxDenominacion;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

            auxDenominacion = tipos[contador].denominacion;
            if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)) && (tipos[contador].estado==estado) )//if para que no me repita el ultimo archivo
            {
                bandera = true;
            }
            contador++;


        }//fin while

        archivoBinario.close();

        return bandera;


    }


    struct TipoMedicamento * buscarArchivoTipoMedicamentoDenominacion(string denominacion)
    {

        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static TipoMedicamento tipos[20];
        static TipoMedicamento aux[20];
        int contador = 0;
        string auxDenominacion;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

            auxDenominacion = tipos[contador].denominacion;
            if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)))//if para que no me repita el ultimo archivo
            {
                aux[contador].id = tipos[contador].id;
                strcpy(aux[contador].denominacion, tipos[contador].denominacion);
                aux[contador].estado = tipos[contador].estado;
            }
            contador++;


        }//fin while

        archivoBinario.close();

        return aux;

    }

    bool comprobarArchivoTipoMedicamentoDenominacion(string denominacion)
    {
        bool bandera = false;
        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static TipoMedicamento tipos[20];

        int contador = 0;
        string auxDenominacion;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

            auxDenominacion = tipos[contador].denominacion;
            if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)))//if para que no me repita el ultimo archivo
            {
                bandera = true;

            }
            contador++;


        }//fin while

        archivoBinario.close();

        return bandera;
    }

    struct TipoMedicamento * buscarArchivoTipoMedicamentoEstado(bool estado)
    {

        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static TipoMedicamento tipos[20];
        static TipoMedicamento aux[20];
        int contador = 0;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));


            if( (!archivoBinario.eof()) && (tipos[contador].estado==estado) )//if para que no me repita el ultimo archivo
            {
                aux[contador].id = tipos[contador].id;
                strcpy(aux[contador].denominacion, tipos[contador].denominacion);
                aux[contador].estado = estado;
            }
            contador++;



        }//fin while

        archivoBinario.close();

        return aux;

    }

    struct TipoMedicamento buscarArchivoTipoMedicamentoId(int id)
    {

        ifstream archivoBinario;

        archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static TipoMedicamento tipos[20];
        static TipoMedicamento aux;
        aux.id=0;
        int contador = 0;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));


            if( (!archivoBinario.eof()) && ( tipos[contador].id==id ) )//if para que no me repita el ultimo archivo
            {

                aux.id = tipos[contador].id;
                strcpy(aux.denominacion, tipos[contador].denominacion);
                aux.estado = tipos[contador].estado;


            }
            contador++;

        }//fin while

        archivoBinario.close();

        return aux;

    }

    void modificarTipoMedicamento(int id, string denominacion, bool estado)
    {
        char denomin[20];
        strcpy(denomin,denominacion.c_str());

        TipoMedicamento tipo;
        tipo.id = id;
        strcpy(tipo.denominacion, denomin);
        tipo.estado = false;

        fstream archivoBinario("TipoMedicamento.dat",ios::out | ios::in | ios::binary);

        if(archivoBinario.is_open())
        {
            archivoBinario.seekp((tipo.id-1)*sizeof(TipoMedicamento));
            archivoBinario.write((char *)&tipo,sizeof(TipoMedicamento));
            archivoBinario.close();
        }

    }
    void eliminarTipoMedicamento(int id)
    {
        fstream archivoBinario("TipoMedicamento.dat", ios::out | ios::in | ios::binary);
        TipoMedicamento aux;

        if (archivoBinario.is_open())
        {
            archivoBinario.seekg((id - 1) * sizeof(TipoMedicamento));
            archivoBinario.write((char *)&aux, sizeof(TipoMedicamento));
            archivoBinario.close();
        }
    }

//MEDICAMENTO
    int escribirMedicamento(int id, string codigoProducto, string nombreComercial, string nombreDescripcionDroga, int idTipoMedicamento )
    {
        char codigoProduct[50];
        char nombreComer[50];
        char descripcionDrog[50];

        //convierto el string a char
        strcpy(codigoProduct,codigoProducto.c_str());
        strcpy(nombreComer,nombreComercial.c_str());
        strcpy(descripcionDrog,nombreDescripcionDroga.c_str());

        //creo archivo binario
        ofstream archivoBinario;
        archivoBinario.open("Medicamento.dat", ios::out | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
        }

        Medicamento medicamento;

        medicamento.id = id;
        strcpy(medicamento.codigoProducto, codigoProduct);
        strcpy(medicamento.nombreComercial, nombreComer);
        strcpy(medicamento.nombreDescripcionDroga, descripcionDrog);


        archivoBinario.write((char *)&medicamento, sizeof(Medicamento));

        archivoBinario.close();

        return medicamento.id;
    }

    int cantidadMedicamento()
    {
        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario medicamento"<<endl;
            exit(1);
        }

        Medicamento medicamento;
        int contador = 0;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamento, sizeof(Medicamento));

            if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
            {
                contador++;
            }



        }//fin while

        archivoBinario.close();

        return contador;
    }

    int agregarArchivoMedicamento(string codigoProducto, string nombreComercial, string nombreDescripcionDroga, int idTipoMedicamento)
    {
        char codigoProduct[50];
        char nombreComer[50];
        char descripcionDrog[50];

        //convierto el string a char
        strcpy(codigoProduct,codigoProducto.c_str());
        strcpy(nombreComer,nombreComercial.c_str());
        strcpy(descripcionDrog,nombreDescripcionDroga.c_str());

        ofstream archivoBinario;
        archivoBinario.open("Medicamento.dat", ios::app | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
        }

        Medicamento medicamento;

        medicamento.id = cantidadMedicamento();
        strcpy(medicamento.codigoProducto, codigoProduct);
        strcpy(medicamento.nombreComercial, nombreComer);
        strcpy(medicamento.nombreDescripcionDroga, descripcionDrog);

        archivoBinario.write((char *)&medicamento, sizeof(Medicamento));

        archivoBinario.close();

        return medicamento.id;

    }

    void leerArchivoMedicamento()
    {
        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        Medicamento medicamento;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamento, sizeof(Medicamento));

            if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
            {
                cout<<"Medicamento"<<endl;
                cout<<"ID: "<<medicamento.id<<endl;
                cout<<"Codigo de producto: "<<medicamento.codigoProducto<<endl;
                cout<<"Nombre comercial: "<<medicamento.nombreComercial<<endl;
                cout<<"Nombre descripcion droga: "<<medicamento.codigoProducto<<endl;
                cout<<"Codigo de producto: "<<medicamento.codigoProducto<<endl;
                TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(medicamento.idTipoMedicamento);
                cout<<"Tipo medicamento: "<<endl;
                cout<<"ID: "<<tipo.id<<endl;
                cout<<"Denominacion: "<<tipo.denominacion<<endl;
                if(tipo.estado == 1)
                {
                    cout<<"Estado: activado"<<endl<<endl;
                }
                else
                {
                    cout<<"Estado: desactivado"<<endl<<endl;
                }
            }



        }//fin while

        archivoBinario.close();
    }

    struct Medicamento * traerArchivoMedicamento()
    {
        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static Medicamento medicamentos[50];
        int contador = 0;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

            if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
            {
                contador++; //es necesario para la carga del vector

            }

        }//fin while

        archivoBinario.close();

        return medicamentos;
    }

    struct Medicamento * buscarArchivoMedicamento(string nombreComercial, string denominacion)
    {
        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static Medicamento medicamentos[20];
        static Medicamento aux[20];
        int contador = 0;


        string auxNombreComercial;
        string auxDenominacion;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

            auxNombreComercial = medicamentos[contador].nombreComercial;
            //traer el tipo
            TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(medicamentos[contador].id);
            auxDenominacion = tipo.denominacion;

            if( (!archivoBinario.eof()) && ( (auxNombreComercial==nombreComercial)) && (auxDenominacion==denominacion) )//if para que no me repita el ultimo archivo
            {
                aux[contador].id = medicamentos[contador].id;
                strcpy(aux[contador].nombreComercial, medicamentos[contador].nombreComercial);
                strcpy(aux[contador].codigoProducto, medicamentos[contador].codigoProducto);
                strcpy(aux[contador].nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
                aux[contador].idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

            }
            contador++;


        }//fin while

        archivoBinario.close();

        return aux;
    }




    bool comprobarArchivoMedicamento(string nombreComercial, string denominacion)
    {
        bool bandera = false;
        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static Medicamento medicamentos[20];
        static Medicamento aux[20];
        int contador = 0;


        string auxNombreComercial;
        string auxDenominacion;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

            auxNombreComercial = medicamentos[contador].nombreComercial;
            //traer el tipo
            TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(medicamentos[contador].id);
            auxDenominacion = tipo.denominacion;

            if( (!archivoBinario.eof()) && ( (auxNombreComercial==nombreComercial)) && (auxDenominacion==denominacion) )//if para que no me repita el ultimo archivo
            {
                bandera = true;

            }
            contador++;


        }//fin while

        archivoBinario.close();

        return bandera;
    }

    struct Medicamento * buscarArchivoMedicamentoNombreComercial(string nombreComercial)
    {
        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static Medicamento medicamentos[20];
        static Medicamento aux[20];
        int contador = 0;


        string auxNombreComercial;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

            auxNombreComercial = medicamentos[contador].nombreComercial;

            if( (!archivoBinario.eof()) && ( (auxNombreComercial==nombreComercial)) )//if para que no me repita el ultimo archivo
            {
                aux[contador].id = medicamentos[contador].id;
                strcpy(aux[contador].nombreComercial, medicamentos[contador].nombreComercial);
                strcpy(aux[contador].codigoProducto, medicamentos[contador].codigoProducto);
                strcpy(aux[contador].nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
                aux[contador].idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

            }
            contador++;


        }//fin while

        archivoBinario.close();

        return aux;



    }

    struct Medicamento * buscarArchivoMedicamentoDenominacion(string denominacion)
    {
        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static Medicamento medicamentos[20];
        static Medicamento aux[20];
        int contador = 0;
        char denomin[50];

        strcpy(denomin,denominacion.c_str());


        string auxDenominacion;


        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

            //traer el tipo
            TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(medicamentos[contador].id);
            auxDenominacion = tipo.denominacion;

            if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)) )//if para que no me repita el ultimo archivo
            {
                aux[contador].id = medicamentos[contador].id;
                strcpy(aux[contador].nombreComercial, medicamentos[contador].nombreComercial);
                strcpy(aux[contador].codigoProducto, medicamentos[contador].codigoProducto);
                strcpy(aux[contador].nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
                aux[contador].idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

            }
            contador++;


        }//fin while

        archivoBinario.close();

        return aux;

    }

    struct Medicamento buscarArchivoMedicamentoId(int id)
    {

        ifstream archivoBinario;

        archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

        if(archivoBinario.fail())//en caso que no se cree el archivo binario
        {
            cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
            exit(1);
        }

        static Medicamento medicamentos[50];
        static Medicamento aux;
        int contador = 0;

        while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
        {
            archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));


            if( (!archivoBinario.eof()) && ( medicamentos[contador].id==id ) )//if para que no me repita el ultimo archivo
            {

                aux.id = medicamentos[contador].id;
                strcpy(aux.nombreComercial, medicamentos[contador].nombreComercial);
                strcpy(aux.codigoProducto, medicamentos[contador].codigoProducto);
                strcpy(aux.nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
                aux.idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

            }
            contador++;

        }//fin while

        archivoBinario.close();

        return aux;

    }

    void modificarMedicamento(int id, string codigoProducto, string nombreComercial, string descripcionDroga, int idTipoMedicamento)
    {

        char codigoProduct[50];
        strcpy(codigoProduct,codigoProducto.c_str());

        char nombreComercia[50];
        strcpy(nombreComercia,nombreComercial.c_str());

        char descripcionDrog[50];
        strcpy(descripcionDrog,descripcionDroga.c_str());


        Medicamento medicamento;
        medicamento.id = id;
        strcpy(medicamento.codigoProducto, codigoProduct);
        strcpy(medicamento.nombreComercial, nombreComercia);
        strcpy(medicamento.nombreDescripcionDroga, descripcionDrog);
        medicamento.idTipoMedicamento = idTipoMedicamento;

        fstream archivoBinario("Medicamento.dat",ios::out | ios::in | ios::binary);

        if(archivoBinario.is_open())
        {
            archivoBinario.seekp((medicamento.id-1)*sizeof(Medicamento));
            archivoBinario.write((char *)&medicamento,sizeof(Medicamento));
            archivoBinario.close();
        }

    }

    void eliminarMedicamento(int id)
    {
        fstream archivoBinario("Medicamento.dat", ios::out | ios::in | ios::binary);
        Medicamento aux;

        if (archivoBinario.is_open())
        {
            archivoBinario.seekg((id - 1) * sizeof(Medicamento));
            archivoBinario.write((char *)&aux, sizeof(Medicamento));
            archivoBinario.close();
        }
    }



};//fin clase servidor

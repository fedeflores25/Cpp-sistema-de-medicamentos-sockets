#include <iostream>
#include <winsock2.h>
#include <fstream>
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

    //CONSTRUCTOR
    Cliente(int puerto, char ip[20])
    {
        WSAStartup(MAKEWORD(2,0), &wsaData);
        servidor = socket(AF_INET, SOCK_STREAM, 0);

        direccion.sin_addr.s_addr = inet_addr(ip);// ile digo que escuche en una ip especifica para conectarme (ip del server)
        direccion.sin_family = AF_INET;//asigna la ip del cliente
        direccion.sin_port = htons(puerto); //puerto al cual conectarme (puerto del server)

    }

    // METODOS*******************************************************

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

    void menuPrincipal(Cliente *cliente)
    {

        string ingresoTeclado=" ";
        int varMenu = 0;
        bool banderaMenu=true;

        // *************** MENU PRINCIPAL ***************
        while(banderaMenu)
        {
            cout<<"Menu Sistema de Administracion de Medicamentos"<<endl;
            cout<<"1 - Tipo de medicamento"<<endl;
            cout<<"2 - Medicamento"<<endl;
            cout<<"3 - Registro"<<endl;
            cout<<"4 - Salir"<<endl;
            cout<<endl<<"Ingrese la opcion que desee: ";

            // El ingreso del teclado es un string para poder atrapar cualquier valor que ingrese y validarlo
            // , de esta forma si ingresan una opcion incorrecta o cualquier caracter no se rompa el programa
            cin>>ingresoTeclado;
            // tomo el valor del string para convertirlo a int
            stringstream geek(ingresoTeclado);
            // lo convierto a int
            geek >> varMenu;


            // switch con opciones
            switch(varMenu)
            {
            case 1:// TIPO DE MEDICAMENTO
            {
                menuTipoMedicamento(cliente);

            };
            break;// fin caso 1 Menu principal********************************
            case 2:// MEDICAMENTO
            {
                menuMedicamento(cliente);

            };
            break;// fin caso 2 Menu principal *********************************
            case 3:// VER REGISTRO DE ACTIVIDADES
            {
                //perdirle al servidor el archivo binario
                //recibir el archivo binario
                //llamar metodo de mostrar registro archivo binario

                cout<<"Registro"<<endl;
            };
            break;// fin caso  3 Menu principal *****************************
            case 4://CERRAR SESION
            {
                cout<<"Desea salir del sistema? 1-si | 2-no"<<endl;
                cin>>ingresoTeclado;
                //convierto la variable a string
                stringstream geek(ingresoTeclado);
                //transfiero el valor de geek a una variable de tipo int
                geek >> varMenu;
                if(varMenu==1)
                {
                    banderaMenu=false;
                    cout<<"Adios";
                }
            };
            break;// fin caso 4**********************************************
            default:
                cout<<"No ingresaste una opcion valida, vuelve a intentarlo!"<<endl;

            }// fin switch
            system("pause");
            system("cls");
        }// fin while

    }

    // MENU TIPO MEDICAMENTO******************************************
    void menutTipoMedicamento(Cliente *cliente)
    {
        system("cls");

        //variables
        bool banderaSubmenu=true;
        string submenu=" ";
        int varSubMenu=0;

        while(banderaSubmenu)
        {

            // el codigo m | Avisa al servidor que vamos a trabajar con el menu tipo medicamento
            cliente->enviar("m");
            cout<<"Sub-Menu Tipo de Medicamento"<<endl;
            cout<<"1 - Crear"<<endl;
            cout<<"2 - Administrar"<<endl;
            cout<<"3 - Volver"<<endl;

            //capturo como cadena y convierto a int para evitar ingresos que no correspondan
            cout<<endl<<"Ingrese la opcion que desee: ";
            cin>>submenu;
            //convierto la variable a string
            stringstream geek(submenu);
            //transfiero el valor de geek a una variable de tipo int
            geek >> varSubMenu;

            switch(varSubMenu)
            {
            case 1://CREAR TIPO DE MEDICAMENTO
            {
                bool bandera=true;
                char opcion;
                char denominacion[50];
                while(bandera)
                {
                    // el codigo o | significa que vamos a crear un tipo de medicamento
                    cliente->enviar("o");
                    cout<<"Crear Tipo de Medicamento"<<endl;
                    cout<<"Por favor ingrese la denominacion: "<<endl;
                    cin>>denominacion;
                    cliente->enviar(denominacion);


                    if(cliente->recibir() == "z")
                    {
                        cout<<"Ya existe el tipo de medicamento"<<endl
                            cout<<"Presione cualquier tecla para continuar"<<endl;
                        system("pause");


                    }
                    else if(cliente->recibir() == "p" )
                    {
                        cout<<"Cliente creado!"<<endl;
                        cout<<"ID: "<<cliente->recibir()<<endl;

                        cout<<"Crear otro medicamento? s | n : ";
                        cin>>opcion;
                        if(opcion == "n")
                        {
                            bandera=false;
                            cout<<"Presione cualquier tecla para continuar"<<endl;
                            system("pause");
                        }
                        else
                        {
                            cout<<"Presione cualquier tecla para continuar"<<endl;
                            system("pause");
                        }//fin if

                    }//fin if que recibio "z" o recibio "p"


                }//fin while
            };
            break;// fin caso 1
            case 2:// ADMINISTRAR TIPO DE MEDICAMENTO
            {
                char denominacion[50], estado[3], id[20];
                bool bandera=true;

                while(bandera)// while 1
                {
                    // el codigo i | le indica al servidor que vamos administrar un tipo de medicamento que se prepare
                    cliente->enviar("i");
                    cout<<"Administrar Medicamento"<<endl;
                    cout<<"Por favor ingrese los criterios de busqueda"<<endl;
                    cout<<"Si no se desea aplicar el filtro escribir '@' "<<endl;

                    cout<<"Id: ";
                    cin>>id;

                    cout<<endl<<"Denominacion: ";
                    cin>>denominacion;

                    cout<<endl<<"Estado(s | n): ";
                    cin>>estado;

                    //le envio al servidor los datos de mi filtro, en denominacion pueden ir letras o cadenas del texto de denominacion
                    cliente->enviar(id);
                    cliente->enviar(denominacion);
                    cliente->enviar(estado);


                    if(cliente->recibir() == "u")//en caso de no encontrarse el criterio de busqueda
                    {
                        cout<<"No se encontró ningún resultado para los criterios seleccionados. Presione ENTER para continuar"<<endl;

                    }
                    else if(cliente->recibir() == "y")// si recibe el codigo y, significa que se encontraron resultados, que se prepare para recibir el texto
                    {
                        bool banderaAuxiliar=true;
                        // estos son los datos encontrados
                        string datos = cliente->recibir();
                        while(banderaAuxiliar)  //while para eleccion de dato a modificar
                        {

                            cout<<datos<<endl;

                            cout<<"Elegir por ID el Tipo de Medicamento a modificar"<<endl;
                            cout<<"Si no se desea modificar el campo escribir '@' "<<endl;

                            cout<<"Ingresar ID: "
                                cin>>id;

                            cliente->enviar(id);
                            if(cliente->recibir()=="p") //if para saber si el id ingresado existe
                            {
                                cout<<endl<<"Ingresar denominacion nueva: ";
                                cin>>denominacion;

                                cout<<endl<<"Ingresar estado nuevo s - alta | n - baja): ";
                                cin>>estado;

                                string confirmacion;
                                cout<<"¿Desea guardar los cambios? s | n : ";
                                cin>>confirmacion;
                                if(confirmacion=="s")
                                {

                                    cliente->enviar(denominacion); //aca tengo que enviar los datos a modificar, me falta agregar algo para que cuando lo envie el servidor entienda que hay que modificar el tipo
                                    cliente->enviar(estado);
                                    //aca recibo la respuesta, si se pudo modificar o no
                                    if(cliente->recibir() == "j")  //if 3
                                    {
                                        cout<<"No se pudo modificar el tipo de medicamento, la denominacion se encuentra repetida, intentelo otra vez"<<endl;
                                        cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                        system("pause");
                                        system("cls");
                                    }
                                    else if(cliente->recibir() == "k")
                                    {
                                        cout<<"Se modifico correctamente el medicamento"<<enld
                                            cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                        system("pause");
                                        system("cls");
                                        banderaAuxiliar=false;
                                    }//fin if 3
                                }//confirmacion de operacion
                            }
                            else if(cliente->recibir()=="v")
                            {
                                cout<<"El id ingresado no se encuentra en la lista"<<endl;
                                cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                system("pause");
                                system("cls");

                            }

                        }//fin while 3

                    }//fin while 2


                    string opcion;
                    cout<<"Administrar otro tipo de medicamento? s | n : ";
                    cin>>opcion;
                    if(opcion == "n")
                    {
                        bandera=false;
                        cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                        system("pause");
                        system("cls");
                    }
                    else
                    {
                        cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                        system("pause");
                        system("cls");
                    }//fin if

                }//fin while 1
            };
            break;// fin caso 2
            case 3:// VOLVER AL ANTERIOR MENU
            {

                cout<<"Desea volver atras? 1-si | 2-no"<<endl;
                cin>>submenu;
                //convierto la variable a string
                stringstream geek(submenu);
                //transfiero el valor de geek a una variable de tipo int
                geek >> varSubMenu;
                if(varSubMenu==1)
                {
                    banderaSubmenu=false;
                    cout<<"Adios"<<endl;
                }//fin if

            };
            break;// fin caso 3
            default:
                cout<<"No ingresaste una opcion valida, vuelve a intentarlo!"<<endl;
            }//fin switch

            cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
            system("pause");
            system("cls");

        }//fin while

    }

    // //MENU  MEDICAMENTO********************************
    void menuMedicamento(Cliente *cliente)
    {
        system("cls");
        //variables para el SUBMENU
        bool banderaSubmenu=true;
        string submenu=" ";
        int varSubMenu=0;

        //el while permite repetir el submenu
        while(banderaSubmenu)
        {
            //menu
            cout<<"Sub-Menu Medicamento"<<endl;
            cout<<"1 - Crear"<<endl;
            cout<<"2 - Administrar"<<endl;
            cout<<"3 - Volver"<<endl;

            //capturo como cadena y convierto a int para evitar ingresos que no correspondan
            cout<<endl<<"Ingrese la opcion que desee: ";
            cin>>submenu;
            //convierto la variable a string
            stringstream geek(submenu);
            //transfiero el valor de geek a una variable de tipo int
            geek >> varSubMenu;

            switch(varSubMenu)
            {
            case 1: // CREAR MEDICAMENTO
            {
                bool bandera=true;
                char opcion;
                char denominacion[50];
                while(bandera)
                {
                    cout<<"Crear Medicamento"<<endl;
                    cout<<"Por favor ingrese la denominacion: "<<endl;
                    cin>>denominacion;
                    //cliente->enviar(denominacion);
                    //cliente->recibir();
                    cout<<"Crear otro medicamento? s | n : ";
                    cin>>opcion;
                    if(opcion == 'n')
                    {
                        bandera=false;
                        system("pause");
                    }
                    else
                    {
                        system("pause");
                    }//fin if


                }//fin while


            };
            break; //fin caso 1
            case 2: // ADMINISTRAR MEDICAMENTO
            {


            };
            break;
            case 3: // volver atras
            {
                cout<<"Desea volver atras? 1-si | 2-no"<<endl;
                cin>>submenu;
                //convierto la variable a string
                stringstream geek(submenu);
                //transfiero el valor de geek a una variable de tipo int
                geek >> varSubMenu;
                if(varSubMenu==1)
                {
                    banderaSubmenu=false;
                    cout<<"Adios"<<endl;
                }//fin if


            };
            break;//fin caso 3
            default:
                cout<<"No ingresaste una opcion valida, vuelve a intentarlo!"<<endl;
                ;
            }//fin switch
            system("pause");
            system("cls");
        }//fin while

    }

};

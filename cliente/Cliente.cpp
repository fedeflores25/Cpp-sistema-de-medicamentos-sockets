#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <sstream>

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

    //DESTRUCTOR
    ~Cliente() {}

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

    void cerrarSocket(Cliente *cliente)
    {
        closesocket(servidor);
        WSACleanup();
        cliente->~Cliente();
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
                menuTipoMedicamento(cliente);

            };
            break;// fin caso 2 Menu principal *********************************
            case 3:// VER REGISTRO DE ACTIVIDADES
            {
                cliente->enviar("5");
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
                    cliente->enviar("6");
                    cliente->cerrarSocket(cliente);
                    banderaMenu=false;
                    cout<<"Adios";
                }
            };
            break;// fin caso 4**********************************************
            default:
                cout<<"No ingresaste una opcion valida, vuelve a intentarlo!"<<endl;
                system("pause");
                system("cls");

            }// fin switch

        }// fin while

    }//fin menu principal

    // MENU TIPO MEDICAMENTO******************************************
    void menuTipoMedicamento(Cliente *cliente)
    {
        system("cls");

        //variables
        bool banderaSubmenu=true;
        string submenu=" ";
        int varSubMenu=0;

        while(banderaSubmenu)
        {

            // el codigo m | Avisa al servidor que vamos a trabajar con el menu tipo medicamento

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
                string opcion;
                char denominacion[50];
                while(bandera)
                {
                    // el codigo 1 | significa que vamos a crear un tipo de medicamento
                    cliente->enviar("1");
                    cout<<"Crear Tipo de Medicamento"<<endl;
                    cout<<"Por favor ingrese la denominacion: "<<endl;
                    cin>>denominacion;
                    cliente->enviar(denominacion);


                    if(cliente->recibir() == "z")// NO PASO LA VALIDACION
                    {
                        cout<<cliente->recibir()<<endl;
                        cout<<"Presione cualquier tecla para continuar"<<endl;
                        system("pause");


                    }
                    else if(cliente->recibir() == "p" ) //TIPO MEDICAMENTO CREADO
                    {
                        cout<<"Tipo de medicamento creado!"<<endl;
                        cout<<"ID: "<<cliente->recibir()<<endl;

                        cout<<"Crear otro medicamento? s | n : ";
                        cin>>opcion;
                        if(opcion == "n")
                        {
                            bandera=false;
                            cout<<"Presione cualquier tecla para continuar"<<endl;
                            system("pause");
                            system("cls");
                        }
                        else
                        {
                            cout<<"Presione cualquier tecla para continuar"<<endl;
                            system("pause");
                            system("cls");
                        }//fin if

                    }//fin if que recibio "z" o recibio "p"
                    else
                    {
                        cout<<"ERROR: respuesta inesperada del servidor"<<endl;
                        cout<<"Presione cualquier tecla para continuar"<<endl;
                        system("pause");
                        system("cls");
                    }//fin if principal


                }//fin while
            };
            break;// fin caso 1
            case 2:// ADMINISTRAR TIPO DE MEDICAMENTO
            {
                char denominacion[50], estado[3], id[20];
                bool bandera=true;

                while(bandera)// while principal
                {
                    // el codigo 2 | le indica al servidor que vamos administrar un tipo de medicamento que se prepare
                    cliente->enviar("2");
                    cout<<"Administrar tipo de medicamento"<<endl;
                    cout<<"Por favor ingrese los criterios de busqueda"<<endl;
                    cout<<"Si no se desea aplicar el filtro escribir '@' "<<endl;


                    cout<<endl<<"Denominacion: ";
                    cin>>denominacion;

                    cout<<endl<<"Estado(s | n): ";
                    cin>>estado;

                    //le envio al servidor los datos de mi filtro, en denominacion pueden ir letras o cadenas del texto de denominacion

                    cliente->enviar(denominacion);
                    cliente->enviar(estado);


                    if(cliente->recibir() == "u")//en caso de no encontrarse el criterio de busqueda
                    {
                        cout<<cliente->recibir()<<endl;
                        system("pause");
                        system("cls");
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

                            cout<<"Ingresar ID: "<<endl;
                            cin>>id;

                            cliente->enviar(id);
                            if(cliente->recibir()=="j") //if para saber si el id ingresado existe
                            {
                                //modificar
                                cout<<"Ingrese los datos a modificar, sino desea modificar ingrese @"<<endl;
                                cout<<endl<<"Ingresar denominacion nueva: "<<endl;
                                cin>>denominacion;

                                cout<<endl<<"Ingresar estado nuevo s - alta | n - baja): "<<endl;
                                cin>>estado;

                                string confirmacion;
                                cout<<"¿Desea guardar los cambios? s | n : "<<endl;
                                cin>>confirmacion;
                                if(confirmacion=="s") //
                                {
                                    //aca tengo que enviar los datos a modificar
                                    cliente->enviar(denominacion);
                                    cliente->enviar(estado);
                                    //aca recibo la respuesta, si se pudo modificar o no

                                    //AGREGAR VALIDACION DE DENOMINACION
                                    cout<<"Se modifico correctamente el medicamento"<<endl;
                                    cout<<cliente->recibir()<<endl;
                                    cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                    system("pause");
                                    system("cls");
                                    banderaAuxiliar=false;

                                }//confirmacion de operacion
                                else
                                {
                                    //confirmar lo inesperado
                                    cout<<"No se modifico el tipo medicamento"<<endl;
                                    cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                    system("pause");
                                    system("cls");
                                    banderaAuxiliar=false;


                                }
                            }
                            else if(cliente->recibir()=="v")//ID NO ENCONTRADO
                            {
                                cout<<"El id ingresado no se encuentra en la lista"<<endl;
                                cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                system("pause");
                                system("cls");

                            }
                            else
                            {
                                cout<<"ERROR: respuesta inesperada del servidor"<<endl;
                                cout<<"Presione cualquier tecla para continuar"<<endl;
                                system("pause");
                            }

                        }//fin while 3

                    }
                    else
                    {
                        cout<<cliente->recibir()<<endl;
                        cout<<"Presione cualquier tecla para continuar"<<endl;
                        system("pause");
                    }//fin if

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

                }//fin while principal
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
                cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                system("pause");
                system("cls");
            }//fin switch



        }//fin while

    }

    // MENU  MEDICAMENTO********************************
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
                string opcion;
                char codigoProducto[50];
                char nombreComercial[50];
                char nombreDescripcionDroga[50];
                char tipoMedicamento[50];
                while(bandera)
                {
                    // el codigo 3 | le indica al servidor que vamos crear un  medicamento que se prepare
                    cliente->enviar("3");
                    cout<<"Crear Medicamento"<<endl;
                    cout<<"Por favor ingrese el codigo de producto: "<<endl;
                    cin>>codigoProducto;

                    cout<<"Por favor el nombre comercial: "<<endl;
                    cin>>nombreComercial;

                    cout<<"Por favor ingrese el nombre de la descripcion de la droga: "<<endl;
                    cin>>nombreDescripcionDroga;

                    cout<<"Por favor ingrese el tipo de medicamento: "<<endl;
                    cin>>tipoMedicamento;


                    cliente->enviar(codigoProducto);
                    cliente->enviar(nombreComercial);
                    cliente->enviar(nombreDescripcionDroga);
                    cliente->enviar(tipoMedicamento);

                    if(cliente->recibir() == "w")
                    {
                        cout<<cliente->recibir()<<endl;
                        cout<<"Presione cualquier tecla para continuar"<<endl;
                        system("pause");
                    }
                    else if(cliente->recibir() == "b")
                    {
                        cout<<"Medicamento creado!"<<endl;
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
                        }//fin if crear otro medicamento

                    }
                    else
                    {
                        cout<<"ERROR: respuesta inesperada del servidor"<<endl;
                        cout<<"Presione cualquier tecla para continuar"<<endl;
                        system("pause");
                    }// fin if w,b

                }//fin while


            };
            break; //fin caso 1
            case 2: // ADMINISTRAR MEDICAMENTO
            {
                bool bandera= false;
                char nombreComercial[50], tipoMedicamento[50], codigoProducto[20], descripcionDroga[50];

                while(bandera) //while principal
                {
                    // el codigo 4 | le indica al servidor que vamos administrar un tipo de medicamento que se prepare
                    cliente->enviar("4");
                    cout<<"Administrar Medicamento"<<endl;
                    cout<<"Por favor ingrese los criterios de busqueda"<<endl;
                    cout<<"Si no se desea aplicar el filtro escribir '@' "<<endl;

                    cout<<"Nombre comercial: ";
                    cin>>nombreComercial;

                    cout<<endl<<"Tipo de medicamento: ";
                    cin>>tipoMedicamento;

                    cliente->enviar(nombreComercial);
                    cliente->enviar(tipoMedicamento);

                    //if principal
                    if(cliente->recibir()=="g")
                    {
                        bool banderaAuxiliar=true;
                        // estos son los datos encontrados
                        string datos = cliente->recibir();

                        while(banderaAuxiliar)
                        {
                            char id[20];
                            cout<<datos<<endl;

                            cout<<"Elegir por ID Medicamento a modificar o eliminar"<<endl;

                            cout<<"Ingresar ID: "<<endl;
                            cin>>id;

                            cliente->enviar(id);

                            if(cliente->recibir() == "r")//no encontrado
                            {
                                cout<<cliente->recibir()<<endl;
                                cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                system("pause");
                                system("cls");

                            }
                            else if(cliente->recibir() == "q") //encontrado
                            {
                                bool banderin = true;

                                while(banderin)
                                {

                                    int opcion = 0;
                                    cout<<"Opciones"<<endl;
                                    cout<<"1- Modificar"<<endl;
                                    cout<<"2 - Eliminar"<<endl;

                                    if(opcion==1)//modificar
                                    {

                                        cout<<"Ingrese los datos a modificar, sino desea modificar ingrese @"<<endl;
                                        cout<<"Ingresar nombre comercial"<<endl;
                                        cin>>nombreComercial;

                                        cout<<"Ingresar codigo de producto"<<endl;
                                        cin>>codigoProducto;

                                        cout<<"Ingresar descripcion de la droga"<<endl;
                                        cin>>descripcionDroga;

                                        cout<<"Ingresar un tipo de medicamento EXISTENTE"<<endl;
                                        cin>>tipoMedicamento;

                                        string confirmacion;
                                        cout<<"¿Desea modificar el medicamento? s | n : ";
                                        cin>>confirmacion;
                                        if(confirmacion == "s")
                                        {
                                            cliente->enviar("aa");

                                            bool bandera5=true;
                                            while(bandera5)
                                            {
                                                cout<<"Si desea omitir la modificacion de algun valor ingresar @"<<endl;
                                                cout<<"Ingresar nuevo nombre comercial"<<endl;
                                                cliente->enviar(nombreComercial);
                                                cout<<"Ingresar nuevo codigo de producto"<<endl;
                                                cliente->enviar(codigoProducto);
                                                cout<<"Ingresar nueva descripcion de la droga"<<endl;
                                                cliente->enviar(descripcionDroga);
                                                cout<<"Ingresar nuevo tipo de medicamento"<<endl;
                                                cliente->enviar(tipoMedicamento);

                                                if(cliente->recibir()=="ac")
                                                {
                                                    cout<<"Se modifico correctamente el registro"<<endl;
                                                    cout<<cliente->recibir()<<endl;
                                                    cout<<"Presione cualquier tecla para continuar"<<endl;
                                                    system("pause");
                                                    system("cls");
                                                    banderin=false;
                                                    banderaAuxiliar=false;
                                                    bandera5=false;
                                                }
                                                else if(cliente->recibir()=="ad")
                                                {
                                                    cout<<cliente->recibir()<<endl;
                                                    cout<<"Volver a intentarlo"<<endl;
                                                    cout<<"Presione cualquier tecla para continuar"<<endl;
                                                    system("pause");
                                                    system("cls");

                                                }
                                                else
                                                {
                                                    cout<<cliente->recibir()<<endl;
                                                    cout<<"Error: respuesta inesperada del servidor"<<endl;
                                                    cout<<"Presione cualquier tecla para continuar"<<endl;
                                                    system("pause");
                                                    system("cls");
                                                }//fin if

                                            }//fin  while



                                            }else
                                            {
                                                //confirmar lo inesperado
                                                cout<<"No se elimino el medicamento"<<endl;
                                                cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                                system("pause");
                                                system("cls");
                                                banderin=false;
                                                banderaAuxiliar=false;
                                            }

                                        }
                                        else if(opcion==2) //eliminar
                                        {
                                            string confirmacion;
                                            cout<<"¿Desea eliminar el medicamento? s | n : ";
                                            cin>>confirmacion;
                                            if(confirmacion == "s")
                                            {
                                                cliente->enviar("ab");
                                                cout<<cliente->recibir()<<endl;
                                                cout<<"Presione cualquier tecla para continuar"<<endl;
                                                system("pause");
                                                system("cls");
                                                banderin=false;
                                                banderaAuxiliar=false;
                                            }
                                            else
                                            {
                                                //confirmar lo inesperado
                                                cout<<"No se elimino el medicamento"<<endl;
                                                cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                                system("pause");
                                                system("cls");
                                                banderin=false;
                                                banderaAuxiliar=false;
                                            }

                                        }
                                        else
                                        {
                                            cout<<"Opcion incorrecta, vuelve a intentarlo"<<endl;
                                            cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                                            system("pause");
                                            system("cls");

                                        }

                                    }

                                }
                                else
                                {
                                    cout<<"ERROR: respuesta inesperada del servidor"<<endl;
                                    cout<<"Presione cualquier tecla para continuar"<<endl;
                                    system("pause");
                                    system("cls");

                                }

                            }//fin while secundario

                        }
                        else if(cliente->recibir()=="h")
                        {

                            cout<<cliente->recibir()<<endl;
                            system("pause");
                            system("cls");

                        }
                        else
                        {
                            cout<<cliente->recibir()<<endl;
                            cout<<"Presione cualquier tecla para continuar"<<endl;
                            system("pause");
                            system("cls");
                        }


                        string opcion;
                        cout<<"Administrar otro medicamento? s | n : ";
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


                    }//fin while principal

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
                        cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                        system("pause");
                        system("cls");
                    }//fin if


                };
                break;//fin caso 3
                default:
                    cout<<"No ingresaste una opcion valida, vuelve a intentarlo!"<<endl;
                    cout<<"Ingrese una tecla cualquiera para continuar"<<endl;
                    system("pause");
                    system("cls");
                    ;
                }//fin switch
                system("pause");
                system("cls");
            }//fin while

        }

    };

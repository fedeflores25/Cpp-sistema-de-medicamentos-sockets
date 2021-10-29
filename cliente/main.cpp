#include <iostream>
#include <string>
#include <sstream>
#include "Cliente.cpp"



using namespace std;

int main()
{
    Cliente *cliente = new Cliente(5555, "192.168.1.102");
    cliente->conectarServidor();
    cout<<"enviando"<<endl;


    cliente->enviar("l");

    if(cliente->recibir() == "s")
    {
        cliente->enviar("fedeflores25");
        cliente->enviar("1234");

    }
    else
    {
        system("pause");
        cliente->cerrarSocket();
    }



    system("pause");

    cliente->cerrarSocket();

    system("pause");

    return 0;
}

/*
//menu 2

    //Cliente *cliente = new Cliente();
    //solicitud de conexion codigo 10 | ingresar puerto e ip

    //cliente->enviar(10);
    //respuesta si esta ocupado codigo 1 o tiene que ingresar contraseña codigo 2
    //cliente->recibir();
    //depende de la respuesta no hace nada o se prepara para ingresar contrase;a
    //cliente->enviar(cod 2);

    //UNA VEZ LOGUEADO
    //variables para el MENU
    string ingresoTeclado=" ";
    int varMenu = 0;
    bool banderaMenu=true;



    while(banderaMenu)
    {
        cout<<"Menu Sistema de Administracion de Medicamentos"<<endl;
        cout<<"1 - Tipo de medicamento"<<endl;
        cout<<"2 - Medicamento"<<endl;
        cout<<"3 - Registro"<<endl;
        cout<<"4 - Salir"<<endl;
        cout<<endl<<"Ingrese la opcion que desee: ";
        cin>>ingresoTeclado;
        //convierto la variable a string
        stringstream geek(ingresoTeclado);
        //transfiero el valor de geek a una variable de tipo int
        geek >> varMenu;


        // switch con opciones
        switch(varMenu)
        {
        case 1://TIPO DE MEDICAMENTO
        {
            //SUBMENU TIPO MEDICAMENTO******************************************
            system("cls");
            //variables para el SUBMENU
            bool banderaSubmenu=true;
            string submenu=" ";
            int varSubMenu=0;

            //el while permite repetir el submenu
            while(banderaSubmenu)
            {
                //menu
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
                case 1:
                {
                    bool bandera=true;
                    char opcion;
                    char denominacion[50];
                    while(bandera)
                    {
                        cout<<"Crear Tipo de Medicamento"<<endl;
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
                break;
                case 2:
                {
                    char denominacion[50], estado[3], id[20];
                    bool bandera=true;


                    while(bandera)// while 1
                    {
                        cout<<"Administrar Tipo de Medicamento"<<endl;
                        cout<<"Por favor ingrese los criterios de busqueda"<<endl;
                        cout<<"Si no se desea aplicar el filtro escribir '@' "<<endl;

                        cout<<"Id: ";
                        cin>>id;

                        cout<<endl<<"Denominacion: ";
                        cin>>denominacion;

                        cout<<endl<<"Estado(s | n): ";
                        cin>>estado;

                        //Cliente->enviar(id,denominacion,estado);
                        //char mensaje[1000] = Cliente->recibir();
                        if(mensaje==NULL);{ //if 1
                            cout<<"No se encontró ningún resultado para los criterios seleccionados. Presione ENTER para continuar"<<endl;
                            system("pause");
                        }else{
                            bool banderaAuxiliar=true;
                            while(banderaAuxiliar){ //while 2

                            cout<<decodificar(mensaje)<<endl;

                            cout<<"Elegir por ID el Tipo de Medicamento a modificar"<<endl;
                            cout<<"Si no se desea modificar el campo escribir '@' "<<endl;

                            cout<<"Ingresar ID: "
                            cinn>>id;

                            cout<<endl<<"Ingresar denominacion nueva: ";
                            cin>>denominacion;

                            cout<<endl<<"Ingresar estado nuevo s - alta | n - baja): ";
                            cin>>estado;

                            char confirmacion;
                            cout<<"¿Desea guardar los cambios? s | n : ";
                            cin>>confirmacion;

                            if(confirmacion=='s'){//if 2
                                Cliente->enviar(id,denominacion,estado); //aca tengo que enviar los datos a modificar, me falta agregar algo para que cuando lo envie el servidor entienda que hay que modificar el tipo
                                char mensaje2[50] = Cliente->recibir(); //aca recibo la respuesta, si se pudo modificar o no

                                if(mensaje2 == NULL){ //if 3
                                    cout<<"No se pudo modificar el tipo de medicamento la denominacion se encuentra repetida, intentelo otra vez"<<endl;
                                }else{
                                    cout<<mensaje2<<enld
                                    banderaAuxiliar=false;
                                }//fin if 3

                            }//fin if 2

                            }//fin while 2

                        }fin if 1



                        char opcion;
                        cout<<"Administrar otro tipo de medicamento? s | n : ";
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

                    }//fin while 1
                };
                break;//fin caso 2
                case 3:
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

        };
        break;//fin caso 1 Menu principal********************************
        case 2:// MEDICAMENTO
        {
            //SUBMENU  MEDICAMENTO********************************
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
                case 1:
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
                case 2: //administrar medicamento
                    ;
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
        };
        break;//fin caso 2 Menu principal *********************************
        case 3:
        {



            cout<<"Registro"<<endl;
        };
        break;//fin caso  3 Menu principal *****************************
        case 4:
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
        break;//fin caso 4**********************************************
        default:
            cout<<"No ingresaste una opcion valida, vuelve a intentarlo!"<<endl;

        }//fin switch
        system("pause");
        system("cls");
    }//fin while

    system("pause");
    return 0;



*/




#include <iostream>
#include <sstream>
#include <string>

#include "src/Cliente.cpp"
#include "src/Servidor.cpp"

using namespace std;

int main()
{
    //Cliente *cliente = new Cliente();

    //solicitud de conexion codigo 10 | ingresar puerto e ip
    //cliente->enviar(10);
    //respuesta si esta ocupado codigo 1 o tiene que ingresar contraseña codigo 2
    //cliente->recibir();
    //depende de la respuesta no hace nada o se prepara para ingresar contrase;a
    //cliente->enviar(/*cod 2*/);

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
        case 1:
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
                char denominacion[50];
                 cout<<"Crear Tipo de Medicamento"<<endl;
                 cout<<"por favor ingrese la denominacion: "<<endl;
                 cin>>denominacion;
                 //cliente->enviar(denominacion);
                 //cliente->recibir();
                };
                break;
                case 2:
                {

                    char denominacion[50], estado[3], id[20];

                    cout<<"Administrar Tipo de Medicamento"<<endl;
                    cout<<"Por favor ingrese los criterios de busqueda"<<endl;
                    cout<<"Si no se desea aplicar el filtro escribir '@' "<<endl;

                    cout<<"Id: ";
                    cin>>id;

                    cout<<endl<<"Denominacion: ";
                    cin>>denominacion;

                    cout<<endl<<"Estado(s | n): ";
                    cin>>estado;
                    //administrar
                };
                break;
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
        case 2:
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
                case 1: //crear
                    ;
                    break;
                case 2: //administrar
                    ;
                    break;
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
}







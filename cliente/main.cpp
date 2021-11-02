#include <iostream>
#include <string>
#include <sstream>
#include "Cliente.cpp"



using namespace std;

int main()
{
    bool banderaMenu=true;
    while(banderaMenu)
    {
        bool bandera=true;
        Cliente *cliente = new Cliente(5555, "192.168.1.102");
        cliente->conectarServidor();
        cout<<"enviando"<<endl;

        // el codigo l | es una solicitud para loguearse
        cliente->enviar("l");

        while(bandera)
        {
            // el codigo s | significa que se puede ingresar usuario y contrase;a
            if(cliente->recibir() == "s")
            {
                cout<<"Ingrese usuario "<<endl;
                cliente->enviar("fedeflores25");
                cout<<"Ingrese clave"<<endl;
                cliente->enviar("1234");

                // el codigo n | significa que el usuario y la contrase;a ingresados no son correctos
                if(cliente->recibir() == "c")
                {
                    cout<<"Logueo exitoso"<<endl;
                    //FALTA ESTA PARTE
                    cliente->menuPrincipal(cliente);
                }
                else if( cliente->recibir() == "n" )
                {
                    cout<<"Usuario o clave incorrecto"<<endl;

                }
                else if( cliente->recibir() == "x" )
                {
                    cout<<"Usuario ingresado supera los 12 caracteres permitidos"<<endl;
                }
                else if( cliente->recibir() == "e" )
                {
                    cout<<"Se superaron los 3 intentos permitidos"<<endl;
                    bandera=false;
                }

            }//fin while


            system("pause");
            cliente->cerrarSocket();
            int varMenu=0;
            string ingresoTeclado=" ";
            cout<<"Salir del sistema? 1 | Volver a intentarlo! 2"<<endl;
            cin>>ingresoTeclado;
            //preparo la variable para ser convertida a int
            stringstream geek(ingresoTeclado);
            //convierto el valor de geek a un int
            geek >> varMenu;
            if(varMenu == 1)
            {
                banderaMenu=false;
                cout<<"Adios";
                cout<<"Presione cualquier tecla"<<endl;
                system("pause");
            }
            else
            {
                cout<<"Presione cualquier tecla"<<endl;
                system("pause");
                system("cls");

            }

        }//fin while

    }//fin while
        return 0;
    }

#include <iostream>
#include <string>


#include "Servidor.cpp"

using namespace std;

int main()
{
    string mensaje;
    Servidor *servidor = new Servidor(5555);

    servidor->aceptar();


    cout<<"mensaje "<<servidor->recibir()<<endl;



    servidor->enviar("s");

    system("pause");
    if( mensaje == "l")
    {
        cout<<"paso por aca"<<endl;
        servidor->enviar("s");

        //usuario
        servidor->recibir();
        //contrase;a
        servidor->recibir();

        //funcion para buscar y comprobar usuario y contrase;a
        servidor->enviar("c");
    }

        system("pause");
        servidor->cerrarSocket();

    system("pause");
    return 0;

}//fin main


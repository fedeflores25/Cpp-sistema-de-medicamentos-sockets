#include <iostream>
#include <string>


#include "Servidor.cpp"

using namespace std;

int main()
{
    Servidor *servidor = new Servidor(5555);

    cout<<servidor->fechaYHora()<<endl;

    servidor->aceptar();


    if(  servidor->recibir() == "l")
    {
        servidor->enviar("s");

        //usuario
        cout<<"Usuario: "<<servidor->recibir()<<endl;
        //contrase;a
        cout<<"Clave: "<<servidor->recibir()<<endl;

        //funcion para buscar y comprobar usuario y contrase;a
        //servidor->enviar("c");
    }

    system("pause");
    servidor->cerrarSocket();

    system("pause");
    return 0;

}//fin main


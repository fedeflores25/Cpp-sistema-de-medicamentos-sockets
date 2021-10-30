#include <iostream>
#include <string>


#include "Servidor.cpp"

using namespace std;

int main()
{
    Servidor *servidor = new Servidor(5555);

    cout<<servidor->fechaYHora()<<endl;

    servidor->aceptar();
    //login
    if(servidor->login(servidor) == true)
    {
    //el codigo c | significa logueo fue exitoso
        servidor->enviar("c");
    //respuestas al menu

    }

    system("pause");
    servidor->cerrarSocket();

    system("pause");
    return 0;

}//fin main








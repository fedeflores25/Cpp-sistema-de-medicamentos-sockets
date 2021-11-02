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
        servidor->menuServidor(servidor);
        //si pasa estas validaciones puede acceder al menu
    }
    else
    {
        // el codigo e | significa que se supero el maximo de intentos permitidos
        servidor->enviar("e");
        servidor->cerrarSocket();
    }




    servidor->cerrarSocketServidor();
    servidor->~Servidor();

    system("pause");
    return 0;

}//fin main








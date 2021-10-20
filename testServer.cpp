#include <iostream>
#include "src/Servidor.cpp"

using namespace std;

int testServer(){

Servidor *servidor = new Servidor(5555);

if(servidor->recibir() == "l")
{
    servidor->enviar("s");

    //usuario
    servidor->recibir();
    //contrase;a
    servidor->recibir();

    //funcion para buscar y comprobar usuario y contrase;a
    servidor->enviar("c");



}






}


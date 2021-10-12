#include <iostream>
#include "src/Servidor.cpp"

using namespace std;

int testServer()
{
    Server *servidor = new Server();

    while(true)
    {
        servidor->recibir();
        servidor->enviar();
    }

    return 0;
}

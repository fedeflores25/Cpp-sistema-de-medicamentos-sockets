#include <iostream>
#include "src/Servidor.cpp"
#include "src/Cliente.cpp"

using namespace std;

int main()
{
    Server *servidor = new Server();

    while(true){
        servidor->recibir();
        servidor->enviar();
    }



    return 0;
}

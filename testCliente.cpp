#include <iostream>
#include "src/Cliente.cpp"

using namespace std;

int testCliente()
{
    Cliente *cliente = new Cliente();
    cout<<"enviando"<<endl;

    while(true)
    {
        cliente->enviar();

    }

    return 0;
}



#include <iostream>
#include <winsock2.h>


using namespace std;

class TipoMedicamento
{
private:
    // atributos
    bool activo;
    char denominacion[20];
    int id;

public:
    // constructor
    TipoMedicamento(bool *activo, char *denominacion[20], int *id)
    {
        this.activo=activo;
        this.denominacion=denominacion;
        this.id=id;
    }
    // destructor
    ~TipoMedicamento();

    //getters y setters


};

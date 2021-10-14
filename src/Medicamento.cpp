#include <iostream>
#include <winsock2.h>
#include "TipoMedicamento.cpp"

using namespace std;

class Medicamento
{
private:
    // atributos
    string codigoDeProducto;
    string nombreDroga;
    bool estado;
    int id;
    string nombreComercial;
    TipoMedicamento tipo;

public:
    //constructor
    Medicamento(string codigoDeProducto, string nombreDroga, bool estado, int id, string nombreComercial, TipoMedicamento tipo)
    {
        this.codigoDeProducto=codigoDeProducto;
        this.nombreDroga=nombreDroga;
        this.estado=estado;
        this.id=id;
        this.nombreComercial=nombreComercial;
        this.tipo=tipo;
    }
    //destructor
    ~Medicamento();

    //getters y setters
    string getCodigoDeProducto()
    {
        return this.codigoDeProducto;
    }

    void setCodigoDeProducto(string codigoDeProducto)
    {
        this.codigoDeProducto = codigoDeProducto;
    }

    string getNombreDroga()
    {
        return this.nombreDroga;
    }

    void setNombreDroga(string nombreDroga)
    {
        this.nombreDroga = nombreDroga;
    }

    bool getEstado()
    {
        return this.estado;
    }

    void setEstado(bool estado)
    {
        this.estado = estado;
    }

    int getId()
    {
        return this.id;
    }

    void setId(int id)
    {
        this.id = id;
    }

    string getNombreComercial()
    {
        return this.nombreComercial;
    }

    void setNombreComercial(string nombreComercial)
    {
        this.nombreComercial = nombreComercial;
    }

    TipoMedicamento getTipoMedicamento()
    {
        return this.tipo;
    }

    void setTipoMedicamento(TipoMedicamento tipo)
    {
        this.tipo=tipo;
    }




};

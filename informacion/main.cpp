#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;


//TIPO DE MEDICAMENTO
//CREAR ARCHIVO BINARIO Y AGREGAR 1 REGISTRO
int escribirTipoMedicamento(int id, string denominacion);
/*listo */int escribirMedicamento(int id, string codigoProducto, string nombreComercial, string nombreDescripcionDroga, int idTipoMedicamento );

//AGREGAR
struct TipoMedicamento agregarArchivoTipoMedicamento(string denominacion);
/*listo */struct Medicamento agregarArchivoMedicamento(string codigoProducto, string nombreComercial, string nombreDescripcionDroga, int idTipoMedicamento);

//LEER EL ARCHIVO
void leerArchivoTipoMedicamento();
/*listo */void leerArchivoMedicamento();

//CANTIDAD DE TIPOS DE MEDICAMENTOS GUARDADOS
int cantidadTipoMedicamento();
/*listo */ int cantidadMedicamento();

//DEVOLVER UN PUNTERO PARA PODER TRABAJAR CON LA ESTRUCTURA
struct TipoMedicamento * traerArchivoTipoMedicamento();
/*listo */struct Medicamento * traerArchivoMedicamento();

//BUSCAR EN EL ARCHIVO
struct TipoMedicamento * buscarArchivoTipoMedicamento(string denominacion, bool estado);
struct TipoMedicamento * buscarArchivoTipoMedicamentoDenominacion(string denominacion);
struct TipoMedicamento * buscarArchivoTipoMedicamentoEstado(bool estado);
struct TipoMedicamento buscarArchivoTipoMedicamentoId(int id);

/*listo */ struct Medicamento * buscarArchivoMedicamento(string nombreComercial, string denominacion);
/*listo */ struct Medicamento * buscarArchivoMedicamento(string nombreComercial);
/*listo */ struct Medicamento * buscarArchivoMedicamento(string denominacion);
/*listo */struct Medicamento buscarArchivoMedicamentoId(int id);

//MODIFICAR
void modificarTipoMedicamento(int id, string denominacion, bool estado);
/*listo */void modificarMedicamento(int id, string codigoProducto, string nombreComercial, string descripcionDroga, int idTipoMedicamento);

//ELIMINAR
void eliminarTipoMedicamento(int id);
void eliminarMedicamento(int id);


struct TipoMedicamento
{
    int id;
    char denominacion[20];
    bool estado;
};

struct Medicamento
{
    int id;
    char codigoProducto[50];
    char nombreComercial[50];
    char nombreDescripcionDroga[50];
    int idTipoMedicamento;
};


int main()
{
    bool bandera = true;
    while(bandera)
    {
        cout<<"Ingrese una opcion"<<endl;
        cout<<"1 - Crear archivo binario Tipo de Medicamento de 0"<<endl;
        cout<<"2 - Agregar informacion al archivo binario Tipo de Medicamento "<<endl;
        cout<<"3 - Leer informacion guardada "<<endl;
        cout<<"4 - Recorrer informacion guardada "<<endl;
        cout<<"5 - Buscar por id "<<endl; //si retorna 0 en la id es porque no encontro nada
        cout<<"6 - Modificar "<<endl;
        cout<<"7 - Eliminar "<<endl;
        cout<<"8 - Salir "<<endl;

        int opcion = 0;
        cin>>opcion;

        switch(opcion)
        {
        case 1:
        {
            system("cls");
            cout << "Crear archivo binario tipo medicamento de 0" << endl;

            int idCreado = escribirTipoMedicamento(1, "Jarabe");

            cout<<"Id del recien creado: "<<idCreado<<endl<<endl;

            leerArchivoTipoMedicamento();


        };
        break;
        case 2:
        {
            system("cls");
            cout << "Agregar tipo de medicamento al archivo binario" << endl;
            TipoMedicamento tipo = agregarArchivoTipoMedicamento("dermatológicos");

            cout<<"ID: "<<tipo.id<<endl;
            cout<<"Denominacion: "<<tipo.denominacion<<endl;
            if(tipo.estado == 1)
            {
                cout<<"Estado: activado"<<endl;
            }
            else
            {
                cout<<"Estado: desactivado"<<endl;
            }

            system("pause");

        };
        break;
        case 3:
        {
            leerArchivoTipoMedicamento();


        };
        break;
        case 4:// TRAER UN ARRAY DE STRUCT TIPO MEDICAMENTO
        {
            int cantidad = cantidadTipoMedicamento();
            cout<<"Cantidad: "<<cantidad<<endl;

            //capturo el puntero
            struct TipoMedicamento *tipos = traerArchivoTipoMedicamento();
            struct TipoMedicamento tipo;


            for(int i=0; i<cantidad; i++)
            {
                //capturo cada posicion del puntero y lo convierto al tipo correcto
                tipo = *((tipos)+i);

                cout<<"Id: "<<tipo.id<<endl;
                cout<<"Denominacion: "<<tipo.denominacion<<endl;
                cout<<"Estado: "<<tipo.estado<<endl<<endl;


            }

        };
        break;
        case 5:
        {
            struct TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(1);

            cout<<"ID: "<<tipo.id<<endl;
            cout<<"Denominacion: "<<tipo.denominacion<<endl;
            if(tipo.estado == 1)
            {
                cout<<"Estado: activado"<<endl;
            }
            else
            {
                cout<<"Estado: desactivado"<<endl;
            }


            system("pause");

        };
        break;
        case 6:
        {
            modificarTipoMedicamento(1,"heroico",false);

        };
        break;
        case 7:
        {
            eliminarTipoMedicamento(1);

        };
        break;
        case 8:
        {
            bandera=false;

        };
        break;
        default:
            cout<<"Opcion no valida"<<endl;
        }//fin switch
    }//fin while

    return 0;


}//FIN MAIN

//Funciones
//TIPO MEDICAMENTO
int escribirTipoMedicamento(int id, string denominacion)
{

    //convierto el string a char
    char denomin[20];
    strcpy(denomin,denominacion.c_str());


    ofstream archivoBinario;
    archivoBinario.open("TipoMedicamento.dat", ios::out | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
    }

    TipoMedicamento tipo;

    tipo.id = id;
    strcpy(tipo.denominacion, denomin);
    //tipo.denominacion = denomin;
    tipo.estado = true;

    archivoBinario.write((char *)&tipo, sizeof(TipoMedicamento));

    archivoBinario.close();

    return tipo.id;

}//fin funcion  escribir tipo medicamento

struct TipoMedicamento agregarArchivoTipoMedicamento(string denominacion)
{
    //convierto el string a char
    char denomin[20];
    strcpy(denomin,denominacion.c_str());

    ofstream archivoBinario;
    archivoBinario.open("TipoMedicamento.dat", ios::app | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
    }

    TipoMedicamento tipo;

    tipo.id = cantidadTipoMedicamento()+1;
    strcpy(tipo.denominacion, denomin);
    //tipo.denominacion = denomin;
    tipo.estado = true;

    archivoBinario.write((char *)&tipo, sizeof(TipoMedicamento));

    archivoBinario.close();

    return tipo;

}

void leerArchivoTipoMedicamento()
{
    ifstream archivoBinario;

    archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    TipoMedicamento tipo;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&tipo, sizeof(TipoMedicamento));

        if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
        {
            cout<<"ID: "<<tipo.id<<endl;
            cout<<"Denominacion: "<<tipo.denominacion<<endl;
            if(tipo.estado == 1)
            {
                cout<<"Estado: activado"<<endl<<endl;
            }
            else
            {
                cout<<"Estado: desactivado"<<endl<<endl;
            }
        }



    }//fin while

    archivoBinario.close();
}

int cantidadTipoMedicamento()
{
    ifstream archivoBinario;

    archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    TipoMedicamento tipo;
    int contador = 0;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&tipo, sizeof(TipoMedicamento));

        if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
        {
            contador++;
        }



    }//fin while

    archivoBinario.close();

    return contador;
}

struct TipoMedicamento * traerArchivoTipoMedicamento()
{
    ifstream archivoBinario;

    archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static TipoMedicamento tipos[20];
    int contador = 0;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

        if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
        {
            contador++;

        }



    }//fin while

    archivoBinario.close();

    return tipos;
}

struct TipoMedicamento * buscarArchivoTipoMedicamento(string denominacion, bool estado)
{

    ifstream archivoBinario;

    archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static TipoMedicamento tipos[20];
    static TipoMedicamento aux[20];
    int contador = 0;

    string auxDenominacion;


    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

        auxDenominacion = tipos[contador].denominacion;
        if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)) && (tipos[contador].estado==estado) )//if para que no me repita el ultimo archivo
        {
            aux[contador].id = tipos[contador].id;
            strcpy(aux[contador].denominacion, tipos[contador].denominacion);
            aux[contador].estado = estado;
        }
        contador++;


    }//fin while

    archivoBinario.close();

    return aux;


}

struct TipoMedicamento * buscarArchivoTipoMedicamentoDenominacion(string denominacion)
{

    ifstream archivoBinario;

    archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static TipoMedicamento tipos[20];
    static TipoMedicamento aux[20];
    int contador = 0;
    string auxDenominacion;


    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));

        auxDenominacion = tipos[contador].denominacion;
        if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)))//if para que no me repita el ultimo archivo
        {
            aux[contador].id = tipos[contador].id;
            strcpy(aux[contador].denominacion, tipos[contador].denominacion);
            aux[contador].estado = tipos[contador].estado;
        }
        contador++;


    }//fin while

    archivoBinario.close();

    return aux;

}

struct TipoMedicamento * buscarArchivoTipoMedicamentoEstado(bool estado)
{

    ifstream archivoBinario;

    archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static TipoMedicamento tipos[20];
    static TipoMedicamento aux[20];
    int contador = 0;


    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));


        if( (!archivoBinario.eof()) && (tipos[contador].estado==estado) )//if para que no me repita el ultimo archivo
        {
            aux[contador].id = tipos[contador].id;
            strcpy(aux[contador].denominacion, tipos[contador].denominacion);
            aux[contador].estado = estado;
        }
        contador++;



    }//fin while

    archivoBinario.close();

    return aux;

}

struct TipoMedicamento buscarArchivoTipoMedicamentoId(int id)
{

    ifstream archivoBinario;

    archivoBinario.open("TipoMedicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static TipoMedicamento tipos[20];
    static TipoMedicamento aux;
    int contador = 0;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&tipos[contador], sizeof(TipoMedicamento));


        if( (!archivoBinario.eof()) && ( tipos[contador].id==id ) )//if para que no me repita el ultimo archivo
        {

            aux.id = tipos[contador].id;
            strcpy(aux.denominacion, tipos[contador].denominacion);
            aux.estado = tipos[contador].estado;


        }
        contador++;

    }//fin while

    archivoBinario.close();

    return aux;

}
void modificarTipoMedicamento(int id, string denominacion, bool estado)
{
    char denomin[20];
    strcpy(denomin,denominacion.c_str());

    TipoMedicamento tipo;
    tipo.id = id;
    strcpy(tipo.denominacion, denomin);
    tipo.estado = false;

    fstream archivoBinario("TipoMedicamento.dat",ios::out | ios::in | ios::binary);

    if(archivoBinario.is_open())
    {
        archivoBinario.seekp((tipo.id-1)*sizeof(TipoMedicamento));
        archivoBinario.write((char *)&tipo,sizeof(TipoMedicamento));
        archivoBinario.close();
    }

}
void eliminarTipoMedicamento(int id)
{
    fstream archivoBinario("TipoMedicamento.dat", ios::out | ios::in | ios::binary);
    TipoMedicamento aux;

    if (archivoBinario.is_open())
    {
        archivoBinario.seekg((id - 1) * sizeof(TipoMedicamento));
        archivoBinario.write((char *)&aux, sizeof(TipoMedicamento));
        archivoBinario.close();
    }
}

//MEDICAMENTO
int escribirMedicamento(int id, string codigoProducto, string nombreComercial, string nombreDescripcionDroga, int idTipoMedicamento )
{
    char codigoProduct[50];
    char nombreComer[50];
    char descripcionDrog[50];

    //convierto el string a char
    strcpy(codigoProduct,codigoProducto.c_str());
    strcpy(nombreComer,nombreComercial.c_str());
    strcpy(descripcionDrog,nombreDescripcionDroga.c_str());

    //creo archivo binario
    ofstream archivoBinario;
    archivoBinario.open("Medicamento.dat", ios::out | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
    }

    Medicamento medicamento;

    medicamento.id = id;
    strcpy(medicamento.codigoProducto, codigoProduct);
    strcpy(medicamento.nombreComercial, nombreComer);
    strcpy(medicamento.nombreDescripcionDroga, descripcionDrog);


    archivoBinario.write((char *)&medicamento, sizeof(Medicamento));

    archivoBinario.close();

    return medicamento.id;
}

int cantidadMedicamento()
{
    ifstream archivoBinario;

    archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario medicamento"<<endl;
        exit(1);
    }

    Medicamento medicamento;
    int contador = 0;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&medicamento, sizeof(Medicamento));

        if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
        {
            contador++;
        }



    }//fin while

    archivoBinario.close();

    return contador;
}

struct Medicamento agregarArchivoMedicamento(string codigoProducto, string nombreComercial, string nombreDescripcionDroga, int idTipoMedicamento)
{
    char codigoProduct[50];
    char nombreComer[50];
    char descripcionDrog[50];

    //convierto el string a char
    strcpy(codigoProduct,codigoProducto.c_str());
    strcpy(nombreComer,nombreComercial.c_str());
    strcpy(descripcionDrog,nombreDescripcionDroga.c_str());

    ofstream archivoBinario;
    archivoBinario.open("Medicamento.dat", ios::app | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo crear el archivo binario tipo de medicamento"<<endl;
    }

    Medicamento medicamento;

    medicamento.id = cantidadMedicamento();
    strcpy(medicamento.codigoProducto, codigoProduct);
    strcpy(medicamento.nombreComercial, nombreComer);
    strcpy(medicamento.nombreDescripcionDroga, descripcionDrog);

    archivoBinario.write((char *)&medicamento, sizeof(Medicamento));

    archivoBinario.close();

    return medicamento;

}

void leerArchivoMedicamento()
{
    ifstream archivoBinario;

    archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    Medicamento medicamento;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&medicamento, sizeof(Medicamento));

        if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
        {
            cout<<"Medicamento"<<endl;
            cout<<"ID: "<<medicamento.id<<endl;
            cout<<"Codigo de producto: "<<medicamento.codigoProducto<<endl;
            cout<<"Nombre comercial: "<<medicamento.nombreComercial<<endl;
            cout<<"Nombre descripcion droga: "<<medicamento.codigoProducto<<endl;
            cout<<"Codigo de producto: "<<medicamento.codigoProducto<<endl;
            TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(medicamento.idTipoMedicamento);
            cout<<"Tipo medicamento: "<<endl;
            cout<<"ID: "<<tipo.id<<endl;
            cout<<"Denominacion: "<<tipo.denominacion<<endl;
            if(tipo.estado == 1)
            {
                cout<<"Estado: activado"<<endl<<endl;
            }
            else
            {
                cout<<"Estado: desactivado"<<endl<<endl;
            }
        }



    }//fin while

    archivoBinario.close();
}

struct Medicamento * traerArchivoMedicamento()
{
    ifstream archivoBinario;

    archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static Medicamento medicamentos[50];
    int contador = 0;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

        if(!archivoBinario.eof())//if para que no me repita el ultimo archivo
        {
            contador++; //es necesario para la carga del vector

        }

    }//fin while

    archivoBinario.close();

    return medicamentos;
}

struct Medicamento * buscarArchivoMedicamento(string nombreComercial, string denominacion)
{
    ifstream archivoBinario;

    archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static Medicamento medicamentos[20];
    static Medicamento aux[20];
    int contador = 0;


    string auxNombreComercial;
    string auxDenominacion;


    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

        auxNombreComercial = medicamentos[contador].nombreComercial;
        //traer el tipo
        TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(medicamentos[contador].id);
        auxDenominacion = tipo.denominacion;

        if( (!archivoBinario.eof()) && ( (auxNombreComercial==nombreComercial)) && (auxDenominacion==denominacion) )//if para que no me repita el ultimo archivo
        {
            aux[contador].id = medicamentos[contador].id;
            strcpy(aux[contador].nombreComercial, medicamentos[contador].nombreComercial);
            strcpy(aux[contador].codigoProducto, medicamentos[contador].codigoProducto);
            strcpy(aux[contador].nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
            aux[contador].idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

        }
        contador++;


    }//fin while

    archivoBinario.close();

    return aux;
}

struct Medicamento * buscarArchivoMedicamentoNombreComercial(string nombreComercial)
{
    ifstream archivoBinario;

    archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static Medicamento medicamentos[20];
    static Medicamento aux[20];
    int contador = 0;


    string auxNombreComercial;


    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

        auxNombreComercial = medicamentos[contador].nombreComercial;

        if( (!archivoBinario.eof()) && ( (auxNombreComercial==nombreComercial)) )//if para que no me repita el ultimo archivo
        {
            aux[contador].id = medicamentos[contador].id;
            strcpy(aux[contador].nombreComercial, medicamentos[contador].nombreComercial);
            strcpy(aux[contador].codigoProducto, medicamentos[contador].codigoProducto);
            strcpy(aux[contador].nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
            aux[contador].idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

        }
        contador++;


    }//fin while

    archivoBinario.close();

    return aux;



}

struct Medicamento * buscarArchivoMedicamentoDenominacion(string denominacion)
{
    ifstream archivoBinario;

    archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static Medicamento medicamentos[20];
    static Medicamento aux[20];
    int contador = 0;
    char denomin[50];

    strcpy(denomin,denominacion.c_str());


    string auxDenominacion;


    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));

        //traer el tipo
        TipoMedicamento tipo = buscarArchivoTipoMedicamentoId(medicamentos[contador].id);
        auxDenominacion = tipo.denominacion;

        if( (!archivoBinario.eof()) && ( (auxDenominacion==denominacion)) )//if para que no me repita el ultimo archivo
        {
            aux[contador].id = medicamentos[contador].id;
            strcpy(aux[contador].nombreComercial, medicamentos[contador].nombreComercial);
            strcpy(aux[contador].codigoProducto, medicamentos[contador].codigoProducto);
            strcpy(aux[contador].nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
            aux[contador].idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

        }
        contador++;


    }//fin while

    archivoBinario.close();

    return aux;

}

struct Medicamento buscarArchivoMedicamentoId(int id)
{

    ifstream archivoBinario;

    archivoBinario.open("Medicamento.dat", ios::in | ios::binary);

    if(archivoBinario.fail())//en caso que no se cree el archivo binario
    {
        cout<<"No se pudo abrir el archivo binario tipo de medicamento"<<endl;
        exit(1);
    }

    static Medicamento medicamentos[50];
    static Medicamento aux;
    int contador = 0;

    while(!archivoBinario.eof())// mientras no haya finalizado de recorrer el archivo binario
    {
        archivoBinario.read((char *)&medicamentos[contador], sizeof(Medicamento));


        if( (!archivoBinario.eof()) && ( medicamentos[contador].id==id ) )//if para que no me repita el ultimo archivo
        {

            aux.id = medicamentos[contador].id;
            strcpy(aux.nombreComercial, medicamentos[contador].nombreComercial);
            strcpy(aux.codigoProducto, medicamentos[contador].codigoProducto);
            strcpy(aux.nombreDescripcionDroga, medicamentos[contador].nombreDescripcionDroga);
            aux.idTipoMedicamento = medicamentos[contador].idTipoMedicamento;

        }
        contador++;

    }//fin while

    archivoBinario.close();

    return aux;

}

void modificarMedicamento(int id, string codigoProducto, string nombreComercial, string descripcionDroga, int idTipoMedicamento)
{

    char codigoProduct[50];
    strcpy(codigoProduct,codigoProducto.c_str());

    char nombreComercia[50];
    strcpy(nombreComercia,nombreComercial.c_str());

    char descripcionDrog[50];
    strcpy(descripcionDrog,descripcionDroga.c_str());


    Medicamento medicamento;
    medicamento.id = id;
    strcpy(medicamento.codigoProducto, codigoProduct);
    strcpy(medicamento.nombreComercial, nombreComercia);
    strcpy(medicamento.nombreDescripcionDroga, descripcionDrog);
    medicamento.idTipoMedicamento = idTipoMedicamento;

    fstream archivoBinario("Medicamento.dat",ios::out | ios::in | ios::binary);

    if(archivoBinario.is_open())
    {
        archivoBinario.seekp((medicamento.id-1)*sizeof(Medicamento));
        archivoBinario.write((char *)&medicamento,sizeof(Medicamento));
        archivoBinario.close();
    }

}

void eliminarMedicamento(int id)
{
    fstream archivoBinario("Medicamento.dat", ios::out | ios::in | ios::binary);
    Medicamento aux;

    if (archivoBinario.is_open())
    {
        archivoBinario.seekg((id - 1) * sizeof(Medicamento));
        archivoBinario.write((char *)&aux, sizeof(Medicamento));
        archivoBinario.close();
    }
}

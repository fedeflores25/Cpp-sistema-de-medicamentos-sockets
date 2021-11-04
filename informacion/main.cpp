#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

using namespace std;

int escribirTipoMedicamento(int id, string denominacion);
struct TipoMedicamento agregarArchivoTipoMedicamento(int id, string denominacion);
void leerArchivoTipoMedicamento();
int cantidadTipoMedicamento();
struct TipoMedicamento * traerArchivoTipoMedicamento();

struct TipoMedicamento
{
    int id;
    char denominacion[20];
    bool estado;
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
        cout<<"6 - Salir "<<endl;

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
            TipoMedicamento tipo = agregarArchivoTipoMedicamento(4, "dermatológicos");
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
        case 4:
        {
            int cantidad = cantidadTipoMedicamento();
            cout<<"Cantidad: "<<cantidad<<endl;

            //capturo el puntero
            struct TipoMedicamento *tipos = traerArchivoTipoMedicamento();
            struct TipoMedicamento tipo;


            for(int i=0;i<cantidad;i++)
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

        };
        break;
        case 6:
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

struct TipoMedicamento agregarArchivoTipoMedicamento(int id, string denominacion)
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

    tipo.id = id;
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

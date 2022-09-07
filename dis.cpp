#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

#define NUMREGISTROS 100
#define CONTENEDOR 4

class Pagos
{
private:
    char Cuenta[ 12 ];
    char nombreTitular[ 35 ];
    char Direccion[ 35 ];
    char Monto[ 10 ];

    int dispersion( const char llave[ 12 ] );
    long int buscarCuenta( const string & );
public:
    Pagos (void);
    Pagos( const Pagos & );
    void setCuenta( const string & );
    void setnombreTitular( const string & );
    void setDireccion( const string & );
    void setMonto( const string & );
    friend ostream &operator<<( ostream &, const Pagos & );

    bool agregar( const Pagos & );
    void mostrar( void );
    bool buscar( const string &, Pagos & );
    bool eliminar( const string &, Pagos & );
    bool modificar( const string &, const Pagos & );
    void mostrarIndice( void );
    Pagos pedirRegistro( void );
    bool contiene( const string & );
    void genera( void );
};

Pagos::Pagos( void )
{
    for( int i = 0; i < sizeof( Cuenta ); Cuenta[ i++ ] = '\0');
    for( int j = 0; j < sizeof( nombreTitular ); nombreTitular[ j++ ] = '\0');
    for( int k = 0; k < sizeof( Direccion ); Direccion[ k++ ] ='\0');
    for( int i = 0; i < sizeof( Monto ); Monto[ i++ ] = '\0');
}

bool Pagos::contiene( const string &CuentaABuscar )
{
    if( buscarCuenta( CuentaABuscar ) == -1 )
        return false;
    return true;
}

Pagos Pagos::pedirRegistro( void )
{
    Pagos registroARetornar;
    string cadena;

    cout << endl << "Ingresa el numero de cuenta: ";
    fflush( stdin );
    getline( cin, cadena );
    while( contiene( cadena ) || cadena.size() == 0 )
    {
        cout << "Ese Cuenta ya existe o la cadena es invalida: ";
        fflush( stdin );
        getline( cin, cadena );
    }
    registroARetornar.setCuenta( cadena );

    cout << "ingresa El nombre del titular: ";
    fflush( stdin );
    getline( cin, cadena );
    registroARetornar.setnombreTitular( cadena );

    cout << "Ingresa la direccion de facturacion: ";
    fflush( stdin );
    getline( cin, cadena );
    registroARetornar.setDireccion( cadena );

    cout << "ingresa el monto: ";
    fflush( stdin );
    getline( cin, cadena );
    registroARetornar.setMonto( cadena );

    return registroARetornar;
}

void Pagos::genera( void )
{
    Pagos p;
    int contador = 0;
    ofstream archivo( "dispersion.txt", ios::out );
    if( !archivo )
        cout << "No se pudo crear el archivo" << endl;
    else
        for( int i = 0; i < NUMREGISTROS; i++ )
        {
            archivo.write( ( char * ) &contador, sizeof( int ) );
            for( int j = 0; j < CONTENEDOR; j++ )
            {
                archivo.write( ( char * ) &p, sizeof( Pagos ) );
            }
        }
    archivo.close();
}


int Pagos::dispersion( const char llave[ 12 ] )
{
    char llaveCopia[ 12 ];
    strcpy( llaveCopia, llave );
    if( strlen( llaveCopia ) < 12 )
        for( int i = strlen( llaveCopia ); i < 12; i++ )
            llaveCopia[ i ] = ' ';
    llaveCopia[ 12 ] = '\0';

    long sum = 0;
    int j = 0;
    while( j < 12 )
    {
        sum = ( sum + 100 * llaveCopia[ j ] + llaveCopia[ j + 1 ] )% 20000;
        j += 2;
    }
    return ( sum % 99 );
}

long int Pagos::buscarCuenta( const string &CuentaABuscar )
{
    Pagos p;
    int contador, posIndice;
    long int posByte;

    ifstream archivo( "dispersion.txt", ios::in );
    if( !archivo )
    {
        cout << "El archivo no existe" << endl;
        archivo.close();
        return -1;
    }
    posIndice = dispersion( CuentaABuscar.c_str() );
    posByte = posIndice * ( ( sizeof( Pagos ) * CONTENEDOR ) + sizeof( int ) );
    archivo.seekg( posByte, ios::beg );
    archivo.read( ( char * ) &contador, sizeof( int ) );
    if( contador > 0 )
    {
        for( int i = 0; i < CONTENEDOR; i++ )
        {
            archivo.read( ( char * ) &p, sizeof( Pagos ) );
            if( strcmp( p.Cuenta, CuentaABuscar.c_str() ) == 0 )
            {
                posByte = ( long )archivo.tellg() - sizeof( Pagos );
                archivo.close();
                return posByte;
            }
        }
    }
    archivo.close();
    return -1;
}


void Pagos::setCuenta( const string &valorCuenta )
{
    int longitud = valorCuenta.size();
    longitud = ( longitud < 12 ? longitud : 11 );
    valorCuenta.copy( Cuenta, longitud );
    Cuenta[ longitud ] = '\0';
}

void Pagos::setnombreTitular( const string &valornombreTitular )
{
    int longitud = valornombreTitular.size();
    longitud = ( longitud < 35 ? longitud : 34 );
    valornombreTitular.copy( nombreTitular, longitud );
    nombreTitular[ longitud ] = '\0';
}

void Pagos::setDireccion( const string &valorDireccion )
{
    int longitud = valorDireccion.size();
    longitud = ( longitud < 35 ? longitud : 34 );
    valorDireccion.copy( Direccion, longitud );
    Direccion[ longitud ] = '\0';
}

void Pagos::setMonto( const string &valorMonto )
{
    int longitud = valorMonto.size();
    longitud = ( longitud < 12 ? longitud : 11 );
    valorMonto.copy( Monto, longitud );
    Monto[ longitud ] = '\0';
}

ostream &operator<<( ostream &salida, const Pagos &Pagos )
{
    salida << "Numero de cuenta: " << Pagos.Cuenta << endl << "Nombre del titular: " << Pagos.nombreTitular << endl << "Direccion de facturacion: " << Pagos.Direccion << endl << "Monto: " << Pagos.Monto << endl;

    return salida;
}

bool Pagos::agregar( const Pagos &nuevaPagos )
{
    Pagos p;
    string cadena;
    int posIndice, contador = 0;
    long int posByte;
    string CuentaString = nuevaPagos.Cuenta;

    if( contiene( CuentaString ) )
        return false;

    fstream archivo( "dispersion.txt", ios::in | ios::out );
    posIndice = dispersion( nuevaPagos.Cuenta );
    cout << "Se guardara en la posicion indice: " << posIndice << endl;
    posByte = posIndice * ( sizeof( Pagos ) * CONTENEDOR + sizeof( int ) );
    archivo.seekg( posByte, ios::beg );
    archivo.read( ( char * ) &contador, sizeof( int ) ); // lee el numero de registros en el contador
    if( contador < CONTENEDOR ) // si el contenedor no esta lleno
    {
// aumenta el contador y lo escribe
        contador++;
        archivo.seekg( posByte, ios::beg );
        archivo.write( ( char * ) &contador, sizeof( int ) );

// escribe el nuevo registro en el contenedor
        for( int i = 0; i < CONTENEDOR; i++ )
        {
            archivo.read( ( char * ) &p, sizeof( Pagos ) );
            if( p.Cuenta[ 0 ] == '\0' )
            {
                archivo.seekg( ( long )archivo.tellg() - sizeof( Pagos ), ios::beg );
                archivo.write( ( char * ) &nuevaPagos, sizeof( Pagos ) );
                archivo.close();
                return true;
            }
        }

    }
    else
        cout << endl << "No hay mas espacio para este registro" << endl;
    archivo.close();
    return false;
}

void Pagos::mostrar( void )
{
    Pagos p;
    int contador;


    ifstream archivo( "dispersion.txt", ios::in );
    if( !archivo )
        cout << "No existe el archivo" << endl;
    else
    {
        cout << endl;
        for( int i = 0; i < NUMREGISTROS; i++ )
        {
            archivo.read( ( char * ) &contador, sizeof( int ) );
            if( contador > 0 )
            {
                for( int j = 0; j < CONTENEDOR; j++ )
                {
                    archivo.read( ( char * ) &p, sizeof( Pagos ) );
                    if( p.Cuenta[ 0 ] != '\0' )
                        cout << p << endl;
                }
            }
            else
                archivo.seekg( sizeof( Pagos ) * CONTENEDOR, ios::cur );
        }
    }
    archivo.close();
}

bool Pagos::buscar( const string &CuentaABuscar, Pagos &PagosEncontrada )
{
    long int posByte;

    if( !contiene( CuentaABuscar ) )
        return false;

    ifstream archivo( "dispersion.txt", ios::in );
    if( !archivo )
    {
        cout << "El archivo no existe" << endl;
        archivo.close();
        return false;
    }

    posByte = buscarCuenta( CuentaABuscar );
    archivo.seekg( posByte, ios::beg );
    archivo.read( ( char * ) &PagosEncontrada, sizeof( Pagos ) );
    archivo.close();
    return true;
}

bool Pagos::modificar( const string &CuentaAModificar, const Pagos &PagosNueva )
{
    Pagos registroLimpio, p;
    int posIndiceAntiguo, posIndiceNuevo, contador;
    long int posByteAntiguo, posByteNuevo;

    if( !contiene( CuentaAModificar ) )
        return false;

    fstream archivo( "dispersion.txt", ios::in | ios::out );
    if( !archivo )
    {
        cout << "El archivo no exite" << endl;
        archivo.close();
        return false;
    }

    posIndiceAntiguo = dispersion( CuentaAModificar.c_str() );
    posByteAntiguo = buscarCuenta( CuentaAModificar );
    posIndiceNuevo = dispersion( PagosNueva.Cuenta );
    posByteNuevo = posIndiceNuevo * ( sizeof( Pagos ) * CONTENEDOR + sizeof( int ) );

    if( posByteAntiguo == posByteNuevo )
    {
        archivo.seekg( posByteAntiguo, ios::beg );
        archivo.write( ( char * ) &PagosNueva, sizeof( Pagos) );
    }
    else
    {
// quita el registro antiguo y resta uno al contador del contenedor
        archivo.seekg( posByteAntiguo, ios::beg );
        archivo.write( ( char * ) &registroLimpio, sizeof( Pagos ) );
        posByteAntiguo = posIndiceAntiguo * ( sizeof( Pagos ) * CONTENEDOR + sizeof( int ) );
        archivo.seekg( posByteAntiguo, ios::beg );
        archivo.read( ( char * ) &contador, sizeof( int ) );
        contador--;
        archivo.seekg( posByteAntiguo, ios::beg );
        archivo.write( ( char * ) &contador, sizeof( int ) );

// intenta meter el nuevo registro en la nueva posicion
        archivo.seekg( posByteNuevo, ios::beg );
        archivo.read( ( char * ) &contador, sizeof( int ) );
        if( contador < CONTENEDOR )
        {
// aumenta el contador y lo escribe
            contador++;
            archivo.seekg( posByteNuevo, ios::beg );
            archivo.write( ( char * ) &contador, sizeof( int ) );

// escribe el nuevo registro en el contenedor
            for( int i = 0; i < CONTENEDOR; i++ )
            {
                archivo.read( ( char * ) &p, sizeof( Pagos ) );
                if( p.Cuenta[ 0 ] == '\0' ) // si el lugar no esta ocupado
                {
                    archivo.seekg( ( long )archivo.tellg() - sizeof( Pagos ), ios::beg );
                    archivo.write( ( char * ) &PagosNueva, sizeof( Pagos ) );
                    archivo.close();
                    return true;
                }
            }
        }
        else // el contenedor esta lleno
            return false;
    }
} // fin funcion modificar

bool Pagos::eliminar( const string &CuentaAEliminar, Pagos &destinoEliminado )
{
    Pagos p;
    int posIndice, posByte, contador;

    if( !contiene( CuentaAEliminar ) )
        return false;

    fstream archivo( "dispersion.txt", ios::in | ios::out );
    if( !archivo )
    {
        cout << "El archivo no exite" << endl;
        archivo.close();
        return false;
    }

    posIndice = dispersion( CuentaAEliminar.c_str() );
    posByte = buscarCuenta( CuentaAEliminar );

    archivo.seekg( posByte, ios::beg );
    archivo.read( ( char * ) &destinoEliminado, sizeof( Pagos) );
    archivo.seekg( posByte, ios::beg );
    archivo.write( ( char * ) &p, sizeof( Pagos) );

    posByte = posIndice * ( sizeof( Pagos ) * CONTENEDOR + sizeof( int ) );
    archivo.seekg( posByte, ios::beg );
    archivo.read( ( char * ) &contador, sizeof( int ) );
    contador--;
    archivo.seekg( posByte, ios::beg );
    archivo.write( ( char * ) &contador, sizeof( int ) );
    archivo.close();
    return true;
}


int main( void )
{
    Pagos pagos, pagosBuscar, pagosModificar, pagosEliminar, registroAgregar;
    string CuentaABuscar, CuentaAModificar, CuentaAEliminar;
    char opcion, op;


    system( "cls" );

    do
    {
        cout<<"\n\n\t\tBienvenido al menu pagos\n\n";
        cout<<"Seleccione una opcion" << endl << "1. Agregar" << endl << "2. Mostrar" << endl << "3. Buscar" << endl << "4. Modificar" << endl << "5. Eliminar" << endl << "6. Salir" << endl << endl;
        cin >> opcion;
        switch( opcion )
        {
        case '1':
            registroAgregar = pagos.pedirRegistro();
            if( pagos.agregar( registroAgregar ) )
                cout << "Pago agregado con exito" << endl;
            else
                cout << "No se agrego el pago" << endl;
            break;

        case '2':
            system( "cls" );
            pagos.mostrar();
            break;

        case '3':
            cout << "Ingrese el numero Cuenta a buscar: ";
            fflush( stdin );
            getline( cin, CuentaABuscar );
            if( pagos.buscar( CuentaABuscar, pagosBuscar ) )
                cout << endl << pagosBuscar << endl;
            else
                cout << "No existe el registro" << endl;
            break;

        case '4':
            cout << "Ingrese el num. Cuenta a modificar: ";
            fflush( stdin );
            getline( cin, CuentaAModificar );
            if( pagos.buscar( CuentaAModificar, pagosBuscar ) )
            {
                cout << endl << pagosBuscar << endl;
                cout << "Desea modificarlo?" << endl;
                cout << "1) Si" << endl;
                cout << "2) No" << endl;
                cin >> op;

                if( op == '1' )
                {
                    pagosModificar = pagos.pedirRegistro();
                    if( pagos.modificar( CuentaAModificar,
                                         pagosModificar ) )
                        cout << endl << "pago modificado con exito" << endl;
                    else
                        cout << endl << "No se pudo modificar" << endl;
                }
            }
            else
                cout << "No existe el registro" << endl;
            break;

        case '5':
            cout << "Ingrese el num. cuenta a eliminar: ";
            fflush( stdin );
            getline( cin, CuentaAEliminar );
            if( pagos.buscar( CuentaAEliminar, pagosBuscar ) )
            {
                cout << endl << pagosBuscar << endl;
                cout << "Desea eliminarlo?" << endl;
                cout << "1) Si" << endl;
                cout << "2) No" << endl;
                cin >> op;

                if( op == '1' )
                {
                   if( pagos.eliminar( CuentaAEliminar, pagosEliminar ) )
                        cout << endl << pagosEliminar << endl << "SE ELIMINO CORRECTAMENTE" << endl;
                }
                else
                cout << "NO SE PUDO ELIMINAR" << endl;
            }
            else
                cout << "No existe el registro" << endl;
            break;
        }
        cout << endl;
        if(opcion != '6')
        {
            system("Pause");
            system( "cls" );
        }
    }
    while(opcion != '6');

    return 0;

}

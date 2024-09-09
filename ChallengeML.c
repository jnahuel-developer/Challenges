/* ------------------------------------ */
/*          SECCION DE INCLUDES         */
/* ------------------------------------ */

// Se definen los includes necesarios para el programa
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>



/* ------------------------------------ */
/*      SECCION DE DEFINES Y DATOS      */
/* ------------------------------------ */

// Se declaran los defines necesarios para generar un codigo mas legible
#define CANTIDAD_DE_ARGUMENTOS_MINIMA       2
#define DURACION_DEL_CICLO_EN_DIAS          360
#define DURACION_DE_UN_ANIO_EN_DIAS         365
#define CANTIDAD_DE_PLANETAS                3
#define VELOCIDAD_PLANETA_1                 -1
#define VELOCIDAD_PLANETA_2                 5
#define VELOCIDAD_PLANETA_3                 -3
#define VELOCIDAD_PLANETA_DEFAULT           1
#define DISTANCIA_PLANETA_1                 500
#define DISTANCIA_PLANETA_2                 1000
#define DISTANCIA_PLANETA_3                 2000
#define DISTANCIA_PLANETA_DEFAULT           1
#define COORDENADA_X_DEFAULT                0
#define COORDENADA_Y_DEFAULT                0
#define TOLERANCIA_PARA_ALINEACION          0.02
#define K_GRADOS_A_RADIANES                 M_PI / 180.0
#define CUATRO_DECIMALES                    4
#define DOS_DECIMALES                       2

// Se definen los codigos de error para generar un codigo mas legible
typedef enum
{
    ERR_NO_ARGUMENTO = 1,       // Codigo de error para la falta de argumento
    ERR_NUMERO_INVALIDO         // Codigo de error para un numero no valido
} CodigoError;

// Se define una estructura para representar una coordenada
typedef struct
{
    double x;                   // Coordenada X
    double y;                   // Coordenada Y
} Coordenada;

// Se define una estructura para representar un planeta
typedef struct
{
    int velocidad;              // Velocidad angular del planeta
    int distancia;              // Distancia respecto al sol del planeta
    Coordenada coordenada;      // Coordenadas del planeta
} Planeta;

// Se define un vector global de planetas
Planeta planetas[ CANTIDAD_DE_PLANETAS ];

// Se define un vector global para las pendientes entre los planetas
double pendientes[ CANTIDAD_DE_PLANETAS ];


/* ------------------------------------ */
/*          SECCION DE PROTOTIPOS       */
/* ------------------------------------ */

// Se define el prototipo para la funcion que inicia el sistema planetario
void iniciarSistemaPlanetario ( void );

// Se define el prototipo para la funcion que obtiene la posicion de todos los planetas
void obtenerPosicionDeLosPlanetas ( int dia );

// Se define el prototipo para la funcion que obtiene la posicion de un planeta en particular
void obtenerPosicionDelPlaneta ( int dia, int planeta );

// Se define el prototipo para la funcion que valida si los planetas estan alineados
bool validarAlineacionDeLosPlanetas ( int dia );

// Se define el prototipo para la funcion que calcula la pendiente entre los planetas, para validar su alineacion
bool calcularPendientes ( int dia );

// Se define el prototipo para la funcion que valida si se incluye al sol cuando forman un triangulo
bool incluyenAlSolEnElTriangulo ( void );

// Se define el prototipo para la funcion que calcula el perimetro del triangulo que incluye al sol
double obtenerPerimetroDelTrianguloFormado ( void );

// Se define el prototipo para la funcion que valida si se incluye al sol en la alineacion
bool validarAlineacionConElSol ( void );

// Se define el prototipo para la funcion que calcula la distancia entre 2 coordenadas
double distanciaEntreDosPuntos( Coordenada PuntoA, Coordenada PuntoB );

// Se define el prototipo para la funcion que recorta la cantidad de decimales en un double
double redondearDecimales ( double numero, int decimales );





/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */

// Se define la funcion principal
int main( int argc, char *argv[] )
{
    /* ******************************************************************************************** */
    /*                          PRIMER BLOQUE - VALIDACIONES                                        */
    /* ******************************************************************************************** */

    // Se verifica si se ha pasado al menos un argumento
    if( argc < CANTIDAD_DE_ARGUMENTOS_MINIMA )
    {
        // Se imprime un mensaje de indicacion sobre como usar el programa
        printf( "Uso: %s <anios>\n", argv[0] );

        // Se retorna el codigo de error correspondiente
        return ERR_NO_ARGUMENTO;
    }

    // Se convierte el argumento a entero
    int anios = atoi( argv[1] );

    // Se verifica si la cantidad de anios suministrada es un numero positivo mayor a cero
    if( anios <= 0 )
    {
        // Se imprime un mensaje de indicacion sobre como usar el programa
        printf( "La cantidad de anios debe ser positiva y mayor a cero.\n" );

        // Se retorna el codigo de error correspondiente
        return ERR_NUMERO_INVALIDO;
    }


    /* *********************************************************************************************** */
    /*                          SEGUNDO BLOQUE - FUNCIONAMIENTO                                        */
    /* *********************************************************************************************** */

    /* Se tienen 3 planetas que giran a velocidades angulares de 1, 3 y 5 grados por dia.               */
    /* Por lo tanto, se tiene un ciclo que tarda 360 dias en completarse y volver a repetirse.          */
    /* Esto permite que se calculen todas las condiciones en un loop de 360 dias, y luego se            */
    /* multiplique por la cantidad de anios solicitados.                                                */
    /* Finalmente, restara calcular el aporte de los 5 dias residuales por cada anio.                   */
    /* Como primer opcion, el programa supone que los 3 planetas parten del origen en el inicio.        */
    /* En una siguiente version, se podria plantear que el programa reciba la posicion inicial de los   */
    /* 3 planetas como punto de partida, o un flag para inciarlos aleatoriamente.                       */

    // Se definen las variables a utilizar
    int     periodosDeCondicionesOptimas = 0;
    int     periodosDeSequia = 0;
    int     periodosDeLluvia = 0;
    int     diaDelPerimetroMaximo = 0;
    int     dia = 0;
    int     diasDelCicloIncompleto;
    int     ciclosCompletos;
    double  perimetroActual = 0;
    double  perimetroMaximo = 0;

    // Se calcula el total de dias que hay en los anios solicitados
    dia = anios * DURACION_DE_UN_ANIO_EN_DIAS;

    // Se obtiene la cantidad de veces que se repite el ciclo en el total de anios solicitados
    ciclosCompletos = dia / DURACION_DEL_CICLO_EN_DIAS;

    // Se obtienen los dias del ciclo que pueda quedar incompleto
    diasDelCicloIncompleto = dia % DURACION_DEL_CICLO_EN_DIAS;

    // Se inician los datos de los planetas en uso
    iniciarSistemaPlanetario();

    // Se recorren los dias hasta completar un ciclo
    for( dia = 0; dia < DURACION_DEL_CICLO_EN_DIAS; dia++ )
    {
        // Se debe calcular la posicion de cada planeta para cada dia
        obtenerPosicionDeLosPlanetas( dia );

        // Se debe verificar si los planetas no se encuentran alineados
        if( !validarAlineacionDeLosPlanetas( dia ) )
        {
            // Se debe verificar si incluyen al sol dentro del triangulo formado
            if( incluyenAlSolEnElTriangulo() )
            {
                // Se debe incrementar el contador de periodos de lluvia
                periodosDeLluvia++;

                // Se calcula el perimetro del triangulo formado
                perimetroActual = obtenerPerimetroDelTrianguloFormado();

                // Se verifica si se alcanzo un nuevo maximo
                if( perimetroActual > perimetroMaximo )
                {
                    // Se actualiza el valor del perimetro maximo alcanzado
                    perimetroMaximo = perimetroActual;

                    // Se actualiza el indicador del dia en que se genero un nuevo maximo
                    diaDelPerimetroMaximo = dia;
                }
            }
        }
        // Caso contrario, estan alineados, y debe verificarse si incluyen al sol o no
        else
        {
            // Se debe verificar si no incluyen al sol en la alineacion
            if( !validarAlineacionConElSol() )
            {
                // Se debe incrementar el contador de periodos de condiciones optimas de presion y temperatura
                periodosDeCondicionesOptimas++;
            }
            // Caso contrario, si se incluye al sol en la alineacion, y se trata de un periodo de sequia
            else
            {
                // Se debe incrementar el contador de periodos de sequia
                periodosDeSequia++;
            }
        }
    }

    // Se multiplican los resultados obtenidos por la cantidad de ciclos completos
    periodosDeSequia *= ciclosCompletos;
    periodosDeCondicionesOptimas *= ciclosCompletos;
    periodosDeLluvia *= ciclosCompletos;

    // Se genera un nuevo loop para completar los dias restantes. No se actualiza el perimetro maximo, ya que las distancias de los planetas no se modifican
    for( dia = 0; dia < diasDelCicloIncompleto; dia++ )
    {
        // Se debe calcular la posicion de cada planeta para cada dia
        obtenerPosicionDeLosPlanetas( dia );

        // Se debe verificar si los planetas no se encuentran alineados
        if( !validarAlineacionDeLosPlanetas( dia ) )
        {
            // Se debe verificar si incluyen al sol dentro del triangulo formado
            if( incluyenAlSolEnElTriangulo() )
            {
                // Se debe incrementar el contador de periodos de lluvia
                periodosDeLluvia++;
            }
        }
        // Caso contrario, estan alineados, y debe verificarse si incluyen al sol o no
        else
        {
            // Se debe verificar si no incluyen al sol en la alineacion
            if( !validarAlineacionConElSol() )
            {
                // Se debe incrementar el contador de periodos de condiciones optimas de presion y temperatura
                periodosDeCondicionesOptimas++;
            }
            // Caso contrario, si se incluye al sol en la alineacion, y se trata de un periodo de sequia
            else
            {
                // Se debe incrementar el contador de periodos de sequia
                periodosDeSequia++;
            }
        }
    }



    // Se imprimen los resultados
    printf( "En total hay %d periodos de sequia en el tiempo indicado\n", periodosDeSequia );
    printf( "En total hay %d periodos de sequia con condiciones optimas de presion y temperatura\n", periodosDeCondicionesOptimas );
    printf( "En total hay %d periodos de lluvia en el tiempo indicado\n", periodosDeLluvia );
    printf( "El dia %d se alcanzo el maximo perimetro, de valor %f\n", diaDelPerimetroMaximo, perimetroMaximo );

    // Fin del programa main
    return 0;
}





/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: iniciarSistemaPlanetario                                                                    */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - void                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - void                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Funcion que inicia el vector de planetas, definiendo las velocidades y distancias de cada planeta   */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
void iniciarSistemaPlanetario ( void )
{
    // Se define una variable para recorrer el vector de planetas
    int planeta;

    // Se recorre el vector de planetas, definiendo la velocidad de cada uno y su posicion inicial
    for( planeta = 0; planeta < CANTIDAD_DE_PLANETAS; planeta++ )
    {
        // Se genera un switch para definir las condiciones particulares de cada planeta
        switch( planeta )
        {
            // Se definen las condiciones para el primer planeta
            case 0:
                // Se define la velocidad
                planetas[ planeta ].velocidad = VELOCIDAD_PLANETA_1;
                // Se define la distancia
                planetas[ planeta ].distancia = DISTANCIA_PLANETA_1;
                break;
            // Se definen las condiciones para el segundo planeta
            case 1:
                // Se define la velocidad
                planetas[ planeta ].velocidad = VELOCIDAD_PLANETA_2;
                // Se define la distancia
                planetas[ planeta ].distancia = DISTANCIA_PLANETA_2;
                break;
            // Se definen las condiciones para el tercer planeta
            case 2:
                // Se define la velocidad
                planetas[ planeta ].velocidad = VELOCIDAD_PLANETA_3;
                // Se define la distancia
                planetas[ planeta ].distancia = DISTANCIA_PLANETA_3;
                break;
            // Se definen las condiciones por default para cualquier otro planeta
            default:
                // Se define la velocidad
                planetas[ planeta ].velocidad = VELOCIDAD_PLANETA_DEFAULT;
                // Se define la distancia
                planetas[ planeta ].distancia = DISTANCIA_PLANETA_DEFAULT;
                break;
        }
    }
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: obtenerPosicionDeLosPlanetas                                                                */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - int dia -> El dia dentro del ciclo en el cual debe calcular las posiciones                        */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - void                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Funcion que recibe el dia dentro del ciclo, para el cual debe calcular la posicion de todos los     */
/*  planetas del sistema en cuestion.                                                                   */
/*  Simplemente recorre un loop donde llama a la funcion que calcula la posicion especifica de un       */
/*  planeta en particular.                                                                              */
/*  Se plantea de esta forma, porque asi sera mas facil la escalabilidad si se quieren utilizar mas     */
/*  planetas dentro del sistema en analisis.                                                            */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
void obtenerPosicionDeLosPlanetas ( int dia )
{
    // Se define una variable para recorrer la cantidad de planetas en uso
    static int planeta;
    
    // Se recorren todos los planetas con un loop
    for( planeta = 0; planeta < CANTIDAD_DE_PLANETAS; planeta++ )
    {
        // Se calcula la posicion de cada planeta en particular
        obtenerPosicionDelPlaneta( dia, planeta );
    }
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: obtenerPosicionDelPlaneta                                                                   */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - int dia -> El dia dentro del ciclo en el cual debe calcular la posicion                           */
/*  - int planeta -> El planeta para el cual debe calcular su posicion                                  */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - void                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Funcion que recibe el dia dentro del ciclo, para el cual debe calcular la posicion del planeta      */
/*  indicado.                                                                                           */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
void obtenerPosicionDelPlaneta ( int dia, int planeta )
{
    // Se define una variable para pasar el angulo de grados a radianes
    static double anguloEnRadianes;

    // Se convierte el angulo de grados a radianes
    anguloEnRadianes = ( dia * planetas[ planeta ].velocidad ) * K_GRADOS_A_RADIANES;

    // Se debe calcular la posicion especifica del planeta indicado, redondeado a 4 decimales
    planetas[ planeta ].coordenada.x = planetas[ planeta ].distancia * redondearDecimales( cos( anguloEnRadianes ), CUATRO_DECIMALES );
    planetas[ planeta ].coordenada.y = planetas[ planeta ].distancia * redondearDecimales( sin( anguloEnRadianes ), CUATRO_DECIMALES );
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: validarAlineacionDeLosPlanetas                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - int dia -> El dia dentro del ciclo en el cual debe calcular la posicion                           */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - bool -> "true" si los planetas se encuentran alineados                                            */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Primero se verifica que la pendiente entre 2 planetas no sea una linea vertical. Esto es para       */
/*  evitar una division por cero. De ser una vertical, ambas pendientes deben serlo, ya que comparten   */
/*  un punto en comun. Si solo una lo es, entonces no estan alineados.                                  */
/*  De no ser verticales, se llama a la funcion que calcula las pendientes y se retorna su resultado.   */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
bool validarAlineacionDeLosPlanetas ( int dia )
{
    // Se verifica si alguna de las pendientes es vertical, para evitar una division por cero
    if( ( ( planetas[ 1 ].coordenada.x - planetas[ 0 ].coordenada.x ) == 0 ) || ( ( planetas[ 2 ].coordenada.x - planetas[ 0 ].coordenada.x ) == 0 ) )
    {
        // En el caso de que alguna sea vertical, ambas deben serlo para estar alineadas
        if( ( ( planetas[ 1 ].coordenada.x - planetas[ 0 ].coordenada.x ) == 0 ) && ( ( planetas[ 2 ].coordenada.x - planetas[ 0 ].coordenada.x ) == 0 ) )
        {
            // Se indica que los planetas estan alineados
            return true;
        }

        // Se indica que los planetas no estan alineados
        return false;
    }

    // En los demas casos, se deben calcular las pendientes entre los planetas y compararlas
    return( calcularPendientes( dia ) );
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: calcularPendientes                                                                          */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - int dia -> El dia dentro del ciclo en el cual debe calcular las pendientes                        */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - bool -> "true" si los planetas se encuentran alineados                                            */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Calcula la pendiente entre los planetas, y verifica si estan alineados. Para esto, deberian tener   */
/*  el mismo valor de pendiente entre los 3 planetas, pero basta con comparar solo 2                    */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
bool calcularPendientes ( int dia )
{
    // Se definen variables para no utilizar directamente los vectores
    static double x1, x2, x3, y1, y2, y3;
    static double limiteSuperior;
    static double limiteInferior;

    // Se toman las coordenadas de los 3 planetas
    x1 = planetas[ 0 ].coordenada.x;
    x2 = planetas[ 1 ].coordenada.x;
    x3 = planetas[ 2 ].coordenada.x;
    y1 = planetas[ 0 ].coordenada.y;
    y2 = planetas[ 1 ].coordenada.y;
    y3 = planetas[ 2 ].coordenada.y;

    // Se otiene la pendiente entre el planeta 1 ( Ferengi ) y el planeta 2 ( Vulcano )
    pendientes[ 0 ] = redondearDecimales( ( y2 - y1 ) / ( x2 - x1 ), DOS_DECIMALES );

    // Se otiene la pendiente entre el planeta 2 ( Vulcano ) y el planeta 3 ( Betasoide )
    pendientes[ 1 ] = redondearDecimales( ( y3 - y2 ) / ( x3 - x2 ), DOS_DECIMALES );

    // Se otiene la pendiente entre el planeta 1 ( Ferengi ) y el planeta 3 ( Betasoide )
    pendientes[ 2 ] = redondearDecimales( ( y3 - y1 ) / ( x3 - x1 ), DOS_DECIMALES );

    // Se calculan los margenes de tolerancia aceptables para comparar las pendientes
    limiteSuperior = pendientes[ 1 ] * ( 1 + TOLERANCIA_PARA_ALINEACION );
    limiteInferior = pendientes[ 1 ] * ( 1 - TOLERANCIA_PARA_ALINEACION );

    // Se verifica si los planetas estan alineados, comparando las pendientes, ya que comparten 1 punto en comun
    if( ( pendientes[ 0 ] >= limiteInferior ) && ( pendientes[ 0 ] <= limiteSuperior ) )
    {
        // Se indica que los planetas estan alineados
        return true;
    }
    // Caso contrario, no estan alineados
    else
    {
        // Se indica que los planetas no estan alineados
        return false;
    }
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: incluyenAlSolEnElTriangulo                                                                  */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - void                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - bool -> "true" si el sol esta dentro del triangulo                                                */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Al tener las 3 pendientes ya calculadas, se aplica el teorema de Bolzano para determinar si el      */
/*  origen esta incluido. Esto significa, que si la proyeccion de la recta del planeta 3 contra el 2    */
/*  corta por un lado del eje Y en el origen, mientras que la recta del planeta 3 contra el 1 corta por */
/*  el otro lado del origen, entonces el centro esta incluido en el angulo formado por los planetas,    */
/*  con vertice en el planeta 3.                                                                        */
/*  Solo resta hacer lo mismo con el angulo formado con vertice en el planeta 1, para corroborar que el */
/*  origen este incorporado dentro del triangulo.                                                       */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
bool incluyenAlSolEnElTriangulo ( void )
{
    // Se definen variables para no utilizar directamente los vectores
    static double x1, x2, x3, y1, y2, y3;
    static double ordenadaAlOrigen_32, ordenadaAlOrigen_31, ordenadaAlOrigen_21;

    // Se toman las coordenadas de los 3 planetas
    x1 = planetas[ 0 ].coordenada.x;
    x2 = planetas[ 1 ].coordenada.x;
    x3 = planetas[ 2 ].coordenada.x;
    y1 = planetas[ 0 ].coordenada.y;
    y2 = planetas[ 1 ].coordenada.y;
    y3 = planetas[ 2 ].coordenada.y;

    // Se verifica si el signo de las 3 coordenadas en Y es igual, para saber que el sol no esta incluido por estar en 2 cuadrantes contiguos
    if( ( ( y1 > 0 ) && ( y2 > 0 ) && ( y3 > 0 ) ) || ( ( y1 < 0 ) && ( y2 < 0 ) && ( y3 < 0 ) ) )
    {
        // Se indica que el sol no esta incluido
        return false;
    }

    // Se verifica si el signo de las 3 coordenadas en X es igual, para saber que el sol no esta incluido por estar en 2 cuadrantes contiguos
    if( ( ( x1 > 0 ) && ( x2 > 0 ) && ( x3 > 0 ) ) || ( ( x1 < 0 ) && ( x2 < 0 ) && ( x3 < 0 ) ) )
    {
        // Se indica que el sol no esta incluido
        return false;
    }

    // Se calculan las ordenadas al origen de las rectas proyectadas entre el planeta 3 y el 2, y entre el planeta 3 y el 1
    ordenadaAlOrigen_32 = y2 - pendientes[ 1 ] * x2;
    ordenadaAlOrigen_31 = y1 - pendientes[ 2 ] * x1;

    // Para que el origen este incluido, es necesario que ambas ordenadas tengan signos opuestos (teorema de Bolzano)
    if( ( ( ordenadaAlOrigen_32 < 0 ) && ( ordenadaAlOrigen_31 > 0 ) ) || ( ( ordenadaAlOrigen_32 > 0 ) && ( ordenadaAlOrigen_31 < 0 ) ) )
    {
        // Se calcula la ordenada al origen de la recta proyectada entre el planeta 2 y el 1
        ordenadaAlOrigen_21 = pendientes[ 0 ] * x1 - y1;

        // Para que el origen este incluido, tiene que tener el signo contrario a la ordenada de la recta proyectada entre el planeta 3 y el 1
        if( ( ( ordenadaAlOrigen_31 < 0 ) && ( ordenadaAlOrigen_21 > 0 ) ) || ( ( ordenadaAlOrigen_31 > 0 ) && ( ordenadaAlOrigen_21 < 0 ) ) )
        {
            // El origen se encuentra incluido
            return true;
        }
        // Caso contrario, el origen no esta incluido en el triangulo
        return false;

    }
    // Caso contrario, el origen no esta incluido en el triangulo
    return false;
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: obtenerPerimetroDelTrianguloFormado                                                         */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - void                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - double -> El perimetro calculado                                                                  */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Calcula el perimetro del triangulo formado por los 3 planetas.                                      */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
double obtenerPerimetroDelTrianguloFormado ( void )
{
    // Se define una variable para llevar la sumatoria y que el codigo sea mas legible
    double perimetro;

    // Se obtienen las distancias
    perimetro = distanciaEntreDosPuntos( planetas[ 0 ].coordenada, planetas[ 1 ].coordenada );
    perimetro += distanciaEntreDosPuntos( planetas[ 0 ].coordenada, planetas[ 2 ].coordenada );
    perimetro += distanciaEntreDosPuntos( planetas[ 1 ].coordenada, planetas[ 2 ].coordenada );

    // Se devuelve el perimetro calculado
    return perimetro;
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: validarAlineacionConElSol                                                                   */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - void                                                                                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - bool -> "true" si los planetas se encuentran alineados con el sol                                 */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Se verifica primero si alguna de las coordenadas es nula, ya que eso indica que incluyen al sol.    */
/*  Caso contrario, se calcula la ordenada al origen con la pendiente entre el primer y el segundo      */
/*  planeta. Se verifica que sea nula para incluir al origen.                                           */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
bool validarAlineacionConElSol ( void )
{
    static double ordenadaAlOrigen;

    // Se verifica si alguna de las coordenadas es nula, porque indicaria que la pendiente pasa por el origen
    if( ( planetas[ 0 ].coordenada.x == 0 ) || ( planetas[ 1 ].coordenada.x == 0 ) || ( planetas[ 2 ].coordenada.x == 0 ) || ( planetas[ 0 ].coordenada.y == 0 ) || ( planetas[ 1 ].coordenada.y == 0 ) || ( planetas[ 2 ].coordenada.y == 0 ) )
    {
        // Se indica que los planetas estan alineados con el sol
        return true;
    }

    // En cualquier otro caso, se debe verificar con la pendiente
    ordenadaAlOrigen = planetas[ 0 ].coordenada.y - pendientes[ 0 ] * planetas[ 0 ].coordenada.x;

    // Si la ordenada al origen es distinta de cero, entonces no estan alineados con el origen
    if( ordenadaAlOrigen == 0 )
    {
        // Se indica que los planetas estan alineados con el sol
        return true;
    }

    // Se indica que los planetas no estan alineados con el sol
    return false;
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: distanciaEntreDosPuntos                                                                     */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - Coordenada PuntoA, Coordenada PuntoB -> Las coordenadas de los puntos a medir la distancia        */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - double -> El resultado de la distancia entre las coordenadas pasadas                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Se calcula la distancia geometrica entre las coordenadas indicadas.                                 */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
double distanciaEntreDosPuntos ( Coordenada PuntoA, Coordenada PuntoB )
{
    // Se retorna la distancia, calculada como la raiz de las potencias de las diferencias
    return sqrt( pow( PuntoA.x - PuntoB.x, 2 ) + pow( PuntoA.y - PuntoB.y, 2 ) );
}



/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Funcion: redondearDecimales                                                                          */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
/* Recibe:                                                                                              */
/*  - double numero -> Numero a recortar                                                                */
/*  - int decimales -> Cantidad de decimales a dejar                                                    */
/* ---------------------------------------------------------------------------------------------------- */
/* Devuelve:                                                                                            */
/*  - double -> El resultado de la distancia entre las coordenadas pasadas                              */
/* ---------------------------------------------------------------------------------------------------- */
/* Descripcion:                                                                                         */
/*  Se recortan los decimales de un double al digito indicado                                           */
/* ---------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------- */
double redondearDecimales ( double numero, int decimales )
{
    // Se define una variable para calcular la potencia de 10, que se usa para recortar decimales
    static int factor;

    // Se calcula el factor para recortar los decimales
    factor = pow( 10, decimales );

    // Se devuelve el valor con los decimales recortados
    return( round( numero * factor ) / factor );
}
#include "restaurant.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define ROJO "\e[31m"
#define AMARILLO "\e[33m"
#define GRIS "\e[90m"
#define CIAN "\e[36m"
#define MAGENTA "\e[35m"
#define RESET "\e[0m"

#define ULTIMA_FILA 19
#define PRIMERA_FILA 0
#define ULTIMA_COLUMNA 19
#define PRIMERA_COLUMNA 0

#define POSICION_0_MESA_4 0
#define POSICION_1_MESA_4 1
#define POSICION_2_MESA_4 2
#define POSICION_3_MESA_4 3

#define POSICION_MOPA 0

#define PEDIDO_A_COCINAR 0
#define PEDIDO_A_ENTREGAR 0

const char MOZO = 'L';
const char COCINA = 'C';
const char MESA = 'T';
const char MESA_OCUPADA = 'X';
const char MESA_OCUPADA_TOMADA = 'X';
const char MONEDAS = 'M';
const char MOPA = 'O';
const char CHARCOS = 'H';
const char PATINES = 'P';
const char CUCARACHA = 'U';

const char MOVER_ADELANTE = 'W';
const char MOVER_ATRAS = 'S';
const char MOVER_DERECHA = 'D';
const char MOVER_IZQUIERDA = 'A';
const char AGARRAR_MOPA = 'O';
const char TOMAR_PEDIDO = 'T';
const char ACTIVAR_PATINES = 'P';

const char SEPARADOR_CASILLEROS = '|';
const char LUGAR_VACIO = ' ';

const int MAX_POS = 20;
const int MIN_POS = 0;
const int INDICE_MAXIMO = 4;
const int INDICE_MINIMO = 1;
const int MAX_PACIENCIA_INICIAL = 200;
const int MIN_PACIENCIA_INICIAL = 100;

const int MAX_DINERO = 150000;
const int MAX_MOVIMIENTOS = 200;
const int MONTO_MESA_4 = 20000;
const int MONTO_MESA_IND = 5000;
const int DINERO_MONEDA = 1000;

const char MILANESA_NAPO = 'M';
const char HAMBURGUESA = 'H';
const char PARRILLA = 'P';
const char RATATOUILLE = 'R';

const int PREPARACION_MILANESA_NAPO = 30;
const int PREPARACION_HAMBURGUESA = 15;
const int PREPARACION_PARRILLA = 20;
const int PREPARACION_RATATOUILLE = 25;

const int INDICE_MILANESA_NAPO = 1;
const int INDICE_HAMBURGUESA = 2;
const int INDICE_PARRILLA = 3;
const int INDICE_RATATOUILLE = 4;

const int TOPE_MESA_4 = 4;
const int TOPE_LUGARES_MESA_4 = 4;
const int TOPE_MESA_IND = 10;
#define TOPE_LUGARES_MESA_IND 1
const int TOPE_MOPA = 1;
const int TOPE_MONEDAS = 9;
const int TOPE_PATINES = 14;
const int TOPE_CHARCOS = 5;

const int DISTANCIA_MAXIMA_TOMAR_PEDIDO = 1;
const int NUEVA_CUCARACHA = 25;
const int DISTANCIA_INACEPTABLE_CUCARACHA = 2;
const int PACIENCIA_POR_CUCARACHA = 3;

const int JUEGO_INICIADO = 0;
const int JUEGO_GANADO = 1;
const int JUEGO_PERDIDO = -1;

const int SIN_MESAS = 0;
const int SIN_HERRAMIENTAS = 0;
const int SIN_OBSTACULOS = 0;
const int SIN_DINERO = 0;

const int SIN_PACIENCIA = 0;
const int SIN_COMENSALES = 0;
const int NUEVOS_COMENSALES = 15;
const int SIN_COMENSALES_A_ASIGNAR = 0;
const int COMENSALES_INICIALES = 1;

const int SIN_PLATOS_LISTOS = 0;
const int SIN_PLATOS_EN_PREPARACION = 0;

const int SIN_PEDIDOS = 0;
const int SIN_PEDIDOS_EN_BANDEJA = 0;
const int SIN_PATINES = 0;
const int PEDIDO_LISTO = 0;

const int CANTIDAD_LUGARES_MESA_IND = 1;

const int MOVIMIENTOS_INICIALES = 0;

// Pre: -
// Post: Itera por cada fila y columna de la matriz, asignandole el valor 'LUGAR_VACIO'.
void inicializar_terreno(char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            terreno[i][j] = LUGAR_VACIO;
        }
    }
}

void inicializar_cantidades_iniciales(juego_t *juego){
    juego->cantidad_mesas = SIN_MESAS;
    juego->cantidad_herramientas = SIN_HERRAMIENTAS;
    juego->cantidad_obstaculos = SIN_OBSTACULOS;
    juego->movimientos = MOVIMIENTOS_INICIALES;
    juego->dinero = SIN_DINERO;
}

// Pre: -
// Post: Genera un número aleatorio entre 0 y 19 que cumplira el rol de la posicion fila o columna de un elemento según la llamada.
int generar_posicion_elemento(){
    int posicion_elemento_random = rand () % MAX_POS + MIN_POS;
    return posicion_elemento_random;
}

// Pre: La matriz terreno de MAX_FILAS y MAX_COLUMNAS y la posicion de las mesas de 4 lugares deben estar previamente inicializados.
// Post: Devuelve true si la matriz 4x4, que comprende en su interior la mesa 2x2 mas sus bordes, esten libres de mesas, que la mesa 2x2 no se inicialice pisando algún objeto y que la mesa
// de 4 lugares este completa en el terreno y no cortada.
bool es_valida_posicion_mesa_4(char terreno[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_mesa_0){
    bool es_valida = true;
    for(int i = posicion_mesa_0.fil-1; i <= posicion_mesa_0.fil+2; i++){
        for(int j = posicion_mesa_0.col-1; j <= posicion_mesa_0.col+2; j++){
                if(terreno[i][j] == MESA){
                    es_valida = false;
                }
                else if(i > ULTIMA_FILA || j > ULTIMA_COLUMNA){
                    es_valida = false;
                }
        }
    }

    for(int i = posicion_mesa_0.fil; i <= posicion_mesa_0.fil+1 && es_valida; i++){
        for(int j = posicion_mesa_0.col; j <= posicion_mesa_0.col+1 && es_valida; j++){
            if(terreno[i][j] != LUGAR_VACIO){
                es_valida = false;
            } 
        }
    }

    return es_valida;
}

// Pre: La matriz terreno debe estar previamente incializada.
// Post Inicializa 4 mesas de 4 lugares cada una, guardandolas en sus respectivos lugares en *juego, verificando que no se pisen entre ellas y esten almenos, a una distancia de un bloque
// cada una en toda su circunferencia al asignar sus lugares en la matriz terreno. La primer posicion se generara aleatoriamente para que las otras tres sean inicializadas en función de
// esta ultima.
void inicializar_mesa_cuatro(mesa_t *mesa, char terreno[MAX_FILAS][MAX_COLUMNAS], int *cant_mesas){
    for(int i = 0; i < TOPE_MESA_4; i++){
        mesa[i].cantidad_lugares = TOPE_LUGARES_MESA_4;
        mesa[i].paciencia = SIN_PACIENCIA;
        mesa[i].cantidad_comensales = SIN_COMENSALES;
        mesa[i].pedido_tomado = false;

        mesa[i].posicion[POSICION_0_MESA_4].fil = generar_posicion_elemento();
        mesa[i].posicion[POSICION_0_MESA_4].col = generar_posicion_elemento();

        while (!es_valida_posicion_mesa_4(terreno, mesa[i].posicion[POSICION_0_MESA_4])){
            mesa[i].posicion[POSICION_0_MESA_4].fil = generar_posicion_elemento();
            mesa[i].posicion[POSICION_0_MESA_4].col = generar_posicion_elemento();
        }

        int posicion_0_fil = mesa[i].posicion[POSICION_0_MESA_4].fil;
        int posicion_0_col = mesa[i].posicion[POSICION_0_MESA_4].col;

        mesa[i].posicion[POSICION_1_MESA_4].fil = posicion_0_fil;
        mesa[i].posicion[POSICION_1_MESA_4].col = (posicion_0_col + 1);

        mesa[i].posicion[POSICION_2_MESA_4].fil = (posicion_0_fil + 1);
        mesa[i].posicion[POSICION_2_MESA_4].col = posicion_0_col;

        mesa[i].posicion[POSICION_3_MESA_4].fil = (posicion_0_fil + 1);
        mesa[i].posicion[POSICION_3_MESA_4].col = (posicion_0_col + 1);

        for(int j = 0; j < TOPE_LUGARES_MESA_4; j++){
            terreno[mesa[i].posicion[j].fil][mesa[i].posicion[j].col] = MESA;
        }
        (*cant_mesas)++;
    }
}

// Pre: La matriz terreno de MAX_FILAS y MAX_COLUMNAS y la posicion de las mesas de 1 lugar deben estar previamente inicializados.
// Post: Devuelve true si esta ubicada en un lugar vacio y si no hay mesas en todo su borde de 1 bloque de grosor.
bool es_valida_posicion_mesas_inidividuales(char terreno[MAX_FILAS][MAX_COLUMNAS], coordenada_t posicion_mesa_ind){
    bool es_valida = true;
    for(int i = posicion_mesa_ind.fil-1; i <= posicion_mesa_ind.fil+1; i++){
        for(int j = posicion_mesa_ind.col-1; j <= posicion_mesa_ind.col+1; j++){
            if(i >= PRIMERA_FILA && i <= ULTIMA_FILA && j >= PRIMERA_COLUMNA && j <= ULTIMA_COLUMNA){
                if(terreno[i][j] == MESA){
                    es_valida = false;
                }
            }
        }
    }

    if(terreno[posicion_mesa_ind.fil][posicion_mesa_ind.col] != LUGAR_VACIO){
        es_valida = false;
    }
    return es_valida;
}

// Pre: La matriz terreno debe estar previamente incializada.
// Post: Inicializa 6 mesas de 1 lugar cada una, guardandolas en sus respectivos lugares en *juego, verificando que no se pisen entre ellas y que mantengan distancia de un bloque cada una
// no solo en las mesas de 1 lugar si no en las de 4 lugares inicializadas previamente tambien al asignar sus lugares en la matriz terreno.
void inicializar_mesa_individual(mesa_t *mesa, char terreno[MAX_FILAS][MAX_COLUMNAS], int *cantidad_mesas){
    for(int i = TOPE_MESA_4; i < TOPE_MESA_IND; i++){
        mesa[i].cantidad_lugares = TOPE_LUGARES_MESA_IND;
        mesa[i].paciencia = SIN_PACIENCIA;
        mesa[i].cantidad_comensales = SIN_COMENSALES;
        mesa[i].pedido_tomado = false;

        mesa[i].posicion[mesa[i].cantidad_lugares-1].fil = generar_posicion_elemento();
        mesa[i].posicion[mesa[i].cantidad_lugares-1].col = generar_posicion_elemento();
        while(!es_valida_posicion_mesas_inidividuales(terreno, mesa[i].posicion[mesa[i].cantidad_lugares-1])){
            mesa[i].posicion[mesa[i].cantidad_lugares-1].fil = generar_posicion_elemento();
            mesa[i].posicion[mesa[i].cantidad_lugares-1].col = generar_posicion_elemento();
        }

        terreno[mesa[i].posicion[mesa[i].cantidad_lugares-1].fil][mesa[i].posicion[mesa[i].cantidad_lugares-1].col] = MESA;

        (*cantidad_mesas)++;        
    }
}

// Pre: La matriz terreno de MAX_FILAS y MAX_COLUMNAS y la posicion del elemento o elementos deben estar previamente inicializados.
// Post: Devuelve true si la posicion del elemento no se pisa con ningun otro en la matriz terreno
bool posicion_elemento_valida(coordenada_t posicion_elemento, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    return(terreno[posicion_elemento.fil][posicion_elemento.col] == LUGAR_VACIO);
}

// Pre: La matriz terreno debe estar previamente incializada.
// Post: Inicializa la cocina en posicion aleatoria, guardando la misma en su respectivo lugar en *juego, verificando que no se pise con otro elemento al asignar su lugar en la matriz
// terreno.
void inicializar_cocina(cocina_t *cocina, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    cocina->posicion.fil = generar_posicion_elemento();
    cocina->posicion.col = generar_posicion_elemento();
    while(!posicion_elemento_valida(cocina->posicion, terreno)){
        cocina->posicion.fil = generar_posicion_elemento();
        cocina->posicion.col = generar_posicion_elemento();
    }
    cocina->cantidad_listos = SIN_PLATOS_LISTOS;
    cocina->cantidad_preparacion = SIN_PLATOS_EN_PREPARACION;
    cocina->platos_preparacion = NULL;
    cocina->platos_listos = NULL;

    terreno[cocina->posicion.fil][cocina->posicion.col] = COCINA;
}

// Pre: La matriz terreno debe estar previamente incializada.
// Post: Inicializa a 'Linguini' (mozo) en posicion aleatoria, guardando la misma en su respectivo lugar en *juego, verificando que no se pise con otro elemento al asignar su lugar en 
// la matriz terreno. Además inicializa en 0 la cantidad de pedidos, paines y bandejas.
void inicializar_mozo(mozo_t *mozo, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    mozo->cantidad_pedidos = SIN_PEDIDOS;    
    mozo->cantidad_bandeja = SIN_PEDIDOS_EN_BANDEJA;
    mozo->cantidad_patines = SIN_PATINES;
    mozo->tiene_mopa = false;
    mozo->patines_puestos = false;

    mozo->posicion.fil = generar_posicion_elemento();
    mozo->posicion.col = generar_posicion_elemento();
    while(!posicion_elemento_valida(mozo->posicion, terreno)){
        mozo->posicion.fil = generar_posicion_elemento();
        mozo->posicion.col = generar_posicion_elemento();
    }

    terreno[mozo->posicion.fil][mozo->posicion.col] = MOZO;
}

// Pre: La matriz terreno debe estar previamente inicializada (la misma corresponde a la inicializada hasta el momento), las cantidades de elemento, los topes y el caracter del elemento a 
// incializar deben ser las correctas para cada elemento al ser llamado a inicializar.
// Post: Asigna a la matriz terreno los elementos en una posición válida, guardando en el vector del elemento incializado las posiciones previamente dichas en su posicion correcta dentro
// del mismo.
void inicializar_elementos(objeto_t *elemento, char terreno[MAX_FILAS][MAX_COLUMNAS], int *cant_elemento, int valor_inicial, int tope_elemento, char elemento_a_inicializar){
    for(int i = valor_inicial; i < tope_elemento; i++){
        elemento[i].posicion.fil = generar_posicion_elemento();
        elemento[i].posicion.col = generar_posicion_elemento();
        while(!posicion_elemento_valida(elemento[i].posicion, terreno)){
            elemento[i].posicion.fil = generar_posicion_elemento();
            elemento[i].posicion.col = generar_posicion_elemento();
        }

        elemento[i].tipo = elemento_a_inicializar;
        terreno[elemento[i].posicion.fil][elemento[i].posicion.col] = elemento[i].tipo;
        (*cant_elemento) += 1;
    }
}

void inicializar_juego(juego_t *juego){
    char terreno[MAX_FILAS][MAX_COLUMNAS] = {};

    inicializar_terreno(terreno);
    inicializar_cantidades_iniciales(juego);
    inicializar_mesa_cuatro(juego->mesas, terreno, &(juego->cantidad_mesas));
    inicializar_mesa_individual(juego->mesas, terreno, &(juego->cantidad_mesas));
    inicializar_cocina(&(juego->cocina), terreno);
    inicializar_mozo(&(juego->mozo), terreno);
    inicializar_elementos(juego->herramientas, terreno, &(juego->cantidad_herramientas), 0, TOPE_MOPA, MOPA);
    inicializar_elementos(juego->herramientas, terreno, &(juego->cantidad_herramientas), TOPE_MOPA, TOPE_MONEDAS, MONEDAS);
    inicializar_elementos(juego->herramientas, terreno, &(juego->cantidad_herramientas), TOPE_MONEDAS, TOPE_PATINES, PATINES);
    inicializar_elementos(juego->obstaculos, terreno, &(juego->cantidad_obstaculos), 0, TOPE_CHARCOS, CHARCOS);

    printf("A jugar!\n");
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

// Pre: -
// Post: Devuelve true si una coordenada de un elemento es igual a la otra.
bool coinciden_posicion_dos_elementos(coordenada_t posicion_elemento1, coordenada_t posicion_elemento2){
    return(posicion_elemento1.fil == posicion_elemento2.fil && posicion_elemento1.col == posicion_elemento2.col);
}

// Pre: El tope del vector y su posición deben estar previamente inicializados.
// Post: Elimina un indice del vector de objetos, a su vez disminuyendo su tope.
void eliminar_objeto(objeto_t *objetos, int posicion_buscada, int *tope_vector){
    for(int i = posicion_buscada; i < *tope_vector-1; i++){
        objetos[i].tipo = objetos[i+1].tipo;
        objetos[i].posicion = objetos[i+1].posicion;
    }
    (*tope_vector)--;
}

// Pre: La cantidad de herramientas debe estar previamente incializada.
// Post: Realiza el procedimiento para que el mozo agarre la mopa, eliminandola físicamente del vector, asegurandose que antes de tomarla el mozo se encuentre encima de la misma.
void accion_agarrar_mopa(mozo_t *mozo, objeto_t *herramientas, int *cantidad_herramientas){
    for(int i = 0; i < *cantidad_herramientas; i++){
        if(herramientas[i].tipo == MOPA){
            if(!mozo->tiene_mopa && coinciden_posicion_dos_elementos(mozo->posicion, herramientas[i].posicion)){
                mozo->tiene_mopa = true;
                eliminar_objeto(herramientas, i, cantidad_herramientas);
                printf("Mopa recogida.\n");
            }
            else{
                printf("Parate sobre la mopa para agarrarla.\n");
            }
        }
    }
}

// Pre: La cantidad de herramientas y obstaculos deben estar previamente inicializadas.
// Post: Devuelve true si la posición de la mopa al ser dejada en el terreno no se superpone con ningún elemento posible.
bool mopa_estara_sobre_elemento(juego_t juego, coordenada_t posicion_mozo){
    bool estara = false;
    bool superposicion_detectada = false;

    if(coinciden_posicion_dos_elementos(juego.cocina.posicion, posicion_mozo)){
        estara = true;
        superposicion_detectada = true;
    }

    for(int i = 0; i < juego.cantidad_herramientas && !superposicion_detectada; i++){
        if(coinciden_posicion_dos_elementos(juego.herramientas[i].posicion, posicion_mozo)){
            estara = true;
            superposicion_detectada = true;
        }
    }

    for(int i = 0; i < juego.cantidad_obstaculos && !superposicion_detectada; i++){
        if(coinciden_posicion_dos_elementos(juego.obstaculos[i].posicion, posicion_mozo)){
            estara = true;
        }
    }

    return estara;
}

// Pre: La cantidad de herramientas debe estar inicializada previamente.
// Post: Mueve todos los elementos del vector una posición a la derecha, para incluir en la posicion 0 a la mopa.
void agregar_mopa_al_vector(objeto_t *herramientas, int *cantidad_herramientas, coordenada_t posicion_mozo){
    for(int i = *cantidad_herramientas; i >= 0; i--){
        herramientas[i+1].posicion = herramientas[i].posicion;
        herramientas[i+1].tipo = herramientas[i].tipo;
        if(i == POSICION_MOPA){
            herramientas[i].posicion = posicion_mozo;
            herramientas[i].tipo = MOPA;
        }
    }
    (*cantidad_herramientas)++;
}

// Pre: -
// Post: Si la posición futura de la mopa sera válida, suelta la mopa del mozo y llama a la función agregar_mopa_al_vector. En caso contrario, imprime un mensaje por pantalla,
void accion_dejar_mopa(juego_t *juego){
    if(mopa_estara_sobre_elemento(*juego, juego->mozo.posicion)){
        printf("No podes dejar la mopa sobre un objeto.\n");
    }
    else{
        agregar_mopa_al_vector(juego->herramientas, &(juego->cantidad_herramientas), juego->mozo.posicion);
        juego->mozo.tiene_mopa = false;
        printf("Mopa soltada.\n");
    }
}

// Pre: -
// Post: Llama a la funciones para agarrar o dejar la mopa según la tenga no el mozo.
void procedimiento_mopa(juego_t *juego){
    if(!juego->mozo.tiene_mopa){
        accion_agarrar_mopa(&juego->mozo, juego->herramientas, &juego->cantidad_herramientas);
    }
    else{
        accion_dejar_mopa(juego);
    }
}

// Pre: -
// Post: Devuelve un indice aleatorio entre 1 y 4 inclusive.
int generar_indice_aleatorio(){
    int nuevos_comensales = rand () % INDICE_MAXIMO + INDICE_MINIMO;
    return nuevos_comensales;
}

// Pre: -
// Post: Asigna una comida del menú según el numero pedido coincida con el indice correspondiente.
void asignar_pedido(int numero_pedido, char *plato){
    if(numero_pedido == INDICE_MILANESA_NAPO){
        *plato = MILANESA_NAPO;
    }
    else if(numero_pedido == INDICE_HAMBURGUESA){
        *plato = HAMBURGUESA;
    }
    else if(numero_pedido == INDICE_PARRILLA){
        *plato = PARRILLA;
    }
    else{
        *plato = RATATOUILLE;
    }
}

// Pre: La cantidad de platos debe estar previamente incializada
// Post: Devuelve true si en el vector platos esta la comida buscada.
bool esta_plato(char *platos, int *cantidad_platos, char comida_buscada){
    bool esta = false;
    for(int i = 0; i < *cantidad_platos && !esta; i++){
        if(platos[i] == comida_buscada){
            esta = true;
        }
    }
    return esta;
}

// Pre: -
// Post: Asigna un tiempo de preparación al pedido según el plato de mayor duración de cocción tengan.
void asignar_tiempo_preparacion(pedido_t *pedido, int numero_pedido){
    if(esta_plato(pedido[numero_pedido].platos, &pedido[numero_pedido].cantidad_platos, MILANESA_NAPO)){
        pedido[numero_pedido].tiempo_preparacion = PREPARACION_MILANESA_NAPO;
    }
    else if(esta_plato(pedido[numero_pedido].platos, &pedido[numero_pedido].cantidad_platos, RATATOUILLE)){
        pedido[numero_pedido].tiempo_preparacion = PREPARACION_RATATOUILLE;
    }
    else if(esta_plato(pedido[numero_pedido].platos, &pedido[numero_pedido].cantidad_platos, PARRILLA)){
        pedido[numero_pedido].tiempo_preparacion = PREPARACION_PARRILLA;
    }
    else if(esta_plato(pedido[numero_pedido].platos, &pedido[numero_pedido].cantidad_platos, HAMBURGUESA)){
        pedido[numero_pedido].tiempo_preparacion = PREPARACION_HAMBURGUESA;
    }
}

// Pre: La posición 0 del vector de pedidos del mozo debe estar previamente incializada.
// Post: Asigna el pedido tomado de la mesa al vector de pedidos del mozo, inicializando todos sus campos.
void pasar_pedido_mozo(mozo_t *mozo, mesa_t *mesas, int numero_mesa){
    mozo->pedidos[mozo->cantidad_pedidos].id_mesa = numero_mesa;
    mozo->pedidos[mozo->cantidad_pedidos].cantidad_platos = mesas[numero_mesa].cantidad_comensales;
    mozo->pedidos[mozo->cantidad_pedidos].tiempo_preparacion = 0;
    for(int i = 0; i < mozo->pedidos[mozo->cantidad_pedidos].cantidad_platos; i++){
        int numero_pedido = generar_indice_aleatorio();
        asignar_pedido(numero_pedido, &mozo->pedidos[mozo->cantidad_pedidos].platos[i]);
    }
    asignar_tiempo_preparacion(mozo->pedidos, mozo->cantidad_pedidos);
    mozo->cantidad_pedidos++;
}

// Pre: -
// Post: Devuelve la distancia manhattan entre dos coordenadas.
int distancia_manhatan(coordenada_t posicion_1, coordenada_t posicion_2){
    int suma_x = abs(posicion_1.fil - posicion_2.fil);
    int suma_y = abs(posicion_1.col - posicion_2.col);
    int distancia_manhatan = suma_x + suma_y;
    return distancia_manhatan;
}

// Pre: -
// Post: Devuelve true si la distancia del mozo para tomar un pedido de una mesa es la correcta, si no se ha tomado el pedido en esa mesa y si tiene comensales.
bool se_puede_tomar_pedido(mozo_t mozo, bool pedido_tomado, int cantidad_comensales, coordenada_t posicion_mesa){
    int distancia_mozo_mesa = distancia_manhatan(mozo.posicion, posicion_mesa);
    return(distancia_mozo_mesa <= DISTANCIA_MAXIMA_TOMAR_PEDIDO && !pedido_tomado && cantidad_comensales > SIN_COMENSALES && mozo.cantidad_pedidos < MAX_PEDIDOS);
}

// Pre: La cantidad de mesas debe estar previamente incializada, al igual que las posiciones de las mesas y el mozo
// Post: Recorre todas las mesas, chequeando que se pueda tomar el pedido. Si se puede, llama a la función correspondiente para hacerlo.
void tomar_pedido_mesa(mozo_t *mozo, mesa_t *mesas, int *cantidad_mesas){
    for(int i = 0; i < *cantidad_mesas; i++){
        bool mesa_examinada = false;
        for(int j = 0; j < mesas[i].cantidad_lugares && !mesa_examinada; j++){
            if(se_puede_tomar_pedido(*mozo, mesas[i].pedido_tomado, mesas[i].cantidad_comensales, mesas[i].posicion[j])){
                mesa_examinada = true;
                mesas[i].pedido_tomado = true;
                pasar_pedido_mozo(mozo, mesas, i);
                printf("Pedido de la mesa %i tomado!\n", i+1);
            }
        }
    }
}

// Pre: -
// Post: Filtra todos los impedimentos que puede tener el mozo al tomar la mopa (Exceso de pedidos y si tiene la mopa encima).
void procedimiento_tomar_pedidos_mesa(mozo_t *mozo, mesa_t *mesas, int *cantidad_mesas){
    if(!mozo->tiene_mopa){
        tomar_pedido_mesa(mozo, mesas, cantidad_mesas);
    }
    else if(mozo->tiene_mopa){
        printf("No podes tomar pedidos de las mesas con la mopa puesta.\n");
    }
    if(mozo->cantidad_pedidos >= MAX_PEDIDOS){
        printf("No podes tomar mas pedidos!\n");
    }

}

// Pre: La cantidad de patines del mozo debe estar previamente inicializada.
// Post: Si el mozo tiene patines, los activa. Si no tiene más, imprime un mensaje por pantalla
void activacion_patines(mozo_t *mozo){
    if(mozo->cantidad_patines == 0){
        printf("No tenes más patines!\n");
    }
    else{
        mozo->patines_puestos = true;
        mozo->cantidad_patines--;
        printf("Patines activados.\n");
    }
}

// Pre: El booleano tiene_mopa debe estar previamente inicializado
// Post: Si el mozo no tiene la mopa, procede a llamar a la función que activa los patines. Si la tiene, imprime un mensaje por pantalla.
void procedimiento_patines(mozo_t *mozo){
    if(!mozo->tiene_mopa){
        activacion_patines(mozo);
    }
    else{
        printf("No podés activar los patines si tenés la mopa.\n");
    }
}

// Pre: La cantidad de mesas debe estar previamente incializada.
// Post: Devuelve true si la posición en la que se va a mover el personaje no esta ocupada por una mesa o excede los límites del terreno.
bool posicion_futura_mozo_valida(mesa_t *mesa, int posicion_futura_mozo_fil, int posicion_futura_mozo_col, int *cantidad_mesas){
    bool es_valida = true;
    for(int i = 0; i < *cantidad_mesas; i++){
        for(int j = 0; j < mesa[i].cantidad_lugares; j++){
            if(mesa[i].posicion[j].fil == posicion_futura_mozo_fil && mesa[i].posicion[j].col == posicion_futura_mozo_col){
                es_valida = false;
            }
        }
    }

    if(posicion_futura_mozo_fil < PRIMERA_FILA || posicion_futura_mozo_fil > ULTIMA_FILA || posicion_futura_mozo_col < PRIMERA_COLUMNA || posicion_futura_mozo_col > ULTIMA_COLUMNA){
        es_valida = false;
    }

    return es_valida;
}

// Pre: La cantidad de herramientas debe estar previamente inicializada.
// Post: Si el mozo pasa por encima de una moneda, la recoge y suma dinero. Si pasa por encima de unos patines, los recoge. Pero si tiene la mopa, no puede realizar ninguna de las
// anteriores acciones.
void comportamiento_herramientas(objeto_t *herramientas, mozo_t *mozo, int *cantidad_herramientas, int *dinero){
    for(int i = 0; i < *cantidad_herramientas; i++){
        if(coinciden_posicion_dos_elementos(herramientas[i].posicion, mozo->posicion) && !mozo->tiene_mopa && herramientas[i].tipo == MONEDAS){
            *dinero += DINERO_MONEDA;
            printf("Moneda recogida!\n");
            eliminar_objeto(herramientas, i, cantidad_herramientas);
        }
        else if(coinciden_posicion_dos_elementos(herramientas[i].posicion, mozo->posicion) && !mozo->tiene_mopa && herramientas[i].tipo == PATINES){
            mozo->cantidad_patines++;
            printf("Patines recogidos!\n");
            eliminar_objeto(herramientas, i, cantidad_herramientas);
        }
        else if(coinciden_posicion_dos_elementos(herramientas[i].posicion, mozo->posicion) && mozo->tiene_mopa){
            printf("Si tenes la mopa, no podes recoger ni patines ni monedas ni matar cucarachas.\n");
        }
    }
}

// Pre: La cantidad de pedidos y la posicion a eliminar deben estar previamente inicializados y ser válidos.
// Post: Elimina un pedido del vector, disminuyendo el tope y moviemdo todos los elementos a la izquierda.
void eliminar_pedido(pedido_t *pedidos, int *cantidad_pedidos, int posicion_a_eliminar){
    for(int i = posicion_a_eliminar; i < *cantidad_pedidos-1; i++){
        pedidos[i].cantidad_platos = pedidos[i+1].cantidad_platos;
        pedidos[i].id_mesa = pedidos[i+1].id_mesa;
        pedidos[i].tiempo_preparacion = pedidos[i+1].tiempo_preparacion;
        for(int j = 0; j < pedidos[i].cantidad_platos; j++){
            pedidos[i].platos[j] = pedidos[i+1].platos[j];
        }
    }
    (*cantidad_pedidos)--;
}

// Pre: La cantidad de pedidos, bandejas, platos en preparación y listos deben estar previamenre inicializados.
// Post: Si el indice de la mesa coincide con el de algun pedido, ya sea en la cocina o del mozo, lo elimina.
void limpiar_pedido_mesa(mozo_t *mozo, cocina_t *cocina, int id_mesa){
    for(int i = 0;i < mozo->cantidad_pedidos; ){
        if(id_mesa == mozo->pedidos[i].id_mesa){
            eliminar_pedido(mozo->pedidos, &mozo->cantidad_pedidos, i);
        }
        else{
            i++;
        }
    }
    for(int i = 0;i < mozo->cantidad_bandeja; ){
        if(id_mesa == mozo->bandeja[i].id_mesa){
            eliminar_pedido(mozo->bandeja, &mozo->cantidad_bandeja, i);
        }
        else{
            i++;
        }
    }
    for(int i = 0;i < cocina->cantidad_preparacion; ){
        if(id_mesa == cocina->platos_preparacion[i].id_mesa){
            eliminar_pedido(cocina->platos_preparacion, &cocina->cantidad_preparacion, i);
        }
        else{
            i++;
        }
    }
    for(int i = 0;i < cocina->cantidad_listos; ){
        if(id_mesa == cocina->platos_listos[i].id_mesa){
            eliminar_pedido(cocina->platos_listos, &cocina->cantidad_listos, i);
        }
        else{
            i++;
        }
    }
}

// Pre: -
// Post: Vacia de comensales la mesa que se le pasa por parámetro.
void vaciar_mesa(mesa_t *mesa, int indice_mesa){
    mesa[indice_mesa].cantidad_comensales = SIN_COMENSALES;
    mesa[indice_mesa].paciencia = SIN_PACIENCIA;
    mesa[indice_mesa].pedido_tomado = false;
}

// Pre: La cantidad de obstaculos debe estar previamente inicializada.
// Post: Si el mozo pasa por encima de una cucaracha, la mata. Si pasa por encima de un charco, pierde un pedido de la bandeja, y los comensales que lo pidieron, se van. Si el mozo tiene
// la mopa, no puede matar cucarachas pero si limpia los charcos.
void comportamiento_obstaculos(objeto_t *obstaculos, mozo_t *mozo, cocina_t *cocina, mesa_t *mesa, int *cantidad_obstaculos){
    for(int i = 0; i < *cantidad_obstaculos; i++){
        if(obstaculos[i].tipo == CUCARACHA && coinciden_posicion_dos_elementos(obstaculos[i].posicion, mozo->posicion) && !mozo->tiene_mopa){
            eliminar_objeto(obstaculos, i, cantidad_obstaculos);
            printf("Cucaracha matada!\n");
        }
        else if(obstaculos[i].tipo == CUCARACHA && coinciden_posicion_dos_elementos(obstaculos[i].posicion, mozo->posicion) && mozo->tiene_mopa){
            printf("Si tenes la mopa, no podes recoger ni patines ni monedas ni matar cucarachas.\n");
        }
        else if(obstaculos[i].tipo == CHARCOS && coinciden_posicion_dos_elementos(obstaculos[i].posicion, mozo->posicion) && !mozo->tiene_mopa && mozo->cantidad_bandeja > 0){
            if(mozo->cantidad_bandeja > SIN_PEDIDOS_EN_BANDEJA){
                int indice_mesa = mozo->bandeja[mozo->cantidad_bandeja-1].id_mesa;
                vaciar_mesa(mesa, indice_mesa);
                limpiar_pedido_mesa(mozo, cocina, indice_mesa);
                printf("Pistaste un charco, guarda! Se te cayeron todos los pedidos de la bandeja.\n");
            }
        }
        else if(obstaculos[i].tipo == CHARCOS && coinciden_posicion_dos_elementos(obstaculos[i].posicion, mozo->posicion) && mozo->tiene_mopa){
            eliminar_objeto(obstaculos, i, cantidad_obstaculos);
            printf("Charco limpiado!\n");
        }
    }
}

// Pre: -
// Post: Llama a las funciones que dirimen los comportamientos del mozo con los objetos del terreno.
void procedimiento_interaccion_mozo_objetos(juego_t *juego){
    comportamiento_herramientas(juego->herramientas, &juego->mozo, &juego->cantidad_herramientas, &juego->dinero);
    comportamiento_obstaculos(juego->obstaculos, &juego->mozo, &juego->cocina, juego->mesas, &juego->cantidad_obstaculos);
}

// Pre: -
// Post: Crea un vector dinámico que se adapta a los pedidos de las mesas que se dejen en la cocina para ser preparados. A su vez, borra del vector de pedidos del mozo aquellos que sean
// dejados.
void dejar_pedidos_cocina(cocina_t *cocina, mozo_t *mozo){
    int cantidad_platos_preparacion_inicial = cocina->cantidad_preparacion;
    cocina->cantidad_preparacion += mozo->cantidad_pedidos;
    cocina->platos_preparacion = realloc(cocina->platos_preparacion, sizeof(pedido_t) * (size_t)cocina->cantidad_preparacion);
    
    if(cocina->platos_preparacion ==  NULL){
        printf("ERROR AL ASIGNAR MEMORIA A LOS PLATOS EN PREPARACION.\n");
        return;
    }

    for(int i = cantidad_platos_preparacion_inicial; i < cocina->cantidad_preparacion; i++){
        cocina->platos_preparacion[i] = mozo->pedidos[PEDIDO_A_COCINAR];
        for(int j = 0; j < cocina->platos_preparacion[i].cantidad_platos; j++){
            cocina->platos_preparacion[i].platos[j] = mozo->pedidos[PEDIDO_A_COCINAR].platos[j];
        }
        eliminar_pedido(mozo->pedidos, &mozo->cantidad_pedidos, PEDIDO_A_COCINAR);
        printf("Pedido cocinandose...\n");
    }
}

// Pre: La cantidad de bandejas del mozo debe estar previamente incializada.
// Post: Pasa los pedidos listos de la cocina a la bandeja del mozo, eliminandolos de la primera.
void tomar_pedidos_cocina(cocina_t *cocina, mozo_t *mozo){
    int cantidad_inicial_bandeja = mozo->cantidad_bandeja;
    mozo->cantidad_bandeja++;

    mozo->bandeja[cantidad_inicial_bandeja] = cocina->platos_listos[PEDIDO_A_ENTREGAR];
    for(int k = 0; k < mozo->bandeja[cantidad_inicial_bandeja].cantidad_platos; k++){
        mozo->bandeja[cantidad_inicial_bandeja].platos[k] = cocina->platos_listos[PEDIDO_A_ENTREGAR].platos[k];
    }
    printf("Pedido de la mesa %i listo para entregar!\n", mozo->bandeja[cantidad_inicial_bandeja].id_mesa+1);

    eliminar_pedido(cocina->platos_listos, &cocina->cantidad_listos, PEDIDO_A_ENTREGAR);
}

// Pre: La posición de indice_mesa en el vector mesas debe estar previamente incializada.
// Post: Vacia de comensales, de paciencia y establece en false el pedido_tomado de la mesa indicada.
void liberar_mesa(mesa_t *mesa, int indice_mesa){
    mesa[indice_mesa].cantidad_comensales = SIN_COMENSALES;
    mesa[indice_mesa].paciencia = SIN_PACIENCIA;
    mesa[indice_mesa].pedido_tomado = false;
}

// Pre: -
// Post: Si la cantidad de lugares de la mesa pasada por parámetro es 1, se suma al dinero el monto establecido para la misma. De lo contrario, se suma el predeterminado para las mesas
// de 4.
void asignar_monto_mesa(int cantidad_lugares_mesa, int *dinero){
    if(cantidad_lugares_mesa == CANTIDAD_LUGARES_MESA_IND){
        *dinero += MONTO_MESA_IND;
        printf("+$%i\n", MONTO_MESA_IND);
    }
    else{
        *dinero += MONTO_MESA_4;
        printf("+$%i\n", MONTO_MESA_4);
    }
}

// Pre: La cantidad de bandejas del mozo debe estar previamente inicializada.
// Post: Si algun indice_mesa de pedidos de la bandeja coincide con el pasado por parámetro, entrega el pedido llamando a las funciones encargadas de ello. Caso contrario imprime un mensaje
// por pantalla.
void pasar_pedido_a_mesas(int numero_mesa, int *dinero, mozo_t *mozo, mesa_t *mesas, cocina_t *cocina){
    bool sin_pedido_para_entregar =  true;
    for(int i = 0; i < mozo->cantidad_bandeja; i++){
        if(mozo->bandeja[i].id_mesa == numero_mesa){
            limpiar_pedido_mesa(mozo, cocina, numero_mesa);
            liberar_mesa(mesas, numero_mesa);
            asignar_monto_mesa(mesas[numero_mesa].cantidad_lugares, dinero);
            sin_pedido_para_entregar = false;
            printf("Pedido entregado para la mesa %i\n", numero_mesa+1);
        }
    }
    if(sin_pedido_para_entregar){
        printf("No hay ningún pedido para entregar en esta mesa.\n");
    }
}

// Pre: La cantidad de mesas debe estar previamente incializada.
// Post: Si encuentra una mesa cercana, llama a la función que evalua la entrega de pedidos.
void buscar_mesa_cercana(mozo_t *mozo, mesa_t *mesas, cocina_t *cocina, int *dinero, int *cantidad_mesas, int distancia_admitida){
    for(int i = 0; i < *cantidad_mesas; i++){
        bool mesa_examinada = false;
        for(int j = 0; j < mesas[i].cantidad_lugares && !mesa_examinada; j++){
            int distancia_mozo_mesa = distancia_manhatan(mozo->posicion, mesas[i].posicion[j]);
            if(distancia_mozo_mesa <= distancia_admitida && mozo->cantidad_bandeja > SIN_PEDIDOS_EN_BANDEJA){
                mesa_examinada = true;
                pasar_pedido_a_mesas(i, dinero, mozo, mesas, cocina);
            }
        }
    }
}

// Pre: -
// Post: Pasa los pedidos del vector en preparación a listos de la cocina, eliminado el pedido del primero.
void pasar_pedido_a_listos(cocina_t *cocina, int indice_pedido_cocinado){
    int cantidad_actual_listos = cocina->cantidad_listos;
    cocina->cantidad_listos++;
    cocina->platos_listos = realloc(cocina->platos_listos, sizeof(pedido_t) * (size_t)cocina->cantidad_listos);

    if(cocina->platos_listos == NULL){
        printf("ERROR AL ASIGNAR MEMORIA");
    }

    cocina->platos_listos[cantidad_actual_listos] = cocina->platos_preparacion[indice_pedido_cocinado];
    for(int i = 0; i < cocina->platos_listos[cantidad_actual_listos].cantidad_platos; i++){
        cocina->platos_listos[cantidad_actual_listos].platos[i] = cocina->platos_preparacion[indice_pedido_cocinado].platos[i];
    }
    eliminar_pedido(cocina->platos_preparacion, &cocina->cantidad_preparacion, indice_pedido_cocinado);
    printf("Pedido cocinado!\n");
}

// Pre: -
// Post: Evalua todas las posibilidades de interacción de los pedidos con el mozo.
void procedimiento_interaccion_mozo_pedidos(mozo_t *mozo, cocina_t *cocina, mesa_t *mesas, int *dinero, int *cantidad_mesas){
    if(!mozo->tiene_mopa){
        if(coinciden_posicion_dos_elementos(mozo->posicion, cocina->posicion)){
            if(mozo->cantidad_pedidos > SIN_PEDIDOS){
                dejar_pedidos_cocina(cocina, mozo);
            }
            int cantidad_listos = cocina->cantidad_listos;
            bool sin_espacio = false;
            for(int i = 0; i < cantidad_listos && !sin_espacio; i++){
                if(cocina->cantidad_listos > SIN_PLATOS_LISTOS && mozo->cantidad_bandeja < MAX_BANDEJA){
                    tomar_pedidos_cocina(cocina, mozo);
                }
                else if(mozo->cantidad_bandeja >= MAX_BANDEJA){
                    sin_espacio = true;
                    printf("No podés tomar más pedidos para entregar.\n");
                }
            }
        }
        else{
            buscar_mesa_cercana(mozo, mesas, cocina, dinero, cantidad_mesas, DISTANCIA_MAXIMA_TOMAR_PEDIDO);
        }
    }
}

// Pre: -
// Post: Si el tiempo de preparación de los platos de la cocina llega a 0, se llama a la función que se ocupa de ello.
void procedimiento_cocinar_pedidos(cocina_t *cocina){
    int cantidad_preparacion = cocina->cantidad_preparacion;
    for(int i = 0; i < cantidad_preparacion; i++){
        if(cocina->platos_preparacion[i].tiempo_preparacion == PEDIDO_LISTO){
            pasar_pedido_a_listos(cocina, i);
        }
    }
}

// Pre: -
// Post: Llama a las funciones que se encarga de manipular el comportamiento de los pedidos.
void procedimiento_pedidos(cocina_t *cocina, mozo_t *mozo, mesa_t *mesas, int *dinero, int *cantidad_mesas){
    procedimiento_interaccion_mozo_pedidos(mozo, cocina, mesas, dinero, cantidad_mesas);
    procedimiento_cocinar_pedidos(cocina);
}

// Pre: -
// Post: Si el tiempo de preparación de algun plato es mayor a 0, se le resta una unidad.
void restar_tiempo_coccion(cocina_t *cocina){
    for(int i = 0; i < cocina->cantidad_preparacion; i++){
        if(cocina->platos_preparacion[i].tiempo_preparacion > PEDIDO_LISTO){
            cocina->platos_preparacion[i].tiempo_preparacion--;        
        }
    }
}

// Pre: -
// Post: Si hay una cucaracha cerca de alguna mesa, devuelve true.
bool cucaracha_cerca(objeto_t *obstaculos, coordenada_t *posicion_mesa, int *cantidad_obstaculos){
    bool esta_cerca = false;
    for(int i = 0; i < *cantidad_obstaculos; i++){
        if(obstaculos[i].tipo == CUCARACHA){
            int distancia_mesa_cucaracha = distancia_manhatan(*posicion_mesa, obstaculos[i].posicion);
            if(distancia_mesa_cucaracha <= DISTANCIA_INACEPTABLE_CUCARACHA){
                esta_cerca = true;
            }
        }
    }
    return esta_cerca;
}

// Pre: -
// Post: Si la mesa tiene comensales pero no tiene una cucaracha cerca, se le resta una unidad de paciencia. Si la mesa tiene una cucaracha cerca y tiene comensales, se le resta tres.
void restar_paciencia(juego_t *juego){
    for(int i = 0; i < juego->cantidad_mesas; i++){
        bool hay_cucaracha_cerca = false;
        for(int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            if(juego->mesas[i].cantidad_comensales != SIN_COMENSALES){
                hay_cucaracha_cerca = cucaracha_cerca(juego->obstaculos, &juego->mesas[i].posicion[j], &juego->cantidad_obstaculos);
            }
        }
        if(juego->mesas[i].cantidad_comensales != SIN_COMENSALES){
            if(hay_cucaracha_cerca){
                juego->mesas[i].paciencia -= PACIENCIA_POR_CUCARACHA;
                printf("Los comensales de la mesa %i se van a poner impacientes... Vieron una cucaracha!\n", i+1);
            }
            else{
                juego->mesas[i].paciencia--;
            }
        }
    }
}

// Pre: La posición del mozo debe estar previmanete inicializada.
// Post: Dependiendo la dirección, el personaje se mueve hasta que encuentra una mesa o el final del mapa, interactuando con todos los elementos a su paso. Sin embargo, solo se le suma
// un movimiento más, al igual que el tiempo de cocción y la resta de paciencia.
void procedimiento_movimiento_con_patines(juego_t *juego, char accion){
    if(accion == MOVER_ADELANTE){
        for(int i = juego->mozo.posicion.fil; i >= PRIMERA_FILA && posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil-1, juego->mozo.posicion.col, &(juego->cantidad_mesas)); i--){
            juego->mozo.posicion.fil--;
            procedimiento_interaccion_mozo_objetos(juego);
            procedimiento_pedidos(&juego->cocina, &juego->mozo, juego->mesas, &juego->dinero, &juego->cantidad_mesas);
        }
    }
    else if(accion == MOVER_ATRAS){
        for(int i = juego->mozo.posicion.fil; i <= ULTIMA_FILA && posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil+1, juego->mozo.posicion.col, &(juego->cantidad_mesas)); i++){
            juego->mozo.posicion.fil++;
            procedimiento_interaccion_mozo_objetos(juego);
            procedimiento_pedidos(&juego->cocina, &juego->mozo, juego->mesas, &juego->dinero, &juego->cantidad_mesas);
        }
    }
    else if(accion == MOVER_DERECHA){
        for(int i = juego->mozo.posicion.col; i <= ULTIMA_COLUMNA && posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil, juego->mozo.posicion.col+1, &(juego->cantidad_mesas)); i++){
            juego->mozo.posicion.col++;
            procedimiento_interaccion_mozo_objetos(juego);
            procedimiento_pedidos(&juego->cocina, &juego->mozo, juego->mesas, &juego->dinero, &juego->cantidad_mesas);
        }
    }
    else{
        for(int i = juego->mozo.posicion.col; i >= PRIMERA_COLUMNA && posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil, juego->mozo.posicion.col-1, &(juego->cantidad_mesas)); i--){
            juego->mozo.posicion.col--;
            procedimiento_interaccion_mozo_objetos(juego);
            procedimiento_pedidos(&juego->cocina, &juego->mozo, juego->mesas, &juego->dinero, &juego->cantidad_mesas);
        }
    }
    juego->mozo.patines_puestos = false;
    juego->movimientos++;
    restar_tiempo_coccion(&juego->cocina);
    restar_paciencia(juego);
}

// Pre: La posición del mozo debe estar previamente inicializada.
// Post: Según la dirección, el mozo se mueve, estableciendo en true el booleano movimiento_realizado pasado por parámetro.
void procedimiento_movimientos_regulares(juego_t *juego, char accion, bool *movimiento_realizado){
    if(accion == MOVER_ADELANTE){
        if(posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil-1, juego->mozo.posicion.col, &(juego->cantidad_mesas))){
            juego->mozo.posicion.fil--;
            *movimiento_realizado = true;
        }
    }
    else if(accion == MOVER_ATRAS){
        if(posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil+1, juego->mozo.posicion.col, &(juego->cantidad_mesas))){
            juego->mozo.posicion.fil++;
            *movimiento_realizado = true;
        }
    }
    else if(accion == MOVER_DERECHA){
        if(posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil, juego->mozo.posicion.col+1, &(juego->cantidad_mesas))){
            juego->mozo.posicion.col++;
            *movimiento_realizado = true;
        }
    }
    else{
        if(posicion_futura_mozo_valida(juego->mesas, juego->mozo.posicion.fil, juego->mozo.posicion.col-1, &(juego->cantidad_mesas))){
            juego->mozo.posicion.col--;
            *movimiento_realizado = true;
        }      
    }
}

// Pre: -
// Post: Si el mozo tiene patines, se llama a la función que realiza el movimiento con ellos. Si no, se llama a la función que evalua los movimientos regulares.
void procedimiento_movimientos(juego_t *juego, char accion, bool *movimiento_realizado){
    if(juego->mozo.patines_puestos){
        procedimiento_movimiento_con_patines(juego, accion);
    }
    else{
        procedimiento_movimientos_regulares(juego, accion, movimiento_realizado);
    }
}

// Pre: -
// Post: Genera un indice de paciencia aleatoriamente entre 200 y 100.
int generar_cant_paciencia(){
    int paciencia_inicial = rand () % MAX_PACIENCIA_INICIAL + MIN_PACIENCIA_INICIAL;
    return paciencia_inicial;
}

// Pre: -
// Post: Asigna comensales nuevos solo una vez a las mesas si en las mismas hay lugar y no tiene ningún comensal previo, estableciendo la paciencia de aquellas mesas.
void asignar_comensales(mesa_t *mesa, int *cantidad_mesas){
    int comensales = generar_indice_aleatorio();
    bool comensales_asignados = false;
    for(int i = *cantidad_mesas-1; i >= 0 && !comensales_asignados; i--){
        if((mesa[i].cantidad_lugares-mesa[i].cantidad_comensales) >= comensales && mesa[i].cantidad_comensales == SIN_COMENSALES){
            mesa[i].cantidad_comensales = comensales;
            mesa[i].paciencia = generar_cant_paciencia();
            comensales = SIN_COMENSALES_A_ASIGNAR;
            comensales_asignados = true;
        }
    }
    if(comensales_asignados){
        printf("Nuevos Comensales!\n");
    }
}

// Pre: Los movimientos deben estar previamente inicializados y ser válidos.
// Post: Devuelve true si los movimientos son múltiplos de 15.
bool vienen_mas_comensales(int *movimientos){
    return(*movimientos % NUEVOS_COMENSALES == 0);
}

// Pre: -
// Post: Devuelve true si la posición de la cucaracha nueva será válida.
bool posicion_valida_cucaracha(juego_t *juego, coordenada_t posicion_cucaracha, int *cantidad_mesas){
    bool es_valida = true;

    for(int i = 0; i < *cantidad_mesas; i++){
        for(int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            if(coinciden_posicion_dos_elementos(juego->mesas[i].posicion[j], posicion_cucaracha)){
                es_valida = false;
            }
        }
    }

    if(coinciden_posicion_dos_elementos(juego->cocina.posicion, posicion_cucaracha)){
        es_valida = false;
    }

    for(int i = 0; i < juego->cantidad_herramientas; i++){
        if(coinciden_posicion_dos_elementos(juego->herramientas[i].posicion, posicion_cucaracha)){
            es_valida = false;
        }
    }

    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(coinciden_posicion_dos_elementos(juego->obstaculos[i].posicion, posicion_cucaracha)){
            es_valida = false;
        }
    }

    return es_valida;
}

// Pre: El vector obstaculos en la posición elegida debe estar previamente inicializado.
// Post: Inicializa una cucaracha en una posición aleatoria nueva.
void inicializar_cucaracha(juego_t *juego, int *cantidad_obstaculos){
    juego->obstaculos[*cantidad_obstaculos].posicion.fil = generar_posicion_elemento();
    juego->obstaculos[*cantidad_obstaculos].posicion.col = generar_posicion_elemento();
    while(!posicion_valida_cucaracha(juego, juego->obstaculos[*cantidad_obstaculos].posicion, &(juego->cantidad_mesas))){
        juego->obstaculos[*cantidad_obstaculos].posicion.fil = generar_posicion_elemento();
        juego->obstaculos[*cantidad_obstaculos].posicion.col = generar_posicion_elemento();
    }
    juego->obstaculos[*cantidad_obstaculos].tipo = CUCARACHA;
    (*cantidad_obstaculos) += 1;
    printf("CUCARACHA!!\n");
}

// Pre: La cantdad de movimientos debe estar previmante inicializada y ser válida
// Post: Si loa movimientos son múltiplos de 25, devuelve true.
bool aparece_cucaracha(int *movimientos){
    return(*movimientos % NUEVA_CUCARACHA == 0);
}

// Pre: -
// Post: Inicializa cucarachas o comensales, o ambos, según la cantidad de movimientos los amerite.
void procedimiento_inicializacion_reiterada_elementos(juego_t *juego){
    if(vienen_mas_comensales(&(juego->movimientos)) || juego->movimientos == COMENSALES_INICIALES){
        asignar_comensales(juego->mesas, &(juego->cantidad_mesas));
    }
    if(aparece_cucaracha(&(juego->movimientos))){
        inicializar_cucaracha(juego, &(juego->cantidad_obstaculos));
    }
}

// Pre: -
// Post: Vacia las mesas que no tienen más paciencia y tienen comensales.
void vaciar_mesas_sin_paciencia(mesa_t *mesas, cocina_t *cocina, mozo_t *mozo, int *cantidad_mesas){
    for(int i = 0; i < *cantidad_mesas; i++){
        if(mesas[i].paciencia <= SIN_PACIENCIA && mesas[i].cantidad_comensales > SIN_COMENSALES){
            vaciar_mesa(mesas, i);
            limpiar_pedido_mesa(mozo, cocina, i);
        }
    }
}

// Pre: -
// Post: Dirime el comportamiento de la paciencia de las mesas, llamando a las funciones encargadas de ello.
void procedimiento_paciencia(juego_t *juego){
    restar_paciencia(juego);
    vaciar_mesas_sin_paciencia(juego->mesas, &juego->cocina, &juego->mozo, &juego->cantidad_mesas);
}

void realizar_jugada(juego_t *juego, char accion){
    bool movimiento_realizado = false;

    if(accion == AGARRAR_MOPA){
        procedimiento_mopa(juego);
    }
    else if(accion == TOMAR_PEDIDO){
        procedimiento_tomar_pedidos_mesa(&juego->mozo, juego->mesas, &juego->cantidad_mesas);
    }
    else if(accion == ACTIVAR_PATINES){
        procedimiento_patines(&juego->mozo);
    }
    else{
        procedimiento_movimientos(juego, accion, &movimiento_realizado);
    }

    if(movimiento_realizado){
        juego->movimientos++;
        procedimiento_inicializacion_reiterada_elementos(juego);
        procedimiento_interaccion_mozo_objetos(juego);
        procedimiento_paciencia(juego);
        restar_tiempo_coccion(&juego->cocina);
        procedimiento_pedidos(&juego->cocina, &juego->mozo, juego->mesas, &juego->dinero, &juego->cantidad_mesas);
    }
}

// Pre: Las mesas y sus posiciones debén estar asignadas en el vector mesa, tanto las individuales como las de 4.
// Post Asigna en el terreno la posicion de las mismas.
void asignar_mesas(mesa_t *mesa, char terreno[MAX_FILAS][MAX_COLUMNAS], int *cantidad_mesas){
    for(int i = 0; i < *cantidad_mesas; i++){
        for(int j = 0; j < mesa[i].cantidad_lugares; j++){
            if(mesa[i].cantidad_comensales != SIN_COMENSALES){
                if(mesa[i].pedido_tomado){
                    terreno[mesa[i].posicion[j].fil][mesa[i].posicion[j].col] = MESA_OCUPADA_TOMADA;
                }
                else{
                    terreno[mesa[i].posicion[j].fil][mesa[i].posicion[j].col] = MESA_OCUPADA;
                }
            }
            else{
                terreno[mesa[i].posicion[j].fil][mesa[i].posicion[j].col] = MESA;
            }
        }
    }
}

// Pre: La posición de la cocina debe estar asignada en el juego
// Post Asigna en el terreno la posicion de la cocina.
void asignar_cocina(cocina_t *cocina, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    terreno[cocina->posicion.fil][cocina->posicion.col] = COCINA;
}

// Pre: Las herramientas y sus posiciones debén estar asignadas en el vector herramientas.
// Post Asigna en el terreno la posicion de las mismas.
void asignar_herramientas(objeto_t *herramienta, char terreno[MAX_FILAS][MAX_COLUMNAS], bool tiene_mopa, int *cant_herramientas){
    for(int i = 0; i < *cant_herramientas; i++){
        terreno[herramienta[i].posicion.fil][herramienta[i].posicion.col] = herramienta[i].tipo;
    }
}

// Pre: Las obstáculos y sus posiciones debén estar asignadas en el vector obstáculos.
// Post Asigna en el terreno la posicion de las mismas.
void asignar_obstaculos(objeto_t *obstaculo, char terreno[MAX_FILAS][MAX_COLUMNAS], int *cant_obstaculos){
    for(int i = 0; i < *cant_obstaculos; i++){
        terreno[obstaculo[i].posicion.fil][obstaculo[i].posicion.col] = obstaculo[i].tipo;
    }
}

// Pre: La posicion del mozo debe estar asignada en el juego
// Post: Asigna en el terreno la posicion del mozo.
void asignar_mozo(mozo_t mozo, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    terreno[mozo.posicion.fil][mozo.posicion.col] = MOZO;
}

// Pre: Los elementos del terreno y la matriz deben estar previamente incializados
// Post: Los elementos quedan correctamente asignados en sus respectivas posiciones en la matriz terreno
void asignar_elementos_terreno(juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    asignar_mesas(juego.mesas, terreno, &(juego.cantidad_mesas));
    asignar_cocina(&(juego.cocina), terreno);
    asignar_herramientas(juego.herramientas, terreno, juego.mozo.tiene_mopa, &(juego.cantidad_herramientas));
    asignar_obstaculos(juego.obstaculos, terreno, &(juego.cantidad_obstaculos));
    asignar_mozo(juego.mozo, terreno);
}

// Pre: La matriz debe estar inicializada previamente.
// Post: Imprime el terreno recorriendo cada columna y fila añadiendole un separador entre posición y posición de la matriz.
void mostrar_terreno(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t juego){
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            if(terreno[i][j] == MESA_OCUPADA){
                printf("|");
                printf(ROJO " %c " RESET, terreno[i][j]);
            }
            else if(terreno[i][j] == MONEDAS && !juego.mozo.tiene_mopa){
                printf("|");
                printf(AMARILLO " %c " RESET, terreno[i][j]);
            }
            else if(terreno[i][j] == CHARCOS && juego.mozo.tiene_mopa){
                printf("|");
                printf(MAGENTA " %c " RESET, terreno[i][j]);
            }
            else if(terreno[i][j] == MOZO && juego.mozo.patines_puestos){
                printf("|");
                printf(CIAN " %c " RESET, terreno[i][j]);
            }
            else if((terreno[i][j] == MONEDAS || terreno[i][j] == PATINES || terreno[i][j] == CUCARACHA) && juego.mozo.tiene_mopa){
                printf("|");
                printf(GRIS " %c " RESET, terreno[i][j]);
            }
            else{

                printf("| %c ", terreno[i][j]);
            }
        }
        printf("|");
        printf("\n");
    }
}

// Pre: -
// Post: Imprime por pantalla información relevante de los pedidos que tiene el mozo.
void interfaz_pedidos_mozo(juego_t juego){
    if(juego.mozo.cantidad_pedidos > SIN_PEDIDOS){
        printf("- - - - - - - PEDIDOS TOMADOS - - - - - - -\n");
        for(int i = 0; i < juego.mozo.cantidad_pedidos; i++){
            printf("Pedido de la mesa %i; Paciencia de la mesa: %i\n", juego.mozo.pedidos[i].id_mesa+1, juego.mesas[juego.mozo.pedidos[i].id_mesa].paciencia);
        }
    }
    else{
        printf("- - - - - Ningún pedido tomado! - - - - -\n");
    }

    if(juego.mozo.cantidad_bandeja > SIN_PEDIDOS_EN_BANDEJA){
        printf("- - - - - - PEDIDOS EN BANDEJA - - - - - -\n");
        for(int i = 0; i < juego.mozo.cantidad_bandeja; i++){
            printf("Pedido de la mesa %i; Paciencia de la mesa: %i\n", juego.mozo.bandeja[i].id_mesa+1, juego.mesas[juego.mozo.bandeja[i].id_mesa].paciencia);
        }
    }
    else{
        printf("- - - - Ningún pedido en bandeja! - - - -\n");
    }
}

// Pre: -
// Post: Imprime por pantalla información relevante de los pedidos que están en la cocina.
void interfaz_pedidos_de_la_cocina(juego_t juego){
    if(juego.cocina.cantidad_preparacion > SIN_PLATOS_EN_PREPARACION){
        printf("- - - - - PEDIDOS EN PREPARACIÓN - - - - -\n");
        for(int i = 0; i < juego.cocina.cantidad_preparacion; i++){
            printf("Pedido de la mesa %i; Pedido preparado en %i\n", juego.cocina.platos_preparacion[i].id_mesa+1, juego.cocina.platos_preparacion[i].tiempo_preparacion);
        }
    }
    else{
        printf("- - - Ningún pedido en preparación! - - -\n");
    }

    if(juego.cocina.cantidad_listos > SIN_PEDIDOS_EN_BANDEJA){
        printf("- - - - - - - PEDIDOS LISTOS - - - - - - -\n");
        for(int i = 0; i < juego.cocina.cantidad_listos; i++){
            printf("Pedido de la mesa %i; Paciencia de la mesa: %i\n", juego.cocina.platos_listos[i].id_mesa+1, juego.mesas[juego.cocina.platos_listos[i].id_mesa].paciencia);
        }
    }
    else{
        printf("- - - - Ningún pedido en bandeja! - - - -\n");
    }
}

void interfaz_datos_utiles(juego_t juego){
    printf("Atende a todas las mesas y junta el dinero necesario ($%i) antes que se acabe la paciencia de los comensales y tus movimientos! (%i)\n", MAX_DINERO, MAX_MOVIMIENTOS);
    printf("Move a Linguini (L) con %c, %c, %c o %c, %c para agarrar/soltar la mopa, %c para activar los patines y %c para tomar los pedidos de las mesas.\n", MOVER_ADELANTE, MOVER_ATRAS, MOVER_DERECHA, MOVER_IZQUIERDA, AGARRAR_MOPA, ACTIVAR_PATINES, TOMAR_PEDIDO);
    printf("Guarda con los charcos cuando vas a entregar los pedidos, si los pisas, se te va a caer tu pedido y los comensales se van a ir!\n");
    printf("Dinero: %i.\n", juego.dinero);
    printf("Pedidos: %i.\n", juego.mozo.cantidad_pedidos);
    printf("Movimientos: %i.\n", juego.movimientos);
    printf("Patines: %i\n", juego.mozo.cantidad_patines);
}

// Pre: -
// Post: Maneja la interfaz gráfica a imprimirse por pantalla de elementos importantes del juego.
void interfaz_grafica_juego(juego_t juego){
    interfaz_datos_utiles(juego);
    interfaz_pedidos_mozo(juego);
    interfaz_pedidos_de_la_cocina(juego);
}

void mostrar_juego(juego_t juego){
    char terreno[MAX_FILAS][MAX_COLUMNAS] = {};
    inicializar_terreno(terreno);
    asignar_elementos_terreno(juego, terreno);
    mostrar_terreno(terreno, juego);
    interfaz_grafica_juego(juego);
}

int estado_juego(juego_t juego){
    int estado = JUEGO_INICIADO;
    if(juego.dinero >= MAX_DINERO && juego.movimientos == MAX_MOVIMIENTOS){
        estado = JUEGO_GANADO;
        system("clear");
        printf(" \
                ######        ###     ##    ##     ###      ######   ########  ######## \n \
                ##    ##     ## ##    ###   ##    ## ##    ##    ##     ##     ##      \n \
                ##          ##   ##   ####  ##   ##   ##   ##           ##     ##      \n \
                ##   ####  ##     ##  ## ## ##  ##     ##   ######      ##     ######  \n \
                ##    ##   #########  ##  ####  #########        ##     ##     ##      \n \
                ##    ##   ##     ##  ##   ###  ##     ##  ##    ##     ##     ##      \n \
                ######     ##     ##  ##    ##  ##     ##   ######      ##     ######## \n");
    }
    else if(juego.dinero < MAX_DINERO && juego.movimientos == MAX_MOVIMIENTOS){
        estado = JUEGO_PERDIDO;
        system("clear");
        printf(" \
                ########  ######## ########  ########  ####  ######  ######## ######## \n \
                ##     ## ##       ##     ## ##     ##  ##  ##    ##    ##    ##       \n \
                ##     ## ##       ##     ## ##     ##  ##  ##          ##    ##       \n \
                ########  ######   ########  ##     ##  ##   ######     ##    ######   \n \
                ##        ##       ##   ##   ##     ##  ##        ##    ##    ##       \n \
                ##        ##       ##    ##  ##     ##  ##  ##    ##    ##    ##       \n \
                ##        ######## ##     ## ########  ####  ######     ##    ######## \n");
    }
    return estado;
}

void destruir_juego(juego_t *juego){
    if(juego->cocina.platos_preparacion != NULL){
        free(juego->cocina.platos_preparacion);
    }
    if(juego->cocina.platos_listos != NULL){
        free(juego->cocina.platos_listos);
    }
}
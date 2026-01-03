#include "restaurant.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int JUEGO_INCIADO = 0;

const char TECLA_ADELANTE = 'W';
const char TECLA_ATRAS = 'S';
const char TECLA_DERECHA = 'D';
const char TECLA_IZQUIERDA = 'A';
const char TECLA_MOPA = 'O';
const char TECLA_TOMAR_PEDIDO = 'T';
const char TECLA_ACTIVAR_PATINES = 'P';

bool tecla_es_valida(char accion){
    return(accion == TECLA_ADELANTE || accion == TECLA_ATRAS || accion == TECLA_DERECHA || 
        accion == TECLA_IZQUIERDA || accion == TECLA_MOPA || accion == TECLA_TOMAR_PEDIDO || accion == TECLA_ACTIVAR_PATINES);
}

int main () {
    srand (( unsigned)time(NULL));
    juego_t juego;
    char accion;

    inicializar_juego(&juego);
    mostrar_juego(juego);
    
    int estado = JUEGO_INCIADO;

    while(estado == JUEGO_INCIADO){
        scanf(" %c", &accion);
        system("clear");
        while(!tecla_es_valida(accion)){
            mostrar_juego(juego);
            printf("Oprimí una tecla válida.\n");
            scanf(" %c", &accion);
        }
        system("clear");
        realizar_jugada(&juego, accion);
        mostrar_juego(juego);
        estado = estado_juego(juego);
    }
    destruir_juego(&juego);
    return 0;
}
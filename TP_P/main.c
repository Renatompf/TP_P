// Author: Renato Miguel Pais Freire - 2018019771
#include <stdio.h>
#include <stdlib.h>
#include "sala.h"
#include "pessoas.h"
#include "propagacao.h"
#include "undo.h"
#include "menu.h"
#include "utils.h"


int main(int argc, char** argv) {
    plocal rooms;
    pPessoa person;
    pPessoa undo[3] = {NULL, NULL, NULL};
    int nrRooms = 0;
    int nrPeople = 0;
    
    initRandom();
    rooms = readRooms(&nrRooms);
    if(rooms == NULL)
        return 1;
    
    printf("\n--------Locais---------\n");
    if (validateRooms(rooms, nrRooms) != 0) {
        printf("ERRO > Validacao de salas deu erro\n");
        return 1;
    }
    showRooms(rooms, nrRooms);


    person = readPeople(&nrPeople);
    if(person == NULL)
        return 1;
    printf("\n--------Pessoas---------\n");
    if (inicializerPeople(person, nrRooms, rooms, nrPeople) == 1) {
        printf("ERRO > Validacao de pessoas deu erro\n");
        return 1;
    }
    showPeople(person, nrPeople);

    menu(person, nrPeople, rooms, nrRooms, undo, 3);

    return (EXIT_SUCCESS);
}


//Renato Miguel Pais Freire - 2018019771
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sala.h"
#include "pessoas.h"
#include "propagacao.h"
#include "undo.h"
#include "utils.h"

void menu(pPessoa person, int nrPeople, plocal rooms, int nrRooms, pPessoa undo[], int tamUndo) {
    int escolha;
    do {
        printf("--------------Menu--------------\n");
        printf("1 - Avancar 1 Iteracao Na Simulacao\n");
        printf("2 - Apresentar Estatistica\n");
        printf("3 - Adicionar Doente\n");
        printf("4 - Transferir Pessoas\n");
        printf("5 - Faz undo(Ate 3 iteracoes)\n");
        printf("6 - Terminar Simulacao\n");
        printf("--------------------------------\n");
        printf("Opcao: ");
        scanf("%d", &escolha);
        switch (escolha) {
            case (1):
                avancaSimulacao(person, nrPeople, rooms, nrRooms);
                saveUndo(undo, person);
                showPeople(person, nrPeople);
                break;
            case (2):
                apresentaEstatistica(person, nrPeople, rooms, nrRooms);
                break;
            case(3):
                adicionaDoente(person, rooms, nrRooms, &nrPeople);
                showPeople(person, nrPeople);
                break;
            case(4):
                transferPeople(person, nrPeople, rooms, nrRooms);
                showPeople(person, nrPeople);
                break;
            case(5):
                person = callUndo(undo, tamUndo, person);
                showPeople(person, nrPeople);
                break;
            case(6):
                terminaSimulacao(person, nrPeople, rooms, nrRooms, undo, tamUndo);
                break;
            case(7):
                return;
        }
    } while (escolha != 6);
}
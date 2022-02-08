//Renato Miguel Pais Freire - 2018019771
#ifndef PESSOAS_H
#define PESSOAS_H
#include "sala.h"

typedef struct pessoas pessoa, *pPessoa;

struct pessoas {
    char id[50];
    int idade;
    char estado;
    int dias;
    plocal sala;
    pPessoa next;
};

pPessoa readPeople(int* nrPeople);

void showPeople(pPessoa person, int nrPeople);

//int inicializerPeople(pPessoa person, int nrRooms, plocal rooms);

int inicializerPeople(pPessoa person, int nrRooms, plocal rooms, int nrPeople);

int checkID(pPessoa person);

int checkNPersonID(pPessoa person, char* newPersonID);

int checkDiasDoenca(int idade, int diasDoenca);

int checkAge(int idade);

int validatePeople(pPessoa person);

pPessoa getPessoa(pPessoa person, int rndPerson);

//void distributePeople(pPessoa person, int nrRooms, plocal rooms);

void distributePeople(pPessoa person, int nrRooms, plocal rooms, int nrPeople);

void emparelhaSala(pPessoa person, plocal room);

int checkRoom(int auxSala, int nrRooms, plocal rooms);

pPessoa addPeople(pPessoa person, plocal aux, int* nrPeople);

void transferePessoas(int idOrigem, int idDestiny, int N, plocal rooms, int nrRooms, pPessoa person, plocal roomDestiny, int originarios, int destinatarios);

int contaPessoas(pPessoa person);

float contaDoentes(pPessoa person);

float contaSaudaveis(pPessoa person);

float contaImunes(pPessoa person);

float contaNDoentes(pPessoa person);

void freeList(pPessoa person);

#endif /* PESSOAS_H */


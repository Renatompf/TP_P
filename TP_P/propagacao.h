//Renato Miguel Pais Freire - 2018019771
#ifndef PROPAGACAO_H
#define PROPAGACAO_H
#include "pessoas.h"
#include "sala.h"

int contaPessoasSala(pPessoa person, plocal room);

pPessoa getPessoaSala(pPessoa person, int indicePessoa, int idSala);

void taxaDisseminacao(pPessoa person, int nrPeople, plocal rooms, int nrRooms);

int verificaImunidade();

void maxDias(pPessoa person);

void verificaRecuperacao(pPessoa person);

float taxaDoentes(pPessoa person);

float taxaSaudaveis(pPessoa person);

float taxaImunes(pPessoa person);

float taxaNDoentes(pPessoa person);

//void distribPessoasV1(pPessoa person, plocal rooms, int nrRooms);

void distribPessoasV2(pPessoa person, plocal rooms, int nrRooms);

void apresentaEstatistica(pPessoa person, int nrPeople, plocal rooms, int nrRooms);

void adicionaDoente(pPessoa person,plocal rooms, int nrRooms, int* nrPeople);

void diminuiDias(pPessoa person);

void transferPeople(pPessoa person, int nrPeople, plocal rooms, int nrRooms);

void avancaSimulacao(pPessoa person, int nrPeople, plocal rooms, int nrRooms);

void terminaSimulacao(pPessoa person, int nrPeople, plocal rooms, int nrRooms, pPessoa undo[], int tamUndo);

#endif /* PROPAGACAO_H */


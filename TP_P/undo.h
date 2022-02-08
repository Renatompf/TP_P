//Renato Miguel Pais Freire - 2018019771
#ifndef UNDO_H
#define UNDO_H
#include "pessoas.h"
#include "sala.h"

void preenchePessoa(pPessoa undo, pPessoa person);

pPessoa reservarMemoria(pPessoa person);

void saveUndo(pPessoa* undo, pPessoa person);

pPessoa callUndo(pPessoa undo[], int tamUndo, pPessoa person);

#endif /* UNDO_H */


//Renato Miguel Pais Freire - 2018019771
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoas.h"
#include "sala.h"
#include "propagacao.h"
#include "utils.h"

void preenchePessoa(pPessoa undo, pPessoa person) { //Função usada para preencher a posicão x do undo
    strcpy(undo->id, person->id);
    undo->idade = person->idade;
    undo->estado = person->estado;
    undo->dias = person->dias;
    undo->sala = person->sala;
    undo->next = person->next;
}

pPessoa reservarMemoria(pPessoa person) { //Reservar memória na posição x do undo
    pPessoa undo = NULL;
    pPessoa lastPerson = NULL;

    pPessoa tmpPerson = (pPessoa) malloc(sizeof (pessoa)); //Alocação de memória
    if (!tmpPerson) {
        printf("ERRO > Na alocação de memoria ao fazer o backup\n");
        return NULL;
    }

    while (person != NULL) { //Percorrer a lista principal
        preenchePessoa(tmpPerson, person); //Preencher a pessoa onde reservamos memória
        if (undo == NULL && lastPerson == NULL) { //Se a lista undo tiver vazia
            undo = tmpPerson; //A primeira posição vai = tmpPerson
            lastPerson = tmpPerson; //A ultima posição vai ser igual à primeira porque são a mesma
        } else { //Se a lista ligada já não tiver vazia
            lastPerson->next = tmpPerson; //A pessoa next á última = Pessoa auxiliar que preenchemos
            lastPerson = lastPerson->next; //E a ultima pessoa da lista ligada = next pessoa
        }

        lastPerson->next = NULL; //Não há pessoa a seguir à ultima

        tmpPerson = (pPessoa) malloc(sizeof (pessoa));
        if (!tmpPerson) {
            printf("ERRO > Na alocação de memoria ao fazer o backup\n");
            return NULL;
        }
        person = person -> next; //avançar na lista
    }
    free(tmpPerson);
    return undo;
}

void saveUndo(pPessoa* undo, pPessoa person) { //undo é ponteiro de ponteiro porque vou mexer diretamente na sua memória
    //Vetor undo organizado entre 0 e 2, sendo 0 o espaço reservado há mais tempo e 2 o mais recente

    for (int i = 0; i < 2; i++) //Reorganizar a lista 0 é a mais antiga, 2 a mais recente
        undo[i] = undo[i + 1];

    undo[2] = reservarMemoria(person); //Guardar a ultima iteração em undo[2]
}

pPessoa callUndo(pPessoa undo[], int tamUndo, pPessoa person) { //Substitui o undo pela lista
    int nrIteracoes;

    for (int i = 2; i < tamUndo && i >= 0; i--) { //Printf indicativo para o user
        if (undo[i] == NULL)
            printf("Posicao %d [NULL]\n", i + 1);
        else
            printf("Posicao %d pronta para undo\n", i + 1);
    }

    do { //Perguntar para qual iteração o user quer ir, enquanto essa estiver entre 1 e 3 e não tiver vazia
        printf("\nQuantas iteracoes quer voltar atras?\nNota: Selecione 0 para nao voltar nenhuma iteracao atras\n");
        scanf("%d", &nrIteracoes);
        if (nrIteracoes < 1 || nrIteracoes > 3) {
            printf("Nao e possivel %d itercoes atras\n", nrIteracoes);
            return person;
        }
        if (nrIteracoes == 0)
            return person;
    } while (undo[3 - nrIteracoes] == NULL);
    printf("\n");

    freeList(person); //limpar lista principal
    person = undo[3 - nrIteracoes]; //person vai ficar a apontar para o espaço de memoria

    for (int i = 0; i < tamUndo; i++) //Faz free a todas as posições do undo menos a que está a ser utilizada
        if (i != (3 - nrIteracoes))
            freeList(*(undo + i));

    for (int i = 0; i < tamUndo; i++) //Mete a null todas a posições do undo menos a que está a ser utilizada
        if (i != (3 - nrIteracoes))
            *(undo + i) = NULL;

    for (int i = 0; i < 2; i++) //Reorganizar a lista 0 é a mais antiga, 2 a mais recente
        undo[i] = undo[i + 1];

    return person;
}
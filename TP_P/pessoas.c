//Renato Miguel Pais Freire - 2018019771
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoas.h"
#include "sala.h"
#include "propagacao.h"
#include "utils.h"

pPessoa readPeople(int* nrPeople) {
    //Criar Ponteiro para 1ª pessoa
    pPessoa firstPerson = NULL;
    pPessoa lastPerson = NULL;

    //Get file name
    char fichName[50];
    printf("Nome do ficheiro das pessoas:");
    scanf("%s", &fichName);

    //Open file
    FILE *f = fopen(fichName, "rt");
    if (!f) { //Erro ao abrir o ficheiro
        printf("ERRO > Impossivel abrir o ficheiro %s", fichName);
        return 0;
    }

    //Criar ponteiro e estrutura auxiliares
    pPessoa aux = (pPessoa) malloc(sizeof (pessoa));


    //Leitura do ficheiro
    while (fscanf(f, " %s %d %c", &(aux->id), &(aux->idade), &(aux->estado)) == 3) {
        //Acaba de preencher estrutura
        aux->sala = NULL;
        if (aux->estado == 'D') //Caso o estado seja D
            fscanf(f, "%d", &(aux->dias));
        else
            aux->dias = 0;

        if (firstPerson == NULL && lastPerson == NULL) {
            //Lista ligada ainda nao existe
            firstPerson = aux;
            lastPerson = aux;
        } else {
            //Lista ligada já existe
            lastPerson->next = aux;
            lastPerson = lastPerson->next;
        }

        lastPerson->next = NULL;

        (*nrPeople)++;

        aux = (pPessoa) malloc(sizeof (pessoa));
    }
    free(aux);
    fclose(f);
    return firstPerson;
}

void showPeople(pPessoa person, int nrPeople) { //Para mostrar as pessoas
    while (person != NULL) { //Enquanto a lista tirar uma próxima pessoa
        printf("%s %d %c ", person->id, person->idade, person->estado);
        if (person->estado == 'D')
            printf("\t%d ", person->dias);
        else
            printf("\t ");
        if (person->sala != NULL)
            printf("\t sala: %d ", person->sala->id);
        else
            printf("nao tem sala");
        printf("\n");
        person = person->next;
    }
    printf("\n");
}

int checkRoom(int auxSala, int nrRooms, plocal rooms) { //Verificar se a sala tem capacidade para ter mais pessoas
    for (int i = 0; i < nrRooms; i++) {
        if (auxSala == i) {
            if (rooms[i].capacidade <= 0) {
                return 1;
            }
        }
    }
    return 0;
}

int checkAge(int idade) { //Verifica se a idade e/ou os dias introduzidos são positivos
    if (idade <= 0)
        return 0;
    return 1;
}

int checkDiasDoenca(int idade, int diasDoenca) {
    if (diasDoenca >= (idade / 10) + 5 || diasDoenca <= 0) //Se os dias das pessoas ultrapassar o limite dito e for negativo
        return 0;
    return 1;
}

int checkID(pPessoa person) { //verifica se existem pessoas com o mesmo id; Retorna 0 se der erro, 1 se der certo
    pPessoa seguinte;
    while (person->next != NULL) { //Percorrer a lsita das pessoas
        seguinte = person->next; //Percorrer a partir da prox pessoa
        while (seguinte != NULL) {
            if (strcmp(seguinte->id, person->id) == 0) { //Se o nome for igual, dá erro
                printf("Existem pessoas com o mesmo ID\n");
                return 0;
            }
            seguinte = seguinte->next;
        }
        person = person->next;
    }
    return 1;
}

int checkNPersonID(pPessoa person, char* newPersonID) { //Verifica se a nova pessoa tem o id igual à algum
    pPessoa tmpPerson = person;
    while (tmpPerson != NULL) {
        if (strcmp(tmpPerson->id, newPersonID) == 0)
            return 0;
        tmpPerson = tmpPerson->next;
    }
    return 1;
}

void emparelhaSala(pPessoa person, plocal room) { //Atribuir uma sala a um paciente
    person->sala = room;
    person->sala->capacidade--;
}

int validatePeople(pPessoa person) { //Verificar se a pessoa apresenta erros
    while (person != NULL) {
        if (person->idade <= 0) {
            printf("ERRO > %s tem uma idade negativa\n", person->id);
            return 0;
        }
        if (person->estado == 'D') {
            if (person->dias <= 0) {
                printf("ERRO > %s tem dias de doenca negativos\n", person->id);
                return 0;
            }
        }
        person = person->next;
    }

    return 1;
}

pPessoa getPessoa(pPessoa person, int rndPerson) { //Ir buscar a pessoa do rndNumber
    int contador = 0;
    while (person != NULL) {
        if (person->sala == NULL) {
            if (contador == rndPerson)
                return person;
            else
                contador++;
        }
        person = person->next;
    }
    return NULL;
}

void distributePeople(pPessoa person, int nrRooms, plocal rooms, int nrPeople) { //Distribuir as pessoas pelas salas
    int auxSala; //Inteiro para guardar o nr Random das salas
    int rndPerson; //Inteiro para guardar o nr Random das pessoas
    int totalPessoas = nrPeople;
    pPessoa auxLista = person; //Lista auxiliar
    pPessoa tmpPerson = NULL; //Ponteiro auxiliar para guardar a pessoa random

    while (totalPessoas != 0) {
        rndPerson = intUniformRnd(0, totalPessoas - 1);
        tmpPerson = getPessoa(person, rndPerson);
        auxSala = intUniformRnd(0, (nrRooms - 1)); //Função random disponibilizada
        if (checkRoom(auxSala, nrRooms, rooms) == 0 && tmpPerson->sala == NULL) {
            emparelhaSala(tmpPerson, rooms + auxSala);
        } else {
            tmpPerson->sala = NULL;
        }
        totalPessoas--;
    }
}

int inicializerPeople(pPessoa person, int nrRooms, plocal rooms, int nrPeople) { //Inicializar as pessoas
    if (validatePeople(person) == 1 && checkID(person) == 1) {
        distributePeople(person, nrRooms, rooms, nrPeople);
    } else
        return 1;
}

pPessoa addPeople(pPessoa person, plocal aux, int* nrPeople) { //Adicionar pessoas à lista ligada
    pPessoa tmpPerson = (pPessoa) malloc(sizeof (pessoa)); //Reservar o espaço para a nova pessoa doente
    if (!tmpPerson) {
        printf("ERRO > Na alocacao de memoria\n");
        return NULL;
    }

    //Introdução dos dados da pessoa doente que quer adicionar à lista ligada
    do {
        printf("Introduza o id do doente:");
        scanf("%s", &tmpPerson->id);
    } while (checkNPersonID(person, tmpPerson->id) == 0);

    do {
        printf("Introduza a idade do doente:");
        scanf("%d", &(tmpPerson->idade));
    } while (checkAge(tmpPerson->idade) == 0);

    do {
        printf("Introduza os dias de infecao do doente:");
        scanf("%d", &(tmpPerson->dias));
    } while (checkDiasDoenca(tmpPerson->idade, tmpPerson->dias) == 0);

    tmpPerson->estado = 'D';
    tmpPerson->sala = aux;
    tmpPerson->next = NULL;
    
    while(person->next != NULL) //Percorrer a lista ligada até ao fim para adicionar a nova pessoa no fim
        person=person->next;
    person->next = tmpPerson; //A última pessa irá ser a que acabamos de "criar"
    (*nrPeople)++; //Incrementamos o nr de pessoas
    
    return person;
}

void transferePessoas(int idOrigem, int idDestiny, int N, plocal rooms, int nrRooms, pPessoa person, plocal roomDestiny, int originarios, int destinatarios) {
    //Originarios e destinarios == nr de pessoas existentes no quarto de origem e quarto de destino
    int i = 0, j = 0;
    pPessoa tmpPerson = NULL;


    if (originarios == 1) { //Se só houver uma pessoa na sala de origem 
        while (person != NULL) {
            if (person->sala->id == idOrigem) { //Vai procurar essa pessoa
                person->sala = NULL;
                emparelhaSala(person, roomDestiny); //Emparelha com a sala destino
            }
            person = person->next;
        }
    } else { //Se houver mais do que uma
        while (i < N) { //Enquanto o i for menor que o nr de pessoas que se querem transferir
            int rndPerson = intUniformRnd(0, originarios); //Gera nr random 
            tmpPerson = getPessoaSala(person, rndPerson, idOrigem); //Pessoa com o nr Random
            tmpPerson->sala = NULL;
            emparelhaSala(tmpPerson, roomDestiny);
            i++;
            originarios--;
        }
    }
}

int contaPessoas(pPessoa person) {
    //Var auxiliar para contar o nr de pessoas
    float nrPessoas = 0;

    while (person != NULL) {
        nrPessoas++; //Incrementa o nr de pessoas
        person = person->next;
    }
    return nrPessoas;
}

float contaDoentes(pPessoa person) { //Conta pessoas doentes
    //Var auxiliar para contar o nr de doentes
    float nrDoentes = 0;

    while (person != NULL) {
        if (person->estado == 'D') //Se o estado for doente
            nrDoentes++; //Incrementa o nr de doentes
        person = person->next;
    }
    return nrDoentes;
}

float contaSaudaveis(pPessoa person) { //Conta pessoas saudaveis
    //Var auxiliar para contar o nr de pessoas cujo estado == 'S'
    float nrSaudaveis = 0;

    while (person != NULL) {
        if (person->estado == 'S') //Se o estado for saudável
            nrSaudaveis++; //Incrementa o nr de saudáveis
        person = person->next;
    }
    return nrSaudaveis;
}

float contaImunes(pPessoa person) { //Conta pessoas imunes
    //Var auxiliar para contar o nr de pessoas imunes
    float nrImunes = 0;

    while (person != NULL) {
        if (person->estado == 'I') //Se o estado for imune
            nrImunes++; //Incrementa o nr de pessoas imunes
        person = person->next;
    }
    return nrImunes;
}

float contaNDoentes(pPessoa person) { //Conta pessoas não doentes
    //Var auxiliar para contar o nr de não doentes
    float nrNDoentes = 0;

    while (person != NULL) {
        if (person->estado != 'D') //Se o estado for doente
            nrNDoentes++; //Incrementa o nr de doentes
        person = person->next;
    }
    return nrNDoentes;
}

void freeList(pPessoa person) { //Liberta lista de pessoas
    pPessoa anterior;
    while (person != NULL) {
        anterior = person;
        person = person->next;
        free(anterior);
    }
}
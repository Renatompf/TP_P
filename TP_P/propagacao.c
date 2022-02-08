//Renato Miguel Pais Freire - 2018019771
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sala.h"
#include "pessoas.h"
#include "propagacao.h"
#include "undo.h"
#include "utils.h"

int contaPessoasSala(pPessoa person, plocal room) { //Conta quantas pessoas estão num determinado local
    int pessoasSala = 0;
    while (person != NULL) {
        if (person->sala->id == room->id) { //Verifica se o id do local é o mesmo
            pessoasSala++;
        }
        person = person->next;
    }
    return pessoasSala;
}

pPessoa getPessoaSala(pPessoa person, int indicePessoa, int idSala) {
    int i = 0;

    while (person != NULL) {
        if (person->sala->id == idSala) {
            if (i == indicePessoa)
                return person;
            else
                i++;
        }
        person = person->next;
    }

    return NULL;
}

void taxaDisseminacao(pPessoa person, int nrPeople, plocal rooms, int nrRooms) {
    //Declaração de vars. auxiliares
    pPessoa auxPerson;
    pPessoa copyList = person;
    int pessoasSala;
    int i = 0;

    //Percorrer a lista ligada
    while (copyList != NULL) {
        if (copyList->estado == 'D') { //Se essa pessoa estiver doente
            pessoasSala = contaPessoasSala(person, copyList->sala); //Conta nr de pessoas na sala
            int percPR = (pessoasSala * 0.05);
            while (i != (int) (pessoasSala * 0.05)) { //Enquanto o i for diferente de 5% das pessoas presentes naquela sala
                int randomPeople = intUniformRnd(0, pessoasSala); //Gera uma pessoa de forma aleatória
                auxPerson = getPessoaSala(person, randomPeople, person->sala->id);
                if (auxPerson->estado != 'I') {
                    auxPerson->estado = 'D';
                    printf("\nPessoa %s foi infetada\n", auxPerson->id);
                }
                i++;
            }
        }
        copyList = copyList->next;
    }
}

int verificaImunidade() { //Verifica se uma pessoa fica imune, ou não
    if (probEvento(0.2) == 1)
        return 1;
    else
        return 0;
}

void maxDias(pPessoa person) { //Verifica se alguma das pessoas presentes na lista ligada já atingiu o nr de dias maximo infetado
    while (person != NULL) { //Percorre a lista ligada
        if (person->estado == 'D') { //Se essa pessoa estiver doente
            int dias = (person->idade / 10); // Calc. auxiliar para dar nr de dias em cada dezena de anos
            if (person->dias == 5 + dias) { //Se estiver doente há (5+1 por cada dez anos) dias
                if (verificaImunidade() == 1) { //Verificar se essa pessoa fica imune
                    person->estado = 'I';
                    person->dias = 0;
                } else { //Se não ficar imune, apenas fica saudavel
                    person->estado = 'S';
                    person->dias = 0;
                }
            }
        }
        person = person->next;
    }
}

void verificaRecuperacao(pPessoa person) { //Verifica se uma pessoa está recuperada, ou não
    while (person != NULL) {
        if (person->estado == 'D') {
            float prob = (1 / (float) person->idade);
            if (probEvento(prob) == 1) { //Se se verificar um possibilidade de (1/idade da pessoa)
                if (verificaImunidade() == 1) { //Verifica se uma pessoa é imune
                    person->estado = 'I';
                    person->dias = 0;
                } else { //Se não for, fica só saudável
                    person->estado = 'S';
                    person->dias = 0;
                }
            }
        }
        person = person->next;
    }
}

float taxaDoentes(pPessoa person) { //Calcula a taxa de doentes total
    float nrDoentes = contaDoentes(person);
    int nrPeople = contaPessoas(person);
    return (nrDoentes / (float) nrPeople);
}

float taxaSaudaveis(pPessoa person) { //Calcula a taxa de pessoas cujo estado == 'S'
    float nrSaudaveis = contaSaudaveis(person);
    int nrPeople = contaPessoas(person);
    return (nrSaudaveis / (float) nrPeople);
}

float taxaImunes(pPessoa person) { //Calcula a taxa de pessoas imunes
    float nrImunes = contaImunes(person);
    int nrPeople = contaPessoas(person);
    return (nrImunes / (float) nrPeople);
}

float taxaNDoentes(pPessoa person) { //Calcula a taxa de pessoas imunes e saudaveis
    float nrNDoentes = contaNDoentes(person);
    int nrPeople = contaPessoas(person);
    return (nrNDoentes / (float) nrPeople);
}

/*void distribPessoasV1(pPessoa person, plocal rooms, int nrRooms) {
    int nrPSala = 0;
    for (int i = 0; i < nrRooms; i++){
        while(person->next != NULL){
            if(rooms[i].id == person->sala->id)
                nrPSala++;
            person = person->next;
        }
        printf("Na sala %d estao %d pessoas\n", rooms[i].id, nrPSala);
        nrPSala = 0;
    }
    
}*/

void distribPessoasV2(pPessoa person, plocal rooms, int nrRooms) { //Distribuir as pessoas pelas salas
    pPessoa auxLinkedList = person;
    for (int i = 0; i < nrRooms; i++) { //percorrer as salas
        printf("Local %d:\n", rooms[i].id);
        while (auxLinkedList != NULL) { //Percorrer as pessoas
            if (rooms[i].id == auxLinkedList->sala->id) //Se tiverem o mesmo id
                printf("%s\n", auxLinkedList->id);
            auxLinkedList = auxLinkedList->next;
        }
        printf("\n");
        auxLinkedList = person;
    }
}

void apresentaEstatistica(pPessoa person, int nrPeople, plocal rooms, int nrRooms) { //Aprensenta resultados retornados em funções feitas em anteriormente
    printf("---------------------Estatisticas---------------------\n");
    printf(">Taxas:\n");
    printf("Taxa de pessoas saudaveis: %0.2f\n", taxaSaudaveis(person));
    printf("Taxa de pessoas infetados: %0.2f\n", taxaDoentes(person));
    printf("Taxa de pessoas imunes: %0.2f\n", taxaImunes(person));
    printf("Taxa de pessoas nao doentes: %0.2f\n", taxaNDoentes(person));
    printf("\n>Distribuicao:\n");
    distribPessoasV2(person, rooms, nrRooms);
}

void adicionaDoente(pPessoa person, plocal rooms, int nrRooms, int* nrPeople) { //Adicionar um novo doente a uma sala especifica
    //declaração de vars auxiliares
    int id;
    plocal auxRoom = NULL;

    while (auxRoom == NULL) {
        printf("Introduza o id do local:");
        scanf("%d", &id);
        auxRoom = searchRoom(id, rooms, nrRooms); //Vai ver se existe um local com este id
    }

    person = addPeople(person, auxRoom, nrPeople);
    if (person == NULL) {
        printf("ERRO > Ao adicionar novo doente\n");
        return;
    }

    auxRoom->capacidade--; //reduz a capacidade desse local

}

void aumentaDias(pPessoa person) { //Diminuir os dias das pessoas que estão doentes
    while (person != NULL) {
        if (person->estado == 'D')
            person->dias++;
        person = person->next;
    }
}

void transferPeople(pPessoa person, int nrPeople, plocal rooms, int nrRooms) { //transferir pessoas de quarto
    int idOrigem, idDestiny, N;
    plocal roomOrigem = NULL;
    plocal roomDestiny = NULL;
    int originarios = 0;
    int destinatarios = 0;

    do {
        do {
            printf("Id do local de origem:");
            scanf("%d", &idOrigem); //ID do local onde a pessoa estava
            roomOrigem = searchRoom(idOrigem, rooms, nrRooms); //Encontrar o quarto de origem
            if (!roomOrigem)//Caso não encontre o quarto de origem
                printf("ERRO > Local com o id %d nao foi encontrado\n", idOrigem);
        } while (roomOrigem == NULL);

        do {
            printf("Id do local de destino:");
            scanf("%d", &idDestiny); //Id do local destino
            roomDestiny = searchRoom(idDestiny, rooms, nrRooms); //Encontrar o quarto de destino
            if (!roomDestiny) //Caso não encontre o quarto de destino
                printf("ERRO > Local com o id %d nao foi encontrado\n", idDestiny);
        } while (roomDestiny == NULL);

        originarios = contaPessoasSala(person, roomOrigem); //Auxiliar para contar quantas pessoas podem ir para o destino
        destinatarios = contaPessoasSala(person, roomDestiny); //Auxiliar para contar quantas pessoas estão no destino
        if (checkLink(idOrigem, idDestiny, rooms, nrRooms) == 1) //Se os locais não tiverem diretamente ligados
            printf("ERRO > Quartos com esse id nao tem ligacao direta\n");
    } while (checkLink(idOrigem, idDestiny, rooms, nrRooms) == 1);

    do {
        printf("Numero de pessoas que quer transferir:");
        scanf("%d", &N); //Numero de pessoas que quer transferir
        printf("\n");
        if (N <= 0) //Caso queira enviar um nr negativos de pessoas
            printf("ERRO > Numero invalido\n");
        if (N > originarios) //Caso queira enviar mais pessoas do que aquelas que estão no quarto
            printf("ERRO > Nao pode enviar mais pessoas do que aquelas que o local tem\n");
        if (roomDestiny->capacidade - N < 0) //Caso ultrapasse a capacidade
            printf("ERRO > Local indicado nao tem capacidade\n");
    } while (N <= 0 || N > originarios || roomDestiny->capacidade - N < 0);

    transferePessoas(idOrigem, idDestiny, N, rooms, nrRooms, person, roomDestiny, originarios, destinatarios);
    printf("\n");
}

void avancaSimulacao(pPessoa person, int nrPeople, plocal rooms, int nrRooms) {
    taxaDisseminacao(person, nrPeople, rooms, nrRooms);
    verificaRecuperacao(person);
    maxDias(person);
    aumentaDias(person);
}

int contaPessoasInfetadasSala(pPessoa person, plocal sala){
    int nrInfetados = 0;
    while(person!=NULL){
        if(person->sala->id == sala->id){
            if(person->estado == 'D')
                nrInfetados++;
        }
        person= person->next;
    }
    return nrInfetados;
}

void terminaSimulacao(pPessoa person, int nrPeople, plocal rooms, int nrRooms, pPessoa undo[], int tamUndo) {
    //Abertura do ficheiro report
    FILE *f = fopen("report.txt", "wt");
    if (!f) {
        printf("ERRO > Na abertura do ficheiro\n");
        return;
    }


    //Preenchimento do ficheiro report
    fprintf(f, "Populacao:\n");
    fprintf(f, "Total de pessoas: %d\n", contaPessoas(person));
    fprintf(f, "Numero de infetados: %.0f\n", contaDoentes(person));
    fprintf(f, "Numero de pessoas com estado saudavel: %.0f\n", contaSaudaveis(person));
    fprintf(f, "Numero de pessoas imunes: %.0f\n", contaImunes(person));
    fprintf(f, "Total de pessoas saudaveis: %.0f\n", contaNDoentes(person));
    fprintf(f, "\n");
    fprintf(f, "Distribuicao da populacao:\n");
    pPessoa auxLinkedList = person;
    for (int i = 0; i < nrRooms; i++) { //percorrer as salas
        fprintf(f, "Local %d:\n", rooms[i].id);
        while (auxLinkedList != NULL) { //Percorrer as pessoas
            if (rooms[i].id == auxLinkedList->sala->id) //Se tiverem o mesmo id
                fprintf(f, "%s\n", auxLinkedList->id);
            auxLinkedList = auxLinkedList->next;
        }
        fprintf(f, "\n");
        auxLinkedList = person;
    }
    fprintf(f, "Taxas:\n");
    fprintf(f, "Taxa de pessoas infetadas: %0.2f\n", taxaDoentes(person));
    fprintf(f, "Taxa de pessoas com estado saudavel: %0.2f\n", taxaSaudaveis(person));
    fprintf(f, "Taxa de pessoas imunes: %0.2f\n", taxaImunes(person));
    fprintf(f, "Taxa de pessoas saudaveis: %0.2f\n", taxaNDoentes(person));
    pPessoa aux = person;
    int maiornrPessoa = 0;
    int idSala;
    while(aux != NULL){
        if(maiornrPessoa < contaPessoasInfetadasSala(person, aux->sala)){
            maiornrPessoa = contaPessoasInfetadasSala(person, aux->sala);
            idSala = aux->sala->id;
        }
        aux = aux->next;
    }
    
    fprintf(f,"A sala com maior nr de infetados e %d", idSala);
    //fechar o ficheiro report
    fclose(f);
    //----------Ficheiro do user------------
    char fich[50];
    printf("Introduza o nome que pretende no ficheiro\n(NOTA:METER '.txt' no final)\nNomeFich:");
    scanf("%s", &fich); //Perguntar ao user qual o nome que quer dar ao ficheiro

    FILE *userFich = fopen(fich, "wt"); //Abrir o ficheiro
    if (!userFich) {
        printf("ERRO > Na abertura do ficheiro\n");
        return;
    }
    auxLinkedList = person;
    //Preenchimento do ficheiro
    fprintf(userFich, "Populacao:\n");
    while (auxLinkedList != NULL) {
        fprintf(userFich, "Id: %s\nIdade:%d \nEstado:%c\n", auxLinkedList->id, auxLinkedList->idade, auxLinkedList->estado);
        if (auxLinkedList->estado == 'D')
            fprintf(userFich, "Dias de infecao: %d\n", auxLinkedList->dias);
        fprintf(userFich, "\n");
        auxLinkedList = auxLinkedList->next;
    }

    //fechar o ficheiro do user
    fclose(userFich);

    //libertação de memória da lista ligada e do array dinâmico
    freeList(person);
    free(rooms);
    for (int i = 0; i < tamUndo; i++) {
        freeList(*(undo + i));
    }
    free(undo);
    printf("\nObrigado por utilizar o programa\n");
}
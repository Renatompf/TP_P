//Renato Miguel Pais Freire - 2018019771
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sala.h"
#include "pessoas.h"
#include "utils.h"

void copySala(plocal origin, plocal dest) {

    /*int id;
    int capacidade;
    int liga[3];*/

    (*dest).id = (*origin).id;
    (*dest).capacidade = (*origin).capacidade;
    for (int i = 0; i < 3; i++)
        (*dest).liga[i] = (*origin).liga[i];
}

/*plocal readRooms(int* nrRooms) {
    //Create local pointer
    plocal loc;

    //Get file name
    char fichName[50];
    printf("Nome do ficheiro dos locais:");
    scanf("%s", &fichName);

    //Open file
    FILE* f = fopen(fichName, "rb");

    if (!f) { //Erro ao abrir o ficheiro
        printf("\nERRO > Na abertura o ficheiro %s", fichName);
        return NULL;
    } else { //Abriu ficheiro
        int i;
        //Este ponteiro existe para evitar alocar mais espaço do que o necessário
        plocal tmpLoc = (plocal) malloc(sizeof (local));
        if (!tmpLoc) {
            printf("ERRO > Na alocacao de memória\n");
            return NULL;
        }

        for (i = 0; fread(tmpLoc, sizeof (local), 1, f) == 1; i++) {
            if (i == 0) { //Primeira sala
                loc = (plocal) malloc(sizeof (local)); //Alocar apenas 1 sala inicialmente 
                if (!loc) {
                    printf("ERRO > Na alocacao de memória\n");
                    return NULL;
                }
            } else { //Restantes salas
                loc = realloc(loc, sizeof (local)*(i + 1));
                if (!loc) {
                    printf("ERRO > Na realocacao de memória\n");
                    return NULL;
                }
            }
            //Copy info from tmp var to destiny
            copySala(tmpLoc, loc + i);
        }//Fim for

        //Libertar espaço
        free(tmpLoc);

        //Update mrRooms
        (*nrRooms) = i;
    }//Fim else
    fclose(f);
    return loc;
}*/

plocal readRooms(int* nrRooms) {
    //Create local pointer
    plocal loc;

    //Get file name
    char fichName[50];
    printf("Nome do ficheiro dos locais:");
    scanf("%s", &fichName);

    //Open file
    FILE* f = fopen(fichName, "rb");
    if (!f) { //Erro ao abrir o ficheiro
        printf("\nERRO > Na abertura o ficheiro %s", fichName);
        return NULL;
    }
    
    //AuxVar
    local tmpLoc;
    
    //Leitura do ficheiro
    while (fread(&tmpLoc, sizeof (local), 1, f) == 1) {
        if (*nrRooms == 0) {//No caso da primeira sala
            loc = (plocal) malloc(sizeof (local));
            if (!loc) {
                printf("ERRO > Na alocacao de memória\n");
                return NULL;
            }
            loc[*nrRooms] = tmpLoc;
            (*nrRooms)++;
        } else {//Nas restantes salas
            loc = realloc(loc, sizeof (local)*((*nrRooms) + 1));
            if (!loc) {
                printf("ERRO > Na realocacao de memória\n");
                return NULL;
            }
            loc[*nrRooms] = tmpLoc;
            (*nrRooms)++;
        }
    }
    //Fechar o ficheiro
    fclose(f);
    //Retornar o ponteiro para o array dinâmico
    return loc;
}

void showRooms(plocal rooms, int nrRooms) { //PAra mostrar as salas
    for (int i = 0; i < nrRooms; i++) { //Percorrer as salas
        printf("Quarto %d:\nCapacidade %d \n", rooms[i].id, rooms[i].capacidade); //Imprime cada id e capacidade
        for (int j = 0; j < 3; j++) {
            printf("Ligacao %d: %d\n", j + 1, rooms[i].liga[j]); //Imprime cada ligação de cada sala
        }
        printf("\n");
    }
}

int validateRoomIDs(plocal rooms, int nrRooms) { //Verificar se os IDs das salas são unicos e positivos
    for (int j = 0; j < nrRooms - 1; j++) {
        for (int i = j + 1; i < nrRooms; i++) {
            if (rooms[j].id >= 0 && rooms[i].id >= 0) { // Verificar se exsitem ids < 0
                if (rooms[j].id == rooms[i].id) { //Verficação para ver se existem salas com o mesmo id
                    printf("ERRO > Existem salas com o mesmo id\n");
                    return 1;
                }
            } else { //Caso hajam salas com o mesmo id
                printf("ERRO > Existem salas com ids negativos\n");
                return 1;
            }
        }
    }
    return 0;
}

int validateRoomCap(plocal rooms, int nrRooms) { //Validar a capacidade de uma sala
    for (int i = 0; i < nrRooms; i++) { //Percorrer as salas
        if (rooms[i].capacidade < 0) { //Caso a capacidade seja negativa dá erro
            printf("ERRO > Capacidade da sala %d e negativa\n", rooms[i].id);
            return 1;
        }
    }
    return 0;
}

int validateRoomlinks(plocal rooms, int nrRooms) { //Validar se as ligações são mutuas
    for (int i = 0; i < nrRooms; i++) { //Percorre Salas
        for (int j = 0; j < 3; j++) { //Percorre as ligações das salas
            if (rooms[i].liga[j] != -1) { //Se tiver ligada a alguma
                for (int r = 0; r < nrRooms; r++) { //Volta a percorrer as salas
                    if (rooms[i].id != rooms[r].id && rooms[i].liga[j] == rooms[r].id) //Se as salas não forem as mesmas, e se o id da sala for o mesmo da ligação
                        if (rooms[r].liga[0] != rooms[i].id && rooms[r].liga[1] != rooms[i].id && rooms[r].liga[2] != rooms[i].id) { //Caso a ligação não seja mutua
                            printf("ERRO > Existem salas com ligacoes erradas!\n");
                            return 1;
                        }
                }
            }
        }
    }
    return 0;
}

int validateRooms(plocal rooms, int nrRooms) { //Função que engloba tudo
    if (validateRoomIDs(rooms, nrRooms) == 0 && validateRoomCap(rooms, nrRooms) == 0 && validateRoomlinks(rooms, nrRooms) == 0) //Se as validações estiverem todas corretas
        return 0;
    else
        return 1;
}

int checkCap(local room) { //Verifica se há capacidade
    if (room.capacidade > 0)
        return 0;
    else {
        printf("O quarto %d esta cheio", room.id);
        return 1;
    }
}

plocal searchRoom(int id, plocal rooms, int nrRooms) { //Procura local
    for (int i = 0; i < nrRooms; i++) { //percorre locais
        if (rooms[i].id == id) { //Se encontrar o local
            return rooms + i;
        }
    }
    return NULL;
}

/*int checkNCap(int id, int N, plocal rooms, int nrRooms) { //Verifica se o local tem capacidade para acolher N pessoas
    plocal localId = searchRoom(id, rooms, nrRooms);
    if (!localId) {
        printf("ERRO > Nao foi possivel encontrar local com id %d\n", id);
        return 1;
    }
    if ((localId->capacidade - N) >= 0)
        return 0;
    else
        return 1;
}*/

int checkLink(int idOrigem, int idDestiny, plocal rooms, int nrRooms) { //Verifica se o quarto com o idOrigem e com o idDestiny têm ligação direta
    int flag = 1;

    plocal origem = searchRoom(idOrigem, rooms, nrRooms);
    if (!origem) {
        printf("ERRO > Nao foi possivel encontrar local com id %d\n", idOrigem);
        return 1;
    }
    plocal destiny = searchRoom(idDestiny, rooms, nrRooms);
    if (!destiny) {
        printf("ERRO > Noi foi possivel encontrar local com id %d\n", idDestiny);
        return 1;
    }

    for (int i = 0; i < 3; i++) { //percorre as ligações do quarto com o idOrigem
        if (origem->liga[i] == idDestiny) {
            for (int j = 0; j < 3; j++) { //percorre as ligações do quarto com o idDestiny
                if (destiny->liga[j] == idOrigem)
                    flag = 0;
            }
        }
    }
    return flag;
}
//Renato Miguel Pais Freire - 2018019771
#ifndef SALA_H
#define SALA_H

typedef struct sala local, *plocal;
struct sala{
    int id;
    int capacidade;
    int liga[3];
};

void copySala(plocal origin, plocal dest);

plocal readRooms(int* nrRooms);

void showRooms(plocal rooms, int nrRooms);

int validateRoomCap(plocal rooms, int nrRooms);

int validateRoomlinks(plocal rooms, int nrRooms);

int validateRoomIDs(plocal rooms, int nrRooms);

int validateRooms(plocal rooms, int nrRooms);

int checkCap(local room);

plocal searchRoom(int id, plocal rooms, int nrRooms);

int checkNCap(int id, int N, plocal rooms, int nrRooms);

int checkLink(int idOrigem, int idDestiny, plocal rooms, int nrRooms);

#endif /* SALA_H */


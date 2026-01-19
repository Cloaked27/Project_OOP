#pragma once
#include <iostream>

using namespace std;
struct Clienti {
    int row;
    int col;
};
class Pachet {
public:
    int id;
    int reward;
    int deadline;
    int row;
    int col;
    int aparitie;
    bool preluat;
    bool livrat;
};
class AtribuitorPachete {
public:
    Clienti listaclienti[100];
    int nrClienti = 0;
    Pachet pachete[100];
    int nrPachete = 0;
    void getnrClienti(char map[50][50], int row, int col);
    void creeazaPachet(int total, int timp);
};
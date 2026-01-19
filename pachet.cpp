#include "pachet.h"
using namespace std;
    void AtribuitorPachete::getnrClienti(char map[50][50], int row, int col) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (map[i][j] == 'D')
                {
                    listaclienti[nrClienti].row = i;
                    listaclienti[nrClienti].col = j;
                    nrClienti++;
                }
            }
        }
    }
    void AtribuitorPachete::creeazaPachet(int total, int timp) {
        for (int i = 0; i < total; i++) {
            int index = rand()%nrClienti;
            pachete[i].id = i;
            pachete[i].reward = rand()%601 +200;
            pachete[i].row = listaclienti[index].row;
            pachete[i].col = listaclienti[index].col;
            pachete[i].aparitie = (i+1) * timp;
            pachete[i].deadline = pachete[i].aparitie+ rand()%11 + 10;
            pachete[i].livrat = false;
            pachete[i].preluat = false;
            nrPachete++;
        }
    }

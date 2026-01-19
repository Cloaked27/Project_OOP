#pragma once
#include "agent.h"
#include "pachet.h"
#include <vector>

using namespace std;

struct Celula {
    int row;
    int col;
};

class HiveMind {
public:
    int getDist(Agent *a, Pachet p);
    int getCostLivrare(Agent *a, Pachet p);
    Celula gasesteBaza(char map[50][50], int n, int m);
    Celula statieApropiata(int x, int y, char map[50][50], int n, int m);
    Celula statieApropiata(Agent *a, char map[50][50], int n, int m);
    bool poateLivra(Agent *a, Pachet &p, int tickCurent);
    bool poateIntoarce(Agent *a, Pachet &p, char map[50][50], int n, int m);
    int calculeazaProfit(Agent *a, Pachet &p, int tickCurent, char map[50][50], int n, int m);
    Agent* alegeAgentPentruPachet(Pachet &p, vector<Agent *> &agenti, int tickCurent, char map[50][50], int n, int m, Celula baza);
    void atribuiePachete(vector<Agent *> &agenti, Pachet pachete[], int nrPachete, int tickCurent, char map[50][50], int n, int m);
    void miscaAgenti(vector<Agent*>& agenti, char map[50][50], int n, int m, int& scorGlobal);
    //pot sa recunosc ca nu am cel mai elegant algoritm de hive mind, dar pe destule dintre teste a functionat ok, so yeah. E destul de brute-force, IMO, a ajuns in forma finala dupa mai multe foi in care m-am jucat cu patrate sa vad ce si cum se intampla
};
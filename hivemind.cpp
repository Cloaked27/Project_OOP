#include "hivemind.h"
#include "simulation_setup.h"
#include <algorithm> // Necesar pentru std::sort
#include <cmath>     // Necesar pentru abs()
#include <iostream>  // Necesar pentru cout

using namespace std;

int HiveMind::getDist(Agent *a, Pachet p) {
    int distx = 0, disty = 0, distance = 0;
    distx = a->getRows() - p.row;
    disty = a->getCols() - p.col;
    distance = abs(distx) + abs(disty);
    return distance;
} //sa putem sa avem distanta dintre orice agent si orice pachet, o sa ne ajute pe parcurs

int HiveMind::getCostLivrare(Agent *a, Pachet p) {
    int cost = 0;
    int timp = (getDist(a, p) + a->getSpeed() - 1) / a->getSpeed(); //e scrisa asa ca daca am avea un nr irational sa il aproximeze in sus
    cost = timp * a->getCost();
    return cost;
} //sa vedem cat ne costa livrarea unui pachet - am facut practic cat dureaza * cat costa

Celula HiveMind::gasesteBaza(char map[50][50], int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (map[i][j] == 'B') //sa gaseasca baza pe harta
                return {i, j};
        }
    }
    throw SimulationException("nu e nicio baza!");
}

Celula HiveMind::statieApropiata(int x, int y, char map[50][50], int n, int m) {
    int minim = 10000;
    Celula tinta = {x, y};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (map[i][j] == 'S' || map[i][j] == 'B') {
                int distance = abs(x - i) + abs(y - j);
                if (distance < minim) {
                    minim = distance;
                    tinta = {i, j};
                }
            }
        }
    }
    return tinta;
} //sa gaseasca statie pe harta

Celula HiveMind::statieApropiata(Agent *a, char map[50][50], int n, int m) {
    return statieApropiata(a->getRows(), a->getCols(), map, n, m);
} //acelasi lucru, dar i-am facut overload, ca aveam nevoie sa dau si agent ca parametru

bool HiveMind::poateLivra(Agent *a, Pachet &p, int tickCurent) {
    int dist = getDist(a, p);
    int timp = (dist + a->getSpeed() - 1) / a->getSpeed();
    int sosire = tickCurent + timp;
    if (sosire > p.deadline + 3)
        return false;
    if (a->getCurrBat() < timp * a->getConsume())
        return false;
    if (a->maxCapacitate() == false)
        return false;
    return true;
} //da aici e partea destul de brute force, in care calculam daca poate sa ajunga pana acolo in timp

bool HiveMind::poateIntoarce(Agent *a, Pachet &p, char map[50][50], int n, int m) {
    int distDus = getDist(a, p);
    int timpDus = (distDus + a->getSpeed() - 1) / a->getSpeed();
    int consumDus = timpDus * a->getConsume();
    Celula statie = statieApropiata(p.row, p.col, map, n, m);
    int distIntors = abs(p.row - statie.row) + abs(p.col - statie.col);
    int timpIntors = (distIntors + a->getSpeed() - 1) / a->getSpeed();
    int consumIntors = timpIntors * a->getConsume();
    int marja = 5 * a->getConsume();
    if (a->getCurrBat() >= (consumDus + consumIntors + marja)) {
        return true;
    }
    return false;
} //aici vedem daca poate sa se si intoarca
int HiveMind::calculeazaProfit(Agent *a, Pachet &p, int tickCurent, char map[50][50], int n, int m) {
    if (!poateLivra(a, p, tickCurent))
        return -100000;
    if (!poateIntoarce(a, p, map, n, m))
        return -100000;
    int dist = getDist(a, p);
    int timp = (dist + a->getSpeed() - 1) / a->getSpeed();
    int momentSosire = tickCurent + timp;
    int penalizareIntarziere = 0;
    if (momentSosire > p.deadline) {
        int tickIntarziere = momentSosire - p.deadline;
        penalizareIntarziere = tickIntarziere * 50;
    }
    int cost = getCostLivrare(a, p);
    return p.reward - cost - penalizareIntarziere;
} //un calcul de profit destul de simplu

Agent* HiveMind::alegeAgentPentruPachet(Pachet &p, vector<Agent *> &agenti, int tickCurent, char map[50][50], int n, int m, Celula baza) {
    Agent *best = nullptr;
    int bestProfit = -100000;
    for (auto a: agenti) {
        if (a->getRows() != baza.row || a->getCols() != baza.col)
            continue;
        if (!a->maxCapacitate())
            continue;
        int profit = calculeazaProfit(a, p, tickCurent, map, n, m);
        if (profit > bestProfit) {
            bestProfit = profit;
            best = a;
        }
    }
    if (bestProfit <= 0) return nullptr;
    return best; //in functia asta din nou facem cam brute force, ne folosim de multe date, calculam care agent ar face cel mai bun profit daca l-am trimite dupa pachet si il trimitem pe el
}
void HiveMind::atribuiePachete(vector<Agent *> &agenti, Pachet pachete[], int nrPachete, int tickCurent, char map[50][50], int n, int m) {
    Celula baza = gasesteBaza(map, n, m);
    vector<Pachet *> pacheteDisponibile;
    for (int i = 0; i < nrPachete; i++) {
        if (pachete[i].preluat == false && pachete[i].livrat == false && tickCurent >= pachete[i].aparitie) {
            pacheteDisponibile.push_back(&pachete[i]); //daca un pachet nu a fost nici preluat, nici livrat si a aparut deja il bagam in "geanta" de pachete
        }
    }
    std::sort(pacheteDisponibile.begin(), pacheteDisponibile.end(),
              [](Pachet *a, Pachet *b) {
                  if (a->reward != b->reward)
                      return a->reward > b->reward;
                  return a->deadline < b->deadline;
              }
    ); //aici sortam pachetele in functie de importanta astfel: pachetele cu cele mai mari rewarduri sunt cele mai importante, iar daca doua pachete au acelasi reward, mai important e cel care expira primul
    for (int i = 0; i < pacheteDisponibile.size(); i++) {
        Pachet *p = pacheteDisponibile[i];
        Agent *a = alegeAgentPentruPachet(*p, agenti, tickCurent, map, n, m, baza);
        if (a != nullptr) {
            a->iaPachet(p);
        }
    } //si aici dam pachetul agentului
}
void HiveMind::miscaAgenti(vector<Agent*>& agenti, char map[50][50], int n, int m, int& scorGlobal) {
    Celula baza = gasesteBaza(map, n, m);
    for (auto a : agenti) {
        if (a->getNrPachete() > 0) {
            Pachet* p = a->getPachet(0);
            a->move(p->row, p->col, map); ///daca are pachet sa mearga cu el la destinatie
            if (a->getRows() == p->row && a->getCols() == p->col) {
                scorGlobal = scorGlobal + p->reward;
                a->daPachet(0);
                //cout << "Agentul a livrat pachetul! Scor curent: " << scorGlobal << endl;
            }
        }
        else {
            int distBaza = abs(a->getRows() - baza.row) + abs(a->getCols() - baza.col);
            int timpBaza = (distBaza + a->getSpeed() - 1) / a->getSpeed();
            int consumBaza = timpBaza * a->getConsume();
            int marja = 5 * a->getConsume();
            if (a->getCurrBat() > (consumBaza + marja)) {
                a->move(baza.row, baza.col, map); //daca poate sa ajunga la baza sa mearga la baza
            }
            else {
                Celula statie = statieApropiata(a, map, n, m);
                a->move(statie.row, statie.col, map); //daca poate la o statie sa mearga la o statie
            }
        }
        char celulaCurenta = map[a->getRows()][a->getCols()];
        a->setStatus(celulaCurenta); //in functie de unde e sa aiba acel status cerut
    }
}
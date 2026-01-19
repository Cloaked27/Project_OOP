#include "agent.h"

using namespace std;

Agent::Agent(int a1, int a2) {
    this->row = a1;
    this->col = a2;
    this->status = "IDLE";
    for (int i = 0; i < 5; i++) {
        pachete[i] = nullptr;
    }
}

int Agent::getRows() const {
    return row;
}

int Agent::getCols() const {
    return col;
}

void Agent::setStatus(char cell) {
    if (status == "DEAD")
        return;
    if (cell == 'B' || cell == 'S') {
        if (currbat < maxbat) {
            status = "CHARGING";
            currbat = currbat + maxbat / 4;
        }
        if (currbat >= maxbat && cell == 'S') {
            currbat = maxbat;
            status = "CHARGING";
        }
        if (currbat >= maxbat && cell == 'B') {
            currbat = maxbat;
            status = "IDLE";
        }
    }
}

string Agent::getStatus() {
    return status;
}

int Agent::getCurrBat() const {
    return currbat;
}

int Agent::getSpeed() const {
    return speed;
}

int Agent::getCost() const {
    return cost;
}

int Agent::getConsume() const {
    return consume;
}

bool Agent::maxCapacitate() {
    if (nrpachete < capacitate)
        return true;
    else
        return false;
}

bool Agent::pachetDetinut() {
    if (nrpachete >= 1)
        return true;
    else
        return false;
}

void Agent::iaPachet(Pachet *p) {
    if (maxCapacitate() == 0)
        return;
    else {
        pachete[nrpachete] = p;
        nrpachete++;
        p->preluat = true;
    }
}

int Agent::getNrPachete() {
    return nrpachete;
}

Pachet *Agent::getPachet(int id) {
    if (id < 0 || id >= nrpachete)
        return nullptr;
    else
        return pachete[id];
}

void Agent::daPachet(int id) {
    if (id < 0 || id >= nrpachete) {
        return;
    }
    pachete[id]->livrat = true;
    pachete[id]->preluat = false;
    pachete[id] = pachete[nrpachete - 1];
    pachete[nrpachete - 1] = nullptr;
    nrpachete--;
}


Drona::Drona(int d1, int d2) : Agent(d1, d2) {
    simbol = '^';
    speed = 3;
    maxbat = 100;
    consume = 10;
    cost = 15;
    capacitate = 1;
    currbat = maxbat;
}

void Drona::move(int destrow, int destcol, char map[50][50]) {
    int contor = 0;
    if (status == "DEAD" || currbat < consume) {
        status = "DEAD";
       // cout << "Drona nu mai are baterie!" << endl;
        return;
    }
    status = "MOVING";
    while (contor < 3) {
        if (row == destrow && col == destcol)
            break;
        if (row > destrow)
            row--;
        else if (col > destcol)
            col--;
        else if (row < destrow)
            row++;
        else if (col < destcol)
            col++;
        contor++;
    }
    currbat = currbat - consume;
    totalcost = totalcost + cost;
}

Robot::Robot(int r1, int r2) : Agent(r1, r2) {
    simbol = 'R';
    speed = 1;
    maxbat = 300;
    consume = 2;
    cost = 1;
    capacitate = 4;
    currbat = maxbat;
}

void Robot::move(int destrow, int destcol, char map[50][50]) {
    if (status == "DEAD" || currbat < consume) {
        status = "DEAD";
        //cout << "Robotul nu mai are baterie!" << endl;
        return;
    }
    status = "Moving";
    if (row > destrow && map[row - 1][col] != '#')
        row--;
    else if (row < destrow && map[row + 1][col] != '#')
        row++;
    else if (col > destcol && map[row][col - 1] != '#')
        col--;
    else if (col < destcol && map[row][col + 1] != '#')
        col++;
    currbat = currbat - consume;
    totalcost = totalcost + cost;
}

Scuter::Scuter(int s1, int s2) : Agent(s1, s2) {
    simbol = 'S';
    speed = 2;
    maxbat = 200;
    consume = 5;
    cost = 4;
    capacitate = 2;
    currbat = maxbat;
}

void Scuter::move(int destrow, int destcol, char map[50][50]) {
    if (status == "DEAD" || currbat < consume) {
        status = "DEAD";
        //cout << "Scuterul nu mai are baterie!" << endl;
        return;
    }
    status = "Moving";
    int contor = 0;
    while (contor < 2) {
        if (row == destrow && col == destcol)
            break;
        if (row > destrow && map[row - 1][col] != '#')
            row--;
        else if (row < destrow && map[row + 1][col] != '#')
            row++;
        else if (col > destcol && map[row][col - 1] != '#')
            col--;
        else if (col < destcol && map[row][col + 1] != '#')
            col++;
        contor++;
    }
    currbat = currbat - consume;
    totalcost = totalcost + cost;
}

Agent *AgentFactory::createAgent(string tip, int r, int c) {
    if (tip == "drona") {
        return new Drona(r,c);
    }
    if (tip == "robot") {
        return new Robot(r,c);
    }
    if (tip == "scuter") {
        return new Scuter(r,c);
    }
    return nullptr;
}

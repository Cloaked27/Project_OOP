#pragma once
#include <iostream>
#include <string>
#include "pachet.h"

class Agent {
protected:
    int row = 0, col = 0;
    int speed = 0;
    int maxbat = 0;
    int consume = 0;
    int cost = 0;
    int currbat = 0;
    int capacitate = 0;
    char simbol = '0';
    int totalcost = 0;
    string status;
public:
    Pachet * pachete[5];
    int nrpachete = 0;
    Agent(int a1, int a2);
    virtual void move(int destrow, int destcol, char map[50][50]) = 0;
    int getRows() const;
    int getCols() const;
    void setStatus(char cell);
    string getStatus();
    int getCurrBat() const;
    int getSpeed() const;
    int getCost() const;
    bool maxCapacitate();
    bool pachetDetinut();
    int getNrPachete();
    void iaPachet(Pachet *p);
    virtual ~Agent(){};
    Pachet* getPachet(int id);
    void daPachet(int id);
    int getConsume() const;
};

class Drona: public Agent {
public:
    Drona(int d1, int d2);
    void move(int destrow, int destcol, char map[50][50]) override;
};
class Robot: public Agent {
public:
    Robot(int r1, int r2);
    void move(int destrow, int destcol, char map[50][50]) override;
};
class Scuter: public Agent {
public:
    Scuter(int s1, int s2);
    void move(int destrow, int destcol, char map[50][50]) override;
};

class AgentFactory {
public:
    static Agent* createAgent(std::string tip, int r, int c);
};
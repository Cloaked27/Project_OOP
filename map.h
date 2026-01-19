#include <iostream>
#include <fstream>
#pragma once

using namespace std;

class IMapGenerator {
public:
    virtual void MapGenerator() = 0;
    virtual char getCelula(int row, int col) = 0;
    virtual int getRow() = 0;
    virtual int getCol() = 0;
    virtual ~IMapGenerator() {}
};

class FileMapLoader: public IMapGenerator {
public:
    char map[50][50];
    int n, m;
    void MapGenerator() override;
    char getCelula(int row, int col) override;
    int getRow() override;
    int getCol() override;
};

class ProceduralMapGenerator: public IMapGenerator {
private:
    bool vizitat[50][50];
    int gasit = 0;
    void FloodFill(int row, int col, int n, int m, char map[50][50]);
public:
    int n, m;
    int statii, clienti;
    int contor = 0;
    char map[50][50];
    void MapGenerator() override;
    char getCelula(int row, int col) override;
    int getRow() override;
    int getCol() override;
    virtual ~ProceduralMapGenerator() = default;

};
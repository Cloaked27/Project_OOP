#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "map.h"
#include "agent.h"
#include "hivemind.h"
#include "pachet.h"
#include "simulation_setup.h"
using namespace std;

int main() {
    try {
        srand(time(0));
        /*IMapGenerator* mapStrategy = nullptr;
        int optiuneHarta;

        cout << "Alege tipul de harta:\n";
        cout << "1. Incarca harta din fisier (matrice.txt)\n";
        cout << "2. Genereaza harta procedural (random)\n";
        cout << "Optiune: ";
        cin >> optiuneHarta;

        if (optiuneHarta == 1) {
            mapStrategy = new FileMapLoader();
        } else {
            mapStrategy = new ProceduralMapGenerator();
        } */  //partea de strategy in care puteam sa alegem intre a lua harta dintr-un fisier txt sau de a o genera random, am comentat-o pentru ca oricum citim dupa din fisier si mi s-a parut redundant sa o mai folosesc dupa
        ProceduralMapGenerator mapGen;
        mapGen.MapGenerator();
        Simulation simulare;
        SimulationOutput rezultat;
        simulare.simulation_setup("simulation_setup.txt");
        int n = mapGen.getRow();
        int m = mapGen.getCol();
        char map[50][50];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                map[i][j] = mapGen.getCelula(i, j); //am salvat harta si aici, ca sa imi fie mai usor cand lucrez sa observ ce se intampla (in testare dadeam repede cout sa vad ce si cum)
        int totalTicks = 0;
        int intervalPachete = 0 ;
        int nrDrone = 0, nrRoboti = 0, nrScutere = 0;
        int totalPackages = 0;
        totalTicks = simulare.max_ticks;
        intervalPachete = simulare.spawn_frequency;
        nrDrone = simulare.drones;
        nrRoboti = simulare.robots;
        nrScutere = simulare.scooters;
        totalPackages = simulare.total_packages;
        HiveMind hive;
        Celula baza = hive.gasesteBaza(map, n, m);
        vector<Agent*> agenti;
        for (int i = 0; i < nrDrone; i++)
            agenti.push_back(AgentFactory::createAgent("drona", baza.row, baza.col));
        for (int i = 0; i < nrRoboti; i++)
            agenti.push_back(AgentFactory::createAgent("robot", baza.row, baza.col));
        for (int i = 0; i < nrScutere; i++)
            agenti.push_back(AgentFactory::createAgent("scuter", baza.row, baza.col));  //am folosit factory design pattern ca sa creez nr necesar de agenti
        AtribuitorPachete postas;
        postas.getnrClienti(map, n, m);
        postas.creeazaPachet(totalPackages, intervalPachete);
        vector<bool> pachetExpiratPenalizat(postas.nrPachete, false);
        vector<bool> agentMortPenalizat(agenti.size(), false);
        vector<bool> pachetEraLivrat(postas.nrPachete, false);  //toti cei 3 vectori sunt folositi pentru a putea sa calculez penalizarile, fara sa risc sa numar de 2 ori acelasi lucru
        int scorGlobal = 0; //asta va fi profitul total
        for (int tick = 0; tick <= totalTicks; tick++) {
            for (size_t i = 0; i < agenti.size(); i++) {
                Agent* a = agenti[i];
                if (a->getStatus() == "DEAD") {
                    if (agentMortPenalizat[i] == false) {
                        scorGlobal = scorGlobal - 500;
                        agentMortPenalizat[i] = true;
                        rezultat.agentipierduti++; //ca sa putem sa bagam in fisierul de output
                        //cout << "Agentul " << i << " a murit!\n";
                    }
                } else {
                    scorGlobal = scorGlobal - a->getCost(); //pierdem costurile de folosire a agentului dupa cum sunt scrise ele in tabel
                }
            }
            for (int i = 0; i < postas.nrPachete; i++) { //aici verificam situatia curenta in care se afla harta, practic ca un stop-cadru in joc
                pachetEraLivrat[i] = postas.pachete[i].livrat;
                if (tick > postas.pachete[i].deadline && postas.pachete[i].livrat == false && postas.pachete[i].preluat == false && tick >= postas.pachete[i].aparitie) { //aici am pus toate aceste conditii sa verifice daca pachetul e o "cauza pierduta"
                    if (pachetExpiratPenalizat[i] == false) { //si daca nu l-am trecut pana acum ca expirat, il trecem acum
                        scorGlobal = scorGlobal - 200;
                        pachetExpiratPenalizat[i] = true;
                        //cout << "Pachetul " << i << " a expirat in depozit!\n";
                    }
                }
            }
            hive.atribuiePachete(agenti, postas.pachete, postas.nrPachete, tick, map, n, m);
            hive.miscaAgenti(agenti, map, n, m, scorGlobal);
            for (int i = 0; i < postas.nrPachete; i++) {
                if (pachetEraLivrat[i] == false && postas.pachete[i].livrat == true){ //conditia din if verifica daca pachetul era livrat cand am intrat in acest tick (si trebuie sa nu fi fost ca sa il luam in considerare), si daca e livrat la finalul tickului
                    rezultat.pachetelivrate++;
                    if (tick > postas.pachete[i].deadline){ //daca a fost livrat cu intarziere platim amenda
                        int intarziere = tick - postas.pachete[i].deadline;
                        int amenda = intarziere * 50;
                        scorGlobal = scorGlobal - amenda;
                        // cout << "(livrare cu delay) Pachet " << i << " livrat cu "
                        //    << intarziere << " ticks intarziere. Amenda: -" << amenda << "\n";
                    }
                }
            }
        }
        rezultat.profitfinal = scorGlobal;
        rezultat.simulation_output("simulation.txt");
        //cout << "PROFIT NET FINAL: " << scorGlobal << endl;
        for (auto a : agenti) delete a;
        return 0;
    }catch (const SimulationException& e) {
        cerr << "EROARE CAPTURATA: " << e.msj << std::endl;
        return 1;
    }
}
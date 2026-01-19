#include "simulation_setup.h"
#include <fstream>
using namespace std;

void Simulation::simulation_setup(string text) {
    ifstream fin(text);
    if (!fin.is_open()) {
        throw SimulationException("Nu am putut deschide " + text);
    }
    fin>>rows >> cols >> max_ticks >> max_stations >> clients_count>> drones >> robots >> scooters >> total_packages >> spawn_frequency;
    fin.close();
}
void SimulationOutput::simulation_output(string text) {
    ofstream fout(text);
    fout << "Pachete Livrate: " << pachetelivrate << endl;
    fout << "Agenti pierduti: " << agentipierduti << endl;
    fout <<" Profit final :" << profitfinal;
}

#include <iostream>
#pragma once

using namespace std;

struct SimulationException {
    string msj;
    SimulationException(string m) : msj(m){}
};
class Simulation {
public:
    int rows=0;
    int cols = 0;
    int max_ticks = 0;
    int max_stations = 0;
    int clients_count = 0;
    int drones  = 0;
    int robots = 0;
    int scooters = 0;
    int total_packages = 0;
    int spawn_frequency = 0;
    void simulation_setup(string text);
};

class SimulationOutput {
public:
    int pachetelivrate = 0;
    int profitfinal = 0;
    int agentipierduti = 0;
    void simulation_output(string text);
};
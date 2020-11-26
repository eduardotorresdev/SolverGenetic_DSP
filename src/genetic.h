#ifndef _H_GENETIC
#define _H_GENETIC
#pragma once
using namespace std;

/**
 * @param 
 * @return
 */

struct chromoTuple
{
    vector<vector<int>> chromo;
    double conflitos;
};
class Chromossome
{
public:
    vector<vector<int>> chrome;

    void initializeChromossome(int disc, int salas, int dias, int horarios);
};

void print_chrome(vector<vector<int>> chrome, int salas, int dias, int horarios);

class GeneticAlg
{
public:
    vector<chromoTuple> population;
    int population_size;
    int elite_size_group;

    GeneticAlg();
    void initializePopulation(int disc, int salas, int dias, int horarios);
    void fitnessFunction(int disc, int salas, int dias, int horarios, vector<vector<int>> disp_sala, vector<vector<int>> pref_horario);
    vector<chromoTuple> slicingPopulation(vector<chromoTuple> &arr, int X, int Y);
    vector<chromoTuple> shufflePopulation(vector<chromoTuple> &arr);
    vector<chromoTuple> selectChrome();
    void crossover();
    vector<vector<int>> mutateChrome(vector<vector<int>> chrome, int disc, int dias, int salas, int horarios);
    void mutation(int disc, int dias, int salas, int horarios);
};

#endif

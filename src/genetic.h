#ifndef _H_GENETIC
#define _H_GENETIC
#pragma once
#include <time.h>
#include <vector>
#include <algorithm>
#include <random>
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

    void initializeChromossome(int &disc, const vector<int> &qtd_salas, int &dias, int &horarios);
};

void print_chrome(const vector<vector<int>> &chrome, int &disc, int &salas, int &dias, int &horarios, int &iteracao, double &fitness);

class GeneticAlg
{
public:
    vector<chromoTuple> population;
    int population_size;
    int elite_size_group;

    GeneticAlg();
    void initializePopulation(int &disc, const vector<int> &qtd_salas, int &dias, int &horarios);
    void fitnessFunction(int &disc, int &salas, int &dias, int &horarios, const vector<vector<int>> &disp_sala, const vector<vector<int>> &pref_horario);
    vector<chromoTuple> slicingPopulation(vector<chromoTuple> &arr, int X, int Y);
    vector<chromoTuple> shufflePopulation(vector<chromoTuple> &arr);
    vector<chromoTuple> selectChrome();
    bool conflito(const vector<int> &aula, const vector<vector<int>> &chromo, const vector<vector<int>> &disp_sala, const vector<vector<int>> &pref_horario, int &i);
    void crossover(const vector<vector<int>> &disp_sala, const vector<vector<int>> &pref_horario);
    vector<vector<int>> mutateChrome(vector<vector<int>> chrome, int &disc, int &dias, const vector<int> &qtd_salas, int &horarios);
    void mutation(int &disc, int &dias, const vector<int> &qtd_salas, int &horarios);
};

#endif

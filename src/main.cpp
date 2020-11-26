#include <iostream>
#include "reader.h"
#include "genetic.h"

using namespace std;

int main()
{
    int disc, dias, salas, horarios;
    vector<vector<int>> disp_salas, pref_horario;
    string dataContent = readFromFile("data/10_5_5_8-1.dat");

    if (dataContent != "err")
    {
        tie(disc, dias, salas, horarios, disp_salas, pref_horario) = middlewareToRemoveNoises(dataContent);
        printFormated(disc, dias, salas, horarios, disp_salas, pref_horario);

        GeneticAlg alg;
        alg.initializePopulation(disc, salas, dias, horarios);
        int gen = 0;
        chromoTuple best_chrome;
        best_chrome.conflitos = 0;
        while (true)
        {
            cout << gen;
            alg.fitnessFunction(disc, salas, dias, horarios, disp_salas, pref_horario);
            if (alg.population[0].conflitos > best_chrome.conflitos)
            {
                best_chrome = alg.population[0];
            }

            if (alg.population[0].conflitos == 1.0 || gen == 10000)
            {
                print_chrome(alg.population[0].chromo, salas, dias, horarios);
                break;
            }

            for (int i = 0; i < 5; i++)
            {
                cout << alg.population[i].conflitos;
            }

            alg.crossover();
            alg.mutation(disc, dias, salas, horarios);
            system("cls||clear");
        }
    }
    else
    {
        cerr << "ERROR: invalid input!" << endl;
    }

    return (0);
}
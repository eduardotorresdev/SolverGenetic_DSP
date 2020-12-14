#include <iostream>
#include <locale>
#include <ctime>
#include "reader.h"
#include "genetic.h"

using namespace std;

int main()
{
    int disc, dias, salas, horarios;
    vector<vector<int>> disp_salas, pref_horario;
    string dataContent = readFromFile("data/10_5_5_8-1.dat");
    setlocale(LC_ALL, "");
    srand(time(NULL));

    if (dataContent != "err")
    {
        tie(disc, dias, salas, horarios, disp_salas, pref_horario) = middlewareToRemoveNoises(dataContent);
        printFormated(disc, dias, salas, horarios, disp_salas, pref_horario);
        vector<int> salas_list;
        int sala = 0;
        clock_t begin = clock();
        for (int j = 1; j < salas + 1; j++)
        {
            while ((2.0 * disc) / (j * 40.0) > 1.0)
            {
                j += 4;
            }
            for (int i = 0; i < j; i++)
            {
                sala = (rand() % salas + 1 % salas);
                while (find(salas_list.begin(), salas_list.end(), sala) != salas_list.end())
                {
                    sala = (rand() % salas + 1 % salas);
                }
                salas_list.push_back(sala);
            }
            cout << "instancia " << j << endl;
            for (int sala_item : salas_list)
            {
                cout << sala_item << ", ";
            }

            GeneticAlg alg;
            alg.initializePopulation(disc, salas_list, dias, horarios);
            int gen = 0;
            chromoTuple best_chrome;
            best_chrome.conflitos = 0;
            while (1)
            {
                gen += 1;
                cout << gen << endl;
                alg.fitnessFunction(disc, salas, dias, horarios, disp_salas, pref_horario);
                if (alg.population[0].conflitos > best_chrome.conflitos)
                {
                    best_chrome = alg.population[0];
                }
                cout << alg.population[0].conflitos << endl;
                if (alg.population[0].conflitos == 1.0 || gen == 10000)
                {
                    print_chrome(alg.population[0].chromo, disc, salas, dias, horarios, j, alg.population[0].conflitos);
                    break;
                }

                alg.crossover(disp_salas, pref_horario);
                alg.mutation(disc, dias, salas_list, horarios);
                system("cls||clear");
            }
            if (alg.population[0].conflitos == 1.0)
            {
                break;
            }
        }
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << elapsed_secs << endl;
    }
    else
    {
        cerr << "ERROR: invalid input!" << endl;
    }

    return (0);
}
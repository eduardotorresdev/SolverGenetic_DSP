#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include "genetic.h"

using namespace std;

void Chromossome::initializeChromossome(int disc, int salas, int dias, int horarios)
{
    srand((unsigned)time(0));

    int discId = 1;
    for (int i = 0; i < 2 * disc + 1; i++)
    {
        vector<int> genes{
            discId,
            (rand() % salas) + 1,
            (rand() % dias) + 1,
            (rand() % horarios) + 1};
        chrome.push_back(genes);
        if (i % 2 == 0)
        {
            discId += 1;
        }
    }
}

void print_chrome(vector<vector<int>> chrome, int salas, int dias, int horarios)
{
    vector<vector<vector<int>>> escalonamento = vector<vector<vector<int>>>{
        vector<vector<vector<int>>>(salas, vector<vector<int>>(dias, vector<int>(horarios, 0)))};

    for (int k = 1; k < salas + 1; k++)
    {
        for (int j = 1; j < horarios + 1; j++)
        {
            for (int i = 1; i < dias + 1; i++)
            {
                for (vector<int> aula : chrome)
                {
                    if (aula[2] == i && aula[3] == j && aula[1] == k)
                    {
                        escalonamento[k - 1][i - 1][j - 1] = aula[0];
                    }
                }
            }
        }
    }

    for (int k = 1; k < salas + 1; k++)
    {
        cout << "SALA - " << std::to_string(k) << "\n";
        cout << "-------------------------------------------------------"
             << "\n";
        cout << "          ";

        for (int i = 1; i < dias + 1; i++)
        {
            cout << "|DIA " << std::to_string(i) << "  |";
        }
        cout << "\n";

        for (int j = 1; j < horarios + 1; j++)
        {
            cout << "-------------------------------------------------------\n";
            cout << std::to_string(j) << "º horário";

            for (int i = 1; i < dias + 1; i++)
            {
                string str = std::to_string(escalonamento[k - 1][i - 1][j - 1]);

                if (escalonamento[k - 1][i - 1][j - 1] == 0)
                {
                    str = "-";
                }

                if (str.length() > 9)
                {
                    cout << "|  " << str << "   |";
                }
                else
                {
                    cout << "|   " << str << "   |";
                }
            }
            cout << "\n";
        }
        cout << "-------------------------------------------------------\n";
        cout << " \n";
    }
}

GeneticAlg::GeneticAlg()
{
    population_size = 500;
    elite_size_group = 5;
}

void GeneticAlg::initializePopulation(int disc, int salas, int dias, int horarios)
{
    for (int i = 0; i < population_size; i++)
    {
        Chromossome chromo;
        chromo.initializeChromossome(disc, salas, dias, horarios);
        population.push_back(chromoTuple{
            chromo.chrome,
            0});
    }
}

void GeneticAlg::fitnessFunction(int disc, int salas, int dias, int horarios, vector<vector<int>> disp_sala, vector<vector<int>> pref_horario)
{
    int index = 0;

    for (chromoTuple c : population)
    {
        double n_conflitos = 0;
        vector<int> salas_utilizadas = vector<int>(salas, 0);
        int i = 0;
        for (vector<int> aula : c.chromo)
        {
            // CHECA SE A DISCIPLINA PODE SER MINISTRADA NAQUELA SALA
            salas_utilizadas[aula[1] - 1] += 1;
            if (disp_sala[aula[0]][aula[1] - 1] == 0)
            {
                n_conflitos += 1;
            }

            int j = 0;
            for (vector<int> outra_aula : c.chromo)
            {
                if (i >= j)
                {
                    if (aula[3] == outra_aula[3] && aula[2] == outra_aula[2] && aula[0] != outra_aula[0])
                    {
                        if (aula[1] == outra_aula[1])
                        {
                            n_conflitos += 1;
                        }
                    }
                }
                j++;
            }
            bool encontrou1 = false;
            for (int p : pref_horario[aula[0] - 1])
            {
                if (p == 1)
                {
                    encontrou1 = true;
                    break;
                }
            }

            if (encontrou1 == true)
            {
                if (pref_horario[aula[0] - 1][aula[3] - 1] != 1)
                {
                    n_conflitos += 1;
                }
            }

            i++;
        }
        i = 0;

        while (i < c.chromo.size())
        {
            int aula_1 = c.chromo[0][i];
            int aula_2 = c.chromo[0][i + 1];

            if (aula_1 == aula_2)
            {
                n_conflitos += 1;
            }

            i += 2;
        }

        for (int sala : salas_utilizadas)
        {
            if (sala != 0)
            {
                n_conflitos += 1;
            }
        }

        population[index].conflitos = 1 / (1 + n_conflitos);
        index += 1;
    }
    std::sort(population.begin(), population.end(), [](auto const &a, auto const &b) { return a.conflitos > b.conflitos; });
}

vector<chromoTuple> GeneticAlg::slicingPopulation(vector<chromoTuple> &arr, int X, int Y)
{

    auto start = arr.begin() + X;
    auto end = arr.begin() + Y;

    vector<chromoTuple> result(Y - X);

    copy(start, end, result.begin());

    return result;
}

vector<chromoTuple> GeneticAlg::shufflePopulation(vector<chromoTuple> &arr)
{
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(arr), std::end(arr), rng);
    return arr;
}

vector<chromoTuple> GeneticAlg::GeneticAlg::selectChrome()
{
    vector<chromoTuple> chromes;

    srand((unsigned)time(0));

    int i = 0;

    while (i < 0.3 * population_size)
    {
        chromes.push_back(population[rand() % population.size()]);
        i += 1;
    }

    std::sort(chromes.begin(), chromes.end(), [](auto const &a, auto const &b) { return a.conflitos > b.conflitos; });
    return chromes;
}

void GeneticAlg::crossover()
{
    srand((unsigned)time(0));
    vector<chromoTuple> new_population = slicingPopulation(population, 0, elite_size_group);
    new_population = shufflePopulation(new_population);

    int i = 0;
    while (i < population_size)
    {
        vector<vector<int>> chrome_filho;
        chromoTuple aula_1 = selectChrome()[0];
        chromoTuple aula_2 = selectChrome()[0];

        for (int j = 0; j < aula_1.chromo.size(); j++)
        {
            if (rand() % 100 > 50)
            {
                chrome_filho.push_back(aula_1.chromo[j]);
            }
            else
            {
                chrome_filho.push_back(aula_2.chromo[j]);
            }
        }

        new_population.push_back(chromoTuple{
            chrome_filho,
            0});
        i += 1;
    }
}

vector<vector<int>> GeneticAlg::mutateChrome(vector<vector<int>> chrome, int disc, int dias, int salas, int horarios)
{
    srand((unsigned)time(0));

    Chromossome c;
    c.initializeChromossome(disc, salas, dias, horarios);

    int i = 0;
    for (vector<int> aula : chrome)
    {
        if (((rand() % 10 + 1) / 10) < 0.1)
        {
            chrome[i] = c.chrome[i];
        }
        i++;
    }
    return chrome;
}

void GeneticAlg::mutation(int disc, int dias, int salas, int horarios)
{
    vector<chromoTuple> new_population = slicingPopulation(population, 0, elite_size_group);
    population = slicingPopulation(population, elite_size_group, population.size());

    for (int i = elite_size_group; i < population.size(); i++)
    {
        new_population.push_back(chromoTuple{
            mutateChrome(population[i].chromo, disc, dias, salas, horarios),
            0});
    }

    population = new_population;
}

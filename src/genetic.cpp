#include <iostream>
#include <chrono>
#include "genetic.h"

using namespace std;

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);
uniform_real_distribution<double> distribution(0.0, 1.0);

void Chromossome::initializeChromossome(int disc, int salas, int dias, int horarios)
{
    int discId = 0;

    for (int i = 0; i < 2 * disc; i++)
    {
        vector<int> genes{
            discId,
            rand() % salas,
            rand() % dias,
            rand() % horarios};

        chrome.push_back(genes);
        if (i % 2 == 0 && i > 0)
        {
            discId += 1;
        }
    }
}

void print_chrome(vector<vector<int>> chrome, int salas, int dias, int horarios)
{
    vector<vector<vector<int>>> escalonamento = vector<vector<vector<int>>>{
        vector<vector<vector<int>>>(salas, vector<vector<int>>(dias, vector<int>(horarios, 0)))};

    for (int k = 0; k < salas; k++)
    {
        for (int j = 0; j < horarios; j++)
        {
            for (int i = 0; i < dias; i++)
            {
                for (vector<int> aula : chrome)
                {
                    if (aula[2] == i && aula[3] == j && aula[1] == k)
                    {
                        escalonamento[k][i][j] = aula[0] - 1;
                    }
                }
            }
        }
    }

    for (int k = 0; k < salas; k++)
    {
        cout << "SALA - " << std::to_string(k + 1) << "\n";
        cout << "-------------------------------------------------------"
             << "\n";
        cout << "            ";

        for (int i = 0; i < dias; i++)
        {
            cout << "|DIA " << std::to_string(i + 1) << "  |";
        }
        cout << "\n";

        for (int j = 0; j < horarios; j++)
        {
            cout << "-------------------------------------------------------\n";
            cout << std::to_string(j + 1) << "º horário";

            for (int i = 0; i < dias; i++)
            {
                string str = std::to_string(escalonamento[k][i][j]);

                if (escalonamento[k][i][j] == 0)
                {
                    str = "-";
                }

                if (escalonamento[k][i][j] > 9)
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
            salas_utilizadas[aula[1]] += 1;

            if (disp_sala[aula[0]][aula[1]] == 0)
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
            for (int p : pref_horario[aula[0]])
            {
                if (p == 1)
                {
                    encontrou1 = true;
                    break;
                }
            }

            if (encontrou1 == true)
            {
                if (pref_horario[aula[0]][aula[3]] != 1)
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

    auto first = arr.cbegin() + X;
    auto last = arr.cbegin() + Y + 1;

    vector<chromoTuple> vec(first, last);
    return vec;
}

vector<chromoTuple> GeneticAlg::shufflePopulation(vector<chromoTuple> &arr)
{
    auto first = arr.cbegin();
    auto last = arr.cbegin() + arr.size();

    vector<chromoTuple> vec(first, last);

    auto rng = default_random_engine{};
    shuffle(begin(vec), end(vec), rng);
    return vec;
}

vector<chromoTuple> GeneticAlg::GeneticAlg::selectChrome()
{
    vector<chromoTuple> chromes;

    int i = 0;

    while (i < (int)(0.3 * population_size))
    {
        chromes.push_back(population[rand() % population.size()]);
        i += 1;
    }

    sort(chromes.begin(), chromes.end(), [](auto const &a, auto const &b) { return a.conflitos > b.conflitos; });
    return chromes;
}

void GeneticAlg::crossover()
{
    vector<chromoTuple> new_population = slicingPopulation(population, 0, elite_size_group - 1);
    vector<chromoTuple> population_shuffled = shufflePopulation(population);
    population = population_shuffled;

    int i = new_population.size();
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

    population = new_population;
}

vector<vector<int>> GeneticAlg::mutateChrome(vector<vector<int>> chrome, int disc, int dias, int salas, int horarios)
{
    Chromossome c;
    c.initializeChromossome(disc, salas, dias, horarios);

    int i = 0;
    for (vector<int> aula : chrome)
    {
        if (distribution(generator) < 0.1)
        {
            chrome[i] = c.chrome[i];
        }
        i++;
    }
    return chrome;
}

void GeneticAlg::mutation(int disc, int dias, int salas, int horarios)
{
    vector<chromoTuple> new_population = slicingPopulation(population, 0, elite_size_group - 1);
    vector<chromoTuple> population_no_elite = slicingPopulation(population, elite_size_group, population.size() - 1);
    population = population_no_elite;

    for (int i = 0; i < population.size(); i++)
    {
        new_population.push_back(chromoTuple{
            mutateChrome(population[i].chromo, disc, dias, salas, horarios),
            0});
    }

    population = new_population;
}

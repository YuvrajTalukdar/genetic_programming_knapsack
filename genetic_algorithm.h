#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
//#include<stdlib.h>
//#include<time.h>
#include<random>

using namespace std;

struct item
{
    int id;
    int weight;
    int value;
    float temp;
};

struct chromosome
{
    int id;
    float fitness;
    vector<bool> gene_vec;
};

class genetic_algorithm
{
    private:
    int population_size=100;
    int mutation_percentage=5;
    float max_possible_weight;
    vector<item> item_list;
    
    vector<chromosome> population;

    void mutation(chromosome& c1);
    
    vector<chromosome> crossover(vector<chromosome>& population);

    vector<chromosome> tournament_selection(vector<chromosome> population);

    vector<chromosome> biased_random_selection(vector<chromosome>& population);

    float calc_fitness(chromosome& new_chromosome,vector<item>& item_list);

    void generate_initial_population(vector<item>& item_list,int& max_weight);

    static bool comparator2(chromosome c1,chromosome c2)
    {
        if(c1.fitness>c2.fitness)
        {   return true;}
        else
        {   return false;}
    }
    public:
    void start(vector<item> item_list,int max_weight);
};
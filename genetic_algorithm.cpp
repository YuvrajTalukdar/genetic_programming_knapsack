#include"genetic_algorithm.h"

int get_random_number(int min,int max)
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist6(min,max); // distribution in range [1, 6]
    return dist6(rng);
}
vector<item> generate_items(int num)
{
    vector<item> item_list;
    for(int a=0;a<num;a++)
    {
        item i;
        i.id=a;
        i.value=get_random_number(1,1000);
        i.weight=get_random_number(1,1000);
        i.temp=((float)i.value)/((float)i.weight);
        item_list.push_back(i);
    }
    return item_list;
}
bool comparator(item i1,item i2)
{
    if(i1.temp>i2.temp)
    {   return true;}
    else
    {   return false;}
}
void classic_algorithm(int max_weight,vector<item> item_list)
{
    sort(item_list.begin(),item_list.end(),comparator);
    int current_weight=0,value=0,a=0;
    do
    {
        //cout<<"("<<item_list[a].value<<","<<item_list[a].weight<<"),";
        current_weight+=item_list[a].weight;
        value+=item_list[a].value;
        a++;
    } 
    while (current_weight<=max_weight);
    a--;
    current_weight-=item_list[a].weight;
    value-=item_list[a].value;
    cout<<"\n\nCA Final Weight= "<<current_weight<<"\nCA Final Value= "<<value;
}

float genetic_algorithm::calc_fitness(chromosome& new_chromosome,vector<item>& item_list)
{
    float value=0,weight=0;
    for(int a=0;a<new_chromosome.gene_vec.size();a++)
    {
        if(new_chromosome.gene_vec[a])
        {   value+=item_list[a].value;weight+=item_list[a].weight;}
    }
    if(weight>max_possible_weight)
    {   return 0;}
    else
    {   return value;}
}

void genetic_algorithm::mutation(chromosome& c1)
{
    int no_of_genes_to_mutate=c1.gene_vec.size()*mutation_percentage/100;
    if(no_of_genes_to_mutate==0)
    {   no_of_genes_to_mutate++;}
    
    vector<int> index_temp;
    for(int a=0;a<no_of_genes_to_mutate;a++)
    {
        point1:
        int index=get_random_number(0,c1.gene_vec.size()-1);
        bool found=false;
        for(int b=0;b<index_temp.size();b++)
        {
            if(index_temp[b]==index)
            {   found=true;break;}
        }
        if(found)
        {   goto point1;}
        c1.gene_vec[index]=!c1.gene_vec[index];
        index_temp.push_back(index);
    }
}

vector<chromosome> genetic_algorithm::crossover(vector<chromosome>& population)
{
    vector<chromosome> new_gen;
    for(int a=0;a<population.size();a+=2)
    {
        if(a!=population.size()-1)
        {
            int crossover_index=get_random_number(0,population[a].gene_vec.size()-1);
            vector<bool> genes1a,genes2a,genes1b,genes2b;//may be removed lateron, but for now keep it to make code look simple.
            //1a, 2a
            //1b, 2b
            //cross 1a, 1b
            //cross 2a, 2b
            genes1a.insert(genes1a.end(),population[a].gene_vec.begin(),population[a].gene_vec.begin()+crossover_index);
            genes2a.insert(genes2a.end(),population[a].gene_vec.begin()+crossover_index,population[a].gene_vec.end());

            genes1b.insert(genes1b.end(),population[a+1].gene_vec.begin(),population[a+1].gene_vec.begin()+crossover_index);
            genes2b.insert(genes2b.end(),population[a+1].gene_vec.begin()+crossover_index,population[a+1].gene_vec.end());

            chromosome new_chromosome1,new_chromosome2;
            new_chromosome1.id=population.size();
            new_chromosome1.gene_vec.insert(new_chromosome1.gene_vec.end(),genes1a.begin(),genes1a.end());
            new_chromosome1.gene_vec.insert(new_chromosome1.gene_vec.end(),genes2b.begin(),genes2b.end());
            //new_chromosome1.fitness=calc_fitness(new_chromosome1,item_list);
            new_gen.push_back(new_chromosome1);
            
            new_chromosome2.id=population.size()+1;
            new_chromosome2.gene_vec.insert(new_chromosome2.gene_vec.end(),genes1b.begin(),genes1b.end());
            new_chromosome2.gene_vec.insert(new_chromosome2.gene_vec.end(),genes2a.begin(),genes2a.end());
            //new_chromosome2.fitness=calc_fitness(new_chromosome2,item_list);
            new_gen.push_back(new_chromosome2);
        }
        else
        {   new_gen.push_back(population[a]);}
    }
    return new_gen;
}

vector<chromosome> genetic_algorithm::biased_random_selection(vector<chromosome>& population)
{
    vector<chromosome> new_gen;
    return new_gen;
}

vector<chromosome> genetic_algorithm::tournament_selection(vector<chromosome> population)
{
    vector<chromosome> selected_populatipn;
    while(population.size()!=0)
    {
        int index1=get_random_number(0,population.size()-1);
        point1:
        int index2=get_random_number(0,population.size()-1);
        if(index1==index2)
        {   goto point1;}
        if(population[index1].fitness>population[index2].fitness)
        {   
            selected_populatipn.push_back(population[index1]);
            if(index1>index2)
            {
                population.erase(population.begin()+index1);
                population.erase(population.begin()+index2);
            }
            else
            {
                population.erase(population.begin()+index2);
                population.erase(population.begin()+index1);
            }
            
        }
        else
        {   
            selected_populatipn.push_back(population[index2]);
            if(index1>index2)
            {
                population.erase(population.begin()+index1);
                population.erase(population.begin()+index2);
            }
            else
            {
                population.erase(population.begin()+index2);
                population.erase(population.begin()+index1);
            }
        }
        if(population.size()==1)
        {
            selected_populatipn.push_back(population[0]);
            population.pop_back();
        }
    }
    return selected_populatipn;
}

void genetic_algorithm::generate_initial_population(vector<item>& item_list,int& max_weight)
{
    int current_weight;
    for(int a=0;a<population_size;a++)
    {
        current_weight=0;
        chromosome new_chromosome;
        new_chromosome.id=a;
        for(int b=0;b<item_list.size();b++)
        {   new_chromosome.gene_vec.push_back(false);}
        while(true)
        {
            int rand_index=get_random_number(0,item_list.size()-1);
            if(!new_chromosome.gene_vec[rand_index])
            {
                new_chromosome.gene_vec[rand_index]=true;
                current_weight+=item_list[rand_index].weight;
            }
            if(current_weight>=max_weight)
            {
                new_chromosome.gene_vec[rand_index]=false;
                break;
            }
        }
        population.push_back(new_chromosome);
    }
}

void genetic_algorithm::start(vector<item> items,int max_weight)
{
    max_possible_weight=max_weight;
    item_list=items;
    generate_initial_population(item_list,max_weight);
    //calculate fitness
    for(int b=0;b<population.size();b++)
    {   population[b].fitness=calc_fitness(population[b],item_list);}
    int iteration=200,total_value=0;
    for(int a=0;a<iteration;a++)
    {
        //selection
        vector<chromosome> selected_parents=tournament_selection(population);
        //crossover
        vector<chromosome> new_gen=crossover(selected_parents);
        selected_parents.clear();
        //mutation
        for(int b=0;b<new_gen.size();b++)
        {   mutation(new_gen[b]);}
        //calc fitness of new gen
        for(int b=0;b<new_gen.size();b++)
        {   new_gen[b].fitness=calc_fitness(new_gen[b],item_list);}
        //adding new gen to population pool
        population.insert(population.end(),new_gen.begin(),new_gen.end());
        //sorting population pool according to fitness
        sort(population.begin(),population.end(),comparator2);
        //removing the last 50 chromosome with lowest fitness
        population.erase(population.end()-(population.size()-100),population.end());
        //cout<<"\n\n";
        total_value=0;
        for(int b=0;b<population.size();b++)
        {   
            total_value+=population[b].fitness;
            //cout<<population[b].fitness<<",";
        }
        //cout<<"\ncheck6 p_size= "<<population.size();
        cout<<"\nIteration: "<<a<<", total_value: "<<total_value;
        //int gh;cin>>gh;
    }
    float weight=0,value=0;
    for(int a=0;a<population[0].gene_vec.size();a++)
    {
        if(population[0].gene_vec[a])
        {
            weight+=item_list[a].weight;
            value+=item_list[a].value;
        }
    }
    cout<<"\n\nGA_Fitness= "<<population[0].fitness;
    cout<<"\nGA_Value= "<<value;
    cout<<"\nGA_Weight= "<<weight;
}

int main()
{
    int max_weight,item_list_size;
    cout<<"bag max weight capacity: ";
    cin>>max_weight;
    cout<<"item list size: ";
    cin>>item_list_size;
    vector<item> item_list=generate_items(item_list_size);
    for(int a=0;a<item_list.size();a++)
    {   cout<<"("<<item_list[a].value<<","<<item_list[a].weight<<"),";}
    cout<<"\n\n";
    genetic_algorithm ga;
    ga.start(item_list,max_weight);
    
    classic_algorithm(max_weight,item_list);
    cout<<"\n";
}
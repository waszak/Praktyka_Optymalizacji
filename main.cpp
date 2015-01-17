#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#define in_range(i, a,b )  for(int i =(a); i < (b); i++)

using namespace std;
typedef  vector<vector<int>> Config;

//void generate_types(int types, int params)
void generate_types(Config &types, int ntypes =17, int params =9){
    types.clear();
    if(ntypes == -1){
        types.push_back({1,1,1});
    }else{
        in_range(i, 0, ntypes){
            vector <int> a = vector <int>();
            in_range(j, 0, params) a.push_back(rand()%5);
            types.push_back(a);
        }
    }
}
void generate_data( int n, vector<int > &data, int ntypes=17){
    data.clear();
    if( n ==-1){
        // Jakieœ testy poprawnoœciowe
        if(ntypes ==1 ){
            in_range(j,0,10){
                data.push_back(1);
            }
        }
        //...
    }else{
        in_range(j,0,n){
            data.push_back( rand()%ntypes);
        }
    }
}

void generate_permutation(vector<vector<double>> &ant_graph, vector<int> &perm){
    // remaining nodes to be selected
    vector<int> remaining = vector<int>(perm.size());
    in_range(i, 0, perm.size())
        remaining[i] = i;

    int start = rand() % perm.size();
    perm[0] = start;
    remaining.erase(remaining.begin()+start);

    // get sum of values of remaining nodes
    auto sum = [] (vector<double> nbrs, vector<int> &rem){
        double result = 0;
        for(int nbr : rem) result += nbrs[nbr];
        return result;
    };
    in_range(i, 0, perm.size() - 1){
        double nbrs_sum = sum(ant_graph[i], remaining); // sum of values to remaining neighbours
        double mult = 1e4;
        int r = rand()%int(nbrs_sum * mult);

        int selected;
        in_range(nbr, 0, remaining.size()){
            if(ant_graph[perm[i]][nbr] * mult < r){
                selected = nbr;
                break;
            }
        }

        perm[i+1] = selected;

    }

}
int compute_cost(vector<int> & data, Config &types, int params, int computers_on_palet=10, int workers = 10){
    int total_time = 0, elements_on_line = 1, idx =0;
    auto comp = [] (tuple<int,int> &a, tuple<int,int> &b) -> bool { return get<1>(a) < get<1>(b); };
    priority_queue<tuple<int,int>,std::vector<tuple<int,int>>, decltype(comp) > pq (comp);

    vector<int> assembly_line = vector<int>();
    vector<int> assembly_times_line = vector<int>();
    vector<int> assembly_computers_on_line = vector<int>();
    vector<int> assembly_workers_on_line_line = vector<int>();
    vector<int> need_workers = vector<int>();

    assembly_line.resize(params, -1);
    assembly_times_line.resize(params,0);
    assembly_computers_on_line.resize(params,0);
    assembly_workers_on_line_line.resize(params, 0);

    do{
        assembly_line[0] = data[idx++];
        assembly_times_line[0] += types[assembly_line[0]][0];
        assembly_computers_on_line[0] ++;
        assembly_workers_on_line_line[0] ++;
        workers --;
    }while ( assembly_computers_on_line[0] < computers_on_palet && idx < data.size() && data[idx] == assembly_line[0]);

    assembly_times_line[0] /= assembly_workers_on_line_line[0];
    pq.push(make_tuple(0,assembly_times_line[0]));

    while( elements_on_line > 0){
        //Print how out line looks atm
        //cin.get();
        cout<<"-------("<<idx<<", "<<elements_on_line<<", "<<workers<<")\n";
        int j = 0;
        for(int i: assembly_line){
            cout<<((i == -1)?-1:assembly_times_line[j])<<" "<< i<<" "<<assembly_workers_on_line_line[j]<<endl;
            j++;
        }
        cout<<"-------\n";
        //We are looking for time of first element who can move
        //because other platform can be blocked.
        tuple<int, int> a = make_tuple(-1, -1);
        while(!pq.empty()){
            a = pq.top();
            pq.pop();
            if(get<0>(a) + 1 == params || assembly_line[get<0>(a) + 1 ] == -1){
                while(!pq.empty()){
                    pq.pop(); // faster would be replacing with new queue.
                }
                break;
            }
        }
        cout<<get<1>(a)<<" "<<assembly_line[get<0>(a)]<<endl;
        total_time += get<1>(a);

        // we move elements. We need to check all elements from Right to Left.
        if(assembly_line[params-1] != -1 && assembly_times_line[params-1] <= get<1>(a)){
            elements_on_line--;
            assembly_line[params - 1 ] = -1;
            assembly_times_line[params - 1] = 0;
            workers += assembly_workers_on_line_line[params - 1];
            assembly_workers_on_line_line[params - 1] = 0;
        }
        in_range(i,1, params){
            if(assembly_line[params-i-1] == -1 ){
                continue;
            }
            if(assembly_times_line[params-i-1] > get<1>(a)){
                assembly_times_line[params-i-1] =- get<1>(a);
                pq.push(make_tuple(params-i-1, assembly_times_line[params-i-1]));
                continue;
            }
            if(assembly_line[params-i ] == -1) {
                assembly_line[params - i] = assembly_line[params-i-1 ];
                assembly_computers_on_line[params - i] = assembly_computers_on_line[params - i - 1];
                assembly_times_line[params -i] = types[assembly_line[params-i]][params-i] * assembly_computers_on_line[params - i];
                //pq.push(make_tuple(params-i, assembly_times_line[params -i] ));
                need_workers.push_back(params -i);

                assembly_line[params-i-1 ]  = -1;
                assembly_times_line[params -i-1] = 0;
                assembly_computers_on_line[params -i-1]  = 0;

                workers += assembly_workers_on_line_line[params - 1];
                assembly_workers_on_line_line[params - 1] = 0;
            }else{
                assembly_times_line[params-i-1] =0;
                pq.push(make_tuple(params-i-1, assembly_times_line[params-i-1]));
            }

        }/*
        cout<<"****\n";
        in_range(i,0, params){
            cout<<assembly_line[i]<<endl;
        }
        cout<<"****\n";
        */

        //add new element if possible
        if(assembly_line[0] == -1 && idx < data.size()){
            do{
                assembly_line[0] = data[idx++];
                assembly_times_line[0] += types[assembly_line[0]][0];
                assembly_computers_on_line[0] += 1;
            }while ( assembly_computers_on_line[0] < computers_on_palet && idx < data.size() && data[idx] == assembly_line[0]);

            //pq.push(make_tuple(0,assembly_times_line[0]));
            need_workers.push_back(0);
            elements_on_line++;
        }

        cout<<"~~~~~~~~~~\n";
        while(need_workers.size()>0){
            int i = need_workers.back();
            if( workers > 0){
                assembly_workers_on_line_line[i]++;
                workers--;
                assembly_times_line[i] /= assembly_workers_on_line_line[i];
                pq.push(make_tuple(i,assembly_times_line[i]));
                need_workers.pop_back();
            }
            else{
                break;
            }
            cout<<i<<endl;
        }
        cout<<"~~~~~~~~~~\n";
    }
    return total_time;
}

int main(){
    srand( time( NULL ) );
    int ntypes= 2/*-1*/, params = 3;

    Config types = Config();
    generate_types(types, ntypes, params);
    cout<<"Types"<<endl;
    for(auto & i : types ){
        for(int j : i){
            cout<<j<<", ";
        }
        cout <<endl;
    }

    cout<<"Generate Id to type"<<endl;
    vector<int> x = vector<int>();
    generate_data(10, x, ntypes);
    for(int i : x ){
        cout<<i<<endl;
    }

    cout<<"Cost of permutation\n"<< compute_cost(x, types, params)<<endl;

    int gsize = x.size();
    vector<vector<double>> ant_graph(gsize);
    in_range(i, 0, gsize){
        ant_graph[i] = vector<double>(gsize, 1.0);
    }

    for(vector<double> r : ant_graph){
        for(int c : r) cout << c;
        cout << endl;
    }

    vector<int> perm = vector<int>(gsize);
    generate_permutation(ant_graph, perm);

    return 0;
}

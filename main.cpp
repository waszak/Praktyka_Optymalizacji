#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#define in_range(i, a,b)  for(int i =(a); i<(b); i++)

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

int compute_cost(vector<int> & data, Config &types, int params){
    int total_time = 0, elements_on_line = 1, idx =0;
    auto comp = [] (tuple<int,int> &a, tuple<int,int> &b) -> bool { return get<1>(a) < get<1>(b); };
    std::priority_queue<tuple<int,int>,std::vector<tuple<int,int>>, decltype(comp) > pq (comp);

    vector<int> assembly_line = vector<int>();
    assembly_line.resize(params, -1);

    assembly_line[0] = data[idx];
    pq.push(make_tuple(0,types[data[idx++]][0]));

    while( elements_on_line > 0){
        //Print how out line looks atm
        cout<<"-------("<<idx<<", "<<pq.size()<<")\n";
        for(int i: assembly_line){
            cout<<i<<endl;
        }
        cout<<"-------\n";
        //We are looking for first element who can move
        //because other platform can be blocked.
        tuple<int, int> a = make_tuple(-1, -1);
        vector<tuple<int,int>> removed_el = vector<tuple<int,int>>();
        while(!pq.empty()){
            a = pq.top();
            pq.pop();
            if(get<0>(a) + 1 == params || assembly_line[get<0>(a) + 1 ] == -1){
                for( auto i: removed_el){
                    pq.push(i);
                }
                break;
            }
            removed_el.push_back(a);
        }

        total_time += get<1>(a);
        if(get<0>(a) + 1 == params){
            elements_on_line--;
        }else{
            assembly_line[get<0>(a) + 1] = assembly_line[get<0>(a)];
            pq.push(make_tuple(get<0>(a)+1,types[data[assembly_line[get<0>(a)]]][get<0>(a)+1]));
        }
        assembly_line[get<0>(a)]  = -1;

        // we move elements
        /*
        * TODO
        */
        if(assembly_line[0] == -1 && idx < data.size() ){
            assembly_line[0] = data[idx];
            pq.push(make_tuple(0,types[data[idx++]][0]));
            elements_on_line++;
        }
    }
    return total_time;
}

int main(){
    srand( time( NULL ) );
    int ntypes= 3/*-1*/, params = 3;

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
    return 0;
}

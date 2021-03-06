#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <assert.h>
#include <algorithm>
#define in_range(i, a,b )  for(int i =(a); i < (b); i++)

using namespace std;
typedef  vector<vector<int>> Config;

//void generate_types(int types, int params)
void generate_types(Config &types, int ntypes =17, int params =9){
    types.clear();
    if(ntypes == -1){
        types.push_back({1,1,1});
        types.push_back({4,4,4});
        types.push_back({2,2,2});
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

        // Jakieś testy poprawnościowe
        if(ntypes ==-1 ){
            in_range(j,0,7){
                data.push_back(j%3);
                data.push_back(j%3);
                data.push_back(j%3);
                data.push_back(j%3);
                data.push_back(j%3);
                data.push_back(j%3);
                data.push_back(j%3);
                data.push_back(j%3);
            }
        }
        //...
    else{
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
    //int start = 0;
    perm[0] = start;
    remaining.erase(remaining.begin()+start);

    // get sum of values of remaining nodes
    auto sum = [] (vector<double> nbrs, vector<int> &rem){
        double result = 0;
        for(int nbr : rem) result += nbrs[nbr];
        return result;
    };
    in_range(i, 0, perm.size() - 1){
        double nbrs_sum = sum(ant_graph[perm[i]], remaining); // sum of values to remaining neighbours
        double mult = 1e5;
        int r = rand()% static_cast<int>(nbrs_sum * mult);

        int selected;
        in_range(nbr, 0, remaining.size()){
            int nbr_cost = static_cast<int>(ant_graph[perm[i]][remaining[nbr]] * mult);
            if(r < nbr_cost){
                selected = remaining[nbr];
                remaining.erase(remaining.begin()+nbr);
                break;
            }
            r -= nbr_cost;
        }

        perm[i+1] = selected;

    }

}
int compute_cost(vector<int> & data, Config &types, int params, int computers_on_palet=10, int workers = 10){
    int total_time = 0, elements_on_line = 1, idx =0;
    auto comp = [] (tuple<int,int> &a, tuple<int,int> &b) -> bool { return get<1>(a) > get<1>(b); };
    priority_queue<tuple<int,int>,std::vector<tuple<int,int>>, decltype(comp) > pq (comp);

    vector<int> assembly_line = vector<int>();
    vector<int> assembly_times_line = vector<int>();
    vector<int> assembly_computers_on_line = vector<int>();
    vector<int> assembly_workers_on_line = vector<int>();
    vector<int> last_time = vector<int>();
    vector<int> need_workers = vector<int>();

    assembly_line.resize(params, -1);
    assembly_times_line.resize(params,0);
    assembly_computers_on_line.resize(params,0);
    assembly_workers_on_line.resize(params, 0);
    last_time.resize(params, 0);

    do{
        assembly_line[0] = data[idx++];
        assembly_times_line[0] += types[assembly_line[0]][0];
        assembly_computers_on_line[0] ++;
        assembly_workers_on_line[0] ++;
        workers --;
    }while ( assembly_computers_on_line[0] < computers_on_palet && idx < data.size() && data[idx] == assembly_line[0]);

    assembly_times_line[0] /= assembly_workers_on_line[0];
    pq.push(make_tuple(0,assembly_times_line[0]));

    while( elements_on_line > 0){
    //cout << "Loop through line...\n";
        //Print how out line looks atm
        //cin.get();
        /*cout<<"-------("<<idx<<", "<<elements_on_line<<", "<<workers<<")\n";
        int j = 0;
        for(int i: assembly_line){
            cout<<((i == -1)?-1:assembly_times_line[j])<<" "<<assembly_workers_on_line[j]<<endl;
            j++;
        }
        cout<<"-------\n";*/
        //We are looking for time of first element who can move
        //because other platform can be blocked.
        tuple<int, int> a = make_tuple(-1, -1);
        while(!pq.empty()){
            a = pq.top();
            pq.pop();
            if(get<1>(a) != 0/*get<0>(a) + 1 == params || assembly_line[get<0>(a) + 1 ] == -1*/){
                while(!pq.empty()){
                    pq.pop(); // faster would be replacing with new queue.
                }
                break;
            }
        }
        //cout<<get<1>(a)<<" "<<assembly_line[get<0>(a)]<<endl;
        total_time += get<1>(a);

        // we move elements. We need to check all elements from Right to Left.
        if(assembly_line[params-1] != -1 && assembly_times_line[params-1] <= get<1>(a)){
            elements_on_line--;
            assembly_line[params - 1 ] = -1;
            assembly_times_line[params - 1] = 0;
            workers += assembly_workers_on_line[params - 1];
            assembly_workers_on_line[params - 1] = 0;
            last_time[params -1] = total_time;
        }else if(assembly_line[params-1] != -1 && assembly_workers_on_line[params -1] != 0 ){
            assembly_times_line[params-1] -= get<1>(a);
            pq.push(make_tuple(params-1, assembly_times_line[params-1]));
        }
        in_range(i,1, params){
            if(assembly_line[params-i-1] == -1 ){
                continue;
            }
            else if(assembly_times_line[params-i-1] > get<1>(a)){
                if(assembly_workers_on_line[params- i -1] != 0)assembly_times_line[params-i-1] -= get<1>(a);
                pq.push(make_tuple(params-i-1, assembly_times_line[params-i-1]));
                continue;
            }
            else if(assembly_line[params-i ] == -1 &&(assembly_workers_on_line[params-i -1] != 0 || assembly_times_line[params-i -1]==0) ) {
                assembly_line[params - i] = assembly_line[params-i-1 ];
                assembly_computers_on_line[params - i] = assembly_computers_on_line[params - i - 1];
                assembly_times_line[params -i] = types[assembly_line[params-i]][params-i] * assembly_computers_on_line[params - i];

                need_workers.push_back(params -i);


                assembly_line[params-i-1 ]  = -1;
                assembly_times_line[params -i-1] = 0;
                assembly_computers_on_line[params -i-1]  = 0;
                workers += assembly_workers_on_line[params -i - 1];
                assembly_workers_on_line[params-i - 1] = 0;
                last_time[params-i -1] = total_time;
            }else if(assembly_workers_on_line[params- i -1] != 0){
                assembly_times_line[params-i-1] =0;
                workers += assembly_workers_on_line[params -i - 1];
                assembly_workers_on_line[params-i - 1] = 0;
                pq.push(make_tuple(params-i-1, assembly_times_line[params-i-1]));
            }

        }

        //add new element if possible
        if(assembly_line[0] == -1 && idx < data.size()){
            do{
                assembly_line[0] = data[idx++];
                assembly_times_line[0] += types[assembly_line[0]][0];
                assembly_computers_on_line[0] += 1;
            }while ( assembly_computers_on_line[0] < computers_on_palet && idx < data.size() && data[idx] == assembly_line[0]);
            need_workers.push_back(0);
            elements_on_line++;
        }
    //cout << "Before orkers loop...\n";
        for(int i = 0; i < need_workers.size(); i++){
            if( assembly_line[ need_workers[i] ] == -1){
                workers+= assembly_workers_on_line[ need_workers[i] ];
                assembly_workers_on_line[ need_workers[i] ] = 0;
                need_workers.erase(need_workers.begin() + i);
            }
        }

        while( workers > 0){

        //cout << "Workers (" << workers << ")\n";
            bool need_worker = false;
            int pos = -1;int id = -1;
            sort(need_workers.begin(), need_workers.end());
            std::reverse(need_workers.begin(),need_workers.end());
            if(!need_worker){
                id=-1;
                 for(int i : need_workers){
           //      cout << "nw -- " << i << "\n";
                    id++;pos =i;
                    if(assembly_workers_on_line[i] == 0){
                        //pos = i;
                        break;
                    }
                    if( i != params -1 && assembly_line[i+1] != -1 ){

                        int d = (assembly_times_line[i] %(assembly_workers_on_line[i] +1) != 0 )?types[assembly_line[i]][i]:0;
                        d = (assembly_times_line[i]/ (assembly_workers_on_line[i] +1)) + d;
                        int e = assembly_times_line[i+1] ;
                        if(assembly_times_line[i+1] == 0 && assembly_workers_on_line[i+1] != 0){
                            e = (assembly_times_line[i+1] %(assembly_workers_on_line[i+1]) != 0 )?types[assembly_line[i+1]][i+1]:0;
                            e = (assembly_times_line[i+1]/ (assembly_workers_on_line[i+1] )) + e;
                        }
                        if(d >= e){
                            //pos = i;
                            break;
                        }
                    }if ( i != 0 && assembly_line[i -1] != -1){
                        int d = (assembly_times_line[i] %(assembly_workers_on_line[i] +1) != 0 )?types[assembly_line[i]][i]:0;
                        d = (assembly_times_line[i]/ (assembly_workers_on_line[i] +1)) + d;
                        int e = assembly_times_line[i-1] ;

                        if( assembly_workers_on_line[i-1] != 0){
                            e = (assembly_times_line[i-1] %(assembly_workers_on_line[i-1]) != 0 )?types[assembly_line[i-1]][i-1]:0;
                            e = (assembly_times_line[i-1]/ (assembly_workers_on_line[i-1] )) + e;
                        }

                        if(d >= e){
                            //pos = i;
                            break;
                        }
                    }
                 }
                if(pos != -1)need_worker = true;
            }

            if(! need_worker || need_workers.size() == 0 || pos == -1){
                break;
            };
            assembly_workers_on_line[pos]++;
            workers--;
            if (assembly_workers_on_line[pos] == assembly_computers_on_line[pos]){
                assembly_times_line[pos] /= assembly_workers_on_line[pos];
                pq.push(make_tuple(pos,assembly_times_line[pos]));
                need_workers.erase(need_workers.begin() + id);
            }

        }
        int id = -1;

        for(int j = 0; j < need_workers.size();j++){
            int i = need_workers[j];
            id++;
            if(assembly_workers_on_line[i]!=0){
                int d = (assembly_times_line[i] %assembly_workers_on_line[i] != 0 )?types[assembly_line[i]][i]:0;
                assembly_times_line[i] = (assembly_times_line[i]/ assembly_workers_on_line[i]) +d;
                pq.push(make_tuple(i,assembly_times_line[i]));
                need_workers.erase(need_workers.begin() + id);
                id--;
                j--;
            }
        }

    }
    assert(total_time == last_time[params -1]);
    return total_time;
}

void perm_test(){
    vector<vector<double>> ant_graph = {
    //    0   1    2    3    4    5    6    7
        { 1.0, 1.2, 1.6, 1.4, 2.8, 4.7, 1.15, 1.7},
        { 0, 1.0, 1.4, 2.7, 1.2, 1.8, 3.2, 1.6 },
        { 0, 1.3, 1.0, 4.5, 2.1, 1.7, 1.2, 2.8},
        { 0, 4.8, 1.5, 1.0, 2.1, 1.8, 2.1, 3.3},
        { 0, 1.7, 4.8, 2.1, 1.0, 1.6, 2,4, 1.9},
        { 0, 1.3, 1,8, 2.1, 5.2, 1.0, 2.2, 1.65},
        { 0, 1.7, 1.8, 2.0, 2.2, 1.75, 1.0, 4.6},
        { 0, 1.9, 2.2, 1.1, 1.65, 2.15, 4.85, 1.0}
    };

    for(vector<double> r : ant_graph){
        for(double c : r) cout << c << " ";
        cout << endl;
    }

    //vector<int> perm = vector<int>(gsize);
    vector<int> perm = vector<int>(8);

    in_range(i, 0, 15){
        generate_permutation(ant_graph, perm);
        cout << "perm " << i << ": ";
        for(double val : perm) cout << val << " ";
        cout << endl;

        in_range(i, 1, perm.size()){
            ant_graph[perm[i-1]][perm[i]] += 0.6;
        }
        in_range(i, 0, perm.size()){
            in_range(j, 0, perm.size()){
                if(ant_graph[i][j] > 0.1) ant_graph[i][j] -= 0.1;
                else ant_graph[i][j] = 0.01;
            }
        }
    }
}

void add_pheromon(vector<int> &perm, vector<vector<double>> &ant_graph, int &cost){
    in_range(i, 1, perm.size())
        ant_graph[ perm[i-1] ][ perm[i] ] += (1.0 / cost);
}

void vaporize_pheromon(vector<vector<double>> &ant_graph, double &p){
    in_range(i, 1, ant_graph.size()){
        in_range(j, 0, ant_graph.size())
            ant_graph[i][j] = floorf( ant_graph[i][j] * (1 - p) * 1e5 ) / 1e5;
    }
}

bool cmp(const tuple<int, int> &a, const tuple<int, int> &b){
    return get<0>(a) < get<0>(b) || (get<0>(a) == get<0>(b) && get<1>(a) > get<1>(b));
}

int main(){
    srand( time( NULL ) );

    int ntypes=/*3*/17, params = 9;
    int computers = 100;
    int countTypes[ntypes]; int p=0;

    Config types = Config();
    //generate_types(types, ntypes, params);
    for (int i = 0; i < ntypes; i++){
		vector <int> a = vector <int>();
		for (int j = 0; j < params; j++){
			int d;
			cin >> d;
			a.push_back(d);
		}
		types.push_back(a);
	}

    cout<<"Types"<<endl;
    for(auto & i : types ){
        for(int j : i){
            cout<<j<<", ";
        }
        cout <<endl;
        countTypes[p++] = 0;
    }

    cout<<"Generate Id to type"<<endl;
    vector<int> x = vector<int>();
    vector<tuple<int, int>> xx = vector <tuple<int,int>>();
    vector<int> xxx = vector<int>();

    generate_data(computers, x, ntypes);
    x.clear();
    for(int i : x ){
        //cout<<i<<endl;
        countTypes[i]++;
    }
    /*for(int i = 0; i < ntypes; i++){
        //cout<<countTypes[i]<<endl;
        while(countTypes[i] >=10){
            countTypes[i] -= 10;
            xx.push_back(make_tuple(i,10));
        }if(countTypes[i]>0){
            xx.push_back(make_tuple(i,countTypes[i]));
            countTypes[i]=0;
        }
    }*/
    int b, d;
	while (cin >> b >> d){
        while(d > 10){
            xx.push_back(make_tuple(b, 10));
            d -= 10;
        }
        if( d >0){
            xx.push_back(make_tuple(b, d));
        }
	}

    for(tuple<int,int> t : xx){
        //cout<<get<0>(t)<<" "<<get<1>(t)<<endl;
        int r = get<1>(t);
        while(r > 0){
            xxx.push_back(get<0>(t));
            r--;
        }
    }
    for(tuple<int,int> t : xx){
        int r = get<1>(t);
        while(r > 0){
            x.push_back(get<0>(t));
            r--;
        }
    }


    // ant graph
    int gsize = xx.size();
    vector<vector<double>> ant_graph(gsize);

    in_range(i, 0, gsize){
        ant_graph[i] = vector<double>(gsize, 1.0);
    }

    int colony = 100;
    int iter = 300;
    double vaporize = 0.05;

    vector<int> perm = vector<int>(gsize);
    cout<<"Cost of sort "<<compute_cost(x, types, params)<<endl;
    int Min = compute_cost(x, types, params);
    int Min_Brute = Min*100;
    vector<vector<int>> ants;
    vector<int> costs;

    in_range(i, 0, iter){

//        cout << "Colony " << i << endl;
        in_range(j, 0, colony){ // generate ants
    //sort(x.begin(), x.end());

            generate_permutation(ant_graph, perm);

                // using permutation
            vector<tuple<int,int>> p = vector<tuple<int,int>>();
            for(int &i : perm){
                p.push_back(xx[i]);
            }
            xxx.clear();
            for(tuple<int,int> t : p){
            //cout<<get<0>(t)<<" "<<get<1>(t)<<endl;
                int r = get<1>(t);
                while(r > 0){
                    xxx.push_back(get<0>(t));
                    r--;
                }
            }

            int cost = compute_cost(xxx, types, params);
//            cout<<"Cost of permutation: "<< cost <<endl;


            ants.emplace_back(perm);
            costs.push_back(cost);

            /*
            add_pheromon(perm, ant_graph, cost);
            */
            if(cost < Min){
                Min = cost;
            }
        }

        in_range(i, 0, ants.size())
            add_pheromon(ants[i], ant_graph, costs[i]);

        vaporize_pheromon(ant_graph, vaporize);

        ants.clear();
        costs.clear();
    }
    //sort(x.begin(), x.end());
    cout<<"Cost of starting perm "<<compute_cost(x, types, params);
    sort(x.begin(), x.end());
    cout<<"\nCost of the Sort "<<compute_cost(x, types, params)<<"\nBest Result "<<Min<<endl;

    do {
        xxx.clear();
        for(tuple<int,int> t : xx){
                int r = get<1>(t);
                while(r > 0){
                    xxx.push_back(get<0>(t));
                    r--;
                }
        }
        int cost = compute_cost(xxx, types, params);
        if( cost < Min_Brute){
            Min_Brute = cost;
            cout<<cost<<endl;
        }
    } while (next_permutation(xx.begin(), xx.end(),cmp ));
    cout<<"We get with brute force: "<<Min_Brute<<endl;
    return 0;
}

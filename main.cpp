#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#define in_range(i, a,b)  for(int i =(a); i<(b); i++)

using namespace std;
typedef  vector<vector<float>> Config;

//void generate_types(int types, int params)
void generate_types(Config &types, int ntypes =17, int params =9){
    types.clear();
    if(ntypes == -1){
        types.push_back({1.0,1.0,1.0});
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
    }else{
        in_range(j,0,n){
            data.push_back( rand()%ntypes);
        }
    }
}

int main(){
    srand( time( NULL ) );

    Config types = Config();
    generate_types(types, -1);
    cout<<"Types"<<endl;
    for(auto & i : types ){
        for(float j : i){
            cout<<j<<", ";
        }
        cout <<endl;
    }

    cout<<"Generate Id to type"<<endl;
    vector<int> x = vector<int>();
    generate_data(10, x);
    for(int i : x ){
        cout<<i<<endl;
    }
    return 0;
}

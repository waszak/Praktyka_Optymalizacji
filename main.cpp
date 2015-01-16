#include <iostream>
#include <vector>
#define in_range(i, a,b)  for(int i =(a); i<(b); i++)
using namespace std;

//void generate_types(int types, int params)
void generate_data( int n, vector<vector<float>> &data, int types=17, int params=9){
    data.clear();
    if( n ==-1){
        // Jakieœ testy poprawnoœciowe
        if(types ==1 ){
            in_range(j,0,10){
                data.push_back({1.0,1.0,1.0});
            }
        }
    }else{
        in_range(j,0,n){
            cout<<j;
        }
    }

}
int main(){
    vector<vector<float>> x = std::vector<vector<float>>();
    generate_data(-1, x, 1);

    for(vector<float> & i : x ){
        for(float j : i){
            cout<<j<<", ";
        }
        cout <<endl;
    }
    return 0;
}

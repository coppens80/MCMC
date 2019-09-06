#include "metropolis-hastings.h"
#include <vector>
#include <random>
#include <stdio.h>
#include <fstream>
using namespace std;

int main(){
    vector<double> a = {0, 1};
    vector<double> b = {1, 20, 2};
    
    std::ofstream f1, f2;
    random_device rd;
    mt19937 generator = mt19937(rd());
    vector<double> Data(30000);
    vector<double> sampleData(1000);
    normal_distribution<double> randData(10,3);
    uniform_int_distribution<unsigned long> sample(0, Data.size());

    f1.open("DATA/testData.csv");
    for (int i=0; i<Data.size(); ++i){
        Data[i] = randData(generator);
        f1 << Data[i] << endl;
    }
    f1.close();

    f2.open("DATA/sampleData.csv");
    for (int i=0; i<sampleData.size(); ++i){
        sampleData[i] = Data[sample(generator)];
        f2 << sampleData[i] << endl;
    }
    f2.close();
    
    MH_normal simpleModel(a, sampleData);
    simpleModel.run_model();
}

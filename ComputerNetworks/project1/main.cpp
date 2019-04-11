#include <iostream>
#include <vector>
#include <fstream>


#include "SimulationEngine.h"

const int COLUMN = 4;

using namespace std;

int main() {
    
    //read in contents from the file
    
    /*put them into an 2D array where each row represents an edge in the
     network's topology array and the first column is the source node,
     second column is the destination node, third column is the weight (cost),
     fourth column is the speed (delay)*/
    ifstream infile;
    infile.open("topology3.txt");
    
    if(!infile) {
        cout << "Error. File Not properly read. Terminating Program" << endl;
        return 0;
    }
    string line;
    int top_size = 0;
    
    while(getline(infile, line))
         top_size++;
    
    infile.close();
    infile.open("topology3.txt");
    
    vector<vector<float> > network(top_size);

    float temp;
    for(int i = 0; i < top_size; i++) {
        for(int j = 0; j < COLUMN; j++) {
            infile >> temp;
            network[i].push_back(temp);
        }
    }
    SimulationEngine simulationEngine(network);
    
    simulationEngine.runSimulation(network, 0, 0);
}


#include "Node.h"
#include <iostream>
#include <vector>
#include <iomanip>


const int INFINITY = 0;

using namespace std;

Node::Node() {
    
}

Node::Node(int rows) {
    identity = 0;
}

//find all the neighbors to each node and cost their cost and time
void Node::findNeighbors(vector<vector<float> > network) {
    bool match = false;
    for(int i = 0; i < network.size(); i++) {
        if(network[i][0] == identity) {
            neighbors.push_back(network[i][1]);
        }
    }
    for(int k = 0; k < network.size(); k++) {
        if(network[k][1] == identity) {
            for(int o = 0; o < neighbors.size(); o++) {
                if(network[k][0] == neighbors[o]) {
                    match = true;
                }
            }
            if(!match) {
                neighbors.push_back(network[k][0]);
            }
        }
    }
}

void Node::buildDistances(vector<vector<float> > network, int numNodes) {
    distances.resize(numNodes);
    //initialize every value in distances table to infinity (10000)

    for(int i = 0; i < numNodes; i++) {
        distances[i].destination = i;
        distances[i].weight = INFINITY;
        distances[i].time = INFINITY;
    }
    //for all cost and time values we know from network, input into distances table
    for(int j = 0; j < network.size(); j++) {
        if(network[j][0] == identity) {
            distances[network[j][1]].weight = network[j][2];
            distances[network[j][1]].time = network[j][3];
        }
    }
    for(int i = 0; i < network.size(); i++) {
        bool match = false;
        if(network[i][1] == identity) {
            for(int k = 0; k < neighbors.size(); k++) {
                if(network[i][0] == neighbors[k]) {
                    match = true;
                }
            }
            if(match) {
                distances[network[i][0]].weight = network[i][2];
                distances[network[i][0]].time = network[i][3];
            }
        }
    }
//set all distances (cost and time) for each node to itself to 0
    distances[identity].weight = 0;
    distances[identity].time = 0;
}

void Node::printDistances() {
    for(int i = 0; i < distances.size(); i++)
        cout << setw(8) << "(" << distances[i].destination
             << "," << distances[i].weight << "," << distances[i].time << ")";
}

void Node::buildRoutingTable() {
    bool foundIt = false;
    
    routes.resize(distances.size() - 1);
    for(int i = 0; i < distances.size() - 1; i++) {
        bool selfFound = false;
        if(distances[i].destination == identity) {
            selfFound = true;
            foundIt = true;
        }
        
        if(selfFound)
            i++;
        
        if(selfFound) {
            if(distances[i].weight == INFINITY) {
                routes[i - 1].destination = distances[i].destination;
                routes[i - 1].nextHop = distances[i].destination;
                routes[i - 1].weight = INFINITY;
                routes[i - 1].time = INFINITY;
            }
            else {
                routes[i - 1].destination = distances[i].destination;
                routes[i - 1].nextHop = distances[i].destination;
                routes[i - 1].weight = distances[i].weight;
                routes[i - 1].time = distances[i].time;
            }
        }
        else {
            if(foundIt) {
                int j = i;
                j++;
                if(distances[j].weight == INFINITY) {
                    routes[i].destination = distances[j].destination;
                    routes[i].nextHop = distances[j].destination;
                    routes[i].weight = INFINITY;
                    routes[i].time = INFINITY;
                }
                else {
                    routes[i].destination = distances[j].destination;
                    routes[i].nextHop = distances[j].destination;
                    routes[i].weight = distances[j].weight;
                    routes[i].time = distances[j].time;
                }
            }
            else {
                if(distances[i].weight == INFINITY) {
                    routes[i].destination = distances[i].destination;
                    routes[i].nextHop = distances[i].destination;
                    routes[i].weight = INFINITY;
                    routes[i].time = INFINITY;
                }
                else {
                    routes[i].destination = distances[i].destination;
                    routes[i].nextHop = distances[i].destination;
                    routes[i].weight = distances[i].weight;
                    routes[i].time = distances[i].time;
                }
            }
        }
            
        
        if(selfFound)
            i--;
        
        if(selfFound && i == distances.size() - 1)
            i++;
    }
}
void Node::printRoutingTable() {
    for(int i = 0; i < routes.size(); i++) {
        if(routes[i].weight != INFINITY) {
            cout << "Destination: " << routes[i].destination;
            cout << ", Next Hop: " << routes[i].nextHop;
            cout << ", Weight: " << routes[i].weight;
            cout << ", Time: " << routes[i].time << endl;
        }
        else {
            cout << "Destination: " << routes[i].destination;
            cout << ", Next Hop: " << "UNKNOWN";
            cout << ", Weight: " << routes[i].weight;
            cout << ", Time: " << routes[i].time << endl;
        }
    }
}

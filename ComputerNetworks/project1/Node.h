#pragma once
#include <iostream>
#include <vector>


using namespace std;

typedef struct Distances {
    int destination;
    float weight, time;
} Distance;

typedef struct RoutingTable {
    int destination, nextHop;
    float weight, time;
} Route;

class Node {
    private:
        int identity;
        vector<int> neighbors;
        vector<Route> routes;
        vector<Distance> distances;
    public:
        Node ();
        Node (int);

        void setId(int i) { identity = i; }
        int getId() { return identity; }
    
        vector<int> getNeighbors() { return neighbors; }
        int getNeighbor(int i) { return neighbors[i]; }
    
        vector<Route> getRoutes() { return routes; }
        Route getRoute(int i) { return routes[i]; }
    
        void setRouteNextHop(int i, int nh) { routes[i].nextHop = nh; }
        int getNextHop(int i) { return routes[i].nextHop; }
    
        void setRouteTime(int i, float t) { routes[i].time = t; }
        float getTime(int i) { return routes[i].time; }
    
        void setRouteWeight(int i, float w) { routes[i].weight = w; }
        float getWeight(int i) { return routes[i].weight; }
    
        void findNeighbors(vector<vector<float > > network);
    
        void buildDistances(vector<vector<float> >, int);
        void printDistances();
    
        void buildRoutingTable();
    
        void printRoutingTable();
};


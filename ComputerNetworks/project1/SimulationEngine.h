#include <iostream>
#include <vector>

#include "Node.h"

using namespace std;

typedef struct Event {
    int time;
    string type;
    vector<Node> nodesInvolved;
    int next;
} Event;

typedef struct DVpacket {
    Node source, destination;
} DVpacket;

class SimulationEngine {
    private:
        vector<Node> node;
        vector<Event> events;
    vector<DVpacket> packets;
        int numberOfNodes;
    public:
        SimulationEngine(vector<vector<float> >);
        void runSimulation(vector<vector<float> >, int, float);
        void sendDVPacket(DVpacket, float &, int &, int &);
        void sendPacket();
    
        void triggerEvent(Event, vector<vector<float> >, float);
    
        void periodicDVUpdate(float, int &, int &);
        void linkFailure(vector<vector<float> >);
    
        bool checkConvergence();
    
    
};

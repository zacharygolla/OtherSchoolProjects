#include <vector>

#include "SimulationEngine.h"
#include "Node.h"


#include <iostream>

const int SPECIAL = 10;

using namespace std;

SimulationEngine::SimulationEngine(vector<vector<float> > network) {
    //build nodes
    
    //find the number of nodes in the numbers
    numberOfNodes = 0;
    for(int i = 0; i < network.size(); i++) {
        if(network[i][0] > numberOfNodes)
            numberOfNodes = network[i][0];
        if(network[i][1] > numberOfNodes)
            numberOfNodes = network[i][1];
    }
    numberOfNodes++;
    
    node.resize(numberOfNodes);
    
    //set the id for each node
    for(int i = 0; i < numberOfNodes; i++)
        node[i].setId(i);
    
    //find each nodes neighbors
    for(int i = 0; i < numberOfNodes; i++)
        node[i].findNeighbors(network);
    
    for(int i = 0; i < numberOfNodes; i++)
        node[i].buildDistances(network, numberOfNodes);
    for(int i = 0; i < numberOfNodes; i++)
        node[i].buildRoutingTable();
}

void SimulationEngine::runSimulation(vector<vector<float> > network, int whichRun, float time) {
    for(int i = 0; i < 50; i++) {
        //create a new event
        //if the new event isnt scheduled to trigger a special case, send periodic update
        events.resize(events.size() + 1);
        if(i != SPECIAL - 1) {
            events[events.size() - 1].type = "Periodic DV Packet";
        }
        
        //wait 1 second before triggering any kind of event
        
        //trigger an event
        triggerEvent(events[i], network, time);
    }
    if (whichRun != 1) {
        events.resize(events.size() + 1);
        events[events.size() - 1].type = "Send Packet";
        triggerEvent(events[events.size() - 1], network, time);
    }
    
    if(whichRun != 1) {
        events.resize(events.size() + 1);
        events[events.size() - 1].type = "Link Failure";
        triggerEvent(events[events.size() - 1], network, time);
    }
}

void SimulationEngine::triggerEvent(Event e, vector<vector<float> > network, float time) {
    if(e.type == "Link Failure")
        linkFailure(network);
    else if(e.type == "Send Packet")
        sendPacket();
    else {
        int counter = 0;
        int convergence = 0;
        periodicDVUpdate(time, convergence, counter);
    }
}

//trigger perioidic DV Update
void SimulationEngine::periodicDVUpdate(float time, int & convergence, int & counter) {
    //create each of the DV packets to send for each node and send packet
    for(int i = 0; i < node.size(); i++) {
        packets.resize(node[i].getNeighbors().size());
        for(int j = 0; j < node[i].getNeighbors().size(); j++) {
            packets[j].source = node[i];
            packets[j].destination = node[node[i].getNeighbor(j)];
            
            sendDVPacket(packets[j], time, convergence, counter);
        }
        
    }
    
}

void SimulationEngine::linkFailure(vector<vector<float> > network) {
    cout << endl << "Link Failure:";
     if(10 > node.size() && node.size() > 0) {
        float time = 10;
        node.clear();
        network.erase(network.begin() + 5);
        
        node.resize(numberOfNodes);
        
        //set the id for each node
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].setId(i);
        }
        
        //find each nodes neighbors
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].findNeighbors(network);
        }
        
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].buildDistances(network, numberOfNodes);
        }
        
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].buildRoutingTable();
        }
        
        runSimulation(network, 1, time);
        
    }
    else if(20 > node.size() && node.size() >= 10) {
        float time = 20;
        node.clear();
        network.erase(network.begin() + 12);
        
        node.resize(numberOfNodes);
        
        //set the id for each node
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].setId(i);
        }
        
        //find each nodes neighbors
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].findNeighbors(network);
        }
        
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].buildDistances(network, numberOfNodes);
        }
        
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].buildRoutingTable();
        }
        
        runSimulation(network, 1, time);
        
    }
    else {
        float time = 30;
        node.clear();
        network.erase(network.begin() + 38);
        
        node.resize(numberOfNodes);
        
        //set the id for each node
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].setId(i);
        }
        
        //find each nodes neighbors
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].findNeighbors(network);
        }
        
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].buildDistances(network, numberOfNodes);
        }
        
        for(int i = 0; i < numberOfNodes; i++) {
            node[i].buildRoutingTable();
        }
        
        runSimulation(network, 1, time);
    }
}

void SimulationEngine::sendPacket() {
    if(10 > node.size() && node.size() > 0) {
        float time = 10;
        cout << endl << "Path when sending data packet: ";
        cout << endl << "Node 0 receives packet destined to node 3 at time 10." << endl;
        cout << "The packet will travel to destination 3 using next hop " << node[0].getNextHop(2);
        time += node[0].getTime(2);
        cout << " and arrive at time " << time << "." << endl;
        time += node[1].getTime(2);
        cout << "It will then travel to its final destination 3, using next hop 3 at time " << time << "." << endl;
    }
    else if(20 > node.size() && node.size() >= 10) {
        float time = 20;
        cout << endl << "Path when sending data packet: ";
        cout << endl << "Node 0 receives packet destined to node 7 at time 20." << endl;
        cout << "The packet will travel to destination 7 using next hop " << node[0].getNextHop(7);
        time += node[0].getTime(7);
        cout << " and arrive at time " << time << "." << endl;
        time += node[1].getTime(7);
        cout << "It will then travel to its final destination 7, using next hop 7 at time " << time << "." << endl;
    }
    else {
        float time = 30;
        cout << "Did not successfully implement run 3" << endl;
    }
    
    
 }

//send all DV Packets to necessary neighbors
void SimulationEngine::sendDVPacket(DVpacket packets, float & time, int & convergence, int & counter) {
    for(int i = 0; i < packets.source.getRoutes().size(); i++) {
        if(packets.source.getRoute(i).destination == packets.destination.getId()) {
            time += packets.source.getRoute(i).time;
        }
    }
    for(int i = 0; i < packets.source.getRoutes().size(); i++) {
        for(int j = 0; j < packets.destination.getRoutes().size(); j++) {
            if(packets.source.getRoute(i).destination == packets.destination.getRoute(j).destination) {
                int temp = 0;
                for(int k = 0; k < packets.destination.getRoutes().size(); k++) {
                    if(packets.destination.getRoute(k).destination == packets.source.getId()) {
                        temp = k;
                    }
                }
                if(packets.destination.getRoute(j).weight == 0 || (packets.destination.getRoute(j).weight > (packets.source.getRoute(i).weight + packets.destination.getRoute(temp).weight))) {
                    packets.destination.setRouteNextHop(j, packets.source.getId());
                    node[packets.destination.getId()].setRouteNextHop(j, packets.source.getId());

                    packets.destination.setRouteWeight(j, packets.destination.getRoute(temp).weight + packets.source.getRoute(i).weight);
                    node[packets.destination.getId()].setRouteWeight(j, packets.destination.getRoute(temp).weight + packets.source.getRoute(i).weight);

                    packets.destination.setRouteTime(j, packets.destination.getRoute(temp).time + packets.source.getRoute(i).time);
                    node[packets.destination.getId()].setRouteTime(j, packets.destination.getRoute(temp).time + packets.source.getRoute(i).time);
                    counter++;
                    if(checkConvergence() && convergence == 0) {
                        cout << endl << "Last Node to converge: " << packets.destination.getId();
                        for(int i = 0; i < node.size(); i++) {
                            cout << endl << "Routing Table Node " << i << ": " << endl;
                            cout << "Number of messages sent before convergence: " << counter << endl;
                    
                            node[i].printRoutingTable();
                            cout << endl;
                        }
                        cout << "The network converged at time: " << time << endl;
                        convergence++;
                    }
                    //trigger update when someones routing table updates
                    for(int m = 0; m < node[packets.destination.getId()].getNeighbors().size(); m++) {
                        packets.source = packets.destination;
                        
                        packets.destination = node[packets.destination.getId()].getNeighbor(m);
                        sendDVPacket(packets, time, convergence, counter);
                    }
                }
            }
        }
    }
}

bool SimulationEngine::checkConvergence() {
    bool converged = true;
    for(int i = 0; i < node.size(); i++) {
        for(int j = 0; j < node[i].getRoutes().size(); j++) {
            if(node[i].getRoute(j).weight == 0)
                converged = false;
        }
    }
    return converged;
}

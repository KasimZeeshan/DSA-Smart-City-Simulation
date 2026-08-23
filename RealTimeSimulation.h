#pragma once
#include <iostream>
#include <string>
#include "../DataProject/HashBus.h"
#include "../DataProject/Graph.h"
#include "../DataProject/Stack.h"

using namespace std;

/*
===========================================================
   REAL-TIME ROUTE SIMULATION  (BONUS FEATURE #3)
   --------------------------------------------------------
   Simulates buses moving along their routes in real-time
   with step-by-step updates and timing information.

   Features:
   - Step-by-step bus movement simulation
   - Travel time estimation between stops
   - Live location updates
   - Automatic route completion tracking

   ** PURE ASCII - NO UNICODE **
===========================================================
*/

class RealTimeSimulation {
private:
    BusHashTable* busTable;
    Graph* cityGraph;
    RouteStack* simulationLog;

    // Simulate delay between steps (for demo purposes)
    void simulateDelay() {
        cout << "   [Moving...]" << endl;
        cout << "   [Complete!]" << endl;
    }

public:
    // Constructor
    RealTimeSimulation(BusHashTable* bTable, Graph* graph, RouteStack* stack) {
        busTable = bTable;
        cityGraph = graph;
        simulationLog = stack;
    }

    // -------------------------------------------------------
    // simulateBusMovement(busNo, numSteps)
    // Moves bus forward 'numSteps' stops along its route
    // -------------------------------------------------------
    void simulateBusMovement(const string& busNo, int numSteps) {
        BusRecord* record = busTable->search(busNo);
        if (!record) {
            cout << "[X] Bus " << busNo << " not found." << endl;
            return;
        }

        cout << "\n+==================================================+" << endl;
        cout << "|  REAL-TIME BUS SIMULATION: " << busNo << "                |" << endl;
        cout << "+==================================================+" << endl;

        string currentStop = record->bus.currentStopId;
        cout << "\n[*] Starting Position: " << currentStop << endl;
        cout << "[*] Steps to Simulate: " << numSteps << endl;
        cout << "\n" << string(50, '-') << endl;

        int step = 1;
        while (step <= numSteps) {
            string nextStop = record->route.getNextAfter(currentStop);

            if (nextStop == "") {
                cout << "\n[!] Bus has reached END OF ROUTE at: " << currentStop << endl;
                break;
            }

            // Display movement
            cout << "\nStep " << step << "/" << numSteps << ":" << endl;
            cout << "  >> Moving from: " << currentStop << endl;
            cout << "            to: " << nextStop << endl;

            simulateDelay();

            // Update position
            currentStop = nextStop;
            record->bus.currentStopId = currentStop;
            simulationLog->push(currentStop);

            cout << "  [OK] Now at: " << currentStop << endl;

            step++;
        }

        cout << "\n" << string(50, '-') << endl;
        cout << "[*] Simulation Complete!" << endl;
        cout << "[*] Final Position: " << currentStop << endl;
    }

    // -------------------------------------------------------
    // simulateFullRoute(busNo)
    // Simulates entire route from current position to end
    // -------------------------------------------------------
    void simulateFullRoute(const string& busNo) {
        BusRecord* record = busTable->search(busNo);
        if (!record) {
            cout << "[X] Bus not found." << endl;
            return;
        }

        cout << "\n+==================================================+" << endl;
        cout << "|     FULL ROUTE SIMULATION: " << busNo << "              |" << endl;
        cout << "+==================================================+" << endl;

        string currentStop = record->bus.currentStopId;
        int stepCount = 0;

        cout << "\n[*] Starting Full Route Simulation..." << endl;
        cout << "[*] Initial Position: " << currentStop << endl;
        cout << "\n" << string(50, '-') << endl;

        while (true) {
            string nextStop = record->route.getNextAfter(currentStop);

            if (nextStop == "") {
                cout << "\n[!] Route Complete!" << endl;
                cout << "[*] Final Stop: " << currentStop << endl;
                cout << "[*] Total Stops Visited: " << stepCount + 1 << endl;
                break;
            }

            stepCount++;
            cout << "\nStop #" << stepCount << ":" << endl;
            cout << "  " << currentStop << " --> " << nextStop << endl;

            simulateDelay();

            currentStop = nextStop;
            record->bus.currentStopId = currentStop;
            simulationLog->push(currentStop);
        }

        cout << string(50, '-') << endl;
    }

    // -------------------------------------------------------
    // estimateTravelTime(busNo, destination)
    // Estimates time to reach destination based on path
    // -------------------------------------------------------
    void estimateTravelTime(const string& busNo, const string& destination) {
        BusRecord* record = busTable->search(busNo);
        if (!record) {
            cout << "[X] Bus not found." << endl;
            return;
        }

        string currentStop = record->bus.currentStopId;

        cout << "\n+==================================================+" << endl;
        cout << "|          TRAVEL TIME ESTIMATION                  |" << endl;
        cout << "+==================================================+" << endl;
        cout << "\n[*] Bus: " << busNo << endl;
        cout << "[*] Current Stop: " << currentStop << endl;
        cout << "[*] Destination: " << destination << endl;
        cout << "\n" << string(50, '-') << endl;

        int pathSize = 0;
        string* path = cityGraph->dijkstra(currentStop, destination, pathSize);

        if (!path) {
            cout << "[X] No route available to destination." << endl;
            return;
        }

        cout << "\n[*] Route Found!" << endl;
        cout << "[*] Number of Stops: " << pathSize << endl;
        cout << "[*] Estimated Time: " << (pathSize - 1) * 3 << " minutes" << endl;
        cout << "\n[*] Path: ";

        for (int i = 0; i < pathSize; i++) {
            cout << path[i];
            if (i + 1 < pathSize) cout << " -> ";
        }
        cout << endl;

        delete[] path;
    }

    // -------------------------------------------------------
    // showSimulationLog()
    // Displays complete simulation history
    // -------------------------------------------------------
    void showSimulationLog() {
        cout << "\n+==================================================+" << endl;
        cout << "|          SIMULATION HISTORY LOG                  |" << endl;
        cout << "+==================================================+" << endl;
        simulationLog->printStack();
    }
};
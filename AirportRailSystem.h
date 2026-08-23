#pragma once
#include <iostream>
#include <string>

#include "../DataProject/Graph.h"
#include "../DataProject/HashAirport.h"   
#include "../DataProject/Stack.h"

using namespace std;

/*
===============================================================
AIRPORT & RAIL INTEGRATION MODULE
---------------------------------------------------------------
BONUS FEATURE (Part-4)
. Adds airports and railway stations as graph nodes.
. Supports linking them with weighted distances.
. Provides shortest-route queries to anywhere using your Graph.
. Tracks travel history using existing Stack.
. Uses Hash Table for quick airport/station lookup.
===============================================================
*/

class AirportRailSystem {
private:
    Graph* cityGraph;          // pointer so we reuse SAME main city graph
    AirportHashTable airportTable;
    RouteStack* travelLog;

public:

    // Constructor receives GRAPH from main system
    AirportRailSystem(Graph* g) {
        cityGraph = g;
        travelLog = new RouteStack(50);
    }

    // -------------------------------
    // REGISTER NEW AIRPORT / STATION
    // -------------------------------
    void addAirport(const string& code, const string& name, double lat, double lon) {
        Airport ap;
        ap.code = code;
        ap.name = name;
        ap.coord.lat = lat;
        ap.coord.lon = lon;

        // Convert Airport -> Stop because Graph only stores Stop
        Stop s;
        s.id = ap.code;
        s.name = ap.name;
        s.coord = ap.coord;

        // Add graph node
        cityGraph->addNode(s.id, s);

        // Insert into hash table
        airportTable.insertAirport(ap);

        cout << "Airport added: " << code << endl;
    }


    // -------------------------------
    // CONNECT AIRPORT/STATION TO ROAD NETWORK
    // -------------------------------
    void connectToRoad(const string& code, const string& stopId, int distance) {
        if (!cityGraph->hasNode(code) || !cityGraph->hasNode(stopId)) {
            cout << "Invalid node. Cannot connect.\n";
            return;
        }

        cityGraph->addEdge(code, stopId, distance);
        cout << "Connected airport/station " << code
            << " <-> " << stopId
            << " Distance = " << distance << endl;
    }

    // -------------------------------
    // PRINT AIRPORT/STATION DETAILS
    // -------------------------------
    void printAirport(const string& code) {
        AirportRecord* rec = airportTable.search(code);
        if (!rec) {
            cout << "No such airport/station found.\n";
            return;
        }

        cout << "Code: " << rec->airport.code
            << "\nName: " << rec->airport.name
            << "\nCoordinates: (" << rec->airport.coord.lat
            << ", " << rec->airport.coord.lon << ")\n";
    }

    // -------------------------------
    // SHORTEST ROUTE TO ANY LOCATION
    // -------------------------------
    void routeTo(const string& fromCode, const string& destination) {
        if (!cityGraph->hasNode(fromCode) || !cityGraph->hasNode(destination)) {
            cout << "Invalid route request\n";
            return;
        }

        int size = 0;
        string* path = cityGraph->dijkstra(fromCode, destination, size);

        if (!path) {
            cout << "No route found.\n";
            return;
        }

        cout << "Route from " << fromCode << " → " << destination << ":\n";
        for (int i = 0; i < size; i++) {
            cout << path[i];
            travelLog->push(path[i]);        // store travel step
            if (i + 1 < size) cout << " -> ";
        }
        cout << "\n";

        delete[] path;
    }

    // -------------------------------
    // SHOW TRAVEL HISTORY
    // -------------------------------
    void showTravelHistory() {
        cout << "\nTravel History (Latest → Oldest):\n";
        travelLog->printStack();
    }

    // -------------------------------
    // DESTRUCTOR
    // -------------------------------
    ~AirportRailSystem() {
        delete travelLog;
    }
};

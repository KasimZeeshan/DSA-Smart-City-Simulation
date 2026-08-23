#pragma once
#include <iostream>
#include <string>

#include "Hashbus.h"        // For BusHashTable + BusRecord
#include "Graph.h"          // For shortest path and stop lookup
#include "LinkedList.h"     // For RouteLinkedList (bus routes)
#include "Types.h"          // For Stop, Coordinate
#include "Stack.h"          // For RouteStack (history)

using namespace std;

/*
===========================================================
   SCHOOL BUS TRACKING SYSTEM  (BONUS FEATURE)
   --------------------------------------------------------
   This module allows the smart city to track the movement
   of any school bus along its registered route.

   Major Features:
   --------------------------------------------------------
   - Track real-time bus location (current stop)
   - Move the bus to the next stop in its route
   - Compute remaining travel distance using Dijkstra
   - Print ETA based on remaining distance
   - Update travel history stack for each movement

   NOTE:
   This module does not modify any existing code.
   It simply uses the data structures already built.
===========================================================
*/

class SchoolBusTracker {
private:
    BusHashTable* busTable;     // pointer to existing bus hash table
    Graph* cityGraph;           // pointer to city graph
    RouteStack* historyStack;   // pointer to shared travel history stack

public:

    // ------------------------------------------------------
    // Constructor: requires existing DS from TransportModule
    // ------------------------------------------------------
    SchoolBusTracker(BusHashTable* bTable, Graph* graph, RouteStack* stack)
    {
        busTable = bTable;
        cityGraph = graph;
        historyStack = stack;
    }

    // ------------------------------------------------------
    // printLocation(busNo)
    // Prints the current stop of a bus
    // ------------------------------------------------------
    void printLocation(const string& busNo)
    {
        BusRecord* record = busTable->search(busNo);
        if (!record)
        {
            cout << "Bus not found." << endl;
            return;
        }

        cout << "Bus " << busNo << " is currently at stop: "
            << record->bus.currentStopId << endl;
    }

    // ------------------------------------------------------
    // moveBusToNext(busNo)
    // Moves the bus to the next stop in its route
    // ------------------------------------------------------
    void moveBusToNext(const string& busNo)
    {
        BusRecord* record = busTable->search(busNo);
        if (!record)
        {
            cout << "Bus not found." << endl;
            return;
        }

        // from route linked list get next stop
        string nextStop = record->route.getNextAfter(record->bus.currentStopId);

        if (nextStop == "")
        {
            cout << "Bus " << busNo << " has reached the end of its route." << endl;
            return;
        }

        // update current stop
        record->bus.currentStopId = nextStop;
        cout << "Bus " << busNo << " moved to next stop: " << nextStop << endl;

        // update travel history stack
        historyStack->push(nextStop);
    }

    // ------------------------------------------------------
    // remainingDistance(busNo, finalStop)
    // Uses Dijkstra to compute remaining distance to destination
    // ------------------------------------------------------
    void remainingDistance(const string& busNo, const string& finalStop)
    {
        BusRecord* record = busTable->search(busNo);
        if (!record)
        {
            cout << "Bus not found." << endl;
            return;
        }

        int pathSize = 0;
        string* path = cityGraph->dijkstra(record->bus.currentStopId, finalStop, pathSize);

        if (!path)
        {
            cout << "No path available from current stop to destination." << endl;
            return;
        }

        cout << "Remaining path: ";
        for (int i = 0; i < pathSize; i++)
        {
            cout << path[i];
            if (i + 1 < pathSize) cout << " -> ";
        }
        cout << endl;

        delete[] path;
    }

    // ------------------------------------------------------
    // trackFullRoute(busNo)
    // Moves the bus along entire route step-by-step
    // ------------------------------------------------------
    void trackFullRoute(const string& busNo)
    {
        BusRecord* record = busTable->search(busNo);
        if (!record)
        {
            cout << "Bus not found." << endl;
            return;
        }

        string current = record->bus.currentStopId;
        cout << "Tracking full route for Bus " << busNo << endl;
        cout << "Starting at: " << current << endl;

        while (true)
        {
            string next = record->route.getNextAfter(current);
            if (next == "")
            {
                cout << "Bus " << busNo << " finished its route." << endl;
                break;
            }

            current = next;
            record->bus.currentStopId = current;
            historyStack->push(current);

            cout << "Bus moved to: " << current << endl;
        }
    }
};


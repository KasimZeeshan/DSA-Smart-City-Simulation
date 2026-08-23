#pragma once
#include <iostream>
#include <string>
#include "../DataProject/Graph.h"
#include "../DataProject/MaxHeap.h"
#include "../DataProject/Types.h"

using namespace std;

/*
===========================================================
   EMERGENCY TRANSPORT ROUTING  (BONUS FEATURE #4)
   --------------------------------------------------------
   Priority routing system for emergency vehicles:
   - Ambulances
   - Fire Trucks
   - Police Vehicles

   Features:
   - Fastest route calculation
   - Nearest hospital/facility lookup
   - Emergency priority handling
   - Real-time status updates

   
===========================================================
*/

class EmergencyRouting {
private:
    Graph* cityGraph;
    MaxHeap emergencyPriority;  // stores emergency levels

    struct EmergencyRequest {
        string type;           // "Ambulance", "Fire", "Police"
        string fromLocation;
        string toLocation;
        int priority;          // 1-10 (10 = critical)
        EmergencyRequest* next;

        EmergencyRequest() {
            priority = 0;
            next = nullptr;
        }
    };

    EmergencyRequest* requestQueueHead;

public:
    // Constructor
    EmergencyRouting(Graph* graph) {
        cityGraph = graph;
        requestQueueHead = nullptr;
    }

    // -------------------------------------------------------
    // requestEmergencyRoute(type, from, to, priority)
    // Creates priority emergency route request
    // -------------------------------------------------------
    void requestEmergencyRoute(const string& type, const string& from,
        const string& to, int priority) {

        cout << "\n+==================================================+" << endl;
        cout << "|       EMERGENCY ROUTE REQUEST RECEIVED           |" << endl;
        cout << "+==================================================+" << endl;

        cout << "\n[!] EMERGENCY TYPE: " << type << endl;
        cout << "[!] FROM: " << from << endl;
        cout << "[!] TO: " << to << endl;
        cout << "[!] PRIORITY LEVEL: " << priority << "/10" << endl;

        // Add to priority queue
        emergencyPriority.insert(priority);

        // Add to request queue
        EmergencyRequest* req = new EmergencyRequest();
        req->type = type;
        req->fromLocation = from;
        req->toLocation = to;
        req->priority = priority;
        req->next = requestQueueHead;
        requestQueueHead = req;

        cout << "\n" << string(50, '-') << endl;
        cout << "[*] Request logged successfully." << endl;
        cout << "[*] Calculating fastest route..." << endl;

        // Calculate route
        calculateEmergencyRoute(from, to, type);
    }

    // -------------------------------------------------------
    // calculateEmergencyRoute(from, to, type)
    // Uses Dijkstra for fastest path
    // -------------------------------------------------------
    void calculateEmergencyRoute(const string& from, const string& to,
        const string& type) {

        int pathSize = 0;
        string* path = cityGraph->dijkstra(from, to, pathSize);

        if (!path) {
            cout << "\n[X] ERROR: No route available!" << endl;
            cout << "[!] Requesting alternative routes..." << endl;
            return;
        }

        cout << "\n[OK] EMERGENCY ROUTE CALCULATED!" << endl;
        cout << "\n" << string(50, '=') << endl;
        cout << "  ROUTE FOR: " << type << endl;
        cout << string(50, '=') << endl;

        cout << "\n[*] Route Path (" << pathSize << " stops):" << endl;
        cout << "    ";
        for (int i = 0; i < pathSize; i++) {
            cout << path[i];
            if (i + 1 < pathSize) cout << " >> ";
        }
        cout << endl;

        // Estimate time (assuming 2 min per stop for emergency)
        int estimatedTime = (pathSize - 1) * 2;
        cout << "\n[*] Estimated Arrival Time: " << estimatedTime << " minutes" << endl;
        cout << "[*] Distance: " << (pathSize - 1) << " stops" << endl;

        cout << "\n" << string(50, '=') << endl;
        cout << "[!] DISPATCH EMERGENCY VEHICLE NOW!" << endl;
        cout << string(50, '=') << endl;

        delete[] path;
    }

    // -------------------------------------------------------
    // findNearestHospital(location)
    // Finds closest hospital for ambulance dispatch
    // -------------------------------------------------------
    void findNearestHospital(const string& emergencyLocation,
        double lat, double lon) {

        cout << "\n+==================================================+" << endl;
        cout << "|     NEAREST HOSPITAL SEARCH (EMERGENCY)          |" << endl;
        cout << "+==================================================+" << endl;

        cout << "\n[!] Emergency Location: " << emergencyLocation << endl;
        cout << "[!] Coordinates: (" << lat << ", " << lon << ")" << endl;

        Coordinate coord;
        coord.lat = lat;
        coord.lon = lon;

        string nearestHospital = cityGraph->nearestNodeByCoord(coord);

        cout << "\n[OK] NEAREST HOSPITAL FOUND!" << endl;
        cout << "[*] Hospital ID: " << nearestHospital << endl;
        cout << "\n[!] Dispatching ambulance from: " << nearestHospital << endl;
        cout << "[!] To emergency location: " << emergencyLocation << endl;

        // Calculate route
        cout << "\n" << string(50, '-') << endl;
        calculateEmergencyRoute(nearestHospital, emergencyLocation, "AMBULANCE");
    }

    // -------------------------------------------------------
    // processHighestPriorityRequest()
    // Handles most critical emergency first
    // -------------------------------------------------------
    void processHighestPriorityRequest() {
        if (emergencyPriority.isEmpty()) {
            cout << "\n[*] No pending emergency requests." << endl;
            return;
        }

        int highestPriority = emergencyPriority.extractMax();

        cout << "\n+==================================================+" << endl;
        cout << "|     PROCESSING HIGHEST PRIORITY REQUEST          |" << endl;
        cout << "+==================================================+" << endl;

        cout << "\n[!] Priority Level: " << highestPriority << "/10" << endl;

        // Find matching request
        EmergencyRequest* curr = requestQueueHead;
        EmergencyRequest* prev = nullptr;

        while (curr != nullptr) {
            if (curr->priority == highestPriority) {
                cout << "[*] Type: " << curr->type << endl;
                cout << "[*] Route: " << curr->fromLocation
                    << " -> " << curr->toLocation << endl;

                calculateEmergencyRoute(curr->fromLocation,
                    curr->toLocation,
                    curr->type);

                // Remove from queue
                if (prev == nullptr) {
                    requestQueueHead = curr->next;
                }
                else {
                    prev->next = curr->next;
                }
                delete curr;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    // -------------------------------------------------------
    // showAllEmergencyRequests()
    // Displays all pending emergency requests
    // -------------------------------------------------------
    void showAllEmergencyRequests() {
        if (!requestQueueHead) {
            cout << "\n[*] No pending emergency requests." << endl;
            return;
        }

        cout << "\n+==================================================+" << endl;
        cout << "|        PENDING EMERGENCY REQUESTS                |" << endl;
        cout << "+==================================================+" << endl;

        EmergencyRequest* curr = requestQueueHead;
        int count = 1;

        while (curr != nullptr) {
            cout << "\n[" << count << "] " << curr->type << endl;
            cout << "    From: " << curr->fromLocation << endl;
            cout << "    To: " << curr->toLocation << endl;
            cout << "    Priority: " << curr->priority << "/10" << endl;
            cout << "    " << string(45, '-') << endl;

            curr = curr->next;
            count++;
        }
    }

    // -------------------------------------------------------
    // emergencyBroadcast(message)
    // Simulates city-wide emergency broadcast
    // -------------------------------------------------------
    void emergencyBroadcast(const string& message) {
        cout << "\n" << string(50, '=') << endl;
        cout << "!!!  EMERGENCY BROADCAST  !!!" << endl;
        cout << string(50, '=') << endl;
        cout << "\n" << message << endl;
        cout << "\n" << string(50, '=') << endl;
    }

    // Destructor
    ~EmergencyRouting() {
        EmergencyRequest* curr = requestQueueHead;
        while (curr != nullptr) {
            EmergencyRequest* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
};
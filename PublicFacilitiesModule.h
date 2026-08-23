#pragma once
#include <iostream>
#include <string>

#include "../DataProject/Types.h"
#include "../DataProject/Graph.h"

using namespace std;

class PublicFacilitiesModule {
    Graph facilityGraph;
    FacilityNode* head;

public:
    PublicFacilitiesModule() {
        head = nullptr;
    };

    // Facility Management
    void addFacility(const string& id, const string& name, const string& type, double lat, double lon) {
        Facility facility;
        facility.id = id;
        facility.name = name;
        facility.type = type;
        facility.coord.lat = lat;
        facility.coord.lon = lon;

        // now just put it in the linked list and place it in start
        FacilityNode* newFacilityNode = new FacilityNode(facility);
        newFacilityNode->next = head;
        head = newFacilityNode;

        // now add to graph for nearest
        Stop FacilityStop;
        FacilityStop.id = id;
        FacilityStop.name = name;
        FacilityStop.coord.lat = lat;
        FacilityStop.coord.lon = lon;

        facilityGraph.addNode(id, FacilityStop);

        cout << "Facility Added " << name << " of type" << type << endl;
    };

    void printAllFacilities() {
        // simple traversal once more
        if (!head) {
            cout << "No Facilities exist." << endl;
            return;
        }

        cout << "Facility:" << endl;
        FacilityNode* temp = head;
        while (temp) {
            cout << "[" << temp->data.name << " | " << temp->data.id << " | " << temp->data.type << "]" << endl;
            temp = temp->next; // keeps it moving
        }
    };
    void nearestFacility(double lat, double lon) {
        Coordinate coord;
        coord.lat = lat;
        coord.lon = lon;

        string nearestID = facilityGraph.nearestNodeByCoord(coord);
        cout << "Nearest Facility by ID is: " << nearestID << endl;
    };
};

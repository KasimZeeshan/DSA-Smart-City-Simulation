#pragma once
#include <iostream>
#include <string>

#include "../DataProject/HashMedicine.h"
#include "../DataProject/Types.h"
#include "../DataProject/MaxHeap.h"
#include "../DataProject/Graph.h"

using namespace std;

class MedicalModule {
    MedicineHashTable medicineTable;
    MaxHeap emergencyHeap;
    Graph medicalGraph;

    HospitalNode* head; // this will store hospital info, it is made linked list so it can grow continously

public:
    MedicalModule() {
        head = nullptr;
    };

    // These are for the Management of the hospital
    void addHospital(const string& id, const string& name, const string& sector, int emergencyBeds, double lat, double lon) {
        Hospital hospital;
        hospital.id = id;
        hospital.name = name;
        hospital.sector = sector;
        hospital.emergencyBeds = emergencyBeds;
        hospital.coord.lat = lat;
        hospital.coord.lon = lon;

        // now just put it in the linked list and place it in start
        HospitalNode* newHospitalNode = new HospitalNode(hospital);
        newHospitalNode->next = head;
        head = newHospitalNode;

        // Now add to emergency heap
        emergencyHeap.insert(hospital.emergencyBeds);

        // now add to graph for nearest
        Stop HospitalStop;
        HospitalStop.id = id;
        HospitalStop.name = name;
        HospitalStop.coord.lat = lat;
        HospitalStop.coord.lon = lon;

        medicalGraph.addNode(id, HospitalStop);

        cout << "Hospital Added " << name << " with emergency beds: " << emergencyBeds << endl;
    };

    void printAllHospitals() {
        // simple linked list traversal
        if (!head) {
            cout << "No hospital Exists yet." << endl;
            return;
        }
        HospitalNode* temp = head;
        cout << "Hospitals: " << endl;
        while (temp) {
            cout << "[" << temp->data.name << " | " << temp->data.id << " | " << temp->data.sector << " | Beds: " << temp->data.emergencyBeds << "]" << endl;
            temp = temp->next; // keeps it moving
        }
    };
    void rankHospitals() {
        if (emergencyHeap.isEmpty()) {
            cout << "No Hospitals Added." << endl;
            return;
        }

        // reset the used flags
        HospitalNode* reset = head;
        while (reset != nullptr) {
            reset->used = false;
            reset = reset->next;
        }

        // first create temporary to preserve, then just keep extracting max
        MaxHeap tempHeap = emergencyHeap;
        cout << "Hospitals ranked by Emergency beds (Most -> Least):" << endl;
        while (!tempHeap.isEmpty()) {
            // first we will extract bed count, compare it with the hospital associated with it, then print them out
            int beds = tempHeap.extractMax();

            HospitalNode* temp = head;
            HospitalNode* match = nullptr;

            // Find a hospital which hasn't already been displayed
            while (temp) {
                if (!temp->used && temp->data.emergencyBeds == beds) {
                    match = temp; // exact match found
                    break;
                }
                temp = temp->next; // move ahead in that case
            }

            if (match) {
                // first set true
                match->used = true;
                cout << match->data.name << " which has Beds: " << beds << "." << endl;
            }
        }
    };
    void nearestHospital(double lat, double lon) {
        Coordinate coordinates;
        coordinates.lat = lat;
        coordinates.lon = lon;

        string nearestID = medicalGraph.nearestNodeByCoord(coordinates);

        cout << "Nearest Hospital by ID is: " << nearestID << endl;
    };

    // Management of Medicinc
    void addMedicineToPharmacy(const Medicine& m, const string& pharmacyName, const string& pharmacySector) {
        medicineTable.insertMedicine(m, pharmacyName, pharmacySector);
        cout << "Added medicine: " << m.name << " in " << pharmacyName << endl;
    }
    void searchMedicine(const string& name) {
        MedicineRecord* searchingMedicine = medicineTable.search(name);
        if (!searchingMedicine) {
            // in case nothing found, nullptr
            cout << "Medicine Not Found." << endl;
            return;
        }
        // and if found?
        cout << "Found: " << searchingMedicine->data.name << endl;
        cout << "Formula: " << searchingMedicine->data.formula << endl;
        cout << "Price: " << searchingMedicine->data.price << endl;
        cout << "Available in pharmacies:" << endl;
        MedicineLocation* loc = searchingMedicine->data.locations;
        while (loc != nullptr) {
            cout << " - " << loc->pharmacyName
                << " (Sector " << loc->pharmacySector << ")" << endl;
            loc = loc->next;
        }
    };
};
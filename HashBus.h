#pragma once
using namespace std;
#include <string>
#include <fstream>
#include <iostream>
#include "Types.h"
#include "LinkedList.h"

// ============================================================
// HashBus.h (FULL IMPLEMENTATION — SEPARATE CHAINING)
// ------------------------------------------------------------
// Required by project:
//   Fast lookup of buses by Bus Number (B101, B305, ...)
//
// What is stored per bus?
//   - BusNo
//   - Company
//   - CurrentStopId
//   - Route (linked list of stops)
//
// Hash function:
//   Simple polynomial rolling hash on bus number string.
// ============================================================

// ===================== BUS RECORD ============================
struct BusRecord {
    Bus bus;                   // busNo, company, currentStop
    RouteLinkedList route;     // full route
    BusRecord* next;           // next record in chain

    BusRecord(const Bus& b) {
        bus = b;
        next = nullptr;
    }
};


// ===================== HASH TABLE FOR BUSES ==================
class BusHashTable {
private:
    static const int TABLE_SIZE = 101;   // prime size for better hashing
    BusRecord* table[TABLE_SIZE];        // array of linked lists

    // ------------------------------------------------------------
    // hashFunction(key)
    // Polynomial rolling hash (simple + safe)
    // ------------------------------------------------------------
    int hashFunction(const string& key) {
        long long hash = 0;
        long long p = 31;
        long long pow = 1;
        const long long MOD = TABLE_SIZE;

        for (int i = 0; i < key.length(); i++) {
            hash = (hash + ((long long)key[i] * pow) % MOD) % MOD;
            pow = (pow * p) % MOD;
        }

        return (int)hash;
    }

public:
    // ------------------------------------------------------------
    // Constructor: initialize table with nulls
    // ------------------------------------------------------------
    BusHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }


    // ------------------------------------------------------------
    // insertBus(bus, routeList)
    // Inserts new bus and assigns its route.
    // ------------------------------------------------------------
    void insertBus(const Bus& b, const RouteLinkedList& route) {
        cout << "  [insertBus] Starting for bus: " << b.busNo << endl;

        int idx = hashFunction(b.busNo);
        cout << "  [insertBus] Hash index: " << idx << endl;

        // Create new record
        BusRecord* rec = new BusRecord(b);
        cout << "  [insertBus] BusRecord created" << endl;

        // SAFE ROUTE COPYING with NULL checks
        if (route.head == nullptr) {
            cout << "  [insertBus] WARNING: Route is empty!" << endl;
        }
        else {
            cout << "  [insertBus] Starting route copy..." << endl;

            RouteNode* curr = route.head;
            int stopNum = 0;

            while (curr != nullptr && stopNum < 50) {
                cout << "  [insertBus]   Stop " << stopNum << ": " << curr->stopId << endl;

                // Create a COPY of the string before adding
                string stopCopy = curr->stopId;
                rec->route.addStop(stopCopy);

                curr = curr->next;
                stopNum++;
            }

            cout << "  [insertBus] Route copy complete. Stops: " << stopNum << endl;
        }

        // Insert into hash table
        cout << "  [insertBus] Inserting into hash table..." << endl;
        rec->next = table[idx];
        table[idx] = rec;

        cout << "  [insertBus] SUCCESS! Bus " << b.busNo << " inserted." << endl;
    }


    // ------------------------------------------------------------
    // search(busNo)
    // Returns pointer to BusRecord or nullptr.
    // ------------------------------------------------------------
    BusRecord* search(const string& busNo) {
        int idx = hashFunction(busNo);
        BusRecord* curr = table[idx];

        while (curr != nullptr) {
            if (curr->bus.busNo == busNo) return curr;
            curr = curr->next;
        }
        return nullptr;
    }


    // ------------------------------------------------------------
    // remove(busNo)
    // Deletes record from chain.
    // ------------------------------------------------------------
    void remove(const string& busNo) {
        int idx = hashFunction(busNo);
        BusRecord* curr = table[idx];
        BusRecord* prev = nullptr;

        while (curr != nullptr) {
            if (curr->bus.busNo == busNo) {
                if (prev == nullptr) table[idx] = curr->next;
                else prev->next = curr->next;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }


    // ------------------------------------------------------------
    // printAll()
    // For debugging
    // ------------------------------------------------------------
    void printAll() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            BusRecord* curr = table[i];
            if (curr != nullptr) {
                cout << "Index " << i << ":\n";
            }
            while (curr != nullptr) {
                cout << "  Bus " << curr->bus.busNo
                    << " | Company: " << curr->bus.company
                    << " | Current Stop: " << curr->bus.currentStopId << "\n";
                cout << "  Route: ";
                curr->route.printRoute();
                curr = curr->next;
            }
        }
    }
};
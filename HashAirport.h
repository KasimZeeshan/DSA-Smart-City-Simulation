#pragma once
#include <iostream>
#include <string>
#include "Types.h"

using namespace std;

/*
===============================================================
 HashAirport.h
 --------------------------------------------------------------
 Separate chaining hash table for storing:
     . Airports
     . Railway Stations
 Each record stores:
     - code  (ex: ISB, RAW)
     - name  (airport or station name)
     - coordinates (lat, lon)

 Used in AirportRailSystem to:
     . register airport/station
     . retrieve details fast
     . integrate with graph routes
===============================================================
*/

// ===================== AIRPORT RECORD =======================

struct AirportRecord {
    Airport airport;          // complete airport info
    AirportRecord* next;      // linked list chaining

    AirportRecord() {
        next = nullptr;
    }
};

// ===================== HASH TABLE ===========================

class AirportHashTable {
private:
    static const int TABLE_SIZE = 211;     // prime size for fewer collisions
    AirportRecord* table[TABLE_SIZE];

    // ------------------------------------------------------------
    // hashFunction(key)
    // ------------------------------------------------------------
    int hashFunction(const string& key) {
        long long hash = 0;
        long long p = 31;
        long long pow = 1;
        const long long MOD = TABLE_SIZE;

        for (int i = 0; i < key.length(); i++) {
            // Handle any character (uppercase, lowercase, numbers)
            long long val = (long long)key[i];
            hash = (hash + (val * pow) % MOD) % MOD;
            pow = (pow * p) % MOD;
        }

        return (int)hash;
    }

public:

    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    AirportHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = nullptr;
    }

    // ------------------------------------------------------------
    // insertAirport(a)
    // Inserts into hash table using chaining
    // ------------------------------------------------------------
    void insertAirport(Airport a) {
        int idx = hashFunction(a.code);
        AirportRecord* curr = table[idx];

        // Check duplicates
        while (curr != nullptr) {
            if (curr->airport.code == a.code) {
                // Already exists → update fields
                curr->airport.name = a.name;
                curr->airport.coord = a.coord;
                return;
            }
            curr = curr->next;
        }

        // New record
        AirportRecord* rec = new AirportRecord();
        rec->airport = a;

        // Insert at head of chain
        rec->next = table[idx];
        table[idx] = rec;
    }

    // ------------------------------------------------------------
    // search(code)
    // Returns pointer to record OR nullptr
    // ------------------------------------------------------------
    AirportRecord* search(const string& code) {
        int idx = hashFunction(code);
        AirportRecord* curr = table[idx];

        while (curr != nullptr) {
            if (curr->airport.code == code)
                return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // ------------------------------------------------------------
    // remove(code)
    // ------------------------------------------------------------
    void remove(const string& code) {
        int idx = hashFunction(code);

        AirportRecord* curr = table[idx];
        AirportRecord* prev = nullptr;

        while (curr != nullptr) {
            if (curr->airport.code == code) {
                if (prev == nullptr)
                    table[idx] = curr->next;
                else
                    prev->next = curr->next;

                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    // ------------------------------------------------------------
    // printAll()
    // ------------------------------------------------------------
    void printAll() {
        cout << "\n===== AIRPORT / RAILWAY HASH TABLE =====\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            AirportRecord* curr = table[i];

            if (curr)
                cout << "Index " << i << ":\n";

            while (curr != nullptr) {
                cout << "  Code: " << curr->airport.code
                    << " | Name: " << curr->airport.name
                    << " | Coordinates: ("
                    << curr->airport.coord.lat
                    << ", " << curr->airport.coord.lon << ")\n";
                curr = curr->next;
            }
        }
    }
};

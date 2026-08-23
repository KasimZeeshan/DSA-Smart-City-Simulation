#pragma once
using namespace std;
#include <string>
#include <fstream>
#include <iostream>
#include "Types.h"
#include "FamilyTree.h"

// ============================================================
// HashCitizen.h (FULL IMPLEMENTATION using  SEPARATE CHAINING)
// ------------------------------------------------------------
// Used for:
//   . Fast lookup of citizens using CNIC (key)
//   . Required by population module
//   . uses separate chaining (linked lists)
// ============================================================

// ===================== CITIZEN RECORD =======================
struct CitizenRecord {
    Citizen data;          // Citizen info (Name, Age, CNIC, etc.)
    CitizenRecord* next;   // Next in chain

    CitizenRecord() {
        next = nullptr;
    }
};

// ===================== CITIZEN HASH TABLE ===================
class CitizenHashTable {
private:
    static const int TABLE_SIZE = 251;   // prime helps reduce collisions
    CitizenRecord* table[TABLE_SIZE];

    // ------------------------------------------------------------
    // hashFunction(key)
    // ------------------------------------------------------------
    int hashFunction(const string& key) {
        long long hash = 0;
        long long p = 37;
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
    // Constructor: initialize with null chains
    // ------------------------------------------------------------
    CitizenHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    // ------------------------------------------------------------
    // insertCitizen(c)
    // Adds new citizen to hash table
    // ------------------------------------------------------------
    void insertCitizen(const Citizen& c) {
        int idx = hashFunction(c.cnic);

        CitizenRecord* rec = new CitizenRecord();
        rec->data = c;
        rec->next = table[idx];
        table[idx] = rec;
    }

    // ------------------------------------------------------------
    // search(cnic)
    // Returns pointer to CitizenRecord OR nullptr
    // ------------------------------------------------------------
    CitizenRecord* search(const string& cnic) {
        int idx = hashFunction(cnic);
        CitizenRecord* curr = table[idx];

        while (curr != nullptr) {
            if (curr->data.cnic == cnic)
                return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // ------------------------------------------------------------
    // remove(cnic)
    // Deletes citizen from chain
    // ------------------------------------------------------------
    void remove(const string& cnic) {
        int idx = hashFunction(cnic);
        CitizenRecord* curr = table[idx];
        CitizenRecord* prev = nullptr;

        while (curr != nullptr) {
            if (curr->data.cnic == cnic) {
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
    // printAllCitizens()
    // ------------------------------------------------------------
    void printAllCitizens() {
        cout << "\n===== CITIZEN HASH TABLE =====\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            CitizenRecord* curr = table[i];
            if (curr != nullptr) cout << "Index " << i << ":\n";

            while (curr != nullptr) {
                cout << "  CNIC: " << curr->data.cnic
                    << " | Name: " << curr->data.name
                    << " | Age: " << curr->data.age
                    << " | Sector: " << curr->data.sector
                    << " | House: " << curr->data.houseNo
                    << "\n";
                curr = curr->next;
            }
        }
    }
};
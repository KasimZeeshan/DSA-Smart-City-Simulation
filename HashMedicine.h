#pragma once
using namespace std;
#include <string>
#include <fstream>
#include <iostream>
#include "Types.h"

// ============================================================
// HashMedicine.h with seperate chainging (linked list)
// ------------------------------------------------------------
// Required For Medical Sector:
//    Fast lookup of medicine by name
//    Store formula + price
//    Must use separate chaining (linked list buckets)
// ============================================================

// ===================== MEDICINE RECORD =======================
struct MedicineRecord {
    Medicine data;            // name, formula, price, pharmacy info
    MedicineRecord* next;

    MedicineRecord() {
        next = nullptr;
    }
};

// ===================== HASH TABLE FOR MEDICINES =============
class MedicineHashTable {
private:
    static const int TABLE_SIZE = 211;   // prime
    MedicineRecord* table[TABLE_SIZE];

    // ------------------------------------------------------------
    // hashFunction(key)
    // Polynomial rolling hash on medicine name
    // ------------------------------------------------------------
    int hashFunction(const string& key) {
        long long hash = 0;
        long long p = 31;
        long long pow = 1;

        for (int i = 0; i < (int)key.length(); i++) {
            unsigned char ch = (unsigned char)key[i]; // safe for any character
            hash = (hash + (long long)ch * pow) % TABLE_SIZE;
            pow = (pow * p) % TABLE_SIZE;
        }
        return (int)(hash % TABLE_SIZE);
    }


public:
    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    MedicineHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    // ------------------------------------------------------------
    // insertMedicine(m)
    // ------------------------------------------------------------
    void insertMedicine(Medicine m, const string& pharmacyName, const string& pharmacySector) {
        int idx = hashFunction(m.name);

        MedicineRecord* curr = table[idx];

        while (curr != nullptr) {
            if (curr->data.name == m.name) {
                // keep adding new location
                MedicineLocation* loc = new MedicineLocation(pharmacyName, pharmacySector);
                loc->next = curr->data.locations;
                curr->data.locations = loc;
                return;
            }
            curr = curr->next;
        }

        // for the new medicine
        MedicineRecord* rec = new MedicineRecord();
        rec->data = m;
        rec->data.locations = new MedicineLocation(pharmacyName, pharmacySector);
        rec->next = table[idx];
        table[idx] = rec;
    }

    // ------------------------------------------------------------
    // search(name)
    // Returns pointer to MedicineRecord OR nullptr
    // ------------------------------------------------------------
    MedicineRecord* search(const string& name) {
        int idx = hashFunction(name);
        MedicineRecord* curr = table[idx];

        while (curr != nullptr) {
            if (curr->data.name == name)
                return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // ------------------------------------------------------------
    // remove(name)
    // ------------------------------------------------------------
    void remove(const string& name) {
        int idx = hashFunction(name);
        MedicineRecord* curr = table[idx];
        MedicineRecord* prev = nullptr;

        while (curr != nullptr) {
            if (curr->data.name == name) {
                if (prev == nullptr) table[idx] = curr->next;
                else prev->next = curr->next;

                // free locations list
                MedicineLocation* loc = curr->data.locations;
                while (loc) {
                    MedicineLocation* tmp = loc;
                    loc = loc->next;
                    delete tmp;
                }

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
        cout << "\n===== MEDICINE HASH TABLE =====\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            MedicineRecord* curr = table[i];
            if (curr != nullptr) {
                cout << "Index " << i << ":\n";
            }
            while (curr != nullptr) {
                // Basic medicine information
                cout << "  Name: " << curr->data.name
                    << " | Formula: " << curr->data.formula
                    << " | Price: " << curr->data.price << endl;
                // for all locations
                cout << "  Available at:" << endl;
                MedicineLocation* loc = curr->data.locations;
                while (loc != nullptr) {
                    cout << "     - " << loc->pharmacyName
                        << " (Sector " << loc->pharmacySector << ")\n";
                    loc = loc->next;
                }
                cout << endl;
                curr = curr->next;
            }
        }
    }

    ~MedicineHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            MedicineRecord* curr = table[i];
            while (curr) {
                // free nested data (locations)
                MedicineLocation* loc = curr->data.locations;
                while (loc) {
                    MedicineLocation* tmp = loc;
                    loc = loc->next;
                    delete tmp;
                }
                MedicineRecord* tmpRec = curr;
                curr = curr->next;
                delete tmpRec;
            }
            table[i] = nullptr;
        }
    }

};
#pragma once
using namespace std;
#include <iostream>
#include <string>
#include "Types.h"


// ============================================================
// HashProduct.h with seperate chainging (linked list buckets)
// ------------------------------------------------------------
// Required For Product Sector:
//    Fast lookup of product by name
//    Store company + category + price
//    Must use separate chaining (linked list buckets)
// ============================================================

// ===================== PRODUCT RECORD =======================
struct ProductRecord {
    Product data;
    ProductRecord* next;

    ProductRecord() {
        next = nullptr;
    }
};

// ===================== HASH TABLE FOR Products =============

class ProductHashTable {
private:
    static const int TABLE_SIZE = 263;  // bigger prime, lower collisions
    ProductRecord* table[TABLE_SIZE];

    // ------------------------------------------------------------
    // hashFunction(key)
    // ------------------------------------------------------------
    int hashFunction(const string& key) {
        long long hash = 0;
        long long p = 31;
        long long pow = 1;

        for (int i = 0; i < (int)key.length(); i++) {
            unsigned char ch = (unsigned char)key[i]; // supports A-Z, a-z, 0-9, space, symbols
            hash = (hash + (long long)ch * pow) % TABLE_SIZE;
            pow = (pow * p) % TABLE_SIZE;
        }

        return (int)hash;
    }


public:
    // ------------------------------------------------------------
    // Constructor
    // ------------------------------------------------------------
    ProductHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = nullptr;
    }

    // ------------------------------------------------------------
    // insertProduct(p)
    // ------------------------------------------------------------
    void insertProduct(Product p, const string& mallName, const string& mallSector) {
        int idx = hashFunction(p.name);

        ProductRecord* curr = table[idx];
        while (curr != nullptr) {
            if (curr->data.name == p.name) {
                // add in additional mall locations
                ProductLocation* loc = new ProductLocation(mallName, mallSector);
                loc->next = curr->data.locations;
                curr->data.locations = loc;
                return;
            }
            curr = curr->next;
        }

        // for new products
        ProductRecord* rec = new ProductRecord();
        rec->data = p;
        rec->data.locations = new ProductLocation(mallName, mallSector);
        rec->next = table[idx];
        table[idx] = rec;
    }

    // ------------------------------------------------------------
    // search(name)
    // Returns pointer to ProductRecord OR nullptr
    // ------------------------------------------------------------
    ProductRecord* search(const string& name) {
        int idx = hashFunction(name);
        ProductRecord* curr = table[idx];

        while (curr) {
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
        ProductRecord* curr = table[idx];
        ProductRecord* prev = nullptr;

        while (curr != nullptr) {
            if (curr->data.name == name) {
                if (prev == nullptr)
                    table[idx] = curr->next;
                else
                    prev->next = curr->next;

                // free linked list of locations
                ProductLocation* loc = curr->data.locations;
                while (loc) {
                    ProductLocation* tmp = loc;
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
        cout << "\n===== PRODUCT HASH TABLE =====\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            ProductRecord* curr = table[i];
            if (curr != nullptr)
                cout << "Index " << i << ":\n";

            while (curr != nullptr) {
                cout << "  Name: " << curr->data.name
                    << " | Company: " << curr->data.company
                    << " | Brand: " << curr->data.brand
                    << " | Category: " << curr->data.category
                    << " | Price: " << curr->data.price << endl;
                cout << "  Available at:" << endl;
                ProductLocation* loc = curr->data.locations;
                while (loc != nullptr) {
                    cout << "     - " << loc->mallName
                        << " (" << loc->mallSector << ")" << endl;
                    loc = loc->next;
                }
                curr = curr->next;
            }
        }
    }

    // ------------------------------------------------------------
    // destructor
    // ------------------------------------------------------------
    ~ProductHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            ProductRecord* curr = table[i];
            while (curr) {
                // free locations
                ProductLocation* loc = curr->data.locations;
                while (loc) {
                    ProductLocation* tmp = loc;
                    loc = loc->next;
                    delete tmp;
                }

                ProductRecord* tmpRec = curr;
                curr = curr->next;
                delete tmpRec;
            }
            table[i] = nullptr;
        }
    }

};

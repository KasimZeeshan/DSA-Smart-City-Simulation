#pragma once
#include <iostream>
#include <string>

#include "../DataProject/HashProduct.h"
#include "../DataProject/Graph.h"
#include "../DataProject/Types.h"

using namespace std;

class MallModule {
    Graph mallGraph;
    ProductHashTable productTable; // just replace some of the values, working is similiar so reused
    MallNode* head;

public:
    MallModule() {
        head = nullptr;
    }

    // Mall Management
    void addMall(const string& id, const string& name, const string& sector, double lat, double lon) {
        Mall mall;
        mall.id = id;
        mall.name = name;
        mall.sector = sector;
        mall.coord.lat = lat;
        mall.coord.lon = lon;

        // now just put it in the linked list and place it in start
        MallNode* newMallNode = new MallNode(mall);
        newMallNode->next = head;
        head = newMallNode;

        // now add to graph for nearest
        Stop MallStop;
        MallStop.id = id;
        MallStop.name = name;
        MallStop.coord.lat = lat;
        MallStop.coord.lon = lon;

        mallGraph.addNode(id, MallStop);

        cout << "Mall Added " << name << " in sector " << sector << endl;
    };

    void printAllMalls() {
        // simple linked list traversal
        if (!head) {
            cout << "No Malls exist." << endl;
            return;
        }

        cout << "Mall:" << endl;
        MallNode* temp = head;
        while (temp) {
            cout << "[" << temp->data.name << " | " << temp->data.id << " | " << temp->data.sector << "]" << endl;
            temp = temp->next; // keeps it moving
        }
    };
    void nearestMall(double lat, double lon) {
        Coordinate coord;
        coord.lat = lat;
        coord.lon = lon;

        string nearestID = mallGraph.nearestNodeByCoord(coord);
        cout << "Nearest mall by ID is: " << nearestID << endl;
    };

    // Product Management
    void addProductToMall(const Product& p, const string& mallName, const string& mallSector)
    {
        productTable.insertProduct(p, mallName, mallSector);
        cout << "Product " << p.name << " added for " << mallName << "." << endl;
    }
    void searchProduct(const string& name) {
        ProductRecord* record = productTable.search(name);

        // if not found
        if (!record) {
            cout << "Product not Found." << endl;
            return;
        }
        // if found?
        cout << "Found: " << record->data.name << endl;
        cout << "Brand: " << record->data.brand << endl;
        cout << "Company: " << record->data.company << endl;
        cout << "Category: " << record->data.category << endl;
        cout << "Price: " << record->data.price << endl;
        cout << "available at: " << endl;
        ProductLocation* loc = record->data.locations;
        while (loc) { // runs loop (using linked list) for all malls the product is available in
            cout << " - " << loc->mallName << " (Sector " << loc->mallSector << ")" << endl;
            loc = loc->next;
        }

    };
};
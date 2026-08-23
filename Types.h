// ===================== Types.h =====================
#pragma once
#include<iostream>
using namespace std;
#include <string>
#include <fstream>


// Simple coordinate structure
struct Coordinate {
    double lat;
    double lon;
};

// Bus Stop information
struct Stop {
    string id;
    string name;
    Coordinate coord;
};

// Bus information (route stored manually via linked list)
struct Bus {
    string busNo;
    string company;
    string currentStopId;
};

// Person for population sector
struct Person {
    string cnic;
    string name;
    int age;
    string sector;
    string street;
    string house;
    string occupation;
};


// ===================== FAMILY & POPULATION STRUCTS =====================

// For simplicity we use raw arrays.
const int MAX_MEMBERS = 10;   // maximum individuals in a family

struct Individual {
    string name;
    int age;
    string occupation;
    string gender;     // optional, used for gender ratio report
};

struct Family {
    string name;              // Optional: family head name
    int memberCount;          // number of people in this family
    Individual members[MAX_MEMBERS];   // raw array of individuals

    Family() {
        name = "";
        memberCount = 0;
    }
};
// ===================== MEDICINE STRUCTURE =====================
struct MedicineLocation {
    string pharmacyName;
    string pharmacySector;
    MedicineLocation* next;

    MedicineLocation(string name, string sector) {
        pharmacyName = name;
        pharmacySector = sector;
        next = nullptr;
    }
};

struct Medicine {
    string name;    // name
    string formula; // formula
    int price;      // price

    MedicineLocation* locations; // linked list for locations

    Medicine() {
        locations = nullptr;
    }
};

struct Hospital {
    string id;
    string name;
    string sector;
    int emergencyBeds;
    Coordinate coord;
};

struct HospitalNode {
    Hospital data;
    HospitalNode* next;
    bool used; // to help with ranking

    HospitalNode(const Hospital& h) {
        data = h;
        next = nullptr;
        used = false; // initally
    }
};


struct Citizen {
    string cnic;        // unique identifier
    string name;
    int age;
    string sector;
    int houseNo;
};

// ======================= MALL STRUCTURE =========================
struct ProductLocation {
    string mallName;
    string mallSector;
    ProductLocation* next;

    ProductLocation(string name, string sector) {
        mallName = name;
        mallSector = sector;
        next = nullptr;
    }
};
struct Product {
    string name;
    string company;
    string brand;
    string category;
    int price;

    ProductLocation* locations;   // in case for multiple Malls

    Product() {
        locations = nullptr;
    }
};

struct Mall {
    string id;
    string name;
    string sector;
    Coordinate coord;
};

struct MallNode {
    Mall data;
    MallNode* next;

    MallNode(const Mall& m) {
        data = m;
        next = nullptr;
    }
};

// ====================== FACILITY STRUCTURE =========================
struct Facility {
    string id;
    string name;
    string type;
    Coordinate coord;
};

struct FacilityNode {
    Facility data;
    FacilityNode* next;

    FacilityNode(const Facility& f) {
        data = f;
        next = nullptr;
    }
};

// ======================= AIRPORT & RAILWAY STRUCTURES =========================

// Basic structure for Airport
struct Airport {
    string code;         // e.g., "ISB"
    string name;         // e.g., "Islamabad International Airport"
    Coordinate coord;    // used for nearest-airport lookup via graph

    Airport() {
        code = "";
        name = "";
        coord.lat = 0;
        coord.lon = 0;
    }
};

// Basic structure for Railway Station
struct RailwayStation {
    string code;         // e.g., "RAW" for Rawalpindi Station
    string name;
    Coordinate coord;

    RailwayStation() {
        code = "";
        name = "";
        coord.lat = 0;
        coord.lon = 0;
    }
};

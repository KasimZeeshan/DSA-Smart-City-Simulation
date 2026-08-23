#pragma once
#include <iostream>
#include <string>

#include "../DataProject/SectorTree.h"
#include "../DataProject/HashCitizen.h"
#include "../DataProject/FamilyTree.h"
#include "../DataProject/Types.h"

using namespace std;

class PopulationModule {
    SectorTree sectorTree;
    CitizenHashTable citizenTable;
    FamilyTree familyTree;

public:
    PopulationModule() {};

    SectorTree& getSectorTree() {
        return sectorTree;
    }


    // location management
    void addSector(const string& sectorName) {
        sectorTree.addSector(sectorName);
        cout << "Sector " << sectorName << " Added." << endl;
    };
    void addStreet(const string& sector, const string& street) {
        sectorTree.addStreet(sector, street);
        cout << "Street " << street << " Added in Sector " << sector << "." << endl;
    };
    void addHouse(const string& sector, const string& street, const string& houseNo, const Family& family) {
        sectorTree.addHouse(sector, street, houseNo, family);
        cout << "House No: " << houseNo << " Added on Street " << street << " in sector " << sector << "." << endl;
        cout << "It is occupied by the " << family.name << " family." << endl;
    };

    void printAllSectors() {
        sectorTree.printAll();
    };

    // citizen management
    void addCitizen(const Citizen& c) {
        citizenTable.insertCitizen(c);
        cout << "Citizen " << c.name << " with CNIC " << c.cnic << " has been added to the table." << endl;
    };
    void searchCitizen(const string& cnic) {
        CitizenRecord* record = citizenTable.search(cnic);
        // if not found
        if (!record) {
            cout << "The citizen could not be found." << endl;
            return;
        }
        // if found?
        cout << "Citizen has been Found!" << endl;
        cout << "Citizen Name: " << record->data.name << endl;
        cout << "Citizen CNIC: " << record->data.cnic << endl;
        cout << "Citizen Age: " << record->data.age << endl;
        cout << "Citizen House: " << record->data.houseNo << endl;
        cout << "Citizen Sector: " << record->data.sector << endl;
    };

    // required reports
    void genderRatio() {
        int NoOfMales = 0;
        int NoOfFemales = 0; // initialize both by 0

        SectorNode* city = sectorTree.getRoot();
        SectorNode* sector = city->firstChild;

        while (sector) {
            SectorNode* street = sector->firstChild;
            while (street) {
                SectorNode* house = street->firstChild;
                while (house) {
                    if (house->isHouse) {
                        for (int i = 0; i < house->family.memberCount; i++) {
                            if (house->family.members[i].gender == "F") {
                                NoOfFemales++;
                            }
                            else {
                                NoOfMales++;
                            }
                        }
                    }
                    house = house->nextSibling;
                }
                street = street->nextSibling;
            }
            sector = sector->nextSibling;
        }

        cout << "Total Number of Males: " << NoOfMales << endl;
        cout << "Total Number of Females: " << NoOfFemales << endl;

        if (NoOfMales == 0 && NoOfFemales == 0) {
            cout << "No population data available." << endl;
            return;
        }
        if (NoOfMales == 0) {
            cout << "Ratio = 0 : 1 (all females)" << endl;
            return;
        }
        if (NoOfFemales == 0) {
            cout << "Ratio = 1 : 0 (all males)" << endl;
            return;
        }

        float ratio;

        if (NoOfMales >= NoOfFemales) {
            ratio = (float)NoOfMales / NoOfFemales;   // M/F
            cout << "Male : Female = " << ratio << " : 1" << endl;
        }
        else {
            ratio = (float)NoOfFemales / NoOfMales;   // F/M
            cout << "Female : Male = " << ratio << " : 1" << endl;
        }
        return;
    };
    void occupationSummary() {
        SectorNode* city = sectorTree.getRoot();
        SectorNode* sector = city->firstChild;

        cout << "Occupation Summary: " << endl;

        while (sector) {
            cout << "In Sector " << sector->name << endl;
            SectorNode* street = sector->firstChild;
            while (street) {
                SectorNode* house = street->firstChild;
                while (house) {
                    if (house->isHouse) {
                        for (int i = 0; i < house->family.memberCount; i++) {
                            if (house->family.members[i].age >= 18) { // we do not consider the occupancy of children
                                cout << "Member" << i << " -> " << house->family.members[i].occupation << endl;
                            }
                        }
                    }
                    cout << endl << "----------------" << endl;
                    house = house->nextSibling;
                }
                street = street->nextSibling;
            }
            sector = sector->nextSibling;
        }
    };
    void populationDensity() {
        SectorNode* city = sectorTree.getRoot();
        SectorNode* sector = city->firstChild;

        cout << "Population Count: " << endl;

        while (sector) {
            int TotalPopulation = 0;
            SectorNode* street = sector->firstChild;
            while (street) {
                SectorNode* house = street->firstChild;
                while (house) {
                    if (house->isHouse) {
                        /*for (int i = 0; i < house->family.memberCount; i++) {
                            TotalPopulation++;
                        }*/
                        TotalPopulation += house->family.memberCount;
                    }
                    house = house->nextSibling;
                }
                street = street->nextSibling;
            }
            // For every Sector
            cout << "In Sector " << sector->name << " Total Population is: " << TotalPopulation << " residents" << endl;
            cout << endl << "----------------" << endl;
            sector = sector->nextSibling;
        }
    };
    void ageDistribution() {
        int Seniors = 0;
        int Adults = 0; // initialize all by 0
        int Children = 0;

        SectorNode* city = sectorTree.getRoot();
        SectorNode* sector = city->firstChild;

        while (sector) {
            SectorNode* street = sector->firstChild;
            while (street) {
                SectorNode* house = street->firstChild;
                while (house) {
                    if (house->isHouse) {
                        for (int i = 0; i < house->family.memberCount; i++) {
                            int age = house->family.members[i].age;
                            if (age < 18) {
                                Children++;
                            }
                            else if (age < 60) {
                                Adults++;
                            }
                            else {
                                Seniors++;
                            }
                        }
                    }
                    house = house->nextSibling;
                }
                street = street->nextSibling;
            }
            sector = sector->nextSibling;
        }

        cout << "Age Distribution Summary: " << endl;
        cout << "Number of Children: " << Children << endl;
        cout << "Number of Adults: " << Adults << endl;
        cout << "Number of Seniors: " << Seniors << endl;
    };
    
};

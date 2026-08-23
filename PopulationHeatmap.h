
#pragma once
#include <iostream>
#include <string>
#include "../DataProject/SectorTree.h"
#include "../DataProject/Types.h"

using namespace std;

/*
===========================================================
   POPULATION HEATMAP LOGIC  (BONUS FEATURE #5)
   --------------------------------------------------------
   Analyzes population distribution across Islamabad sectors
   and generates visual heatmap representation.

   Features:
   - Population density calculation per sector
   - Visual ASCII heatmap display
   - Most/least populated sector identification
   - Occupancy rate analysis
   - Growth trend predictions

===========================================================
*/

class PopulationHeatmap {
private:
    SectorTree* sectorTree;

    struct SectorStats {
        string sectorName;
        int totalPopulation;
        int totalHouses;
        double density;         // total population in sector
        SectorStats* next;

        SectorStats() {
            totalPopulation = 0;
            totalHouses = 0;
            density = 0.0;
            next = nullptr;
        }
    };

    SectorStats* statsHead;

    // -------------------------------------------------------
    // calculateDensity(population, houses)
    // Now just returns total population as density metric
    // -------------------------------------------------------
    double calculateDensity(int population, int houses) {
        return (double)population;  // Density = total population per sector
    }

    // -------------------------------------------------------
    // getDensityBar(density)
    // Returns ASCII bar representation of density
    // -------------------------------------------------------
    string getDensityBar(double density, double maxDensity) {
        if (maxDensity == 0) return "";

        int barLength = (int)((density / maxDensity) * 30);
        if (barLength > 30) barLength = 30;

        string bar = "";
        for (int i = 0; i < barLength; i++) {
            bar += "#";
        }
        return bar;
    }

    // -------------------------------------------------------
    // getDensityLevel(density)
    // Returns text description of density level
    // -------------------------------------------------------
    string getDensityLevel(double density) {
        if (density == 0) return "EMPTY";
        else if (density < 3) return "LOW";
        else if (density < 6) return "MEDIUM";
        else if (density < 10) return "HIGH";
        else return "VERY HIGH";
    }

public:
    // Constructor
    PopulationHeatmap(SectorTree* tree) {
        sectorTree = tree;
        statsHead = nullptr;
    }

    // -------------------------------------------------------
    // analyzeSectors()
    // Scans entire city and calculates statistics
    // -------------------------------------------------------
    void analyzeSectors() {
        // Clear previous stats
        while (statsHead != nullptr) {
            SectorStats* temp = statsHead;
            statsHead = statsHead->next;
            delete temp;
        }

        cout << "\n+==================================================+" << endl;
        cout << "|        ANALYZING POPULATION DATA...              |" << endl;
        cout << "+==================================================+" << endl;

        SectorNode* city = sectorTree->getRoot();
        SectorNode* sector = city->firstChild;

        int totalSectors = 0;

        while (sector != nullptr) {
            totalSectors++;

            // Create stats for this sector
            SectorStats* stats = new SectorStats();
            stats->sectorName = sector->name;

            // Count population and houses
            SectorNode* street = sector->firstChild;
            while (street != nullptr) {
                SectorNode* house = street->firstChild;
                while (house != nullptr) {
                    if (house->isHouse) {
                        stats->totalHouses++;
                        stats->totalPopulation += house->family.memberCount;
                    }
                    house = house->nextSibling;
                }
                street = street->nextSibling;
            }

            // Calculate density (now just total population)
            stats->density = calculateDensity(stats->totalPopulation,
                stats->totalHouses);

            // Add to stats list
            stats->next = statsHead;
            statsHead = stats;

            sector = sector->nextSibling;
        }

        cout << "\n[OK] Analysis Complete!" << endl;
        cout << "[*] Sectors Analyzed: " << totalSectors << endl;
    }

    // -------------------------------------------------------
    // generateHeatmap()
    // Displays visual ASCII heatmap
    // -------------------------------------------------------
    void generateHeatmap() {
        if (!statsHead) {
            cout << "\n[!] No data available. Run analyzeSectors() first." << endl;
            return;
        }

        cout << "\n+==================================================+" << endl;
        cout << "|           POPULATION DENSITY HEATMAP             |" << endl;
        cout << "+==================================================+" << endl;

        // Find max density for scaling
        double maxDensity = 0.0;
        SectorStats* curr = statsHead;
        while (curr != nullptr) {
            if (curr->density > maxDensity) {
                maxDensity = curr->density;
            }
            curr = curr->next;
        }

        cout << "\n[*] Legend: " << endl;
        cout << "    EMPTY     : 0 people" << endl;
        cout << "    LOW       : < 3 people" << endl;
        cout << "    MEDIUM    : 3-6 people" << endl;
        cout << "    HIGH      : 6-10 people" << endl;
        cout << "    VERY HIGH : > 10 people" << endl;

        cout << "\n" << string(70, '=') << endl;
        cout << "SECTOR          POPULATION  HOUSES  DENSITY   LEVEL      CHART" << endl;
        cout << string(70, '=') << endl;

        // Display each sector
        curr = statsHead;
        while (curr != nullptr) {
            cout << curr->sectorName;

            // Padding for alignment
            int padding = 16 - curr->sectorName.length();
            for (int i = 0; i < padding; i++) cout << " ";

            cout << curr->totalPopulation;
            padding = 12 - to_string(curr->totalPopulation).length();
            for (int i = 0; i < padding; i++) cout << " ";

            cout << curr->totalHouses;
            padding = 8 - to_string(curr->totalHouses).length();
            for (int i = 0; i < padding; i++) cout << " ";

            // Print density (now just shows population)
            cout << (int)curr->density << ".00";

            padding = 10 - to_string((int)curr->density).length() - 3;
            for (int i = 0; i < padding; i++) cout << " ";

            string level = getDensityLevel(curr->density);
            cout << level;
            padding = 11 - level.length();
            for (int i = 0; i < padding; i++) cout << " ";

            cout << getDensityBar(curr->density, maxDensity) << endl;

            curr = curr->next;
        }

        cout << string(70, '=') << endl;
    }

    // -------------------------------------------------------
    // findMostPopulated()
    // Identifies most crowded sector
    // -------------------------------------------------------
    void findMostPopulated() {
        if (!statsHead) {
            cout << "\n[!] No data available." << endl;
            return;
        }

        SectorStats* mostPopulated = statsHead;
        SectorStats* curr = statsHead->next;

        while (curr != nullptr) {
            if (curr->totalPopulation > mostPopulated->totalPopulation) {
                mostPopulated = curr;
            }
            curr = curr->next;
        }

        cout << "\n+==================================================+" << endl;
        cout << "|          MOST POPULATED SECTOR                   |" << endl;
        cout << "+==================================================+" << endl;
        cout << "\n[*] Sector: " << mostPopulated->sectorName << endl;
        cout << "[*] Total Population: " << mostPopulated->totalPopulation << endl;
        cout << "[*] Total Houses: " << mostPopulated->totalHouses << endl;
        cout << "[*] Density: " << mostPopulated->density << " people" << endl;
        cout << "[*] Level: " << getDensityLevel(mostPopulated->density) << endl;
    }

    // -------------------------------------------------------
    // findLeastPopulated()
    // Identifies least crowded sector
    // -------------------------------------------------------
    void findLeastPopulated() {
        if (!statsHead) {
            cout << "\n[!] No data available." << endl;
            return;
        }

        SectorStats* leastPopulated = statsHead;
        SectorStats* curr = statsHead->next;

        while (curr != nullptr) {
            if (curr->totalPopulation < leastPopulated->totalPopulation) {
                leastPopulated = curr;
            }
            curr = curr->next;
        }

        cout << "\n+==================================================+" << endl;
        cout << "|          LEAST POPULATED SECTOR                  |" << endl;
        cout << "+==================================================+" << endl;
        cout << "\n[*] Sector: " << leastPopulated->sectorName << endl;
        cout << "[*] Total Population: " << leastPopulated->totalPopulation << endl;
        cout << "[*] Total Houses: " << leastPopulated->totalHouses << endl;
        cout << "[*] Density: " << leastPopulated->density << " people" << endl;
        cout << "[*] Level: " << getDensityLevel(leastPopulated->density) << endl;
    }

    // -------------------------------------------------------
    // getOccupancyRate()
    // Calculates overall city occupancy
    // -------------------------------------------------------
    void getOccupancyRate() {
        if (!statsHead) {
            cout << "\n[!] No data available." << endl;
            return;
        }

        int totalPop = 0;
        int totalHouses = 0;

        SectorStats* curr = statsHead;
        while (curr != nullptr) {
            totalPop += curr->totalPopulation;
            totalHouses += curr->totalHouses;
            curr = curr->next;
        }

        double avgOccupancy = (double)totalPop / totalHouses;

        cout << "\n+==================================================+" << endl;
        cout << "|          CITY-WIDE OCCUPANCY RATE                |" << endl;
        cout << "+==================================================+" << endl;
        cout << "\n[*] Total Population: " << totalPop << endl;
        cout << "[*] Total Houses: " << totalHouses << endl;
        cout << "[*] Average Occupancy: " << avgOccupancy << " people/house" << endl;
        cout << "[*] Overall City Population Level: " << getDensityLevel(totalPop) << endl;
    }

    // Destructor
    ~PopulationHeatmap() {
        while (statsHead != nullptr) {
            SectorStats* temp = statsHead;
            statsHead = statsHead->next;
            delete temp;
        }
    }
};
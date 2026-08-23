#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*
===========================================================
   DATA LOADER - CSV FILE HANDLER
   --------------------------------------------------------
   Loads all sample data from CSV files into the system.

   Files Supported:
   - stops.csv        (Bus stops with coordinates)
   - buses.csv        (Bus information and routes)
   - schools.csv      (School data)
   - hospitals.csv    (Hospital data)
   - pharmacies.csv   (Medicine data)
   - population.csv   (Citizen data)
   
  
===========================================================
*/

class DataLoader {
private:

    // -------------------------------------------------------
    // Helper: Split string by delimiter
    // -------------------------------------------------------
    void split(const string& str, char delimiter, string parts[], int& count) {
        count = 0;
        string current = "";

        for (int i = 0; i < str.length(); i++) {
            if (str[i] == delimiter || str[i] == '\t') {  // <-- CHANGED THIS LINE
                if (current.length() > 0) {
                    parts[count++] = current;
                    current = "";
                }
            }
            else {
                current += str[i];
            }
        }

        if (current.length() > 0) {
            parts[count++] = current;
        }
    }

    // -------------------------------------------------------
    // Helper: Remove quotes from string
    // -------------------------------------------------------
    string removeQuotes(const string& str) {
        string result = "";
        for (int i = 0; i < str.length(); i++) {
            if (str[i] != '"' && str[i] != '\r') {
                result += str[i];
            }
        }
        return result;
    }

    // -------------------------------------------------------
    // Helper: Convert string to int
    // -------------------------------------------------------
    int stringToInt(const string& str) {
        int result = 0;
        bool negative = false;
        int start = 0;

        if (str.length() > 0 && str[0] == '-') {
            negative = true;
            start = 1;
        }

        for (int i = start; i < str.length(); i++) {
            if (str[i] >= '0' && str[i] <= '9') {
                result = result * 10 + (str[i] - '0');
            }
        }

        return negative ? -result : result;
    }

    // -------------------------------------------------------
    // Helper: Convert string to float
    // -------------------------------------------------------
    float stringToFloat(const string& str) {
        float result = 0.0f;
        float decimal = 0.0f;
        bool isDecimal = false;
        float divisor = 10.0f;
        bool negative = false;
        int start = 0;

        if (str.length() > 0 && str[0] == '-') {
            negative = true;
            start = 1;
        }

        for (int i = start; i < str.length(); i++) {
            if (str[i] == '.') {
                isDecimal = true;
            }
            else if (str[i] >= '0' && str[i] <= '9') {
                if (!isDecimal) {
                    result = result * 10.0f + (str[i] - '0');
                }
                else {
                    decimal += (str[i] - '0') / divisor;
                    divisor *= 10.0f;
                }
            }
        }

        result += decimal;
        return negative ? -result : result;
    }

    // -------------------------------------------------------
    // Helper: Parse coordinates "(lat, lon)"
    // -------------------------------------------------------
    void parseCoordinates(const string& coordStr, double& lat, double& lon) {
        string cleaned = removeQuotes(coordStr);
        string parts[10];
        int count = 0;

        // Remove parentheses
        string temp = "";
        for (int i = 0; i < cleaned.length(); i++) {
            if (cleaned[i] != '(' && cleaned[i] != ')') {
                temp += cleaned[i];
            }
        }

        split(temp, ',', parts, count);

        if (count >= 2) {
            lat = stringToFloat(parts[0]);
            lon = stringToFloat(parts[1]);
        }
    }

public:

    // -------------------------------------------------------
    // loadStops(filename, transportModule)
    // Loads bus stops from CSV
    // -------------------------------------------------------
    template<typename TransportModule>
    void loadStops(const string& filename, TransportModule& transport) {
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "[X] Error: Could not open " << filename << endl;
            return;
        }

        cout << "\n[*] Loading bus stops from " << filename << "..." << endl;

        string line;
        getline(file, line); // Skip header

        int count = 0;
        while (getline(file, line)) {
            string parts[10];
            int partCount = 0;
            split(line, ',', parts, partCount);

            if (partCount >= 3) {
                string stopId = removeQuotes(parts[0]);
                string name = removeQuotes(parts[1]);
                double lat, lon;
                parseCoordinates(parts[2], lat, lon);

                transport.addBusStop(stopId, name, lat, lon);
                count++;
            }
        }

        file.close();
        cout << "[OK] Loaded " << count << " bus stops." << endl;
    }

    // -------------------------------------------------------
    // loadBuses(filename, transportModule)
    // Loads buses and routes from CSV
    // -------------------------------------------------------
    template<typename TransportModule>
    void loadBuses(const string& filename, TransportModule& transport) {
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "[X] Error: Could not open " << filename << endl;
            return;
        }

        cout << "\n[*] Loading buses from " << filename << "..." << endl;

        string line;
        getline(file, line); // Skip header
        cout << "[DEBUG] Header: " << line << endl;

        int count = 0;
        int lineNum = 1;

        while (getline(file, line)) {
            lineNum++;
            cout << "[DEBUG] Line " << lineNum << ": " << line << endl;

            if (line.empty() || line.length() < 5) {
                cout << "[DEBUG] Skipping empty/short line" << endl;
                continue;
            }

            string parts[10];
            int partCount = 0;
            split(line, ',', parts, partCount);

            cout << "[DEBUG] Parts found: " << partCount << endl;

            if (partCount >= 4) {
                string busNo = removeQuotes(parts[0]);
                string company = removeQuotes(parts[1]);
                string currentStop = removeQuotes(parts[2]);
                string routeStr = removeQuotes(parts[3]);

                cout << "  Parsing bus: " << busNo << endl;
                cout << "  Company: " << company << endl;
                cout << "  Current: " << currentStop << endl;
                cout << "  Route string: " << routeStr << endl;

                // Parse route with format: Stop1 > Stop2 > Stop3
                RouteLinkedList route;
                string stops[20];
                int stopCount = 0;

                string temp = "";
                for (int i = 0; i < routeStr.length(); i++) {
                    char c = routeStr[i];

                    if (c == '>') {
                        string cleaned = "";
                        for (int j = 0; j < temp.length(); j++) {
                            if (temp[j] != ' ' && temp[j] != '\r' && temp[j] != '\n' && temp[j] != '\t') {
                                cleaned += temp[j];
                            }
                        }

                        if (cleaned.length() > 0 && stopCount < 20) {
                            stops[stopCount++] = cleaned;
                            cout << "    Stop: " << cleaned << endl;
                        }
                        temp = "";
                    }
                    else if (c == ' ' || c == '\t') {
                        continue;
                    }
                    else if (c != '\r' && c != '\n') {
                        temp += c;
                    }
                }

                // Last stop
                if (temp.length() > 0 && stopCount < 20) {
                    string cleaned = "";
                    for (int j = 0; j < temp.length(); j++) {
                        if (temp[j] != ' ' && temp[j] != '\r' && temp[j] != '\n' && temp[j] != '\t') {
                            cleaned += temp[j];
                        }
                    }
                    if (cleaned.length() > 0) {
                        stops[stopCount++] = cleaned;
                        cout << "    Stop: " << cleaned << endl;
                    }
                }

                // Add stops to route
                for (int j = 0; j < stopCount; j++) {
                    route.addStop(stops[j]);
                }

                cout << "  [DEBUG] About to register bus..." << endl;

                // Register the bus
                transport.registerBus(busNo, company, currentStop, route);

                cout << "  [DEBUG] Bus registered successfully!" << endl;
                count++;
            }
            else {
                cout << "[WARNING] Line " << lineNum << " has only " << partCount << " parts, expected 4" << endl;
            }
        }

        file.close();
        cout << "[OK] Loaded " << count << " buses." << endl;
    }

    // -------------------------------------------------------
    // loadSchools(filename, educationModule)
    // Loads schools from CSV
    // -------------------------------------------------------
    template<typename EducationModule>
    void loadSchools(const string& filename, EducationModule& education) {
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "[X] Error: Could not open " << filename << endl;
            return;
        }

        cout << "\n[*] Loading schools from " << filename << "..." << endl;

        string line;
        getline(file, line); // Skip header

        int count = 0;
        while (getline(file, line)) {
            string parts[10];
            int partCount = 0;
            split(line, ',', parts, partCount);

            if (partCount >= 5) {
                string schoolId = removeQuotes(parts[0]);
                string name = removeQuotes(parts[1]);
                string sector = removeQuotes(parts[2]);
                float rating = stringToFloat(removeQuotes(parts[3]));
                string subjects = removeQuotes(parts[4]);

                education.addSchool(name, rating);

                // Parse and add subjects as departments
                string depts[10];
                int deptCount = 0;
                split(subjects, ' ', depts, deptCount);

                for (int i = 0; i < deptCount; i++) {
                    education.addDepartment(name, depts[i]);
                }

                count++;
            }
        }

        file.close();
        cout << "[OK] Loaded " << count << " schools." << endl;
    }

    // -------------------------------------------------------
    // loadHospitals(filename, medicalModule)
    // Loads hospitals from CSV
    // -------------------------------------------------------
    template<typename MedicalModule>
    void loadHospitals(const string& filename, MedicalModule& medical) {
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "[X] Error: Could not open " << filename << endl;
            return;
        }

        cout << "\n[*] Loading hospitals from " << filename << "..." << endl;

        string line;
        getline(file, line); // Skip header

        int count = 0;
        while (getline(file, line)) {
            string parts[10];
            int partCount = 0;
            split(line, ',', parts, partCount);

            if (partCount >= 4) {
                string hospitalId = removeQuotes(parts[0]);
                string name = removeQuotes(parts[1]);
                string sector = removeQuotes(parts[2]);
                int beds = stringToInt(removeQuotes(parts[3]));

                // Default coordinates (can be enhanced)
                double lat = 33.7 + (count * 0.01);
                double lon = 73.0 + (count * 0.01);

                medical.addHospital(hospitalId, name, sector, beds, lat, lon);
                count++;
            }
        }

        file.close();
        cout << "[OK] Loaded " << count << " hospitals." << endl;
    }

    // -------------------------------------------------------
    // loadPharmacies(filename, medicalModule)
    // Loads medicines and pharmacies from CSV
    // -------------------------------------------------------
    template<typename MedicalModule>
    void loadPharmacies(const string& filename, MedicalModule& medical) {
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "[X] Error: Could not open " << filename << endl;
            return;
        }

        cout << "\n[*] Loading pharmacies from " << filename << "..." << endl;

        string line;
        getline(file, line); // Skip header

        int count = 0;
        while (getline(file, line)) {
            string parts[10];
            int partCount = 0;
            split(line, ',', parts, partCount);

            if (partCount >= 6) {
                string pharmacyId = removeQuotes(parts[0]);
                string pharmacyName = removeQuotes(parts[1]);
                string sector = removeQuotes(parts[2]);
                string medicineName = removeQuotes(parts[3]);
                string formula = removeQuotes(parts[4]);
                int price = stringToInt(removeQuotes(parts[5]));

                Medicine med;
                med.name = medicineName;
                med.formula = formula;
                med.price = price;
                med.locations = nullptr;

                medical.addMedicineToPharmacy(med, pharmacyName, sector);
                count++;
            }
        }

        file.close();
        cout << "[OK] Loaded " << count << " medicines." << endl;
    }

    // -------------------------------------------------------
    // loadPopulation(filename, populationModule)
    // Loads citizen data from CSV
    // -------------------------------------------------------
    template<typename PopulationModule>
    void loadPopulation(const string& filename, PopulationModule& population) {
        ifstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "[X] Error: Could not open " << filename << endl;
            return;
        }

        cout << "\n[*] Loading population from " << filename << "..." << endl;

        string line;
        getline(file, line); // Skip header

        int count = 0;
        while (getline(file, line)) {
            string parts[10];
            int partCount = 0;
            split(line, ',', parts, partCount);

            if (partCount >= 7) {
                string cnic = removeQuotes(parts[0]);
                string name = removeQuotes(parts[1]);
                int age = stringToInt(removeQuotes(parts[2]));
                string sector = removeQuotes(parts[3]);
                string street = removeQuotes(parts[4]);
                string houseNo = removeQuotes(parts[5]);
                string occupation = removeQuotes(parts[6]);

                // Add sector, street, house
                population.addSector(sector);
                population.addStreet(sector, street);

                // Create family with single member
                Family fam;
                fam.name = name;
                fam.memberCount = 1;
                fam.members[0].name = name;
                fam.members[0].age = age;
                fam.members[0].occupation = occupation;
                fam.members[0].gender = (count % 2 == 0) ? "M" : "F"; // Alternate for demo

                population.addHouse(sector, street, houseNo, fam);

                // Add to citizen table
                Citizen citizen;
                citizen.cnic = cnic;
                citizen.name = name;
                citizen.age = age;
                citizen.sector = sector;
                citizen.houseNo = stringToInt(houseNo);

                population.addCitizen(citizen);
                count++;
            }
        }

        file.close();
        cout << "[OK] Loaded " << count << " citizens." << endl;
    }

    // -------------------------------------------------------
    // connectStopsAutomatically(transportModule)
    // Creates edges between stops
    // -------------------------------------------------------
    template<typename TransportModule>
    void connectStopsAutomatically(TransportModule& transport) {
        cout << "\n[*] Connecting bus stops..." << endl;

        // Manual connections based on sample data
        transport.connectStops("Stop1", "Stop2", 5);
        transport.connectStops("Stop2", "Stop3", 8);
        transport.connectStops("Stop3", "Stop4", 6);
        transport.connectStops("Stop1", "Stop5", 7);
        transport.connectStops("Stop5", "Stop8", 10);
        transport.connectStops("Stop6", "Stop2", 9);
        transport.connectStops("Stop2", "Stop7", 4);
        transport.connectStops("Stop7", "Stop9", 12);
        transport.connectStops("Stop4", "Stop10", 5);
        transport.connectStops("Stop10", "Stop11", 7);
        transport.connectStops("Stop8", "Stop6", 6);
        transport.connectStops("Stop6", "Stop3", 5);

        cout << "[OK] Stop connections established." << endl;
    }
};
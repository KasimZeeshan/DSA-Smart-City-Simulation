#include <iostream>
#include <string>
using namespace std;

// ==================== ALL HEADERS ====================
#include "TransportModule.h"
#include "EducationModule.h"
#include "MedicalModule.h"
#include "MallModule.h"
#include "PublicFacilitiesModule.h"
#include "PopulationModule.h"

#include "SchoolBusTracker.h"
#include "RealTimeSimulation.h"
#include "AirportRailSystem.h"
#include "EmergenyTransportingRoute.h"
#include "PopulationHeatmap.h"

#include "DataLoader.h"

// ==================== GLOBAL MODULES ====================
TransportModule* transport = nullptr;
EducationModule* education = nullptr;
MedicalModule* medical = nullptr;
MallModule* mall = nullptr;
PublicFacilitiesModule* facilities = nullptr;
PopulationModule* population = nullptr;

// Bonus modules
SchoolBusTracker* busTracker = nullptr;
RealTimeSimulation* simulation = nullptr;
AirportRailSystem* airportSys = nullptr;
EmergencyRouting* emergency = nullptr;
PopulationHeatmap* heatmap = nullptr;

DataLoader loader;
bool dataLoaded = false;

// ==================== UI HELPERS ====================
void pause() {  
    cout << "\n\nPress ENTER to continue...";

    // Clear any error state
    cin.clear();

    // Ignore ALL remaining input
    cin.ignore(10000, '\n');

    // Wait for user
    cin.get();
}
void printLine() {
    cout << string(60, '=') << endl;
}

void printHeader(const string& title) {
    cout << "\n";
    printLine();
    cout << "  " << title << endl;
    printLine();
}

void printBanner() {
    cout << "\n\n";
    printLine();
    cout << "        SMART CITY ISLAMABAD MANAGEMENT SYSTEM" << endl;
    cout << "              Data Structures Project" << endl;
    printLine();
    cout << "\n";
}

// ==================== DATA LOADING ====================
void loadAllData() {
    if (dataLoaded) {
        cout << "\n[!] Data already loaded!" << endl;
        pause();
        return;
    }

    printHeader("LOADING SAMPLE DATA FROM CSV FILES");

    cout << "\n[*] Initializing system..." << endl;
    cout << string(60, '-') << endl;

    // Load all CSV files
    loader.loadStops("C:/Users/Kasim/Desktop/UNI/3rd Sem/DataStructures/Project/SmartCity_dataset/stops.csv", *transport);
    loader.connectStopsAutomatically(*transport);
    loader.loadBuses("C:/Users/Kasim/Desktop/UNI/3rd Sem/DataStructures/Project/SmartCity_dataset/buses.csv", *transport);
    loader.loadSchools("C:/Users/Kasim/Desktop/UNI/3rd Sem/DataStructures/Project/SmartCity_dataset/schools.csv", *education);
    loader.loadHospitals("C:/Users/Kasim/Desktop/UNI/3rd Sem/DataStructures/Project/SmartCity_dataset/hospitals.csv", *medical);
    loader.loadPharmacies("C:/Users/Kasim/Desktop/UNI/3rd Sem/DataStructures/Project/SmartCity_dataset/pharmacies.csv", *medical);
    loader.loadPopulation("C:/Users/Kasim/Desktop/UNI/3rd Sem/DataStructures/Project/SmartCity_dataset/population.csv", *population);

    // Initialize bonus modules
    busTracker = new SchoolBusTracker(&transport->BusTable, &transport->cityGraph, transport->travelHistory);
    simulation = new RealTimeSimulation(&transport->BusTable, &transport->cityGraph, transport->travelHistory);
    airportSys = new AirportRailSystem(&transport->cityGraph);
    emergency = new EmergencyRouting(&transport->cityGraph);
    heatmap = new PopulationHeatmap(&population->getSectorTree());

    // Add sample airports
    airportSys->addAirport("ISB", "Islamabad International Airport", 33.617, 72.989);
    airportSys->addAirport("RAW", "Rawalpindi Railway Station", 33.599, 73.055);
    airportSys->connectToRoad("ISB", "Stop8", 15);
    airportSys->connectToRoad("RAW", "Stop1", 8);

    // Add sample malls
    mall->addMall("M01", "Centaurus Mall", "F-8", 33.708, 73.041);
    mall->addMall("M02", "Giga Mall", "DHA-2", 33.526, 73.132);

    // Add sample facilities
    facilities->addFacility("F01", "Faisal Mosque", "Mosque", 33.729, 73.038);
    facilities->addFacility("F02", "F-9 Park", "Park", 33.700, 73.030);

    cout << "\n";
    printLine();
    cout << "[SUCCESS] ALL DATA LOADED!" << endl;
    printLine();

    dataLoaded = true;
    pause();
}

// ==================== TRANSPORT MENU ====================
void transportMenu() {
    while (true) {
        printHeader("TRANSPORT MANAGEMENT SYSTEM");

        cout << "\n  === VIEW OPTIONS ===" << endl;
        cout << "  1. View All Buses" << endl;
        cout << "  2. Search Bus by Number" << endl;
        cout << "  3. Find Shortest Path" << endl;
        cout << "  4. Find Nearest Stop" << endl;
        cout << "  5. Show Travel History" << endl;

        cout << "\n  === MANUAL INPUT OPTIONS ===" << endl;
        cout << "  6. Add New Bus Stop" << endl;
        cout << "  7. Connect Two Stops" << endl;
        cout << "  8. Register New Bus" << endl;

        cout << "\n  === PASSENGER OPTIONS ===" << endl;
        cout << "  9. Enqueue Passenger" << endl;
        cout << "  10. Dequeue Passenger" << endl;

        cout << "\n  0. Back to Main Menu" << endl;
        cout << "\n  Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printHeader("ALL BUSES");
            transport->printAllBuses();
            pause();
            break;

        case 2: {
            printHeader("SEARCH BUS");
            cout << "\n  Enter bus number (e.g., B101): ";
            string busNo;
            getline(cin, busNo);
            transport->printBus(busNo);
            pause();
            break;
        }

        case 3: {
            printHeader("SHORTEST PATH FINDER");
            cout << "\n  Enter source stop: ";
            string src;
            getline(cin, src);
            cout << "  Enter destination stop: ";
            string dest;
            getline(cin, dest);
            transport->findShortestPath(src, dest);
            pause();
            break;
        }

        case 4: {
            printHeader("NEAREST BUS STOP");
            cout << "\n  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();
            transport->nearestStop(lat, lon);
            pause();
            break;
        }

        case 5:
            printHeader("TRAVEL HISTORY");
            transport->showTravelHistory();
            pause();
            break;

            // NEW: Manual Bus Stop Addition
        case 6: {
            printHeader("ADD NEW BUS STOP");
            cout << "\n  Enter stop ID (e.g., Stop15): ";
            string id;
            getline(cin, id);
            cout << "  Enter stop name: ";
            string name;
            getline(cin, name);
            cout << "  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();

            transport->addBusStop(id, name, lat, lon);
            cout << "\n  [SUCCESS] Bus stop added!" << endl;
            pause();
            break;
        }

              // NEW: Connect Stops
        case 7: {
            printHeader("CONNECT TWO STOPS");
            cout << "\n  Enter first stop ID: ";
            string from;
            getline(cin, from);
            cout << "  Enter second stop ID: ";
            string to;
            getline(cin, to);
            cout << "  Enter distance (km): ";
            int distance;
            cin >> distance;
            cin.ignore();

            transport->connectStops(from, to, distance);
            cout << "\n  [SUCCESS] Stops connected!" << endl;
            pause();
            break;
        }

              // NEW: Register Bus
        case 8: {
            printHeader("REGISTER NEW BUS");
            cout << "\n  Enter bus number (e.g., B999): ";
            string busNo;
            getline(cin, busNo);
            cout << "  Enter company name: ";
            string company;
            getline(cin, company);
            cout << "  Enter current stop ID: ";
            string currentStop;
            getline(cin, currentStop);

            cout << "\n  Enter route (stops separated by spaces, e.g., Stop1 Stop2 Stop3): ";
            string routeInput;
            getline(cin, routeInput);

            // Parse route
            RouteLinkedList route;
            string stopId = "";
            for (int i = 0; i < routeInput.length(); i++) {
                if (routeInput[i] == ' ') {
                    if (stopId.length() > 0) {
                        route.addStop(stopId);
                        stopId = "";
                    }
                }
                else {
                    stopId += routeInput[i];
                }
            }
            if (stopId.length() > 0) route.addStop(stopId);

            transport->registerBus(busNo, company, currentStop, route);
            cout << "\n  [SUCCESS] Bus registered!" << endl;
            pause();
            break;
        }

        case 9: {
            printHeader("ADD PASSENGER TO QUEUE");
            cout << "\n  Enter passenger name: ";
            string name;
            getline(cin, name);
            cout << "  Enter destination stop: ";
            string dest;
            getline(cin, dest);
            transport->enqueuePassenger(name, dest);
            cout << "\n  [OK] Passenger added to queue!" << endl;
            pause();
            break;
        }

        case 10:
            printHeader("REMOVE PASSENGER FROM QUEUE");
            transport->dequeuePassenger();
            pause();
            break;

        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

// ==================== EDUCATION MENU ====================
void educationMenu() {
    while (true) {
        printHeader("EDUCATION MANAGEMENT SYSTEM");

        cout << "\n  === VIEW OPTIONS ===" << endl;
        cout << "  1. View All Schools" << endl;
        cout << "  2. Search School" << endl;
        cout << "  3. Rank Schools by Rating" << endl;

        cout << "\n  === MANUAL INPUT OPTIONS ===" << endl;
        cout << "  4. Add New School" << endl;
        cout << "  5. Add Department to School" << endl;
        cout << "  6. Add Class to Department" << endl;

        cout << "\n  0. Back to Main Menu" << endl;
        cout << "\n  Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printHeader("ALL SCHOOLS");
            education->printAllSchools();
            pause();
            break;

        case 2: {
            printHeader("SEARCH SCHOOL");
            cout << "\n  Enter school name: ";
            string name;
            getline(cin, name);
            education->searchSchool(name);
            pause();
            break;
        }

        case 3:
            printHeader("SCHOOL RANKINGS");
            education->rankSchools();
            pause();
            break;

            // NEW: Add School
        case 4: {
            printHeader("ADD NEW SCHOOL");
            cout << "\n  Enter school name: ";
            string name;
            getline(cin, name);
            cout << "  Enter rating (0.0 - 5.0): ";
            float rating;
            cin >> rating;
            cin.ignore();

            education->addSchool(name, rating);
            cout << "\n  [SUCCESS] School added!" << endl;
            pause();
            break;
        }

              // NEW: Add Department
        case 5: {
            printHeader("ADD DEPARTMENT");
            cout << "\n  Enter school name: ";
            string schoolName;
            getline(cin, schoolName);
            cout << "  Enter department name (e.g., Computer Science): ";
            string deptName;
            getline(cin, deptName);

            education->addDepartment(schoolName, deptName);
            cout << "\n  [SUCCESS] Department added!" << endl;
            pause();
            break;
        }

              // NEW: Add Class
        case 6: {
            printHeader("ADD CLASS");
            cout << "\n  Enter school name: ";
            string schoolName;
            getline(cin, schoolName);
            cout << "  Enter department name: ";
            string deptName;
            getline(cin, deptName);
            cout << "  Enter class name (e.g., CS-101): ";
            string className;
            getline(cin, className);

            education->addClass(schoolName, deptName, className);
            cout << "\n  [SUCCESS] Class added!" << endl;
            pause();
            break;
        }

        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

// ==================== MEDICAL MENU ====================
void medicalMenu() {
    while (true) {
        printHeader("MEDICAL SERVICES SYSTEM");

        cout << "\n  === VIEW OPTIONS ===" << endl;
        cout << "  1. View All Hospitals" << endl;
        cout << "  2. Rank Hospitals by Emergency Beds" << endl;
        cout << "  3. Find Nearest Hospital" << endl;
        cout << "  4. Search Medicine" << endl;

        cout << "\n  === MANUAL INPUT OPTIONS ===" << endl;
        cout << "  5. Add New Hospital" << endl;
        cout << "  6. Add Medicine to Pharmacy" << endl;

        cout << "\n  0. Back to Main Menu" << endl;
        cout << "\n  Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printHeader("ALL HOSPITALS");
            medical->printAllHospitals();
            pause();
            break;

        case 2:
            printHeader("HOSPITAL RANKINGS");
            medical->rankHospitals();
            pause();
            break;

        case 3: {
            printHeader("NEAREST HOSPITAL");
            cout << "\n  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();
            medical->nearestHospital(lat, lon);
            pause();
            break;
        }

        case 4: {
            printHeader("SEARCH MEDICINE");
            cout << "\n  Enter medicine name: ";
            string name;
            getline(cin, name);
            medical->searchMedicine(name);
            pause();
            break;
        }

              // NEW: Add Hospital
        case 5: {
            printHeader("ADD NEW HOSPITAL");
            cout << "\n  Enter hospital ID (e.g., H99): ";
            string id;
            getline(cin, id);
            cout << "  Enter hospital name: ";
            string name;
            getline(cin, name);
            cout << "  Enter sector: ";
            string sector;
            getline(cin, sector);
            cout << "  Enter emergency beds: ";
            int beds;
            cin >> beds;
            cout << "  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();

            medical->addHospital(id, name, sector, beds, lat, lon);
            cout << "\n  [SUCCESS] Hospital added!" << endl;
            pause();
            break;
        }

              // NEW: Add Medicine
        case 6: {
            printHeader("ADD MEDICINE TO PHARMACY");
            Medicine med;
            cout << "\n  Enter medicine name: ";
            getline(cin, med.name);
            cout << "  Enter formula: ";
            getline(cin, med.formula);
            cout << "  Enter price: ";
            cin >> med.price;
            cin.ignore();
            cout << "  Enter pharmacy name: ";
            string pharmacyName;
            getline(cin, pharmacyName);
            cout << "  Enter pharmacy sector: ";
            string sector;
            getline(cin, sector);

            medical->addMedicineToPharmacy(med, pharmacyName, sector);
            cout << "\n  [SUCCESS] Medicine added!" << endl;
            pause();
            break;
        }

        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

// ==================== MALL MENU ====================
void mallMenu() {
    while (true) {
        printHeader("COMMERCIAL SECTOR - SHOPPING MALLS");

        cout << "\n  === VIEW OPTIONS ===" << endl;
        cout << "  1. View All Malls" << endl;
        cout << "  2. Find Nearest Mall" << endl;
        cout << "  3. Search Product" << endl;

        cout << "\n  === MANUAL INPUT OPTIONS ===" << endl;
        cout << "  4. Add New Mall" << endl;
        cout << "  5. Add Product to Mall" << endl;

        cout << "\n  0. Back to Main Menu" << endl;
        cout << "\n  Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printHeader("ALL MALLS");
            mall->printAllMalls();
            pause();
            break;

        case 2: {
            printHeader("NEAREST MALL");
            cout << "\n  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();
            mall->nearestMall(lat, lon);
            pause();
            break;
        }

        case 3: {
            printHeader("SEARCH PRODUCT");
            cout << "\n  Enter product name: ";
            string name;
            getline(cin, name);
            mall->searchProduct(name);
            pause();
            break;
        }

              // NEW: Add Mall
        case 4: {
            printHeader("ADD NEW MALL");
            cout << "\n  Enter mall ID (e.g., M99): ";
            string id;
            getline(cin, id);
            cout << "  Enter mall name: ";
            string name;
            getline(cin, name);
            cout << "  Enter sector: ";
            string sector;
            getline(cin, sector);
            cout << "  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();

            mall->addMall(id, name, sector, lat, lon);
            cout << "\n  [SUCCESS] Mall added!" << endl;
            pause();
            break;
        }

              // NEW: Add Product
        case 5: {
            printHeader("ADD PRODUCT TO MALL");
            Product prod;
            cout << "\n  Enter product name: ";
            getline(cin, prod.name);
            cout << "  Enter company: ";
            getline(cin, prod.company);
            cout << "  Enter brand: ";
            getline(cin, prod.brand);
            cout << "  Enter category: ";
            getline(cin, prod.category);
            cout << "  Enter price: ";
            cin >> prod.price;
            cin.ignore();
            cout << "  Enter mall name: ";
            string mallName;
            getline(cin, mallName);
            cout << "  Enter mall sector: ";
            string sector;
            getline(cin, sector);

            mall->addProductToMall(prod, mallName, sector);
            cout << "\n  [SUCCESS] Product added!" << endl;
            pause();
            break;
        }

        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

// ==================== FACILITIES MENU ====================
void facilitiesMenu() {
    while (true) {
        printHeader("PUBLIC FACILITIES");

        cout << "\n  === VIEW OPTIONS ===" << endl;
        cout << "  1. View All Facilities" << endl;
        cout << "  2. Find Nearest Facility" << endl;

        cout << "\n  === MANUAL INPUT OPTIONS ===" << endl;
        cout << "  3. Add New Facility" << endl;

        cout << "\n  0. Back to Main Menu" << endl;
        cout << "\n  Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printHeader("ALL FACILITIES");
            facilities->printAllFacilities();
            pause();
            break;

        case 2: {
            printHeader("NEAREST FACILITY");
            cout << "\n  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();
            facilities->nearestFacility(lat, lon);
            pause();
            break;
        }

              // NEW: Add Facility
        case 3: {
            printHeader("ADD NEW FACILITY");
            cout << "\n  Enter facility ID (e.g., F99): ";
            string id;
            getline(cin, id);
            cout << "  Enter facility name: ";
            string name;
            getline(cin, name);
            cout << "  Enter type (Mosque/Park/Other): ";
            string type;
            getline(cin, type);
            cout << "  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();

            facilities->addFacility(id, name, type, lat, lon);
            cout << "\n  [SUCCESS] Facility added!" << endl;
            pause();
            break;
        }

        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

// ==================== POPULATION MENU ====================
void populationMenu() {
    while (true) {
        printHeader("POPULATION & HOUSING SYSTEM");

        cout << "\n  === VIEW OPTIONS ===" << endl;
        cout << "  1. View Sector Tree" << endl;
        cout << "  2. Search Citizen by CNIC" << endl;
        cout << "  3. Gender Ratio Report" << endl;
        cout << "  4. Occupation Summary" << endl;
        cout << "  5. Population Density" << endl;
        cout << "  6. Age Distribution" << endl;

        cout << "\n  === MANUAL INPUT OPTIONS ===" << endl;
        cout << "  7. Add New Sector" << endl;
        cout << "  8. Add Street to Sector" << endl;
        cout << "  9. Add House with Family" << endl;
        cout << "  10. Add Citizen" << endl;

        cout << "\n  0. Back to Main Menu" << endl;
        cout << "\n  Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
        case 1:
            printHeader("ISLAMABAD SECTOR TREE");
            population->printAllSectors();
            pause();
            break;

        case 2: {
            printHeader("SEARCH CITIZEN");
            cout << "\n  Enter CNIC: ";
            string cnic;
            getline(cin, cnic);
            population->searchCitizen(cnic);
            pause();
            break;
        }

        case 3:
            printHeader("GENDER RATIO REPORT");
            population->genderRatio();
            pause();
            break;

        case 4:
            printHeader("OCCUPATION SUMMARY");
            population->occupationSummary();
            pause();
            break;

        case 5:
            printHeader("POPULATION DENSITY BY SECTOR");
            population->populationDensity();
            pause();
            break;

        case 6:
            printHeader("AGE DISTRIBUTION");
            population->ageDistribution();
            pause();
            break;

            // NEW: Add Sector
        case 7: {
            printHeader("ADD NEW SECTOR");
            cout << "\n  Enter sector name (e.g., I-10): ";
            string sector;
            getline(cin, sector);

            population->addSector(sector);
            cout << "\n  [SUCCESS] Sector added!" << endl;
            pause();
            break;
        }

              // NEW: Add Street
        case 8: {
            printHeader("ADD STREET TO SECTOR");
            cout << "\n  Enter sector name: ";
            string sector;
            getline(cin, sector);
            cout << "  Enter street name: ";
            string street;
            getline(cin, street);

            population->addStreet(sector, street);
            cout << "\n  [SUCCESS] Street added!" << endl;
            pause();
            break;
        }

              // NEW: Add House
        case 9: {
            printHeader("ADD HOUSE WITH FAMILY");
            cout << "\n  Enter sector: ";
            string sector;
            getline(cin, sector);
            cout << "  Enter street: ";
            string street;
            getline(cin, street);
            cout << "  Enter house number: ";
            string houseNo;
            getline(cin, houseNo);

            Family fam;
            cout << "  Enter family name: ";
            getline(cin, fam.name);
            cout << "  Enter number of members (max 10): ";
            cin >> fam.memberCount;
            cin.ignore();

            for (int i = 0; i < fam.memberCount && i < MAX_MEMBERS; i++) {
                cout << "\n  --- Member " << (i + 1) << " ---" << endl;
                cout << "  Name: ";
                getline(cin, fam.members[i].name);
                cout << "  Age: ";
                cin >> fam.members[i].age;
                cin.ignore();
                cout << "  Occupation: ";
                getline(cin, fam.members[i].occupation);
                cout << "  Gender (M/F): ";
                getline(cin, fam.members[i].gender);
            }

            population->addHouse(sector, street, houseNo, fam);
            cout << "\n  [SUCCESS] House added!" << endl;
            pause();
            break;
        }

              // NEW: Add Citizen
        case 10: {
            printHeader("ADD CITIZEN");
            Citizen c;
            cout << "\n  Enter CNIC: ";
            getline(cin, c.cnic);
            cout << "  Enter name: ";
            getline(cin, c.name);
            cout << "  Enter age: ";
            cin >> c.age;
            cin.ignore();
            cout << "  Enter sector: ";
            getline(cin, c.sector);
            cout << "  Enter house number: ";
            cin >> c.houseNo;
            cin.ignore();

            population->addCitizen(c);
            cout << "\n  [SUCCESS] Citizen added!" << endl;
            pause();
            break;
        }

        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

// ==================== BONUS MENU ====================
void bonusMenu() {
    while (true) {
        printHeader("BONUS FEATURES");

        cout << "\n  === SCHOOL BUS TRACKER ===" << endl;
        cout << "  1. Track Bus Location" << endl;
        cout << "  2. Move Bus to Next Stop" << endl;
        cout << "  3. Track Full Route" << endl;
        cout << "  4. Calculate Remaining Distance" << endl;

        cout << "\n  === REAL-TIME SIMULATION ===" << endl;
        cout << "  5. Simulate Bus Movement (Steps)" << endl;
        cout << "  6. Simulate Full Route" << endl;
        cout << "  7. Estimate Travel Time" << endl;
        cout << "  8. Show Simulation Log" << endl;

        cout << "\n  === AIRPORT & RAILWAY ===" << endl;
        cout << "  9. View Airport/Station Info" << endl;
        cout << "  10. Add New Airport/Station" << endl;
        cout << "  11. Connect Airport to Road Network" << endl;
        cout << "  12. Find Route to Airport" << endl;
        cout << "  13. Show Travel History" << endl;

        cout << "\n  === EMERGENCY ROUTING ===" << endl;
        cout << "  14. Request Emergency Route" << endl;
        cout << "  15. Find Nearest Hospital (Emergency)" << endl;
        cout << "  16. Process Highest Priority Request" << endl;
        cout << "  17. Show All Emergency Requests" << endl;
        cout << "  18. Emergency Broadcast" << endl;

        cout << "\n  === POPULATION HEATMAP ===" << endl;
        cout << "  19. Analyze Sectors" << endl;
        cout << "  20. Generate Heatmap" << endl;
        cout << "  21. Find Most Populated Sector" << endl;
        cout << "  22. Find Least Populated Sector" << endl;
        cout << "  23. City-Wide Occupancy Rate" << endl;

        cout << "\n  0. Back to Main Menu" << endl;
        cout << "\n  Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        switch (choice) {
            // ========== SCHOOL BUS TRACKER ==========
        case 1: {
            printHeader("TRACK BUS LOCATION");
            cout << "\n  Enter bus number: ";
            string busNo;
            getline(cin, busNo);
            busTracker->printLocation(busNo);
            pause();
            break;
        }

        case 2: {
            printHeader("MOVE BUS TO NEXT STOP");
            cout << "\n  Enter bus number: ";
            string busNo;
            getline(cin, busNo);
            busTracker->moveBusToNext(busNo);
            pause();
            break;
        }

        case 3: {
            printHeader("TRACK FULL ROUTE");
            cout << "\n  Enter bus number: ";
            string busNo;
            getline(cin, busNo);
            busTracker->trackFullRoute(busNo);
            pause();
            break;
        }

        case 4: {
            printHeader("CALCULATE REMAINING DISTANCE");
            cout << "\n  Enter bus number: ";
            string busNo;
            getline(cin, busNo);
            cout << "  Enter final destination stop: ";
            string finalStop;
            getline(cin, finalStop);
            busTracker->remainingDistance(busNo, finalStop);
            pause();
            break;
        }

              // ========== REAL-TIME SIMULATION ==========
        case 5: {
            printHeader("SIMULATE BUS MOVEMENT (STEPS)");
            cout << "\n  Enter bus number: ";
            string busNo;
            getline(cin, busNo);
            cout << "  Enter number of steps to simulate: ";
            int steps;
            cin >> steps;
            cin.ignore();
            simulation->simulateBusMovement(busNo, steps);
            pause();
            break;
        }

        case 6: {
            printHeader("SIMULATE FULL ROUTE");
            cout << "\n  Enter bus number: ";
            string busNo;
            getline(cin, busNo);
            simulation->simulateFullRoute(busNo);
            pause();
            break;
        }

        case 7: {
            printHeader("ESTIMATE TRAVEL TIME");
            cout << "\n  Enter bus number: ";
            string busNo;
            getline(cin, busNo);
            cout << "  Enter destination stop: ";
            string destination;
            getline(cin, destination);
            simulation->estimateTravelTime(busNo, destination);
            pause();
            break;
        }

        case 8: {
            printHeader("SIMULATION HISTORY LOG");
            simulation->showSimulationLog();
            pause();
            break;
        }

              // ========== AIRPORT & RAILWAY SYSTEM ==========
        case 9: {
            printHeader("VIEW AIRPORT/STATION INFO");
            cout << "\n  Enter airport/station code (e.g., ISB, RAW): ";
            string code;
            getline(cin, code);
            airportSys->printAirport(code);
            pause();
            break;
        }

        case 10: {
            printHeader("ADD NEW AIRPORT/STATION");
            cout << "\n  Enter code (e.g., LHE for Lahore): ";
            string code;
            getline(cin, code);
            cout << "  Enter name: ";
            string name;
            getline(cin, name);
            cout << "  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();

            airportSys->addAirport(code, name, lat, lon);
            cout << "\n  [SUCCESS] Airport/Station added!" << endl;
            pause();
            break;
        }

        case 11: {
            printHeader("CONNECT AIRPORT TO ROAD NETWORK");
            cout << "\n  Enter airport/station code: ";
            string code;
            getline(cin, code);
            cout << "  Enter nearby stop ID: ";
            string stopId;
            getline(cin, stopId);
            cout << "  Enter distance (km): ";
            int distance;
            cin >> distance;
            cin.ignore();

            airportSys->connectToRoad(code, stopId, distance);
            cout << "\n  [SUCCESS] Connection established!" << endl;
            pause();
            break;
        }

        case 12: {
            printHeader("FIND ROUTE TO AIRPORT");
            cout << "\n  Enter starting airport/station code: ";
            string fromCode;
            getline(cin, fromCode);
            cout << "  Enter destination: ";
            string destination;
            getline(cin, destination);
            airportSys->routeTo(fromCode, destination);
            pause();
            break;
        }

        case 13: {
            printHeader("AIRPORT TRAVEL HISTORY");
            airportSys->showTravelHistory();
            pause();
            break;
        }

               // ========== EMERGENCY ROUTING ==========
        case 14: {
            printHeader("REQUEST EMERGENCY ROUTE");
            cout << "\n  Enter emergency type (Ambulance/Fire/Police): ";
            string type;
            getline(cin, type);
            cout << "  Enter from location: ";
            string from;
            getline(cin, from);
            cout << "  Enter to location: ";
            string to;
            getline(cin, to);
            cout << "  Enter priority (1-10, 10 = most critical): ";
            int priority;
            cin >> priority;
            cin.ignore();

            emergency->requestEmergencyRoute(type, from, to, priority);
            pause();
            break;
        }

        case 15: {
            printHeader("FIND NEAREST HOSPITAL (EMERGENCY)");
            cout << "\n  Enter emergency location: ";
            string location;
            getline(cin, location);
            cout << "  Enter latitude: ";
            double lat;
            cin >> lat;
            cout << "  Enter longitude: ";
            double lon;
            cin >> lon;
            cin.ignore();

            emergency->findNearestHospital(location, lat, lon);
            pause();
            break;
        }

        case 16: {
            printHeader("PROCESS HIGHEST PRIORITY REQUEST");
            emergency->processHighestPriorityRequest();
            pause();
            break;
        }

        case 17: {
            printHeader("ALL PENDING EMERGENCY REQUESTS");
            emergency->showAllEmergencyRequests();
            pause();
            break;
        }

        case 18: {
            printHeader("EMERGENCY BROADCAST");
            cout << "\n  Enter broadcast message: ";
            string message;
            getline(cin, message);
            emergency->emergencyBroadcast(message);
            pause();
            break;
        }

               // ========== POPULATION HEATMAP ==========
        case 19: {
            printHeader("ANALYZE SECTORS");
            cout << "\n  Analyzing all sectors..." << endl;
            heatmap->analyzeSectors();
            cout << "\n  [SUCCESS] Analysis complete!" << endl;
            pause();
            break;
        }

        case 20: {
            printHeader("GENERATE POPULATION HEATMAP");
            heatmap->generateHeatmap();
            pause();
            break;
        }

        case 21: {
            printHeader("MOST POPULATED SECTOR");
            heatmap->findMostPopulated();
            pause();
            break;
        }

        case 22: {
            printHeader("LEAST POPULATED SECTOR");
            heatmap->findLeastPopulated();
            pause();
            break;
        }

        case 23: {
            printHeader("CITY-WIDE OCCUPANCY RATE");
            heatmap->getOccupancyRate();
            pause();
            break;
        }

        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

//// ==================== BONUS QUICK ACCESS SUBMENU (Optional) ====================
////if the main bonus menu gets too long(Extra EFFORT TRYING TO GET THAT 4.0 GPA :) )
//
//void schoolBusTrackerMenu() {
//    while (true) {
//        printHeader("SCHOOL BUS TRACKER SYSTEM");
//
//        cout << "\n  1. Track Current Bus Location" << endl;
//        cout << "  2. Move Bus to Next Stop" << endl;
//        cout << "  3. Track Complete Route" << endl;
//        cout << "  4. Calculate Remaining Distance" << endl;
//        cout << "  0. Back" << endl;
//        cout << "\n  Enter choice: ";
//
//        int choice;
//        cin >> choice;
//        cin.ignore();
//
//        if (choice == 0) break;
//
//        switch (choice) {
//        case 1: {
//            cout << "\n  Enter bus number: ";
//            string busNo;
//            getline(cin, busNo);
//            busTracker->printLocation(busNo);
//            pause();
//            break;
//        }
//        case 2: {
//            cout << "\n  Enter bus number: ";
//            string busNo;
//            getline(cin, busNo);
//            busTracker->moveBusToNext(busNo);
//            pause();
//            break;
//        }
//        case 3: {
//            cout << "\n  Enter bus number: ";
//            string busNo;
//            getline(cin, busNo);
//            busTracker->trackFullRoute(busNo);
//            pause();
//            break;
//        }
//        case 4: {
//            cout << "\n  Enter bus number: ";
//            string busNo;
//            getline(cin, busNo);
//            cout << "  Enter destination: ";
//            string dest;
//            getline(cin, dest);
//            busTracker->remainingDistance(busNo, dest);
//            pause();
//            break;
//        }
//        default:
//            cout << "\n  [X] Invalid choice!" << endl;
//            pause();
//        }
//    }
//}
//
//void airportRailwayMenu() {
//    while (true) {
//        printHeader("AIRPORT & RAILWAY SYSTEM");
//
//        cout << "\n  === VIEW OPTIONS ===" << endl;
//        cout << "  1. View Airport/Station Info" << endl;
//        cout << "  2. Show Travel History" << endl;
//
//        cout << "\n  === MANUAL INPUT ===" << endl;
//        cout << "  3. Add New Airport/Station" << endl;
//        cout << "  4. Connect to Road Network" << endl;
//        cout << "  5. Find Route to Destination" << endl;
//
//        cout << "\n  0. Back" << endl;
//        cout << "\n  Enter choice: ";
//
//        int choice;
//        cin >> choice;
//        cin.ignore();
//
//        if (choice == 0) break;
//
//        switch (choice) {
//        case 1: {
//            cout << "\n  Enter code: ";
//            string code;
//            getline(cin, code);
//            airportSys->printAirport(code);
//            pause();
//            break;
//        }
//        case 2: {
//            airportSys->showTravelHistory();
//            pause();
//            break;
//        }
//        case 3: {
//            cout << "\n  Enter code: ";
//            string code;
//            getline(cin, code);
//            cout << "  Enter name: ";
//            string name;
//            getline(cin, name);
//            cout << "  Enter latitude: ";
//            double lat;
//            cin >> lat;
//            cout << "  Enter longitude: ";
//            double lon;
//            cin >> lon;
//            cin.ignore();
//            airportSys->addAirport(code, name, lat, lon);
//            cout << "\n  [SUCCESS]" << endl;
//            pause();
//            break;
//        }
//        case 4: {
//            cout << "\n  Enter airport code: ";
//            string code;
//            getline(cin, code);
//            cout << "  Enter stop ID: ";
//            string stopId;
//            getline(cin, stopId);
//            cout << "  Enter distance: ";
//            int dist;
//            cin >> dist;
//            cin.ignore();
//            airportSys->connectToRoad(code, stopId, dist);
//            cout << "\n  [SUCCESS]" << endl;
//            pause();
//            break;
//        }
//        case 5: {
//            cout << "\n  Enter from code: ";
//            string from;
//            getline(cin, from);
//            cout << "  Enter destination: ";
//            string to;
//            getline(cin, to);
//            airportSys->routeTo(from, to);
//            pause();
//            break;
//        }
//        default:
//            cout << "\n  [X] Invalid choice!" << endl;
//            pause();
//        }
//    }
//}
//
//void emergencyRoutingMenu() {
//    while (true) {
//        printHeader("EMERGENCY TRANSPORT ROUTING");
//
//        cout << "\n  1. Request Emergency Route" << endl;
//        cout << "  2. Find Nearest Hospital" << endl;
//        cout << "  3. Process Highest Priority" << endl;
//        cout << "  4. View All Requests" << endl;
//        cout << "  5. Send Emergency Broadcast" << endl;
//        cout << "  0. Back" << endl;
//        cout << "\n  Enter choice: ";
//
//        int choice;
//        cin >> choice;
//        cin.ignore();
//
//        if (choice == 0) break;
//
//        switch (choice) {
//        case 1: {
//            cout << "\n  Type (Ambulance/Fire/Police): ";
//            string type;
//            getline(cin, type);
//            cout << "  From: ";
//            string from;
//            getline(cin, from);
//            cout << "  To: ";
//            string to;
//            getline(cin, to);
//            cout << "  Priority (1-10): ";
//            int priority;
//            cin >> priority;
//            cin.ignore();
//            emergency->requestEmergencyRoute(type, from, to, priority);
//            pause();
//            break;
//        }
//        case 2: {
//            cout << "\n  Emergency location: ";
//            string loc;
//            getline(cin, loc);
//            cout << "  Latitude: ";
//            double lat;
//            cin >> lat;
//            cout << "  Longitude: ";
//            double lon;
//            cin >> lon;
//            cin.ignore();
//            emergency->findNearestHospital(loc, lat, lon);
//            pause();
//            break;
//        }
//        case 3: {
//            emergency->processHighestPriorityRequest();
//            pause();
//            break;
//        }
//        case 4: {
//            emergency->showAllEmergencyRequests();
//            pause();
//            break;
//        }
//        case 5: {
//            cout << "\n  Message: ";
//            string msg;
//            getline(cin, msg);
//            emergency->emergencyBroadcast(msg);
//            pause();
//            break;
//        }
//        default:
//            cout << "\n  [X] Invalid choice!" << endl;
//            pause();
//        }
//    }
//}
//
//void populationHeatmapMenu() {
//    while (true) {
//        printHeader("POPULATION HEATMAP ANALYSIS");
//
//        cout << "\n  1. Analyze All Sectors" << endl;
//        cout << "  2. Generate Visual Heatmap" << endl;
//        cout << "  3. Find Most Populated Sector" << endl;
//        cout << "  4. Find Least Populated Sector" << endl;
//        cout << "  5. City-Wide Occupancy Rate" << endl;
//        cout << "  0. Back" << endl;
//        cout << "\n  Enter choice: ";
//
//        int choice;
//        cin >> choice;
//        cin.ignore();
//
//        if (choice == 0) break;
//
//        switch (choice) {
//        case 1: {
//            cout << "\n  Analyzing..." << endl;
//            heatmap->analyzeSectors();
//            cout << "  [DONE]" << endl;
//            pause();
//            break;
//        }
//        case 2: {
//            heatmap->generateHeatmap();
//            pause();
//            break;
//        }
//        case 3: {
//            heatmap->findMostPopulated();
//            pause();
//            break;
//        }
//        case 4: {
//            heatmap->findLeastPopulated();
//            pause();
//            break;
//        }
//        case 5: {
//            heatmap->getOccupancyRate();
//            pause();
//            break;
//        }
//        default:
//            cout << "\n  [X] Invalid choice!" << endl;
//            pause();
//        }
//    }
//}

// ==================== MAIN MENU ====================
void mainMenu() {
    while (true) {
        printBanner();

        cout << "  MAIN MENU" << endl;
        cout << "  " << string(58, '-') << endl;
        cout << "\n  [1] Transport System" << endl;
        cout << "  [2] Education System" << endl;
        cout << "  [3] Medical Services" << endl;
        cout << "  [4] Commercial Sector (Malls)" << endl;
        cout << "  [5] Public Facilities" << endl;
        cout << "  [6] Population & Housing" << endl;
        cout << "\n  " << string(58, '-') << endl;
        cout << "  BONUS FEATURES" << endl;
        cout << "  " << string(58, '-') << endl;
        cout << "\n  [7] Bonus Modules Menu" << endl;
        cout << "\n  " << string(58, '-') << endl;
        cout << "  SYSTEM" << endl;
        cout << "  " << string(58, '-') << endl;
        cout << "\n  [0] Load Sample Data (DO THIS FIRST!)" << endl;
        cout << "  [8] Exit System" << endl;

        if (!dataLoaded) {
            cout << "\n  [!] WARNING: Please load sample data first (Option 0)" << endl;
        }

        cout << "\n  Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            // Input failed (user entered text instead of number)
            cin.clear();              // Clear error state
            cin.ignore(10000, '\n');  // Discard bad input
            choice = -1;              // Set to invalid choice
        }
        cin.ignore(10000, '\n');      // Clear rest of line

        if (choice == 8) {
            printHeader("THANK YOU!");
            cout << "\n  Exiting Smart City Islamabad System..." << endl;
            cout << "  Goodbye!" << endl;
            cout << "\n";
            break;
        }

        if (choice == 0) {
            loadAllData();
            continue;
        }

        if (!dataLoaded && choice != 0) {
            cout << "\n  [X] Please load data first (Option 0)!" << endl;
            pause();
            continue;
        }

        switch (choice) {
        case 1: transportMenu(); break;
        case 2: educationMenu(); break;
        case 3: medicalMenu(); break;
        case 4: mallMenu(); break;
        case 5: facilitiesMenu(); break;
        case 6: populationMenu(); break;
        case 7: bonusMenu(); break;
        default:
            cout << "\n  [X] Invalid choice!" << endl;
            pause();
        }
    }
}

// ==================== MAIN FUNCTION ====================
int main() {
    // Initialize all modules
    transport = new TransportModule();
    education = new EducationModule();
    medical = new MedicalModule();
    mall = new MallModule();
    facilities = new PublicFacilitiesModule();
    population = new PopulationModule();

    // Run main menu
    mainMenu();

    // Cleanup
    delete transport;
    delete education;
    delete medical;
    delete mall;
    delete facilities;
    delete population;

    if (dataLoaded) {
        delete busTracker;
        delete simulation;
        delete airportSys;
        delete emergency;
        delete heatmap;
    }

    return 0;
}
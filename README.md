# Smart City Simulation

A C++ simulation of a smart city, built for my Data Structures course. Instead of implementing data structures in isolation, this project ties each one to a real subsystem of a city — transport, population, healthcare, education, and more — so every module is backed by a specific data structure doing real work.

## Modules & Data Structures Used

| Module | Data Structure(s) |
|---|---|
| Airport & Rail System | Hash Table (`HashAirport`) |
| Bus Tracking | Hash Table (`HashBus`) |
| Public Transport / Emergency Routing | Graph, Circular Queue |
| Population Tracking & Heatmap | Hash Table (`HashCitizen`), custom heatmap logic |
| Family Records | Tree (`FamilyTree`) |
| School System | Tree (`SchoolTree`), Bus Tracker |
| Medical System | Hash Table (`HashMedicine`) |
| Mall / Retail | Hash Table (`HashProduct`) |
| Sector Management | Tree (`SectorTree`) |
| Priority-based Services | Max-Heap, Min-Heap |
| Core Utilities | Stack, Linked List, Circular Queue |

## About

The simulation loads city data (`DataLoader.h`) and runs modules that interact with each other in real time (`RealTimeSimulation.h`) — for example, emergency vehicles routing through the transport graph, or population changes reflected in the heatmap and hospital/school load.

## Tech Stack

- **Language:** C++
- **Build:** Visual Studio (.sln / .vcxproj included)

## How to Build

1. Open `DataProject.sln` in Visual Studio.
2. Build and run — entry point is `Main.cpp`.

## What I Learned

- Choosing the right data structure for a given real-world problem (hash tables for lookups, graphs for routing, trees for hierarchical data, heaps for priority-based scheduling)
- Designing multiple interacting modules that share underlying data
- Structuring a larger C++ project across many headers instead of one monolithic file

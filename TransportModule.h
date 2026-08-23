//#pragma once
//#include <iostream>
//#include <string>
//
//#include "../DataProject/Graph.h"
//#include "../DataProject/Hashbus.h"
//#include "../DataProject/LinkedList.h"
//#include "../DataProject/Stack.h"
//#include "../DataProject/CircularQueue.h"
//
//using namespace std;
//
//class TransportModule {
//	Graph cityGraph;
//	BusHashTable BusTable;
//	RouteStack* travelHistory;
//	CircularQueue* passengerQueue;
//
//public:
//	// Constructor
//	TransportModule() {
//		travelHistory = new RouteStack(50);
//		passengerQueue = new CircularQueue(20);
//	};
//
//	// Management
//		//// this adds new bus Stops
//	void addBusStop(const string& id, const string& name, double lat, double lon) {
//		Stop newStop;
//		newStop.id = id;
//		newStop.name = name;
//		newStop.coord.lat = lat;
//		newStop.coord.lon = lon;
//
//		// now add this newNode into graph
//		cityGraph.addNode(newStop.id, newStop);
//		cout << "We have added the new stop: " << id << "(" << name << ")" << endl;
//	};
//	//// This connects the given Stops with each other
//	void connectStops(const string& from, const string& to, int distance) {
//		if (!cityGraph.hasNode(from) || !cityGraph.hasNode(to)) { // in case one of the nodes does not exist, return
//			cout << "Invalid Nodes." << endl;
//			return;
//		}
//		cityGraph.addEdge(from, to, distance);
//		cout << "Connected the node " << from << " with " << to << " with a distance of " << distance << endl;
//	};
//
//	// Registration of Buses
//	void registerBus(const string& busNo, const string& company, const string& currentStop, RouteLinkedList route) {
//		Bus newBus;
//		newBus.busNo = busNo;
//		newBus.company = company;
//		newBus.currentStopId = currentStop;
//
//		BusTable.insertBus(newBus, route);
//		cout << "New bus " << busNo << " registered." << endl;
//	};
//
//	// route and trvel
//	void printBus(const string& busNo) {
//		// first you must find said bus in the bus table
//		BusRecord* record = BusTable.search(busNo);
//		if (!record) {
//			// in case not found, just return
//			cout << "Bus not found." << endl;
//			return;
//		}
//		cout << "Bus Number: " << record->bus.busNo << ", Company: " << record->bus.company << ", Current Stop: " << record->bus.currentStopId << endl;
//		record->route.printRoute(); // to get the bus route
//	};
//	void printAllBuses() {
//		BusTable.printAll();
//	};
//	void findShortestPath(const string& source, const string& destination) {
//		int pathSize = 0;
//		string* path = cityGraph.dijkstra(source, destination, pathSize);
//		if (!path) {
//			cout << "No viable route exists." << endl;
//			return;
//		}
//
//		cout << "Shortest path from " << source << " -> " << destination << ":" << endl;
//		for (int i = 0; i < pathSize; i++) {
//			cout << path[i];
//			if (i + 1 < pathSize) cout << " -> ";
//		}
//		cout << endl;
//
//		delete[] path; // MUST use delete[] because dijkstra allocated array
//	}
//
//
//		// for memory management
//		delete path;
//		path = nullptr;
//	};
//	void nearestStop(double lat, double lon) {
//		Coordinate coordinates;
//		coordinates.lat = lat;
//		coordinates.lon = lon;
//
//		// find nearest stop
//		string nearest = cityGraph.nearestNodeByCoord(coordinates);
//		cout << "Nearest Stop is: " << nearest << endl;
//	};
//
//	// passenger handling
//	void enqueuePassenger(const string& name, const string& destinationStop) {
//		Passenger pass;
//		pass.name = name;
//		pass.destinationStop = destinationStop;
//
//		// now simply enqueue in the queue
//		passengerQueue->enqueue(pass);
//	};
//	void dequeuePassenger() {
//		// similiar to enqueue, now just dequeue
//		Passenger pass = passengerQueue->dequeue();
//		if (pass.name != "") {
//			cout << "Removed Passenger is: " << pass.name << " at destination " << pass.destinationStop << endl;
//		}
//	};
//
//	// travel history
//	void pushTravelHistory(const string& stopId) {
//		// very simple, just push into stack
//		travelHistory->push(stopId);
//	};
//	void showTravelHistory() {
//		// very simple, just print all of it using the function
//		travelHistory->printStack();
//	};
//
//	~TransportModule() {
//		if (travelHistory) { delete travelHistory; travelHistory = nullptr; }
//		if (passengerQueue) { delete passengerQueue; passengerQueue = nullptr; }
//		// BusTable should have its own destructor to free records if needed
//	}
//
//};


#pragma once
#include <iostream>
#include <string>

#include "../DataProject/Graph.h"
#include "../DataProject/Hashbus.h"
#include "../DataProject/LinkedList.h"
#include "../DataProject/Stack.h"
#include "../DataProject/CircularQueue.h"

using namespace std;

struct TransportModule {
	Graph cityGraph;
	BusHashTable BusTable;
	RouteStack* travelHistory;
	CircularQueue* passengerQueue;

public:
	// Constructor
	TransportModule() {
		travelHistory = new RouteStack(50);
		passengerQueue = new CircularQueue(20);
	}

	// Management
	// This adds new bus stops
	void addBusStop(const string& id, const string& name, double lat, double lon) {
		Stop newStop;
		newStop.id = id;
		newStop.name = name;
		newStop.coord.lat = lat;
		newStop.coord.lon = lon;

		// Now add this newNode into graph
		cityGraph.addNode(newStop.id, newStop);
		cout << "We have added the new stop: " << id << " (" << name << ")" << endl;
	}

	// This connects the given stops with each other
	void connectStops(const string& from, const string& to, int distance) {
		if (!cityGraph.hasNode(from) || !cityGraph.hasNode(to)) {
			cout << "Invalid Nodes." << endl;
			return;
		}
		cityGraph.addEdge(from, to, distance);
		cout << "Connected the node " << from << " with " << to << " with a distance of " << distance << endl;
	}

	// Registration of Buses
	void registerBus(const string& busNo, const string& company, const string& currentStop, RouteLinkedList route) {
		Bus newBus;
		newBus.busNo = busNo;
		newBus.company = company;
		newBus.currentStopId = currentStop;

		BusTable.insertBus(newBus, route);
		cout << "New bus " << busNo << " registered." << endl;
	}

	// Route and travel
	void printBus(const string& busNo) {
		// First you must find said bus in the bus table
		BusRecord* record = BusTable.search(busNo);
		if (!record) {
			cout << "Bus not found." << endl;
			return;
		}
		cout << "Bus Number: " << record->bus.busNo
			<< ", Company: " << record->bus.company
			<< ", Current Stop: " << record->bus.currentStopId << endl;
		record->route.printRoute();
	}

	void printAllBuses() {
		BusTable.printAll();
	}

	void findShortestPath(const string& source, const string& destination) {
		int pathSize = 0;
		string* path = cityGraph.dijkstra(source, destination, pathSize);
		if (!path) {
			cout << "No viable route exists." << endl;
			return;
		}

		cout << "Shortest path from " << source << " -> " << destination << ":" << endl;
		for (int i = 0; i < pathSize; i++) {
			cout << path[i];
			if (i + 1 < pathSize) cout << " -> ";
		}
		cout << endl;

		// Memory management
		delete[] path;
	}

	void nearestStop(double lat, double lon) {
		Coordinate coordinates;
		coordinates.lat = lat;
		coordinates.lon = lon;

		// Find nearest stop
		string nearest = cityGraph.nearestNodeByCoord(coordinates);
		cout << "Nearest Stop is: " << nearest << endl;
	}

	// Passenger handling
	void enqueuePassenger(const string& name, const string& destinationStop) {
		Passenger pass;
		pass.name = name;
		pass.destinationStop = destinationStop;

		passengerQueue->enqueue(pass);
	}

	void dequeuePassenger() {
		Passenger pass = passengerQueue->dequeue();
		if (pass.name != "") {
			cout << "Removed Passenger is: " << pass.name
				<< " at destination " << pass.destinationStop << endl;
		}
	}

	// Travel history
	void pushTravelHistory(const string& stopId) {
		travelHistory->push(stopId);
	}

	void showTravelHistory() {
		travelHistory->printStack();
	}

	// Destructor
	~TransportModule() {
		if (travelHistory) {
			delete travelHistory;
			travelHistory = nullptr;
		}
		if (passengerQueue) {
			delete passengerQueue;
			passengerQueue = nullptr;
		}
	}
};
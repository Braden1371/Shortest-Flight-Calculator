//-----------------------------------------------------------------------------------------
// Braden Harris Wyatt Hooper
// CS-355
// Final Project
// 11/28/23
// This program is used to solve a logistical problem for people trying to determine
//	which flights they can take to reach their final destination with as few 
//	connecting flights as possible, and the shortest distance.
//----------------------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <cctype>
#include "Graph.h"
using namespace std;

int main() {
	Graph graph;
	Vertex v1;
	char choiceToPrintFullList;
	int choice;
	int stateChoice;
	int weight;
	string AirportID;
	string AirportName;
	string destination;
	string startAirport;
	string destAirport;
	string airportCode;
	string airportToRemove;

	// Add the airports and flights from the airport and flights files
	graph.addAirportsFile("airports.txt");
	graph.addFlightsFile("flights.txt");
	
	do {
		cout << "Welcome to H&H Airline Services! Please select an option from our menu." << endl;
		cout << "(1) Print." << endl;
		cout << "(2) Add a flight." << endl;
		cout << "(3) Remove an airport." << endl;
		cout << "(4) Find the shortest path between two airports." << endl;
		cout << "(0) Quit." << endl;
		
		// Get the choice for the menu option from the user
		cin >> choice;
		
		// Option for printing every connection in the graph for every airport		
		if(choice == 1) {
			cout << "Printing Graph: " << endl;
			graph.printGraph();
		}
		
		// Option to add an airport and flight	
		else if(choice == 2) {
			cout << "EDGE ADDING" << endl;
			cout << "Enter Starting Airport: ";
			cin >> startAirport;
			cout << endl << " Enter your destiantion Airport: ";
			cin >> destAirport;

			// Call the function to check if the airports exist
			if(graph.checkIfAirportExists(startAirport) && graph.checkIfAirportExists(destAirport)){
				weight = graph.calculateDist(startAirport, destAirport);
				graph.addFlight(startAirport, destAirport, weight);
			}
			else
				cout << "INVALID at least one airport does not exist!" << endl;
		}

		// Option to remove an airport
		else if(choice == 3) {
			cout << "Choose an airport to remove: ";
			cin >> airportToRemove;
			if(graph.checkIfAirportExists(airportToRemove)){
				graph.deleteAirport(airportToRemove);
				cout << "Airport deleted from file!" << endl;
			}
			else{
				cout << "Airport not on file!" << endl;
			}
			
		}

		// Option to find the shortest path between two airports
		else if(choice == 4) { 
			cout << "Find the shortest path from one airport to another." << endl;
			cout << "Choose state of departure: " << endl;

			// Uses the value returned by the displayAirportChoices function as the parameter for dijkstra's algorithm
			airportCode = graph.displayAirportChoices();
			graph.dijkstra(airportCode);
			
			cout << "Enter the airport you would like to fly to: " << endl;

			// Uses the value returned by the getAirportFunction as the parameter for the destination airport
			destination = graph.displayAirportChoices();
			graph.printSpecificShortestPath(destination);

			cout << "Would you like to see the shortest path to all aiports connected to " << airportCode << "? (Y or N)";
			cin >> choiceToPrintFullList;

			// Allow the user to print out the shortest path to every reachable airport
			if(choiceToPrintFullList == 'Y' || choiceToPrintFullList == 'y') {
				graph.printShortestPath();
			}
		}
	} while (choice != 0);
	
	return 0;
}

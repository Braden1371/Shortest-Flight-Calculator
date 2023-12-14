//-----------------------------------------------------------------------------------------
// Braden Harris Wyatt Hooper
// CS-355
// Final Project
// 11/28/23
// This program is used to solve a logistical problem for people trying to determine
//	which flights they can take to reach their final destination with as few 
//	connecting flights as possible, and the shortest distance.
//----------------------------------------------------------------------------------------
#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iterator>
#include <unordered_map>
#include "Vertex.h"
using namespace std;

class Graph {
	private:
		vector<Vertex> vertices;
		unordered_map<string, int> dist;
		unordered_map<string, string> parent;

	public:	
		//Graph functions will be in this file
		bool checkIfAirportExists(string AID);
		void addAirport(Vertex newAirport);
		void addFlight(string sP, string dV, int weight);
		void deleteAirport(string airport);
		void deleteFlight(string, string);
		void printGraph();
		void addAirportsFile(string);
		void addFlightsFile(string);
		double calculateDist(string, string);
		void dijkstra(string);
		void printShortestPath();
		void printSpecificShortestPath(string&);
		string displayAirportChoices();
};
#endif

//-----------------------------------------------------------------------------------------
// Braden Harris Wyatt Hooper
// CS-355
// Final Project
// 11/28/23
// This program is used to solve a logistical problem for people trying to determine
//	which flights they can take to reach their final destination with as few 
//	connecting flights as possible, and the shortest distance.
//----------------------------------------------------------------------------------------
#ifndef EDGE_H
#define EDGE_H

#include <iostream>
using namespace std;

class Edge {
	private:
		string destinationAirportID;
		int weight;

	public:
		Edge(string dV, int w);
		
		string getDestID();
		
		int getWeight();
		
		void setEdge(string dV, int w);
		
		void setWeight(int w);
};
#endif

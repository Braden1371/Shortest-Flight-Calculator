//-----------------------------------------------------------------------------------------
// Braden Harris Wyatt Hooper
// CS-355
// Final Project
// 11/28/23
// This program is used to solve a logistical problem for people trying to determine
//	which flights they can take to reach their final destination with as few 
//	connecting flights as possible, and the shortest distance.
//----------------------------------------------------------------------------------------
#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <list>
#include "Edge.h"

class Vertex {
	private: 
		string airportID;
		string airportName;
		double latitude;
		double longitude;

	public:	
		list<Edge> edgeList;

		Vertex();
		
		Vertex(string AID, string apName);
		
		string getAirportID();
		
		string getAirportName();
		
		void setAirportID(string AID);
		
		void setAirportName(string apName);
		
		void printFlightConnections();
		
		list<Edge> getEdgeList();

		double getLatitude();

		double getLongitude();

		void setLatitude(double);

		void setLongitude(double);
};


#endif

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
#include <cmath>
#include "Vertex.h"


using namespace std;

//Function Name: Vertex()
//Function Purpose: This function is a base constructor
//Function Author: Wyatt Hooper
Vertex::Vertex(){
}

//Function Name: Vertex()
//Function Purpose: This function is a base constructor that initializes
//	ID and airport name.
//Function Author: Wyatt Hooper
Vertex::Vertex(string AID, string apName) {
	airportID = AID;
	airportName = apName;
}

//Function Name: getAirportID()
//Function Purpose: This function returns the airport ID
//Function Author: Wyatt Hooper
string Vertex::getAirportID() {
	return airportID;
}

//Function Name: getAirportName()
//Function Purpose: This function returns the airport name
//Function Author: Wyatt Hooper
string Vertex::getAirportName() {
	return airportName;
}

//Function Name: setAirportID()
//Function Purpose: This function updates the airport ID
//Function Author: Wyatt Hooper
void Vertex::setAirportID(string AID) {
	airportID = AID;
}

//Function Name: setAirportName()
//Function Purpose: This function updates the airport name
//Function Author: Wyatt Hooper
void Vertex::setAirportName(string apName) {
	airportName = apName;
}

//Function Name: printFlightConnections()
//Function Purpose: This function iterates throught the edgeList for a given vertex
//	and outputs every connnecting flight.
//Function Author: Wyatt Hooper
void Vertex::printFlightConnections() {
	cout << "[";
    for (auto it = edgeList.begin(); it != edgeList.end(); it++) {
      cout << it -> getDestID() << "(" << it -> getWeight() << ") --> ";
    }
    cout << "]";
    cout << endl;
}

//Function Name: getEdgeList()
//Function Purpose: This function returns the edgeList for a given vertex
//Function Author: Wyatt Hooper
list<Edge> Vertex::getEdgeList() {
	return edgeList;
}

//Function Name: getLatitude()
//Function Purpose: This function returns the latitude of a given vertex
//Function Author: Braden Harris
double Vertex::getLatitude(){
	return latitude;
}

//Function Name: getLongitude()
//Function Purpose: This function returns the longitude of a given vertex
//Function Author: Braden Harris
double Vertex::getLongitude(){
	return longitude;
}

//Function Name: setLatitude()
//Function Purpose:This function updates the latitude of a given vertex
//Function Author: Braden Harris
void Vertex::setLatitude(double lat){
	latitude = lat;
}

//Function Name: setLongitude()
//Function Purpose: This function updates the longitude of a given vertex
//Function Author: Braden Harris
void Vertex::setLongitude(double lon){
	longitude = lon;
}

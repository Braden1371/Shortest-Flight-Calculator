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
#include "Edge.h"
using namespace std;

//Function Name: Edge()
//Function Purpose: This a constructor for the edge class 
//	it sets the destination and the weight.
//Function Author: Wyatt Hooper
Edge::Edge(string dV, int w) {
	destinationAirportID = dV;
	weight = w;
}

//Function Name: getDestID()
//Function Purpose: Function returns the ID for the destination
//	airport.
//Dunction Author: Wyatt Hooper
string Edge::getDestID() {
	return destinationAirportID;
}

//Function Name: getWeight()
//Function Purpose: Function returns the weight for the given
//	flight.
//Function Author: Wyatt Hooper
int Edge::getWeight() {
	return weight;
}

//Function Name: setEdge()
//Function Purpose: Function updates the destination and weight 
//	of a given edge.
//Function Author: Wyatt Hooper
void Edge::setEdge(string dV, int w) {
	destinationAirportID = dV;
	weight = w;
}

//Function Name: setWeight()
//Function Purpose: The function updates the weight of a given edge
//Function Author: Wyatt Hooper
void Edge::setWeight(int w) {
	weight = w;
}

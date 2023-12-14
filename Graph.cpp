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
#include <fstream>
#include <string>
#include <queue>
#include <limits>
#include <unordered_map>
#include <cmath>
#include <cctype>
#include "Graph.h"
using namespace std;

// Function Name: checkIfAirportExists()
// Function Purpose: We want the user to be able to add a custom flight if they want to.
// 	this could be for the purpose of a person with a priivately owned airport that we were not
// 	aware of. This function will help check and make sure we don't already have the airport in 
// 	our system and prevent duplicate vertices.
// Function Author: Wyatt Hooper
bool Graph::checkIfAirportExists(string AID) {
	for (int i = 0; i < vertices.size(); i++){
		if(vertices.at(i).getAirportID() == AID) { //.at() is used to access individual elements in the vector. It is a predefined function in the stl library.
			return true;
		}
	}
	return false;
}

// Function Name: addAirport()
// Function Purpose: We want the user to be able to add a custom flight if they want to.
// 	this could be for the purpose of a person with a priivately owned airport that we were not
// 	aware of.
// Function Author: Wyatt Hooper
void Graph::addAirport(Vertex newAirport) {
	bool checkIAE = checkIfAirportExists(newAirport.getAirportID());
	if(checkIAE == true) {
		cout << "Airport already in our system!" << endl;
	}
	
	else {
		vertices.push_back(newAirport);
	}
}

// Function Name: printGraph()
// Function Purpose: Print the graph with the airport IATA codes (3 letter codes) and the city the
// 	user is flying from.
// Function Author: Wyatt Hooper
void Graph::printGraph() {
	for(int i = 0; i < vertices.size(); i++) {
		Vertex print;
		print = vertices.at(i);
		cout << "Airport: " << print.getAirportName();
		print.printFlightConnections();
	}
}

// Function Name: addFlight()
// Function Purpose: Adds a connection between a starting and destination airport.
// Function Author: Braden Harris
void Graph::addFlight(string startingPoint, string destination, int weight) {
	// Here, we are checking to see if the start and endpoint airports exist.
	bool checkStartingPoint = checkIfAirportExists(startingPoint);
	bool checkDestination = checkIfAirportExists(destination);
	
	if(checkStartingPoint == true && checkDestination == true) {
		for(int i = 0; i < vertices.size(); i++) {
			// Connecting the edge to the starting point.
			if (vertices.at(i).getAirportID() == startingPoint) {
				Edge edge(destination, weight);
				vertices.at(i).edgeList.push_back(edge);
			}
			// We want to connect the edge to the destination as well. So we used an else if statement to look for the
			// destination vertex.
			else if (vertices.at(i).getAirportID() == destination) {
				Edge edge(startingPoint, weight);
				vertices.at(i).edgeList.push_back(edge);
			}
		}
	}
}

//Function Name: addAirportsFile()
//Function Purpose: Read in airports from a file. Each line of the
//	file contains airport ID, airport name, latitude, and 
//	longitude of the given airport.
//Function Author: Braden Harris
void Graph::addAirportsFile(string filename){
	ifstream inFile;
	inFile.open(filename);
	Vertex airport;
	string line, temp;
	int pos;
	double latitude, longitude;
	
	//Checks to see if file can be opened
	if(inFile.is_open()){
		//Keeps reading until there are no more lines in the file
		while(getline(inFile, line)){
			pos = line.find(",");//finds the first comma
			temp = line.substr(0,pos);//reads to the first comma
			airport.setAirportID(temp);//sets the ID

			line.erase(0,++pos);//erases the old string + the comma
			pos = line.find(",");
			temp = line.substr(0,pos);
			airport.setAirportName(temp);//sets the Name
			
			line.erase(0,++pos);
			pos = line.find(",");
			latitude = stod(line.substr(0,pos));//converts the string to a double
			airport.setLatitude(latitude);//sets the Latitude

			line.erase(0,++pos);
			pos = line.find(",");
			longitude = stod(line.substr(0,pos));//converts the string to a double
			airport.setLongitude(longitude);//sets the Longitude


			addAirport(airport);//adds the airport to the graph
		}
	}

	else
		cout << "Unable to open airport file" << endl;	

	inFile.close();
}

//Function Name: addFlightsFile()
//Function Purpose: The function reads in from a file a list of
//	connection for a given airport. These flights are added
//	to an adjacency list for the given airport.
//Function Author: Braden Harris
void Graph::addFlightsFile(string filename){
	ifstream inFile;
	inFile.open(filename);
	string temp, line, start, dest;
	int pos;

	//Check to see if file can be opened
	if(inFile.is_open()){
		//Keep reading while there are still flights in the file
		while(getline(inFile,line)){
			//The starting airport is always the first in the file
			start = line.substr(0,3);
			pos = line.find(",");//Update pos to the first comma after the new line

			//Keep iterating until the end of the line
			while(pos < line.length()){	
				//Looks one place after the last comma and to the next comma
				dest = line.substr(pos+1,line.find(","));
				double dist = calculateDist(start, dest);
				addFlight(start, dest, int(dist));
				pos += 4;
			}
		}
	}

	else
		cout << "Unable to open flight file" << endl;

	inFile.close();
}

//Function Name: deleteAirport()
//Function Purpose: This function removes the vertex of the corresponding airport.
//	This is used to make an airport unavailible for travel.
//Function Author: Wyatt Hooper
void Graph::deleteAirport(string airport) {
	int index = 0;
	for (int i = 0; i < vertices.size(); i++) {
		if(vertices.at(i).getAirportID() == airport) {
			index = i;
		}
	}

	for (int i = 0; i < vertices.size(); i++) {
		for (auto flight = vertices.at(i).edgeList.begin(); flight != vertices.at(i).edgeList.end(); flight++) {
			if(flight->getDestID() == airport) {
				vertices.at(i).edgeList.erase(flight);
				break;
			}
		}
	}
	vertices.erase(vertices.begin() + index);
	cout << "Airport Deleted" << endl;
}

//Function Name: deleteFlight()
//Function Purpose: This function deletes the connecting flights to the deleted airport.
//Function Author: Wyatt Hooper
void Graph::deleteFlight(string startingAirport, string destination) {
	for (int i = 0; i < vertices.size(); i++) {
		if (vertices.at(i).getAirportID() == startingAirport) {
			for (auto flight = vertices.at(i).edgeList.begin(); flight != vertices.at(i).edgeList.end(); flight++) {
			       if (flight->getDestID() == destination) {
				       vertices.at(i).edgeList.erase(flight);
			       break;
			       }
			}
		}
		if(vertices.at(i).getAirportID() == destination) {
			for (auto flight = vertices.at(i).edgeList.begin(); flight != vertices.at(i).edgeList.end(); flight++) {
				if (flight->getDestID() == startingAirport) {
					vertices.at(i).edgeList.erase(flight);
				}
			}
		}
	}
}

//Function Name: calculateDist()
//Function Purpose: This function uses the haversine formula to calculate the distance
//	between two points given their latitude and longitude.
//Function Author: Braden Harris
double Graph::calculateDist(string start, string dest){
	double startLat, startLong, destLat, destLong, diffLat, diffLong;
	double earthRadius = 6371.0; // This is earth's radius in km

	//Ensure both airports exist
	bool checkStart = checkIfAirportExists(start);
	bool checkDest = checkIfAirportExists(dest);
	
	if(checkStart && checkDest){
		//Iterate until through the vertices looking for the desired airports
		for(int i = 0; i < vertices.size(); i++){
			if(vertices.at(i).getAirportID() == start){
				startLat = vertices.at(i).getLatitude();
				startLong = vertices.at(i).getLongitude();
			}

			
			if(vertices.at(i).getAirportID() == dest){
				destLat = vertices.at(i).getLatitude();
				destLong = vertices.at(i).getLongitude();
			}
		}

		//Convert the latitude and longitude to radians
		startLat = startLat * (M_PI / 180.0);
		startLong = startLong * (M_PI / 180.0);
		destLat = destLat * (M_PI / 180.0);
		destLong = destLong * (M_PI / 180.0);

		//Find the distance between each latitude and longitude
		diffLat = destLat - startLat;
		diffLong = destLong - startLong;

		//The haversine formula for calculating distance
		double a = pow(sin(diffLat / 2), 2) + pow(sin(diffLong / 2), 2) * cos(startLat) * cos(destLat);
		double c = (2 * asin(sqrt(a))) * earthRadius;

		//This converts the answer to miles
		return c * 0.621371;

	}

	else
		return 9999;
}

// Function Name: Dijkstra
// Function Purpose: This is our shortest path algorithm
// Function Author: Wyatt Hooper / Braden Harris
void Graph::dijkstra(string startingAirport) {
	for(int i = 0; i < startingAirport.length(); i++){
		startingAirport[i] = toupper(startingAirport[i]);
	}
	string airport;
	int distance;
	int flightWeight;

	// Ensure that the maps for parent and dist are clear before we do anything with them. We added this because the maps were still populated when we tried to run it a second time. This 		resulted in an infinitely looping print statement in printShortestPath(). These 2 lines remedy that
	parent.clear();
	dist.clear();

	// Initialize the distances of the items in the map to infinity
	for (auto flight : vertices){
		dist[flight.getAirportID()] = 9999;
	}
	
	// Initialize the starting vertex to 0
	dist[startingAirport] = 0;

	// Now we are creating a priority queue to store the airports along with their distance
	// using key-value pairs
	priority_queue<pair<string, int>, vector<pair<string, int>>, greater<pair<string, int>>> pq;

	//Enqueue the starting airport
	pq.push({startingAirport, 0});

	while(!pq.empty()){
		// Top the first element, AKA the airport code and assign it to the current airport variable
		airport = pq.top().first;
		// Top the second element, AKA the distance and assign it to the distance variable
		distance = pq.top().second;
		// Pop the element after visiting it
		pq.pop();
		for (auto& vertex : vertices) {
			if(vertex.getAirportID() == airport) {
				// Iterate over the edges coming out of the current vertex
				for (auto& flight : vertex.getEdgeList()) {
					string neighbor = flight.getDestID();
					int flightWeight = flight.getWeight();

					// Relax the the edges and append to a new K-V pair to the PQ for every relaxation
					if(distance + flightWeight < dist[neighbor]) {
						dist[neighbor] = distance + flightWeight;
						parent[neighbor] = vertex.getAirportID();
						pq.push({neighbor, dist[neighbor]});
					}
				}
			}
		}
	}
}

//Function Name: printShortestPath()
//Function Purpose: This is a basic function that displays the shortest path to every destination.
//Function Author: Wyatt Hooper 
void Graph::printShortestPath() {
	cout << "Shortest Paths from the starting airport: " << endl;
	for (const auto& pair : parent) {
		cout << "To: " << pair.first << ", Path: ";

		string current = pair.first;
		while(current != "") {
			cout << current;
			if (parent[current] != "") {
				cout << " <- ";
			}
			current = parent[current];
		}
		cout << ", Distance: " << dist[pair.first] << " miles." << endl;
	}
}


//Funcion Name: printSpecificShortestPath
//Function Purpose: This is just a modified version of the printShortestPath function from above to print the shortest 
//	path to a specific airport. The above function is very thorough but the output is bulky and can be difficult 
//	to find a specific airport in the path. This function should fix that.
//Function Author: Wyatt Hooper
void Graph::printSpecificShortestPath(string& destination) {

	for(int i = 0; i < destination.length(); i++){
		destination[i] = toupper(destination[i]);
	}

	if(parent.find(destination) == parent.end()) {
		cout << "There is no path to the destination airport." << endl;
		// There is no path in the map to the destination, so we will break out of the function
		return;
	}

	else {
		cout << "Shortest distance to " << destination << " is: " << endl;

		string current = destination;
		while(current != "") {
			cout << current;
			if (parent[current] != "") {
				cout << " <- ";
			}
			current = parent[current];
		}
		cout << " Distance: " << dist[destination] << " miles." << endl;
	}
}

//Function Name: displayAirportChoices()
//Function Purpose: This function displays all states then shows the user the airport
//	choices of the given state.
//Function Author: Wyatt Hooper / Braden Harris
string Graph::displayAirportChoices(){
	string airportCode;
	int stateChoice;
	
	cout << "(1) Alabama\t\t" <<  "(2) Alaska\t\t" << "(3) Arizona\t\t" << "(4) Arkansas\t\t" << "(5) California\t" << endl;
	cout << "(6) Colorado\t\t" << "(7) Conneticut\t\t" << "(8) Deleware\t\t" << "(9) Florida\t\t" << "(10) Georgia\t" << endl;
	cout << "(11) Hawaii\t\t"  << "(12) Idaho\t\t" << "(13) Illinois\t\t" << "(14) Indiana\t\t" << "(15) Iowa\t" << endl;
	cout << "(16) Kansas\t\t" << "(17) Kentucky\t\t" << "(18) Louisiana\t\t" << "(19) Maine\t\t" << "(20) Maryland\t" << endl;
	cout << "(21) Massachusetts\t" << "(22) Michigan\t\t" << "(23) Minnesota\t\t" << "(24) Mississippi\t" << "(25) Missouri\t" << endl;
	cout << "(26) Montana\t\t" << "(27) Nebraska\t\t" << "(28) Nevada\t\t" << "(29) New Hampshire\t" << "(30) New Jersey\t" << endl;
	cout << "(31) New Mexico\t\t" << "(32) New York\t\t" << "(33) North Carolina\t" << "(34) North Dakota\t" << "(35) Ohio\t" << endl;
	cout << "(36) Oklahoma\t\t" << "(37) Oregon\t\t" << "(38) Pennsylvania\t" << "(39) Rhode Island\t" << "(40) South Carolina\t" << endl;
	cout << "(41) South Dakota\t" << "(42) Tennessee\t\t" << "(43) Texas\t\t" << "(44) Utah\t\t" << "(45) Vermont\t" << endl;
	cout << "(46) Virginia\t\t" << "(47) Washington\t\t" << "(48) West Virginia\t" << "(49) Wisconsin\t\t" << "(50) Wyoming\t" << endl;
	cin >> stateChoice;
	
		switch(stateChoice){
			//Alabama
			case 1:
				cout << "Airports are: " << endl;
				cout << "Birmingham International (BHM)" << endl;
				cout << "Huntsville International (HSV)" << endl;
				cout << "Mobile Regional (MOB)" << endl;
				cout << "Montgomery Regional Airport (MGM)" << endl;
				break;	
			
			//Alaska
			case 2:
				cout << "Airports are: " << endl;
				cout << "Ted Stevens Anchorage International (ANC)" << endl;
				cout << "Fairbanks International (FAI)" << endl;
				cout << "Juneuau International (JNU)" << endl;
				cout << "Ralph Wien Memorial (OTZ)" << endl;
				cout << "Yakatut Airport (YAK)" << endl;
				break;
	
			//Arizona
			case 3:
				cout << "Airports are: " << endl;
				cout << "Phoenix Sky Harbor International (PHX)" << endl;
				cout << "Tucson International (TUS)" << endl;
				break;
			
			//Arkansas
			case 4:
				cout << "Airports are: " << endl;
				cout << "Fort Smith Regional (FSM)" << endl;
				cout << "Adams (LIT)" << endl;
				break;
	
			//California
			case 5:
				cout << "Airports are: " << endl;
				cout << "Burbank-Glendale-Pasadena (BUR)" << endl;
				cout << "Los Angeles International (LAX)" << endl;
				cout << "Oakland International (OAK)" << endl;
				cout << "Ontario International (ONT)" << endl;
				cout << "Palm Springs International (PSP)" << endl;
				cout << "San Diego International-Lindbergh (SAN)" << endl;
				cout << "San Francisco International (SFO)" << endl;
				cout << "San Jose International (SJC)" << endl;
				cout << "Santa Maria Pub Capt G Allan HAncock (SMX)" << endl;
				break;
	
	
			//Colorado
			case 6:
				cout << "Airports are: " << endl;
				cout << "City of Colorado Spring Muni (COS)" << endl;
				cout << "Denver Intl (DEN)" << endl;
				cout << "Yampa Valley (HDN)" << endl;
				break;
	
			//Conneticut
			case 7:
				cout << "Airports are: " << endl;
				cout << "Bradley International (BDL)" << endl;
				break;
			
			//Deleware
			case 8:
				cout << "Airports are: " << endl;
				cout << "New Castle County (ILG)" << endl;
				break;
	
			//Florida
			case 9:
				cout << "Airports are: " << endl;
				cout << "Daytona Beach International (DAB)" << endl;
				cout << "Fort Lauderdale-Hollywood Intl (FLL)" << endl;
				cout << "Orlando International  (MCO)" << endl;
				cout << "Palm Beach International (PBI)" << endl;
				cout << "St. Petersburg International (PIE)" << endl;
				cout << "Pensacloa Regional (PNS)" << endl;
				cout << "Southwest Florida International (RSW)" << endl;
				cout << "Tallahassee Regional (TLH)" << endl;
				cout << "Tampa International (TPA)" << endl;
				break;
	
			//Georgia
			case 10:
				cout << "Airports are: " << endl;
				cout << "William B. Hartsfield-Atlanta Intl (ATL)" << endl;
				cout << "Bush (AGS)" << endl;
				break;
	
			//Hawaii
			case 11:
				cout << "Airports are: " << endl;
				cout << "Honolulu International (HNL)" << endl;
				cout << "Kahului (OGG)" << endl;
				cout << "Kona International at Keahole (KOA)" << endl;
				cout << "Lihue (LIH)" << endl;
				break;
	
			//Idaho
			case 12:
				cout << "Airports are: " << endl;
				cout << "Boise Air Terminal (BOI)" << endl;
				cout << "Spokane Intl (GEG)" << endl;
				cout << "Idaho Falls Regional (IDA)" << endl;
				cout << "Pocatello Regional (PIH)" << endl;
				cout << "Friedman Memorial (SUN)" << endl;
				break;
	
			//Illinois
			case 13:
				cout << "Airports are: " << endl;
				cout << "Central Illinois Regional (BMI)" << endl;
				cout << "Chicago O'Hare International (ORD)" << endl;
				cout << "Chicago Midway (MDW)" << endl;
				break;
	
			//Indiana
			case 14:
				cout << "Airports are: " << endl;
				cout << "Indianapolis International (IND)" << endl;
				break;
			
			//Iowa
			case 15:
				cout << "Airports are: " << endl;
				cout << "Eastern Iowa (CID)" << endl;
				break;
	
			//Kansas
			case 16:
				cout << "Airports are: " << endl;
				cout << "Kansas City International (MCI)" << endl;
				break;
	
			//Kentucky
			case 17:
				cout << "Airports are: " << endl;
				cout << "Cincinnati Northern Kentucky Intl (CVG)" << endl;
				cout << "Louisville International-Standford (SDF)" << endl;
				break;
	
			//Louisiana
			case 18:
				cout << "Airports are: " << endl;
				cout << "Baton Rouge Metropolitan (BTR)" << endl;
				cout << "Lake Charles Regional (LCH)" << endl;
				break;
	
			//Maine
			case 19:
				cout << "Airports are: " << endl;
				cout << "Bangor International (BGR)" << endl;
				cout << "Portland International Airport (PWM)" << endl;
				break;
			
			//Maryland
			case 20:
				cout << "Airports are: " << endl;
				cout << "Baltimore-Washington International (BWI)" << endl;
				cout << "Ronald Reagan Washington National (DCA)" << endl;
				break;
	
			//Massachusetts
			case 21:
				cout << "Airports are: " << endl;
				cout << "Gen Edw L Logan Intl (BOS)" << endl;
				cout << "Nantucket Memorial (ACK)" << endl;
				break;
	
			//Michigan
			case 22:
				cout << "Airports are: " << endl;
				cout << "Detroit Metropolitan-Wayne County (DTW)" << endl;
				cout << "Kent County International (GRR)" << endl;
				cout << "Kalamazoo Count (AZO)" << endl;
				cout << "Capital City (LAN)" << endl;
				cout << "MBS International (MBS)" << endl;
				break;
	
			//Minnesota
			case 23:
				cout << "Airports are: " << endl;
				cout << "Minneapolis-St. Paul International (MSP)" << endl;
				cout << "Duluth International (DLH)" << endl;
				break;
	
			//Mississippi
			case 24:
				cout << "Airports are: " << endl;
				cout << "Gulfport-Biloxi Regional (GPT)" << endl;
				cout << "Jackson International (JAN)" << endl;
				cout << "Key (MEI)" << endl;
				break;
	
			//Missouri
			case 25:
				cout << "Airports are: " << endl;
				cout << "Kansas City International (MCI)" << endl;
				cout << "Lambert-St. Louis International (STL)" << endl;
				cout << "Springfield-Branson Regional (SGF)" << endl;
				break;
	
			//Montana
			case 26:
				cout << "Airports are: " << endl;
				cout << "Billings Logan Intl (BIL)" << endl;
				cout << "Gallatin (BZN)" << endl;
				cout << "Great Falls Intl (GTF)" << endl;
				cout << "Missoula International (MSO)" << endl;
				break;
	
			//Nebraska
			case 27:
				cout << "Airports are: " << endl;
				cout << "Eppley Airfield (OMA)" << endl;
				cout << "Central Nebraska Regional(GRI)" << endl;
				break;
	
			//Nevada
			case 28:
				cout << "Airports are: " << endl;
				cout << "McCarram International (LAS)" << endl;
				cout << "Reno Tahoe International (RNO)" << endl;
				break;
	
			//New Hampshire
			case 29:
				cout << "Airports are: " << endl;
				cout << "Manchester (MHT)" << endl;
				break;
	
			//New Jersey
			case 30:
				cout << "Airports are: " << endl;
				cout << "Atlantic City International (ACY)" << endl;
				cout << "Newark Intl (EWR)" << endl;
				cout << "Trenton-Mercer County (TTN)" << endl;
				break;
	
			//New Mexico
			case 31:
				cout << "Airports are: " << endl;
				cout << "Albuquerque International (ABQ)" << endl;
				cout << "Santa Fe Municipal (SAF)" << endl;
				break;
			
			//New York
			case 32:
				cout << "Airports are: " << endl;
				cout << "Buffalo Niagara Intl (BUF)" << endl;
				cout << "John F Kennedy Intl (JFK)" << endl;
				cout << "LaGuardia (LGA)" << endl;
				cout << "Greater Rochester Intl (ROC)" << endl;
				cout << "Stewart (SWF)" << endl;
				cout << "Syracuse-Hancock Intl (SYR)" << endl;
				break;
	
			//North Carolina
			case 33:
				cout << "Airports are: " << endl;
				cout << "Charlotte Douglas International (CLT)" << endl;
				cout << "Raleigh-Durhan International (RDU)" << endl;
				cout << "Piedmont Triad International (GSO)" << endl;
				cout << "Albert J. Ellis (OAJ)" << endl;
				cout << "Wilmington International (ILM)" << endl;
				break;
	
			//North Dakota
			case 34:
				cout << "Airports are: " << endl;
				cout << "Bismark Municipal (BIS)" << endl;
				cout << "Hector International (FAR)" << endl;
				cout << "Grand Forks International (GFK)" << endl;
				break;
	
			//Ohio
			case 35:
				cout << "Airports are: " << endl;
				cout << "Cleveland-Hopkins Intl (CLE)" << endl;
				cout << "Port Columbus Intl (CMH)" << endl;
				cout << "Cincinnati Northern Kentucky Intl (CVG)" << endl;
				cout << "James M Cox Dayton Intl (DAY)" << endl;
				cout << "Toledo Express (TOL)" << endl;
				break;
	
			//Oklahoma
			case 36:
				cout << "Airports are: " << endl;
				cout << "Will Rogers World (OKC)" << endl;
				cout << "Tulsa International (TUL)" << endl;
				cout << "Lawton-Ft. Sill Regional (LAW)" << endl;	
				break;
	
			//Oregon
			case 37:
				cout << "Airports are: " << endl;
				cout << "Portland Intl (PDX)" << endl;
				cout << "Rogue Valley International (MFR)" << endl;
				cout << "Roberts (RDM)" << endl;
				break;
	
			//Pennsylvania
			case 38:
				cout << "Airports are: " << endl;
				cout << "Lehigh Valley International (ABE)" << endl;
				cout << "Harrisburg Intl (MDT)" << endl;
				cout << "Philadelphia Intl (PHL)" << endl;
				cout << "Pittsburh International (PIT)" << endl;
				cout << "Wilkes-Barre Scranton Intl (AVP)" << endl;
				break;
	
			//Rhode Island
			case 39:
				cout << "Airports are: " << endl;
				cout << "Theodore F Green State (PVD)" << endl;
				break;
	
			//South Carolina
			case 40:
				cout << "Airports are: " << endl;
				cout << "Charleston AFB International (CHS)" << endl;
				cout << "Greenville-Spartanburg (GSP)" << endl;
				break;
	
			//South Dakota
			case 41:
				cout << "Airports are: " << endl;
				cout << "Joe Foss (FSD)" << endl;
				cout << "Rapid City REgional (RAP)" << endl;
				break;
	
			//Tennessee
			case 42:
				cout << "Airports are: " << endl;
				cout << "Nashville International (BNA)" << endl;
				cout << "Memphis International (MEM)" << endl;
				cout << "McGhee-Tyson (TYS)" << endl;
				break;
	
			//Texas
			case 43:
				cout << "Airports are: " << endl;
				cout << "Abilene Regional (ABI)" << endl;
				cout << "Rick Husband Amarillo Intl (AMA)" << endl;
				cout << "Austin-Bergstrom International (AUS)" << endl;
				cout << "Corpus Christi International (CRP)" << endl;
				cout << "Dallas-Fort Worth International (DFW)" << endl;
				cout << "William P Hobby (HOU)" << endl;
				cout << "George Bush Intercontinental (IAH)" << endl;
				cout << "El Paso International (ELP)" << endl;
				cout << "Valley International (HRL)" << endl;
				cout << "San Antonio International (SAT)" << endl;
				break;
	
			//Utah 
			case 44:
				cout << "Airports are: " << endl;
				cout << "Salt Lake City International (SLC)" << endl;
				break;
	
			//Vermont
			case 45:
				cout << "Airports are: " << endl;
				cout << "Burlington International (BTV)" << endl;
				break;
	
			//Virginia
			case 46:
				cout << "Airports are: " << endl;
				cout << "Norfolk International (ORF)" << endl;
				cout << "Richmond International (RIC)" << endl;
				cout << "Ronald Reagan Washington National (DCA)" << endl;
				break;
	
			//Washignton
			case 47:
				cout << "Airports are: " << endl;
				cout << "Seattle-Tacoma International (SEA)" << endl;
				cout << "Spokane Intl (GEG)" << endl;
				cout << "Tri-Cities (PSC)" << endl;
				break;
	
			//West Virginia
			case 48:
				cout << "Airports are: " << endl;
				cout << "Yeager (CRW)" << endl;
				break;
	
			//Wisconsin
			case 49:
				cout << "Airports are: " << endl;
				cout << "Outagamie County (ATW)" << endl;
				cout << "General Mitchell International (MKE)" << endl;
				cout << "Dane County Regional-Truax Field (MSN)" << endl;
				break;
	
			//Wyoming
			case 50:
				cout << "Airports are: " << endl;
				cout << "Yellowstone Regional (COD)" << endl;
				break;
	
			default:
				cout << "Enter in a valid state number" << endl;
			}

	cout << "Enter your choice: " << endl;
	cin >> airportCode;

	return airportCode;
}


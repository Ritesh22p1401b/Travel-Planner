#ifndef GRAPHFUNCTIONS_H
#define GRAPHFUNCTIONS_H

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <stack>
#include <cmath>
#include <cfloat>
#include <algorithm> // for transform

#include "Location.h"
#include "Route.h"
#include "FileOperations.h"

using namespace std;

// Helper to convert strings to lowercase for case-insensitive comparison
string toLower(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}



class Graph {
public:
    vector<Location*> cities;
    vector<Route*> routes;

    int numExists;

    Graph(const string& nodesFile, const string& edgesFile);

    int getCityIndex(const string& key);
    Location* getCity(const string& key);

    float getWeight(const string& startS, const string& endS, bool costOrTime);
    float getWeight(Location* start, Location* end, bool costOrTime);

    void Dijkstras(const string& startS, bool costOrTime);

    vector<Location*> adjacentLocations(Location* city);
    Route* getRoute(Location* start, bool costOrTime, float totalDistance);

    vector<Route*> adjacentRoutes(Location* city);

    stack<Location*> cityStacker(const string& destinationS);
    stack<Route*> routeStacker(const string& destinationS, bool costOrTime);
};

// ---------------------------------------------------------------------
// Function Implementations
// ---------------------------------------------------------------------

Graph::Graph(const string& nodesFile, const string& edgesFile) {
    routes = routeParser(edgesFile);
    cities = locationParser(nodesFile, routes);
    numExists = cities.size();
}

int Graph::getCityIndex(const string& key) {
    for (size_t i = 0; i < cities.size(); ++i) {
        if (toLower(cities[i]->capital) == toLower(key)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

Location* Graph::getCity(const string& key) {
    int i = getCityIndex(key);
    return (i != -1) ? cities[i] : nullptr;
}

float Graph::getWeight(const string& startS, const string& endS, bool costOrTime) {
    Location* start = getCity(startS);
    Location* end = getCity(endS);
    return getWeight(start, end, costOrTime);
}

float Graph::getWeight(Location* start, Location* end, bool costOrTime) {
    for (Route* route : routes) {
        if (route->doesConnect(start, end)) {
            return costOrTime ? route->cost : route->time;
        }
    }
    return -1.0f;
}

void Graph::Dijkstras(const string& startS, bool costOrTime) {
    Location* start = getCity(startS);
    if (!start) return;

    // Reset all cities
    for (auto city : cities) {
        city->lengthFromStart = FLT_MAX;
        city->exists = true;
        city->previous = nullptr;
    }

    start->lengthFromStart = 0;
    priority_queue<Location*, vector<Location*>, compareLocation> minHeap;
    minHeap.push(start);

    while (!minHeap.empty()) {
        Location* smallest = minHeap.top();
        minHeap.pop();

        if (!smallest->exists) continue;

        smallest->exists = false;

        auto adjacentCities = adjacentLocations(smallest);
        for (Location* adjacent : adjacentCities) {
            float distance = getWeight(smallest, adjacent, costOrTime) + smallest->lengthFromStart;

            if (distance < adjacent->lengthFromStart) {
                adjacent->lengthFromStart = distance;
                adjacent->previous = smallest;
                minHeap.push(adjacent);
            }
        }
    }
}

vector<Location*> Graph::adjacentLocations(Location* city) {
    vector<Location*> output;
    for (Route* route : city->routes) {
		if (route->destination && route->destination->exists) {
			output.push_back(route->destination);
		}
	}
	
    return output;
}

vector<Route*> Graph::adjacentRoutes(Location* city) {
    vector<Route*> output;
    for (Route* route : routes) {
        if (route->origin->capital == city->capital) {
            output.push_back(route);
        }
    }
    return output;
}

Route* Graph::getRoute(Location* start, bool costOrTime, float totalDistance) {
    auto possibleRoutes = adjacentRoutes(start);
    float epsilon = 1e-5f;

    for (Route* r : possibleRoutes) {
        float diff = costOrTime ? r->cost : r->time;
        if (fabs((totalDistance - diff) - r->origin->lengthFromStart) < epsilon) {
            return r;
        }
    }
    return nullptr;
}

stack<Location*> Graph::cityStacker(const string& destinationS) {
    stack<Location*> output;
    Location* destination = getCity(destinationS);

    while (destination != nullptr) {
        output.push(destination);
        destination = destination->previous;
    }

    return output;
}

stack<Route*> Graph::routeStacker(const string& destinationS, bool costOrTime) {
    stack<Route*> output;
    Location* destination = getCity(destinationS);
    float totalDistance = destination->lengthFromStart;

    while (destination && destination->previous != nullptr) {
        Route* r = getRoute(destination->previous, costOrTime, totalDistance);
        if (r != nullptr) output.push(r);

        destination = destination->previous;
        totalDistance = destination->lengthFromStart;
    }

    return output;
}

#endif // GRAPHFUNCTIONS_H

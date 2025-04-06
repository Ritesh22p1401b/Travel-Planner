#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>
#include <stack>
#include <algorithm> // For transform

#include "FileOperations.h"
#include "Route.h"
#include "GraphFunctions.h"

using namespace std;

int main(int argc, char* argv[]) {
    string citiesFilename, routesFilename, outputFilename;
    string origin, destination, preference;
    bool biPreference = false;

    if (argc > 1) {
        citiesFilename = argv[1];
    } else {
        cout << "Enter filename containing cities: ";
        cin >> citiesFilename;
    }

    if (argc > 2) {
        routesFilename = argv[2];
    } else {
        cout << "Enter filename containing routes: ";
        cin >> routesFilename;
    }

    if (argc > 3) {
        outputFilename = argv[3];
    } else {
        cout << "Enter filename for output (.html): ";
        cin >> outputFilename;
    }

    if (argc > 4) {
        origin = argv[4];
    } else {
        cout << "Origin: ";
        cin >> origin;
    }

    if (argc > 5) {
        destination = argv[5];
    } else {
        cout << "Destination: ";
        cin >> destination;
    }

    if (argc > 6) {
        preference = argv[6];
    } else {
        cout << "Enter a preference (fastest/cheapest): ";
        cin >> preference;
    }

    // Convert preference to lowercase to make comparison case-insensitive
    transform(preference.begin(), preference.end(), preference.begin(), ::tolower);

    if (preference == "cheapest") {
        biPreference = true;
    } else if (preference == "fastest") {
        biPreference = false;
    } else {
        cout << "Invalid preference entered. Use 'fastest' or 'cheapest'." << endl;
        return 0;
    }

    Graph graph(citiesFilename, routesFilename);

    if (graph.getCity(origin) == NULL || graph.getCity(destination) == NULL) {
        cout << "Invalid origin or destination. Please check spelling and try again." << endl;
        return 0;
    }

    graph.Dijkstras(origin, biPreference);

    stack<Location*> cityStack = graph.cityStacker(destination);
    stack<Route*> routeStack = graph.routeStacker(destination, biPreference);

    outputGenerator(outputFilename.c_str(), cityStack, routeStack, biPreference);

    cout << "Output written to: " << outputFilename << endl;
    return 0;
}

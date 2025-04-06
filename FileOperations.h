#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <sstream>

#include "Location.h"
#include "Route.h"

#define MULTI 2.0 // Assumed multiplier for plane route cost

using namespace std;

vector<Location*> locationParser(const string& filename, vector<Route*> routes) {
    ifstream locations(filename);
    vector<Location*> cities;

    string line;
    while (getline(locations, line)) {
        stringstream ss(line);
        string country, city, latitude, longitude;

        getline(ss, country, ',');
        getline(ss, city, ',');
        getline(ss, latitude, ',');
        getline(ss, longitude);

        auto node = new Location(country, city, atof(latitude.c_str()), atof(longitude.c_str()));

        for (auto* route : routes) {
            if (route->originS == node->capital) {
                route->origin = node;
                node->routes.push_back(route);
            } else if (route->destinationS == node->capital) {
                route->destination = node;
            }
        }

        cities.push_back(node);
    }

    cout << "Cities Parsed from: " << filename << endl;
    return cities;
}

vector<Route*> routeParser(const string& filename) {
    ifstream routes(filename);
    vector<Route*> allRoutes;

    string line;
    while (getline(routes, line)) {
        stringstream ss(line);
        string originS, destinationS, type, time, cost, note;

        getline(ss, originS, ',');
        getline(ss, destinationS, ',');
        getline(ss, type, ',');
        getline(ss, time, ',');
        getline(ss, cost, ',');
        getline(ss, note);

        Route* edge = new Route(nullptr, nullptr, type, atof(time.c_str()), atof(cost.c_str()), note);
        edge->originS = originS;
        edge->destinationS = destinationS;

        allRoutes.push_back(edge);
    }

    cout << "Routes Parsed from: " << filename << endl;
    return allRoutes;
}

void outputGenerator(const string& filename, stack<Location*> cities, stack<Route*> routes, bool costOrTime) {
    ofstream output(filename);
    output << "<HTML><HEAD><TITLE>Shortest Path</TITLE></HEAD>"
           << "<script type='text/javascript' src='http://maps.google.com/maps/api/js?sensor=false'></script>"
           << "<script>function initialize() {"
           << "var myOptions = { zoom: 3, center: new google.maps.LatLng(0, 0), mapTypeId: google.maps.MapTypeId.ROADMAP};"
           << "var map=new google.maps.Map(document.getElementById('map'), myOptions);\n";

    int markerCount = 0;
    int contentStringCount = 0;

    while (!cities.empty() && !routes.empty()) {
        Location* origin = cities.top();
        cities.pop();
        if (cities.empty()) break;
        Location* destination = cities.top();

        output << "var marker" << markerCount++ << " = new google.maps.Marker({ position: new google.maps.LatLng(" << origin->lat << ", " << origin->lon << "), map: map, title: \"" << origin->capital << ", " << origin->country << "\"});\n";

        output << "var marker" << markerCount++ << " = new google.maps.Marker({ position: new google.maps.LatLng(" << destination->lat << ", " << destination->lon << "), map: map, title: \"" << destination->capital << ", " << destination->country << "\"});\n";

        Route* route = routes.top();
        routes.pop();

        float cost = route->cost;
        if (route->transport == "plane") {
            cost /= MULTI;
        }

        output << "var contentString" << contentStringCount << " = \"" << origin->capital << ", " << origin->country
               << " --> " << destination->capital << ", " << destination->country << " (" << route->transport
               << " - " << route->time << " hours - $" << cost << ")\";\n";

        output << "var path" << contentStringCount << " = new google.maps.Polyline({ path: ["
               << "new google.maps.LatLng(" << origin->lat << ", " << origin->lon << "), "
               << "new google.maps.LatLng(" << destination->lat << ", " << destination->lon << ")], "
               << "strokeColor: '#0000FF', strokeOpacity: 1.0, strokeWeight: 2});\n";

        output << "path" << contentStringCount << ".setMap(map);\n";
        output << "google.maps.event.addListener(path" << contentStringCount << ", 'click', function(event) { alert(contentString" << contentStringCount << ");});\n";

        contentStringCount++;
    }

    output << "} google.maps.event.addDomListener(window, 'load', initialize); </script>"
           << "</HEAD><BODY><div id='map' style='width:100%;height:100%;'></div></BODY></HTML>";
    output.close();

    cout << "Output File Generated: " << filename << endl;
}

#endif // FILEOPERATIONS_H

#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include <vector>

#include "Route.h"

class Route;

class Location {
public:
    std::string country;
    std::string capital;
    float lat;
    float lon;

    std::vector<Route*> routes;

    bool exists;
    Location* previous;
    float lengthFromStart;

    Location();
    ~Location();
    Location(std::string count, std::string cap);
    Location(std::string count, std::string cap, float lt, float lg);

    float getWeight(Location* start, Location* end);

    bool operator==(const Location& rhs) const;
    bool operator<(const Location& rhs) const;
    bool operator>(const Location& rhs) const;

    bool operator()(Location& l1, Location& l2);
};

Location::Location()
    : country(""), capital(""), lat(0), lon(0),
      lengthFromStart(999999), exists(true), previous(nullptr) {}

Location::~Location() {}

Location::Location(std::string count, std::string cap)
    : country(count), capital(cap), lat(0), lon(0),
      lengthFromStart(999999), exists(true), previous(nullptr) {}

Location::Location(std::string count, std::string cap, float lt, float lg)
    : country(count), capital(cap), lat(lt), lon(lg),
      lengthFromStart(999999), exists(true), previous(nullptr) {}

bool Location::operator==(const Location& rhs) const {
    return capital == rhs.capital;
}

bool Location::operator<(const Location& rhs) const {
    return lengthFromStart < rhs.lengthFromStart;
}

bool Location::operator>(const Location& rhs) const {
    return lengthFromStart > rhs.lengthFromStart;
}

class compareLocation {
public:
    bool operator()(const Location* l1, const Location* l2) const {
        return l1->lengthFromStart > l2->lengthFromStart;
    }
};

#endif // LOCATION_H

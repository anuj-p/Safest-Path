#include "utils.h"

ReaderUtils::RoadEntry::RoadEntry(std::string argName, double argLength, std::list<std::pair<double, double>> argCoordsList) : name(argName), length(argLength), coordinates_list(argCoordsList) {}

ReaderUtils::TrafficEntry::TrafficEntry(std::string argName, unsigned int argTraffic, std::list<std::pair<double, double>> argCoordsList) : road_name(argName), traffic(argTraffic), coordinates_list(argCoordsList) {}

ReaderUtils::CrashEntry::CrashEntry(unsigned int argVehicles, std::pair<double, double> argCoords) : vehicles(argVehicles), coordinates(argCoords) {}
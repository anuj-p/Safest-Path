#pragma once

#include <string>
#include <list>

class ReaderUtils {
    public:
        struct RoadEntry {
            std::string name;
            double length;
            std::list<std::pair<double, double>> coordinates_list;

            RoadEntry();
            RoadEntry(std::string argName, double argLength, std::list<std::pair<double, double>> argCoordsList);
        };
        struct TrafficEntry {
            std::string road_name;
            unsigned int traffic;
            std::list<std::pair<double, double>> coordinates_list;

            TrafficEntry();
            TrafficEntry(std::string argName, unsigned int argTraffic, std::list<std::pair<double, double>> argCoordsList);
        };
        struct CrashEntry {
            unsigned int vehicles;
            std::pair<double, double> coordinates;

            CrashEntry();
            CrashEntry(unsigned int argVehicles, std::pair<double, double> argCoords);
        };
};
#pragma once

// TODO
// query multiple parameters at once (just make start_idx a parameter)

#include <optional>
#include <string>
#include <list>

class Reader {
    public:
        struct RoadEntry {
            std::string name;
            
            double length;

            std::list<std::pair<double, double>> coordinates_list;
        };

        struct TrafficEntry {
            std::string road_name;
            
            unsigned int traffic;
            
            std::list<std::pair<double, double>> coordinates_list;
        };

        struct CrashEntry {
            unsigned int vehicles;

            std::pair<double, double> coordinates;
        };

        std::list<RoadEntry> getRoadEntries();
        std::list<TrafficEntry> getTrafficEntries();
        std::list<CrashEntry> getCrashEntries();

    private:
        std::optional<std::string> findString(std::string line, std::string parameter, bool space);
        std::optional<int> findInt(std::string line, std::string parameter, bool space);
        std::optional<double> findDouble(std::string line, std::string parameter, bool space);
        std::optional<std::list<std::pair<double, double>>> findCoordinatesList(std::string line, std::string parameter, unsigned int dim, bool space);
};
#pragma once

#include <vector>
#include <string>

class Reader {
    public:
        struct RoadEntry {
            std::string name;
            
            unsigned int length;

            std::vector<std::pair<double, double>> coordinates_list;
        };

        struct TrafficEntry {
            std::string road_name;
            
            unsigned int traffic;
            
            std::vector<std::pair<double, double>> coordinates_list;
        };

        struct CrashEntry {
            std::pair<double, double> coordinates;
        };

        std::vector<RoadEntry> getRoadEntries();
        std::vector<TrafficEntry> getTrafficEntries();
        std::vector<CrashEntry> getCrashEntries();

    private:
        std::string findValue(std::string parameter);

};
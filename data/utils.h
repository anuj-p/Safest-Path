#pragma once

#include <string>
#include <list>

/**
 * ReaderUtils class
*/
class ReaderUtils {
    public:
        /**
         * RoadEntry struct
        */
        struct RoadEntry {
            /**
             * Internal representation of a RoadEntry's name, length, and position
             * 
             * "name" is a string that stores the name of the road
             * "length" is a double that stores the length of the road
             * "coordinates_list" is a list of pairs of doubles that describes the road by a list of sequential signed GPS coordinates that lie on the road in the format {longitude, latitude}
            */
            std::string name;
            double length;
            std::list<std::pair<double, double>> coordinates_list;

            /**
             * Defaulted empty constructor for RoadEntry
            */
            RoadEntry();

            /**
             * Constructor for RoadEntry with name "argName", length "argLength", and coordinates "argCoordsList"
            */
            RoadEntry(std::string argName, double argLength, std::list<std::pair<double, double>> argCoordsList);
        };
        /**
         * TrafficEntry struct
        */
        struct TrafficEntry {
            /**
             * Internal representation of a TrafficEntry's name, traffic fequency, and position
             * 
             * "name" is a string that stores the name of the road
             * "traffic" is an unsigned int that stores the daily traffic fequency on the road
             * "coordinates_list" is a list of pairs of doubles that describes the road by a list of sequential signed GPS coordinates that lie on the road in the format {longitide, latitude}
            */
            std::string road_name;
            unsigned int traffic;
            std::list<std::pair<double, double>> coordinates_list;

            /**
             * Defaulted empty constructor for TrafficEntry
            */
            TrafficEntry();

            /**
             * Constructor for TrafficEntry with name "argName", traffic frequency "argTraffic", and coordinates "argCoordsList"
            */
            TrafficEntry(std::string argName, unsigned int argTraffic, std::list<std::pair<double, double>> argCoordsList);
        };
        /**
         * CrashEntry struct
        */
        struct CrashEntry {
            /**
             * Internal representation of a CrashEntry's crash fequency, and position
             * 
             * "vehicles" is an unsigned int that stores the number of vehicles involved in the crash
             * "coordinates" is a pair of doubles that stores the signed GPS position of the crash in the format {longitude, latitude}
            */
            unsigned int vehicles;
            std::pair<double, double> coordinates;

            /**
             * Defaulted empty constructor for CrashEntry
            */
            CrashEntry();

            /**
             * Constructor for CrashEntry with number of vehicles involved "argVehicles" and coordinates "argCoords"
            */
            CrashEntry(unsigned int argVehicles, std::pair<double, double> argCoords);
        };
};
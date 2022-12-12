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
            RoadEntry() = default;

            /**
             * Constructor for RoadEntry with name "argName", length "argLength", and coordinates "argCoordsList"
            */
            RoadEntry(std::string argName, double argLength, std::list<std::pair<double, double>> argCoordsList);

            /**
             * Default copy-assignment operator for RoadEntry
             * 
             * @param rhs A RoadEntry to be copy-assigned
            */
            RoadEntry& operator=(const RoadEntry& rhs) = default;

            /**
             * Default move-assignment operator for RoadEntry
             * 
             * @param rhs A RoadEntry to be move-assigned from
            */
            RoadEntry& operator=(RoadEntry&& rhs) = default;

            /**
             * Default copy-constructor for RoadEntry
             * 
             * @param other A RoadEntry to be copy-constructed
            */
            RoadEntry(const RoadEntry& other) = default;

            /**
             * Defauly move-constructor for RoadEntry
             * 
             * @param other A RoadEntry to be move-constructed from
            */
            RoadEntry(RoadEntry&& other) = default; 
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
            TrafficEntry() = default;

            /**
             * Constructor for TrafficEntry with name "argName", traffic frequency "argTraffic", and coordinates "argCoordsList"
            */
            TrafficEntry(std::string argName, unsigned int argTraffic, std::list<std::pair<double, double>> argCoordsList);

            /**
             * Default move-assignment operator for TrafficEntry
             * 
             * @param rhs A TrafficEntry to be move-assigned from
            */
            TrafficEntry& operator=(TrafficEntry&& rhs) = default;

            /**
             * Default copy-assignment operator for TrafficEntry
             * 
             * @param rhs A TrafficEntry to be copy-assigned
            */
            TrafficEntry& operator=(const TrafficEntry& rhs) = default;

            /**
             * Default copy-constructor for TrafficEntry
             * 
             * @param other A TrafficEntry to be copy-constructed
            */
            TrafficEntry(const TrafficEntry& other) = default;

            /**
             * Default move-constructor for TrafficEntry
             * 
             * @param other A TrafficEntry to be move-constructed from
            */
            TrafficEntry(TrafficEntry&& other) = default;
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
            CrashEntry() = default;

            /**
             * Constructor for CrashEntry with number of vehicles involved "argVehicles" and coordinates "argCoords"
            */
            CrashEntry(unsigned int argVehicles, std::pair<double, double> argCoords);

            /**
             * Default move-assignment operator for CrashEntry
             * 
             * @param rhs A CrashEntry to be move-assigned from
            */
            CrashEntry& operator=(CrashEntry&& rhs) = default;

            /**
             * Default copy-assignment operator for CrashEntry
             * 
             * @param rhs A CrashEntry to be copy-assigned
            */
            CrashEntry& operator=(const CrashEntry& rhs) = default;

            /**
             * Default copy-constructor for CrashEntry
             * 
             * @param other A CrashEntry to be copy-constructed
            */
            CrashEntry(const CrashEntry& other) = default;

            /**
             * Defauly move-constructor for CrashEntry
             * 
             * @param other A CrashEntry to be move-constructed from
            */
            CrashEntry(CrashEntry&& other) = default;
        };
};

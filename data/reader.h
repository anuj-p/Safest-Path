#pragma once

// TODO
// query multiple parameters at once (just make start_idx a parameter)

#include "utils.h"

#include <optional>
#include <string>
#include <list>
#include <tuple>

/**
 * Reader class
*/
class Reader {
    public:
        /**
         * A static function that returns a list of RoadEntrys from the file located at "filename"
         * 
         * @param filename A string storing the address of the dataset to be read
         * @return A list of RoadEntrys storing the roads in "filename"
        */
        static std::list<ReaderUtils::RoadEntry> getRoadEntries(const std::string& filename);

        /**
         * A static function that returns a list of TrafficEntrys from the file located at "filename"
         * 
         * @param filename A string storing the address of the dataset to be read
         * @return A list of TrafficEntrys storing the traffic frequencies of roads in "filename"
        */
        static std::list<ReaderUtils::TrafficEntry> getTrafficEntries(const std::string& filename);

        /**
         * A static function that rteurns a list of CrashEntrys from the file located at "filename"
         * 
         * @param filename A string storing the address of the dataset to be read
         * @return A list of CrashEntrys storing crashes listed in "filename"
        */
        static std::list<ReaderUtils::CrashEntry> getCrashEntries(const std::string& filename);

    private:
        /**
         * A static function that finds the JSON string data corresponding to key "parameter" in "line"
         * 
         * @param line A string storing the line to be read
         * @param parameter A string storing the key to be looked up
         * @param space A bool indicating if there is spacing in the dataset that needs to be adjusted for
         * @return An optional of string that stores the string of interest
        */
        static std::optional<std::string> findString(const std::string& line, const std::string& parameter, bool space);

        /**
         * A static function that finds the JSON int data corresponding to key "parameter" in "line"
         * 
         * @param line A string storing the line to be read
         * @param parameter A string storing the key to be looked up
         * @param space A bool indicating if there is spacing in the dataset that needs to be adjusted for
         * @return An optional of int that stores the integer of interest
        */
        static std::optional<int> findInt(const std::string& line, const std::string& parameter, bool space);

        /**
         * A static function that finds the JSON double data corresponding to key "parameter" in "line"
         * 
         * @param line A string storing the line to be read
         * @param parameter A string storing the key to be looked up
         * @param space A bool indicating if there is spacing in the dataset that needs to be adjusted for
         * @return An optional of double that stores the float of interest
        */
        static std::optional<double> findDouble(const std::string& line, const std::string& parameter, bool space);

        /**
         * A static function that finds the JSON coordinates data corresponding to key "parameter" in "line"
         * 
         * @param line A string storing the line to be read
         * @param parameter A string storing the key to be looked up
         * @param space A bool indicating if there is spacing in the dataset that needs to be adjusted for
         * @return An optional of list of pairs od doubles that stores the singed GPS coordinates of interest in format {longitude, latitude}
        */
        static std::optional<std::list<std::pair<double, double>>> findCoordinatesList(const std::string& line, const std::string& parameter, unsigned int dim, bool space);
};

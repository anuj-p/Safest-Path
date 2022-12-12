#pragma once

// TODO
// query multiple parameters at once (just make start_idx a parameter)

#include "utils.h"

#include <optional>
#include <string>
#include <list>
#include <tuple>

class Reader {
    public:
        static std::list<ReaderUtils::RoadEntry> getRoadEntries(const std::string& filename);
        static std::list<ReaderUtils::TrafficEntry> getTrafficEntries(const std::string& filename);
        static std::list<ReaderUtils::CrashEntry> getCrashEntries(const std::string& filename);

    private:
        static std::optional<std::string> findString(std::string line, std::string parameter, bool space);
        static std::optional<int> findInt(std::string line, std::string parameter, bool space);
        static std::optional<double> findDouble(std::string line, std::string parameter, bool space);
        static std::optional<std::list<std::pair<double, double>>> findCoordinatesList(std::string line, std::string parameter, unsigned int dim, bool space);
};

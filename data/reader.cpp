#include "reader.h"

#include <fstream>
#include <algorithm>

std::list<ReaderUtils::RoadEntry> Reader::getRoadEntries(const std::string& filename) {
    std::list<ReaderUtils::RoadEntry> entries;
    std::ifstream ifs{filename};
    if (ifs.fail()) {
        throw std::runtime_error("Unable to open " + filename + ".");
    }
    for (std::string line; std::getline(ifs, line); line = "]}") { // ends on ]}
        std::optional<std::string> name = findString(line, "name", false);
        std::optional<double> length = *findDouble(line, "SHAPE_Leng", false);
        std::optional<std::list<std::pair<double, double>>> coordinates_list = findCoordinatesList(line, "coordinates", 2, false);
        if (name && length && coordinates_list) {
            entries.push_back(ReaderUtils::RoadEntry(*name, *length, *coordinates_list));
        }
    }
    return entries;
}

std::list<ReaderUtils::TrafficEntry> Reader::getTrafficEntries(const std::string& filename) {
    std::list<ReaderUtils::TrafficEntry> entries;
    std::ifstream ifs{filename};
    if (ifs.fail()) {
        throw std::runtime_error("Unable to open " + filename + ".");
    }
    for (std::string line; std::getline(ifs, line); line = "") { // ends on new line
        std::optional<std::string> road_name = findString(line, "ROAD_NAME", true);
        std::optional<int> traffic = findInt(line, "AADT", true);
        std::optional<std::list<std::pair<double, double>>> coordinates_list = findCoordinatesList(line, "coordinates", 3, true);
        
        if (road_name && traffic && coordinates_list) {
            entries.push_back(ReaderUtils::TrafficEntry(*road_name, *traffic, *coordinates_list));
        }
    }
    return entries;
}

std::list<ReaderUtils::CrashEntry> Reader::getCrashEntries(const std::string& filename) {
    std::list<ReaderUtils::CrashEntry> entries;
    std::ifstream ifs{filename};
    if (ifs.fail()) {
        throw std::runtime_error("Unable to open " + filename + ".");
    }
    for (std::string line; std::getline(ifs, line); line = "") { // ends on new line
        std::optional<int> vehicles = findInt(line, "NumberOfVehicles", true); 
        std::optional<double> latitude = findDouble(line, "TSCrashLatitude", true);
        std::optional<double> longitude = findDouble(line, "TSCrashLongitude", true);
        if (vehicles && latitude && longitude) {
            entries.push_back(ReaderUtils::CrashEntry(*vehicles, {*longitude, *latitude}));
        }
    }
    return entries;
}

std::optional<std::string> Reader::findString(const std::string& line, const std::string& parameter, bool space) {
    size_t start_idx = line.find(parameter);
    if (start_idx == std::string::npos) return std::nullopt;
    while (line.substr(start_idx + parameter.length(), 2) != "\":") {
        start_idx = line.find(parameter, start_idx + parameter.length());
        if (start_idx == std::string::npos) return std::nullopt;
    }

    unsigned int next_idx = start_idx + parameter.length() + 3 + space;
    if (line[next_idx - 1] != '\"') throw std::invalid_argument("");

    unsigned int end_idx = line.find_first_of("\",", next_idx); // assuming no parameter contains ",
    std::string result = line.substr(next_idx, end_idx - next_idx);
    if (result == "null") return std::nullopt;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper); // standardize by making all uppercase
    return result;
}

std::optional<int> Reader::findInt(const std::string& line, const std::string& parameter, bool space) {
    size_t start_idx = line.find(parameter);
    if (start_idx == std::string::npos) return std::nullopt;
    while (line.substr(start_idx + parameter.length(), 2) != "\":") {
        start_idx = line.find(parameter, start_idx + parameter.length());
        if (start_idx == std::string::npos) return std::nullopt;
    }

    unsigned int next_idx = start_idx + parameter.length() + 2 + space;
    if (line[next_idx] == '\"' || line[next_idx] == '[') throw std::invalid_argument("");
    
    unsigned int end_idx = line.find_first_of(",", next_idx); // assuming no parameter contains ,
    std::string result = line.substr(next_idx, end_idx - next_idx);
    if (result == "null") return std::nullopt;
    return stoi(result);
}

std::optional<double> Reader::findDouble(const std::string& line, const std::string& parameter, bool space) {
    size_t start_idx = line.find(parameter);
    if (start_idx == std::string::npos) return std::nullopt;
    while (line.substr(start_idx + parameter.length(), 2) != "\":") {
        start_idx = line.find(parameter, start_idx + parameter.length());
        if (start_idx == std::string::npos) return std::nullopt;
    }

    unsigned int next_idx = start_idx + parameter.length() + 2 + space;
    if (line[next_idx] == '\"' || line[next_idx] == '[') throw std::invalid_argument("");

    unsigned int end_idx = line.find_first_of(",", next_idx); // assuming no parameter contains ,
    std::string result = line.substr(next_idx, end_idx - next_idx);
    if (result == "null") return std::nullopt;
    return stod(result);
}

std::optional<std::list<std::pair<double, double>>> Reader::findCoordinatesList(const std::string& line, const std::string& parameter, unsigned int dim, bool space) {
    size_t start_idx = line.find(parameter);
    if (start_idx == std::string::npos) return std::nullopt;
    while (line.substr(start_idx + parameter.length(), 2) != "\":") {
        start_idx = line.find(parameter, start_idx + parameter.length());
        if (start_idx == std::string::npos) return std::nullopt;
    }

    unsigned int next_idx = start_idx + parameter.length() + 3 + space;
    if (line[next_idx - 1] != '[') throw std::invalid_argument("");

    std::vector<double> vals;
    std::string val = "";
    while (line[next_idx] != '\"' && line[next_idx] != '}') {
        if (line[next_idx] != '0' && line[next_idx] != '1' && line[next_idx] != '2' && line[next_idx] != '3' && line[next_idx] != '4' && line[next_idx] != '5' && line[next_idx] != '6' && line[next_idx] != '7' && line[next_idx] != '8' && line[next_idx] != '9' && line[next_idx] != '.' && line[next_idx] != '-') {
            if (val.length() >= 3) {
                if (val[1] == '.' || (val.length() >= 4 && val[2] == '.') || (val.length() >= 5 && val[3] == '.') || (val.length() >= 6 && val[4] == '.')) {
                    vals.push_back(stod(val));
                }
            }
            val = "";
        } else {
            val += line[next_idx];
        }
        next_idx++;
    }

    std::list<std::pair<double, double>> coordinates_list;
    for (unsigned int i = 0; i < vals.size(); i += dim) {
        coordinates_list.push_back({vals[i], vals[i + 1]});
    }

    return coordinates_list;
}

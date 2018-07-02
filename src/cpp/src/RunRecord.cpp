#include "RunRecord.h"

std::string RunRecord::getTimestamp(){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    //Todo: customise format to suit R best
    return oss.str();
}


RunRecord::RunRecord(std::vector<std::string> columnNames){
    for(auto s : columnNames){
        columns[s] = "";
    }
}

RunRecord::RunRecord(std::vector<std::string> columnNames, std::vector<std::string> defaults){
    if(columnNames.size() != defaults.size())
        throw "You must specify the same number of columns and default values";

    for(auto i = 0; i < columnNames.size(); i++){
        columns[columnNames[i]] = defaults[i];
    }
}

RunRecord::RunRecord(std::map<std::string, std::string> initMap) : columns(initMap){}

std::string RunRecord::setRow(std::string column, std::string value){
    if(columns.find(column) == columns.end())
        throw RowNameException();
    
    auto ret = columns[column];
    columns[column] = value;
    return ret;
}

std::string RunRecord::getValue(std::string column){
    return columns[column];
}

std::string RunRecord::toString(){
    std::string s = "";
    for(auto c : columns)
        s += c.second + "\t";
    return s + "\n";
}

std::string RunRecord::getHeader(){
    std::string s = "";
    for(auto c : columns)
        s += c.first + "\t";
    return s + "\n";
}
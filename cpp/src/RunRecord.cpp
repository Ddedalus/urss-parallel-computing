#include "RunRecord.h"

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (! system(nullptr))
        exit (EXIT_FAILURE);
    
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    if(result.back() == '\n')
        result.pop_back();
    return result;
}

std::string RunRecord::getTimestamp(){
    return exec("date '+%Y-%m-%d %H:%M:%S'");
}


RunRecord::RunRecord(std::vector<std::string> columnNames){
    for(auto s : columnNames){
        columns[s] = "";
    }
    columns["timestamp"] = getTimestamp();
}

RunRecord::RunRecord(std::vector<std::string> columnNames, std::vector<std::string> defaults){
    if(columnNames.size() != defaults.size())
        std::cout<< "You must specify the same number of columns and default values";

    for(auto i = 0; i < columnNames.size(); i++){
        columns[columnNames[i]] = defaults[i];
    }
    columns["timestamp"] = getTimestamp();
}

RunRecord::RunRecord(std::map<std::string, std::string> initMap) : columns(initMap){
    columns["timestamp"] = getTimestamp();
}

std::string& RunRecord::operator[](std::string column){
    return columns[column];
}

std::string RunRecord::toString(){
    std::string s = "";
    for(auto c : columns)
        s += c.second + "\t";
    s[s.size()-1] = '\n';
    return s;
}

std::string RunRecord::getHeader(){
    std::string s = "";
    for(auto c : columns)
        s += c.first + "\t";
    s[s.size()-1] = '\n';
    return s;
}
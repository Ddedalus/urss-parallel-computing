#include "RecordWriter.h"

using namespace std;

int RecordWriter::getNextID(RunRecord run){
    //checkFiles();
    unsigned int c;
    counterfile.seekg(0);
    counterfile >> c;
    counterfile.seekp(0); c++;
    counterfile << c << "\n";
    return c;
}

void RecordWriter::checkFiles(){
    if(!counterfile.good()){
        throw "Can't access counter file!";
    }
    
    if (!file.good()){
        throw "Can't access output file!";
    }
    // cout<< "check passed\n";
}

bool RecordWriter::checkRecordCorrect(RunRecord run){
    if(header.size() == 0)
        return true;
    
    for(auto pair : run.columns){
        auto f = find(header.begin(), header.end(), pair.first);
        if(f == header.end())
            return false;
    }
    if(run.columns.size() < header.size() - 1)  //all except the _id were matched.
        return false;
    return true;
}

std::vector<std::string> RecordWriter::getFileHeader(){
    checkFiles();
    file.seekg(0);
    string s;
    if(!getline(file, s)){
        file.clear();
        return header;
    }   //header vect is empty for now, indicating new file
    
    stringstream str(s);
    while(getline(str, s, delim)){
        header.push_back(s);
    }
    return header;
}

RecordWriter::RecordWriter(string filename, bool run, char delim) : delim(delim){
    ofstream temp(filename, ofstream::app); temp.close();
    // ^ touch as fstream fails to create new file...
    file.open(filename, fstream::in | fstream::out | fstream::app);
    // ^ always append to the end, read and write access
    if(run)
        counterfile.open(run_counter_path, fstream::in | fstream::out); // ::in prevents overwrite
    else
        counterfile.open(instance_counter_path, fstream::in | fstream::out);
    getFileHeader();
}

RecordWriter::~RecordWriter(){
    file.close();
    counterfile.close();
}

string RecordWriter::getString(RunRecord run){
    string ret = "";
    for(auto pair : run.columns)
        if(pair.second == "")
            ret += na + delim;
        else
            ret += pair.second + delim;
    ret[ret.size() - 1] = '\n'; // replace the last, unnecessary delimiter
    return ret;
}

string RecordWriter::getHeader(RunRecord run){
    string ret = "_id";
    for(auto el : run.columns)
        ret += delim + el.first;
    return ret + '\n';
}

void RecordWriter::write(RunRecord run){
    checkFiles();
    if(header.size() == 0){ // need to wirte a new header, run is correct by definition
        file << getHeader(run);
        getFileHeader();
    } else{
        if(checkRecordCorrect(run)){ //check against an existing header
            // just write a new row with unique id
            file<<getNextID(run) << delim;
            file<<getString(run);   
        }       
    }
}
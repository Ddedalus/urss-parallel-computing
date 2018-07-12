#ifndef __RECORDWRITER_H__
#define __RECORDWRITER_H__

#include "public.h"

#include <fstream>
#include <sstream>
#include <map>
#include <exception>
#include <algorithm>
#include <iterator>

#include <RunRecord.h>
// #include <InstanceRecord.h>

/**
 * @brief Use this class to write *Record objects into a .csv file. Every object is written as a single row.
 * This class tires to enforce file integrity by checking the file content (header row) before appending a new record.
 * Each row will be appended by machine-unique id number on write.
 * 
 * TODO: implement instance writting
 * 
 * @exceptions: throws FileIOException if either a counter file (source of unique IDs) or specified output .csv is not available.
 */
class RecordWriter
{
    friend void record_writer_test();
public:
    static std::string run_counter_path;
    static std::string instance_counter_path;
    /**
     * @brief Value written to the file in case empty string occurs in the input Record may be specified here.
     * This setting affects all instances of RecordWritter.
     */
    static std::string na;

private:
    char delim;
    std::string filename;
    std::fstream file, counterfile;
    std::vector<std::string> header; // length 0 indicates new file with no header yet written

    /**
     * @brief Get a next unique id by reading an int from a file located in run/instance_counter_path and
     * incrementing content of the file. Thread behavious is not defined.
     * 
     * @return int 32-bit unsigned counter
     */
    int getNextID(RunRecord& run); // counter file is selected on object creation
    // int getNextID(InstanceRecord inst);
    void checkFiles();  // throws an exception if file access failed
    bool checkRecordCorrect(RunRecord& run);  //if the file has a header, it must match with any new record
    // bool checkRecordCorrect(InstanceRecord run);
public:
    std::vector<std::string> getFileHeader();   //returns empty vector if no valid header is found
    /**
     * @brief Construct a new Record Writer object. Appends the specified file.
     * TODO: implement run as enum type.
     * 
     * @param filename path to a file where records are to be written.
     * @param run Determines which counter file to use. ture=run, false=instance.
     * @param delim delimiter between .csv fields. Defaults to ';', may want ',' or '\t' instead.
     */
    RecordWriter(std::string filename, bool run = true, char delim=';');
    ~RecordWriter();    // make sure files are closed.

    /**
     * @brief Peek what is beeing written to file from such Record object.
     * Doesn't include the ID number which is always written in the first collumn.
     *  
     * @return std::string "$(ID)$(delim)" would be appended to the front of this and written to file.
     */
    std::string getString(RunRecord& run);   
    //TODO: std::string getString(InstanceRecord instance);

    /**
     * @brief Peek what would be a header for this Record.
     */
    std::string getHeader(RunRecord& run);

    /**
     * @brief Append a single record to the file.
     * If this is the first record in a file, header is generated and written first.
     * If a header already exists and is not compatible with the Record, a RowNameException is thrown.
     */
    void write(RunRecord& run);

};


#endif
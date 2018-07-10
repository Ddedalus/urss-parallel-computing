#ifndef __RUNRECORD_H__
#define __RUNRECORD_H__

#include "public.h"

#include <map>
#include <fstream>

// timestamp handling
#include <iomanip>
#include <ctime>
#include <sstream>

class RowNameException: public std::exception{
  virtual const char* what() const throw()
  {
    return "No such column exists in this object. A typo?";
  }
};

/**
 * @brief Stores results of a single algorithm run, prepared to write into .csv file.
 * Capable of storing a map of <key, value> pairs, will add a timestamp.
 * Values may be arbitrarily updated.
 */
class RunRecord
{
    /**
     * @brief Class printing this record into a file.
     */
    friend class RecordWriter;
    friend void run_record_test();
private:
    std::map<std::string, std::string> columns; // map will write in an lexicographic order

    /**
     * @brief Get current time as string
     */
    static std::string getTimestamp();
public:
    /**
     * @brief Construct a new Run Record object with specified column names initialised as empty strings.
     * 
     * @param columnNames list of strings which will constitute header file if written.
     */
    RunRecord(std::vector<std::string> columnNames);
    /**
     * @brief Construct a new Run Record object with specified columns and default values given in the same order.
     * Note: ordering of pairs may not be preserved by this class after constructor joins keys and values.
     * 
     * @param columnNames list of strings which will constitute header file if written.
     * @param defaults list of the same length, specifying default value.
     */
    RunRecord(std::vector<std::string> columnNames, std::vector<std::string> defaults);
    /**
     * @brief Construct a new Run Record object with the map as key-value pairs stored.
     */
    RunRecord(std::map<std::string, std::string> initMap);

    /**
     * @brief Once a RecordWritter object is created, any existing column value may be updated.
     * 
     * @param column Name of the column. New column will be added if not yet in header
     * @return old value found in this column. Overwritten.
     */
    std::string setValue(std::string column, std::string value);
    std::string getValue(std::string column);

    /**
     * @brief Get values of all elements as a single, tab delimited string.
     * Useful for printing, finishes with \n.
     */
    std::string toString();

    /**
     * @brief Get names of all columns as a single, tab delimited string.
     * Useful for printing, finishes with \n. 
     */
    std::string getHeader();

};


#endif

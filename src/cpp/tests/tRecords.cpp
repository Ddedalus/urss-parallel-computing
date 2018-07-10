#include "tests.h"

using namespace std;
void run_record_test(){

    RunRecord r1({"id", "elapsed", "algo"});
    RunRecord r2({"id", "elapsed", "algo"}, {"2", "1.345", "bitset"});
    map<string, string> m1, m2, m3;
    string t = RunRecord::getTimestamp();
    m1["id"] = ""; m1["elapsed"] = ""; m1["algo"] = ""; m1["timestamp"]=t;
    m2["id"] = "2"; m2["elapsed"] = "1.345"; m2["algo"] = "bitset"; m2["timestamp"]=t;
    m3["id"] = "3"; m3["elapsed"] = "23.4"; m3["algo"] = "bfs"; m3["timestamp"]=t;
    RunRecord r3(m3);

    cout << "    --   RunRecord  --  " << endl;
    cout<< "Empty constructor: " << (r1.columns == m1) <<endl;
    cout<< "Defaults constructor: " << (r2.columns == m2) <<endl;
    cout<< "Map constructor: " << (r3.columns == m3) <<endl;

    r1.setValue("id", "1");
    cout << "Set&get value: " << (r1.getValue("id") =="1") << endl;

    cout << "getHeader: " << (r2.getHeader().compare("algo\telapsed\tid\ttimestamp\n")==0) << endl;
    cout << "toString: " << (r2.toString() == "bitset\t1.345\t2\t" + r2.getTimestamp() + "\n") << endl;

}

void record_writer_test(){
    RunRecord r1({"id", "elapsed", "algo"});
    RunRecord r2({"id", "elapsed", "algo"}, {"2", "1.345", "bitset"});
    map<string, string> m1, m2, m3;
    m1["id"] = ""; m1["elapsed"] = ""; m1["algo"] = "";
    m2["id"] = "2"; m2["elapsed"] = "1.345"; m2["algo"] = "bitset";
    m3["id"] = "3"; m3["elapsed"] = "23.4"; m3["algo"] = "bfs";
    RunRecord r3(m3);

    cout << "    --   RecordWriter  --  " << endl;

    RecordWriter::run_counter_path="../output/stub-counter.txt";
    string csvpath = "../output/stub_run.csv";
    remove(RecordWriter::run_counter_path.c_str());
    remove(csvpath.c_str());    // start from scratch every time

    ofstream sc(RecordWriter::run_counter_path);
    sc<<"1\n"; sc.close();  // fake counter

    RecordWriter rw1(csvpath); //supposed to create a file
    
    ifstream check(csvpath);
    cout<< "Created new .csv: " << check.good() << endl;
    check.close();

    cout << "Check files: ";
    try{rw1.checkFiles();}catch(...){cout << "0 ";} cout << "1 ";
    rw1.file.close(); rw1.file << "blah";    
    try{rw1.checkFiles();}catch(...){cout << "1";}
    cout <<" (of 1 1)\n";


    ofstream sr(csvpath);
    sr << "_id;algo;elapsed;id;timestamp\n";sr.close();   // fake csv with header
    RecordWriter rw2(csvpath);
    cout << "Imported header: " << (rw2.header.size() == 5) << endl;
    cout << "getString: "
    << (rw2.getString(r2) == "bitset;1.345;2;" + r2.getValue("timestamp") + "\n") << endl;
    cout << "getHeader: " << (rw2.getHeader(r2) == "_id;algo;elapsed;id;timestamp\n") << endl;
    cout << "checkRecordCorrect: " << rw2.checkRecordCorrect(r2) << endl;
  
    cout << "getNextID: " << (rw2.getNextID(r1) == 2)
        << " " << (rw2.getNextID(r1) == 3) << endl;
    cout << "write: inspect stub_run.csv\n";
    rw2.write(r1); rw2.write(r2); rw2.write(r3);

}
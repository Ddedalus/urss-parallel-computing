#include "tests.h"
#include "RunRecord.h"
#include <map>

using namespace std;
void run_record_test(){

    RunRecord r1({"id", "elapsed", "algo"});
    RunRecord r2({"id", "elapsed", "algo"}, {"2", "1.345", "bitset"});
    map<string, string> m1, m2, m3;
    string t = RunRecord::getTimestamp();
    m1["id"] = ""; m1["elapsed"] = ""; m1["algo"] = ""; m1["timestamp"] = t;
    m2["id"] = "2"; m2["elapsed"] = "1.345"; m2["algo"] = "bitset"; m2["timestamp"] = t;
    m3["id"] = "3"; m3["elapsed"] = "23.4"; m3["algo"] = "bfs"; m3["timestamp"] = t;
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
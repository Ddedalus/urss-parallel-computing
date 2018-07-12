#include "public.h"
#include "tests.h"
#include "tails.h"
#include "Graph.h"

using namespace std;

int main(int argc, char *argv[])
{

    // bitset_test();

    // graph_io_test();

    // run_record_test();

    // record_writer_test();

    // tails_test();

    // graph_test();

    std::fstream file, counterfile;
    counterfile.open(RecordWriter::run_counter_path, fstream::in | fstream::out);

    for (int i = 0; i < 100; i++)
    {
        unsigned int c = 0;
        counterfile.seekg(0);
        if(! (counterfile >> c))
            cout<<"fuck, can't read";
        counterfile.seekp(0);
        c++;
        counterfile << c << "\n";
        counterfile.flush();
        cout<<c<<endl;
    }
    counterfile.close();
    return 0;
}
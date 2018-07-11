#include "public.h"
#include "tests.h"
#include "tails.h"
#include "Graph.h"


using namespace std;

int main(int argc, char* argv[]){

    bitset_test();

    graph_io_test();

    run_record_test();

    record_writer_test();

    tails_test();

    graph_test();

}
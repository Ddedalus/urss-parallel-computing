#include "tests.h"

void tests(){
    std::cout << "   ---   Testing   ---   \n" << std::endl;
    bitset_test();

    graph_io_test();
    std::cout << " --- Finished Testing --- \n" << std::endl;
}
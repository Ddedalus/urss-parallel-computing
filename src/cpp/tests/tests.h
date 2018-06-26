#ifndef __TESTS_MINE_H__
#define __TESTS_MINE_H__

#include <iostream>
#include <string>
#include <stdint.h>
#include <fstream>

void bitset_test();

void bitst_rough();

void graph_io_test();

void all_tests(){
    std::cout << "   ---   Testing   ---   \n" << std::endl;
    bitset_test();
    std::cout << " --- Finished Testing --- \n" << std::endl;
}

#endif
#include "tests.h"

using namespace std;

// void all_tests(){
//     std::cout << "   ---   Testing   ---   \n" << std::endl;
//     bitset_test();
//     graphIO_test();
//     std::cout << " --- Finished Testing --- \n" << std::endl;
// }

void bitset_test(){
    cout << "    --   Bitset  --  " << endl;
    Bitset b = Bitset(3), l = Bitset(70);

    b.set(2); l.set(67, true);
    std::cout << "Set&get: " << (b.get(2)==true) << " " << (l.get(67)==true) << endl;

    l.set(55, false);
    std::cout << "Unset: " << (l.get(55) == false) << endl;

    Bitset l2(70);
    l2.set(2); l.set(67);
    auto u = l2 + l;
    cout << "Union: " << (u.get(2)==1) <<" "<< (u.get(67)==1) <<" "<<  (u.get(23)==0) << endl;
    
    l2 += l;
    cout << "Inplace union: " << (l2.get(67)==1) <<" "<<
                                 (l2.get(2)==1) <<" "<<
                                 (l2.get(1)==0)<< endl;

    cout<<"Count: "<<(l2.count()==2)<<endl;

    cout<<"Diff_size: "<<(l2.diff_size(l)==1)<<endl;
     
    cout<<"Diff: "<<(l2.diff(l).get(67)==0 )<<" "<<
                    (l2.diff(l).get(2)==1) <<" "<<
                    (l2.diff(l).count()==1)<<endl;

    l2.copy_from(l);
    cout<<"Copy_from: "<< (l2.count()==1)<<" "<< (l2.get(67)==1) << endl;

    b.set(0); b.set(1);
    for(int i = 0; i < 70; i++) l.set(i);   //should be filled now
    cout<<"is_full: "<<(b.is_full()) <<" "<<
                        (!l2.is_full()) <<" "<<
                        (l.is_full())<< endl;
    // for(int i = 0; i < 70; i++) cout<<l.get(i) << " ";

}

void bitset_rough(){
    cout<< "All 1s in hex: " << hex << ~0ul << endl;
    cout<< "2^63 in hex: " << hex << (1ul << 63) << endl;
}
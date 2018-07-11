#include "tests.h"

using namespace std;
void graph_test(){
    
    cout << "\n     --    Graph   --   " << std::endl;
    cout << "Densely Numbered Graph:\n";
    denselyNumberedGraph dg(10);
    dg.addEdge(1, 2);
    dg.addEdge(2, 3);
    cout<< "Counting: " <<(dg.nodes() == 10) << " " << (dg.edges() == 2) << endl;
    vector<uint> dg2 = {1, 3};
    cout<< "Edge access: " << (dg[2] == dg2) << " " << (dg[5].empty()) << endl;

    cout << "Sparsely Numbered Graph:\n";
    sparselyNumberedGraph sg;
    sg.addEdge(1, 2);
    sg.addEdge(2, 3);
    cout<< "Counting: " <<(sg.nodes() == 3) << " " << (sg.edges() == 2) << endl;
    vector<uint> sg2 = {1, 3};
    cout<< "Edge access: " << (sg[2] == sg2) << " " << (sg[5].empty()) << endl;
    cout<< "Erase mark: " << (sg.getEraseMark() == 4);
    sg[2][0] = sg.getEraseMark();
    sg.eraseMarked();
    cout << " " << (sg[2].size() == 1) << endl;

    unordered_map<nodeID, nodeID> tr;
    denselyNumberedGraph new_d(sg, tr);
    cout << "Copy to dense: " << (new_d.nodes() == 3) << endl;

    sparselyNumberedGraph new_s(new_d);
    cout << "Copy to sparse: " << (new_s.nodes()== 3) << endl;
}

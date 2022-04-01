#include <iostream>
#include <vector>
#include <functional>
#include "utils.cpp"
#include "odd_even_sort.cpp"
#include "utimer.cpp"

using namespace std;

int main(int argc, char **argv) {
  
    if(argc != 6) {
        cout << "Usage is: " << argv[0] << " v-len max-v seed debug n-worker " << endl;
        return -1;
    }

    int n     = atoi(argv[1]);
    int max   = atoi(argv[2]);
    int seed  = atoi(argv[3]);
    bool debug = (atoi(argv[4]) == 0 ? false : true);
    int nw    = atoi(argv[5]);

    vector<int> v_seq = create_random_vector(n, max, seed);

    if(debug)
        read_vector(v_seq);

    // Sequential
    long t_seq;
    {
        utimer oesort("Sequential odd even sort", &t_seq);
        odd_even_sort_sequential(v_seq);
    }
    if(debug)
        read_vector(v_seq);

    cout << (is_sorted(v_seq.begin(), v_seq.end())? "The vector is sorted in Sequential! " : "The vector is NOT sorted in Sequential! ") << endl;

    vector<int> v_par = create_random_vector(n, max, seed);

    // Parallel
    long t_par;
    {
        utimer oesort("Parallel odd even sort", &t_par);
        odd_even_sort_parallel(v_par, nw);
    }

    if(debug)
        read_vector(v_par);

    cout << (is_sorted(v_par.begin(), v_par.end())? "The vector is sorted in Parallel! " : "The vector is NOT sorted in Parallel! ") << endl;

    cout << "Speedup t_seq / t_par(nw) = " << ((float) t_seq) / ((float) t_par) << endl;

    return 0;
}
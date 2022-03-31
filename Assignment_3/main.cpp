# include <iostream>
# include <vector>
# include <functional>
# include "utils.cpp"
# include "odd_even_sort.cpp"

using namespace std;

int main(int argc, char **argv) {
  
    if(argc != 6) {
        cout << "Usage is: " << argv[0] << " v-len max-v seed thread/async n-worker " << endl;
        return -1;
    }

    int  n      = atoi(argv[1]);
    int  max    = atoi(argv[2]);
    int  seed   = atoi(argv[3]);
    bool th     = atoi(argv[4]);
    int  nw     = atoi(argv[5]);

    vector<int> v = create_random_vector(n, max, seed);

    read_vector(v);

    odd_even_sort(v, nw);

    return 0;
}
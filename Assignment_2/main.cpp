# include <iostream>
# include <vector>
# include <functional>
# include "utils.cpp"
# include "map.cpp"

int main(int argc, char **argv) {
  
    if(argc != 7) {
        cout << "Usage is: " << argv[0] << " v-len max-v seed n-worker chunks/round-robin thread/async " << endl;
        return(0);
    }

    int  n      = atoi(argv[1]);
    int  max    = atoi(argv[2]);
    int  seed   = atoi(argv[3]);
    int  nw     = atoi(argv[4]);
    bool chunks = atoi(argv[5]);
    bool th     = atoi(argv[6]);

    vector<float> v = create_random_vector(n, max, seed);

    function<float(float)> f = [] (float x) { active_delay(1); return 2 * x; };

    vector<float> res = map(v, f, nw, chunks, th);

    return 0;
}
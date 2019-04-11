#include <vector>

using namespace std;

class Perceptron {
    private:
        float weight;
        float threshold;
        int expectedOutput;
    public:
        Perceptron(vector<float>, int);
    
        float weightedSum(vector<float>);
        int activationFunction(float);
};

#include <iostream>
#include <vector>
#include "Perceptron.h"

using namespace std;

/*constructor initializes perceptron and runs the full perceptron algorithm*/
//end of constructor outputs the expected value
Perceptron::Perceptron(vector<float> inputs, int k) {
    expectedOutput = 0.0;
    //weight is 1/n
    weight = 1.0/inputs.size();
    threshold = k;
    
    //result of weighted sum is put to activation function which gives expected output
    expectedOutput = activationFunction(weightedSum(inputs));
    
    cout << "Expected Output: " << expectedOutput << endl;
}

//weight sum takes all inputs, multiplies them by weight, and adds them together
//send weight is same for every input algebra was slightly different
float Perceptron::weightedSum(vector<float> inputs) {
    float weightedSum = 0.0;
    for(int i = 0; i < inputs.size(); i++)
        weightedSum += inputs[i];
    
    weightedSum = weightedSum*(weight);
    return weightedSum;
}

//activation function uses binary step to determine expected output
int Perceptron::activationFunction(float sum) {
    double binary_step = 0.0;
    threshold *= weight;
    
    binary_step = sum - threshold;
    
    if(binary_step < 0)
        return 0;
    else
        return 1;
}

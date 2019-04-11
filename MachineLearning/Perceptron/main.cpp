#include <iostream>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Perceptron.h"

using namespace std;

int main() {
    //create a table of inputs to run through the perceptron
    //the table will have a max of 20 values and a minimum of 10
    //all values will either be ones or zeros
    srand(time(NULL));
    
    int numberOfInputs = rand()%(20-10 + 1) + 10;
    
    vector<float> inputs;
    inputs.resize(numberOfInputs);
    
    for(int i = 0; i < numberOfInputs; i++)
        inputs[i] = rand() % 2;
    
    int numberOfOnes = 0, numberOfZeros = 0;
    
    for(int i = 0; i < inputs.size(); i++) {
        if(inputs[i] == 1.0)
            numberOfOnes++;
        else
            numberOfZeros++;
    }
    
    //have the user input what they would like k to be
    int k = 0;
    bool valueGood = false;
    cout << "Please choose the number of 'k' values (value must be between 1-"
         << numberOfInputs << "): ";
    cin >> k;

    //make sure value is between 1 and what user chooses as k
    while(!valueGood) {
        if(k < 1 || k > numberOfInputs) {
            cout << "Value is not between 1 and "
                 << numberOfInputs << ". Please enter a value between 1 and "
                 << numberOfInputs << ": ";
            cin >> k;
        }
        else {
            valueGood = true;
            //if true, put inputs through perceptron
            Perceptron perceptron(inputs, k);
        }
    }
    
    cout << "Ran" << endl;
    return 0;
    
}

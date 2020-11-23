//*****************************
// Jacob Saunders
//*****************************

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "json.hpp"
#include "priorityqueue.h"

nlohmann::json inputFile(const std::string& fileName);


int main(int argc, char** argv) {

    //checks for correct input
    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " file1.json";
        return 1;
    }

    //creates input and output JSON objects
    nlohmann::json commandFile = nullptr;
    nlohmann::json outputFile;

    //inputs from json file to json object
    try {
        commandFile = inputFile(argv[1]);
    }catch (std::invalid_argument & e){
        std::cerr << e.what() << '\n';
        return 1;
    }


    //gets numOperations and maxHeapSize
    int numOperations = commandFile["metadata"]["numOperations"];
    int maxHeapSize = commandFile["metadata"]["maxHeapSize"];

    //creates priority queue
    PriorityQueue priorityQueue(maxHeapSize);
    //creates variables
    int key;
    std::string operation;
    //finds number of digits in numOperations
    int numDigits = (int) floor(log10((double) numOperations)) + 1;
    int numDigitsCurr;
    
    //iterates for each operation
    for(int i = 1; i <= numOperations; i++)
    {
        //identifies the number of digits in the current operation
        if(i == 0)
        {
            numDigitsCurr = 1;
        }else
        {
            numDigitsCurr = (int) floor(log10((double) i)) + 1;
        }
        
        //start making key name
        std::string currentOp = "Op";
        //adds a zero to make sure the number of digits in the key is correct
        for(int j = numDigitsCurr; j < numDigits; j++)
        {
            currentOp += "0";
        }
        //add number to key
        currentOp += std::to_string(i);
        //gets operation for the created key
        operation = commandFile[currentOp]["operation"];
        //identifies type of operation and does it
        if (operation.compare("insert") == 0)
        {
            key = commandFile[currentOp]["key"];
            priorityQueue.insert(key);
        }else if(operation.compare("removeMin") == 0)
        {
            priorityQueue.removeMin();
        }
        
        
    }

    //gets the ending priority queue
    outputFile = priorityQueue.JSON();
    //adds numOperations and maxHeapSize to output
    outputFile["metadata"]["maxHeapSize"] = maxHeapSize;
    outputFile["metadata"]["numOperations"] = numOperations;


    //outputs the json object
    std::cout << outputFile.dump(2);

    return 0;

}



nlohmann::json inputFile(const std::string& fileName)
{
    //creates instream
    std::ifstream inFile;
    //opens file
    inFile.open(fileName);

    nlohmann::json newObject;
    //checks if file is open
    if(!inFile.is_open()){
        throw std::invalid_argument("Error: " + fileName + " cannot be opened");
    }
    //moves file to json object
    inFile >> newObject;
    //closes file
    inFile.close();
    //returns json object
    return newObject;
}
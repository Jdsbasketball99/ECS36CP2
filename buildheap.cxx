#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "json.hpp"
#include "priorityqueue.h"

nlohmann::json inputFile(const std::string& fileName);


int main(int argc, char** argv) {

    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " file1.json";
        return 1;
    }

    nlohmann::json commandFile = nullptr;
    nlohmann::json outputFile;

    try {
        commandFile = inputFile(argv[1]);
    }catch (std::invalid_argument & e){
        std::cerr << e.what() << '\n';
        return 1;
    }



    int numOperations = commandFile["metadata"]["numOperations"];
    int maxHeapSize = commandFile["metadata"]["maxHeapSize"];


    PriorityQueue priorityQueue(maxHeapSize);

    int key;
    std::string operation;
    int numDigits = (int) floor(log10((double) numOperations)) + 1;
    int numDigitsCurr;
    

    for(int i = 1; i <= numOperations; i++)
    {
        
        if(i == 0)
        {
            numDigitsCurr = 1;
        }else
        {
            numDigitsCurr = (int) floor(log10((double) i)) + 1;
        }
        

        std::string currentOp = "Op";
        for(int j = numDigitsCurr; j < numDigits; j++)
        {
            currentOp += "0";
        }
        currentOp += std::to_string(i);
        
        operation = commandFile[currentOp]["operation"];
        
        if (operation.compare("insert") == 0)
        {
            key = commandFile[currentOp]["key"];
            priorityQueue.insert(key);
        }else if(operation.compare("removeMin") == 0)
        {
            priorityQueue.removeMin();
        }
        
        
    }


    outputFile = priorityQueue.JSON();
    
    outputFile["metadata"]["maxHeapSize"] = maxHeapSize;
    outputFile["metadata"]["numOperations"] = numOperations;

    std::cout << outputFile.dump(2);

    return 0;

}



nlohmann::json inputFile(const std::string& fileName)
{
    std::ifstream inFile;

    inFile.open(fileName);

    nlohmann::json newObject;

    if(!inFile.is_open()){
        throw std::invalid_argument("Error: " + fileName + " cannot be opened");
    }

    inFile >> newObject;

    inFile.close();

    return newObject;
}
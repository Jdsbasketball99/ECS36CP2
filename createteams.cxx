#include <iostream>
#include <string>
#include "json.hpp"
#include "priorityqueue.h"
#include <fstream>
#include <math.h>
#include <vector>

nlohmann::json inputFile(const std::string& fileName);

int main(int argc, char** argv) {

    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " file1.json";
        return 1;
    }

    nlohmann::json teamData;
    nlohmann::json finalTeams;


    try {
        teamData = inputFile(argv[1]);
    }catch (std::invalid_argument & e){
        std::cerr << e.what() << '\n';
        return 1;
    }
    
    int numPlayers = teamData["metadata"]["numPlayers"];
    int max = (numPlayers*(numPlayers-1))/2;

    int playerOne;
    int playerTwo;
    double winPercentage;
    double key;

    PriorityQueue teamDataQueue(max);

    for(int i = 0; i < max; i++)
    {
        playerOne = teamData["teamStats"][i]["playerOne"];
        playerTwo = teamData["teamStats"][i]["playerTwo"];
        winPercentage = teamData["teamStats"][i]["winPercentage"];

        key = fabs(winPercentage - 50);

        teamDataQueue.insert(std::make_pair(key, std::make_pair(playerOne, playerTwo)));

    }

    int teamsDone = 0;
    KeyValuePair currentMin;
    std::vector<int> playersUsed;

    while(teamsDone < numPlayers/2)
    {
        currentMin = teamDataQueue.removeMin();

        bool used = false;
        for(unsigned int i = 0; i < playersUsed.size(); i++)
        {
            if(playersUsed[i] == currentMin.second.first)
            {
                used = true;
            }
            if(playersUsed[i] == currentMin.second.second)
            {
                used = true;
            }
        }
        
        if(!used)
        {
            finalTeams["teams"][teamsDone] = currentMin.second;
            playersUsed.push_back(currentMin.second.first);
            playersUsed.push_back(currentMin.second.second);
            teamsDone++;
        }
        
    }


    std::cout << finalTeams.dump(2);

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
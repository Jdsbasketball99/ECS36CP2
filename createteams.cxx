//*****************************
// Jacob Saunders
//*****************************
#include <iostream>
#include <string>
#include "json.hpp"
#include "priorityqueue.h"
#include <fstream>
#include <math.h>
#include <vector>

nlohmann::json inputFile(const std::string& fileName);

int main(int argc, char** argv) {
    //checks for correct input
    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " file1.json";
        return 1;
    }
    //creates input and output json objects
    nlohmann::json teamData;
    nlohmann::json finalTeams;

    //inputs the file
    try {
        teamData = inputFile(argv[1]);
    }catch (std::invalid_argument & e){
        std::cerr << e.what() << '\n';
        return 1;
    }
    //gets numPlayers and calculates max from json object
    int numPlayers = teamData["metadata"]["numPlayers"];
    int max = (numPlayers*(numPlayers-1))/2;
    //creates variables
    int playerOne;
    int playerTwo;
    double winPercentage;
    double key;
    //creates priority queue
    PriorityQueue teamDataQueue(max);

    //iterates through each team stat
    for(int i = 0; i < max; i++)
    {
        //gets data from team stats
        playerOne = teamData["teamStats"][i]["playerOne"];
        playerTwo = teamData["teamStats"][i]["playerTwo"];
        winPercentage = teamData["teamStats"][i]["winPercentage"];

        //creates the key based on winPercentage
        key = fabs(winPercentage - 50);

        //puts data into the queue
        teamDataQueue.insert(std::make_pair(key, std::make_pair(playerOne, playerTwo)));

    }
    //creates variables 
    int teamsDone = 0;
    KeyValuePair currentMin;
    std::vector<int> playersUsed;
    //checks how man teams were created
    while(teamsDone < numPlayers/2)
    {
        //gets the team with closest to 50% win percentage
        currentMin = teamDataQueue.removeMin();

        bool used = false;
        //checks if the players are already assigned to a team
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
        //if not assigned to team
        if(!used)
        {
            //add to final teams and to vector of players that have been used already
            finalTeams["teams"][teamsDone] = currentMin.second;
            playersUsed.push_back(currentMin.second.first);
            playersUsed.push_back(currentMin.second.second);
            //increase teamsDone counter
            teamsDone++;
        }
        
    }

    //output finalTeams
    std::cout << finalTeams.dump(2);

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
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

ifstream inFile("System.in");
ofstream outFile("System.out");
map<string, vector<string>> mapNodeAndScales;

/**
 @function: Helps to read system.in file 
 @return map of node and scales.
 key: string as node
 value: vector of strings as scales
 e.g. B1 => ["10", "B2"]
*/
map<string, vector<string>> readNodeScalesFromSystem() {
    
    string strScaleInput;
    map<string, vector<string>> mapNodeAndScale;
    while (getline (inFile, strScaleInput)) {
        string token = "";
        string key = "";
        istringstream tokenStream(strScaleInput);
        vector<string> scales = {};
        while (getline(tokenStream, token, ','))
        {
            if(key == "") {
                key = token;
            } else {
                scales.push_back(token);
            }
            mapNodeAndScale[key] = scales;
        }
    }
    inFile.close();
    return mapNodeAndScale;
}

/**
 @function: Helps to write System.out file
 @input: map<string, vector<string>>::iterator
To iterate map
 */
void writeBalacedScalesToSystem(map<string, vector<string>>::iterator &rootNode) {
    while(rootNode!=mapNodeAndScales.end()) {
        outFile << rootNode->first << "," << rootNode->second[0] << "," << rootNode->second[1] << "\n";
        rootNode++;
    }
    outFile.close();
}

/**
 @function: Balance the scales of node considering input always in incresing order. like B1, B2, B3,...BN
 @input: map<string, vector<string>>::iterator
 Recursively we are calculating Node value from B1 to BN
 Considering B1 as Node and ( 10, B2 ) as Left and Right node of B1 respectively.
 */
int balanceScales(map<string, vector<string>>::iterator &rootNode) {
    
    int leftScale = -1, rightScale = -1, diffenceOfScale = 0;

    // consider second[0] as left node of root
    std::map<string, vector<string>>::iterator leftNode = mapNodeAndScales.find(rootNode->second[0]);
    
    if(leftNode != mapNodeAndScales.end()) {
        leftScale = balanceScales(leftNode);
    } else {
        stringstream scale(rootNode->second[0]);
        scale >> leftScale;
    }

    //consider second[1] as right node of root
    std::map<string, vector<string>>::iterator rightNode = mapNodeAndScales.find(rootNode->second[1]);
    if(rightNode != mapNodeAndScales.end()) {
        rightScale = balanceScales(rightNode);
    } else {
        stringstream scale(rootNode->second[1]);
        scale >> rightScale;
    }

    // Calculate differnce of scales and assign new scales to left and right of rootNode
    if(leftScale > rightScale) {
        diffenceOfScale = leftScale - rightScale;
        rootNode->second[0] = "0"; // left
        rootNode->second[1] = to_string(diffenceOfScale); //right
    } else if(rightScale > leftScale) {
        diffenceOfScale = rightScale - leftScale;
        rootNode->second[1] = "0"; // left
        rootNode->second[0] = to_string(diffenceOfScale); //right
    } else {
        // if both are same
        rootNode->second[1] = "0"; // left
        rootNode->second[0] = "0"; // right
    }
    
    // Adding one as ever root has its own 1 scale
    return diffenceOfScale + rightScale + leftScale + 1;
}

int main() {

    // As per desccription of program we will 
    // The scales are in alphebetic order [A...Z and 0..9] i.e incresing order so we can use map 
    // in map we have sorted data like B1, B2, .... BN
    mapNodeAndScales = readNodeScalesFromSystem();
    map<string, vector<string>>::iterator itr = mapNodeAndScales.begin();
     
    balanceScales(itr);
    
    writeBalacedScalesToSystem(itr);
    return 0;
}
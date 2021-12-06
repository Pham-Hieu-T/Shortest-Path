/*Hieu Pham
UTA ID 1001631515
run: g++ find_route.cpp
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;



class Node{
    private:
        string city;
        int distance;
        int level;
        int totalCost;
        Node* tracking;
    public:
        void setCity(string s){
            city = s;
        }
        void setDistance(int d){
            distance = d;
        }
        void setLevel(int l){
            level = l;
        }
        void setTracking(Node * t){
            tracking = t;
        }
        void setTotalCost(int tc){
            totalCost = tc;
        }

        string getCity() { return city;}
        int getDistance() { return distance; }
        int getLevel() { return level; }
        int  getTotalCost(){ return totalCost;}
        Node * getTracking() { return tracking;}

};

string start;
string goal;
int maxExpended = 0;
int maxGenerated = 0;
int nodesPopped = 0;

vector<Node *> fringe;
vector<string*> cityMap;
vector<string> nodesVisted;
vector<string*> heuristic;
Node* closedNode;

// Get informations from an input file.
bool getMap(string fileName){
    char delim = ' ';
    string line;
    int countWord;
    ifstream myfile(fileName);
    if(!myfile){
        cout <<"Invalid File.\n";
        return false;
    }
    getline(myfile,line);
    while (line.compare("END OF INPUT") != 0 && myfile){
        countWord = 0;
        istringstream ss(line);
        string token;
        string *route = new string[3];
        while (getline(ss,token,delim)){ 
            route[countWord++] = token;
        }
        cityMap.push_back(route);
        getline(myfile,line);
    }
    myfile.close();
    return true;
}
bool getHeuristicInfo(string fileNameHeuristic){
    char delim = ' ';
    string line;
    int countWord;
    ifstream myfile(fileNameHeuristic);
    if(!myfile){
        cout <<"Invalid File.\n";
        return false;
    }
    getline(myfile,line);
    while (line.compare("END OF INPUT") != 0 && myfile){
        countWord = 0;
        istringstream ss(line);
        string token;
        string *heuInfo = new string[2];
        while (getline(ss,token,delim)){ 
            heuInfo[countWord++] = token;
        }
        heuristic.push_back(heuInfo);
        getline(myfile,line);
    }
    myfile.close();
    return true;
}


bool compareNodeDistance(Node *node1, Node *node2){
    return node1->getDistance() < node2->getDistance();
}
bool compareNodeTotalCost(Node *node1, Node *node2){
    return node1->getTotalCost() < node2->getTotalCost();
}
bool isNodeVisited(Node * currentNode){
    if(nodesVisted.empty())
        return false;
    else{
        for(int i = 0;  i < nodesVisted.size(); i++)
            if(nodesVisted[i].compare(currentNode->getCity()) ==0)
                return true;
    }
    return false;

}
int getHeuristic(string city){
    for(int i = 0; i < heuristic.size(); i++){
        if(heuristic[i][0].compare(city) ==0)
            return stoi(heuristic[i][1]);
    }
    return 0;
}
int expandNode(bool hasHeuristic){
    if(fringe.empty()) return -1;

    Node *nodeExpanding = fringe.front();
    string cityExpanding = nodeExpanding->getCity();
    if(goal.compare(cityExpanding) == 0){
        closedNode = fringe.front();
        fringe.erase(fringe.begin());
        nodesPopped++;
        return 1;
    }
    else if( isNodeVisited(nodeExpanding)){
        //cout << cityExpanding << " is visited.\n";
        //cout << "Popping Node " << fringe.front()->getCity() << endl;
        fringe.erase(fringe.begin());
        
        nodesPopped++;
        return 0;
    }
    else{
        //cout << cityExpanding << " is being expanded.\n";
        for(int i = 0; i < cityMap.size(); i++){
            if(cityExpanding.compare(cityMap[i][0]) ==0){
                int newDistance = nodeExpanding->getDistance()+stoi(cityMap[i][2]);
                int newLevel = nodeExpanding->getLevel() + 1;
                Node * newNode = new Node;
                newNode->setCity(cityMap[i][1]);
                newNode->setDistance(newDistance);
                newNode->setLevel(newLevel);
                newNode->setTracking(nodeExpanding);
                if(hasHeuristic){
                    newNode->setTotalCost(newDistance+getHeuristic(cityMap[i][1]));
                }

                fringe.push_back(newNode);
                maxGenerated++;
            }
            if(cityExpanding.compare(cityMap[i][1]) ==0){
                int newDistance = nodeExpanding->getDistance()+stoi(cityMap[i][2]);
                int newLevel = nodeExpanding->getLevel() + 1;
                Node * newNode = new Node;
                newNode->setCity(cityMap[i][0]);
                newNode->setDistance(newDistance);
                newNode->setLevel(newLevel);
                newNode->setTracking(nodeExpanding) ;
                if(hasHeuristic){
                    newNode->setTotalCost(newDistance+getHeuristic(cityMap[i][0]));
                }


                fringe.push_back(newNode);
                maxGenerated++;
            }
        }
        nodesVisted.push_back(cityExpanding);
        fringe.erase(fringe.begin());
        
        nodesPopped++;
        maxExpended++;


    }
    return 0;

}


int uninformSearch(){

    int result = 0;

    while(result==0){
        sort(fringe.begin(),fringe.end(),compareNodeDistance);
        result = expandNode(false);
    }
    return result;
}
int informSearch(){
    int result = 0;

    while (result==0){
        sort(fringe.begin(),fringe.end(),compareNodeTotalCost);
        result = expandNode(true);
    }
    

    return result;
}


int main(int argc, char** argv){
    int found;
    if(argc < 4 || argc > 6){
        printf("Invalid run format: <find_route> input_filename origin_city destination_city heuristic_filename\n");
        return EXIT_FAILURE;
    }
    
    if(getMap(argv[1])){
        start = argv[2];
        goal = argv[3];
        Node * begin = new Node;
        begin->setCity(start);
        begin->setDistance(0);
        begin->setLevel(0);
        begin->setTracking((Node*) NULL);

        fringe.push_back(begin);
        maxGenerated++;


        if(argc == 4) found = uninformSearch();
        else{
            begin->setTotalCost(0);
            cout << argv[4]<< endl;
            if(getHeuristicInfo(argv[4]))
                found=informSearch();
        }

        
        cout << "Nodes Popped: "<<nodesPopped<< endl;
        cout << "Nodes Expended: "<< maxExpended<< endl;
        cout << "Nodes Generated: " << maxGenerated<< endl;
        cout << "Distance: ";
        if(found == -1)
            cout << "Infinity"<< endl;
        else
            cout <<closedNode->getDistance() <<".0 km"<<endl;
        cout << "Route: \n";
        if(found == 1){
            while(closedNode->getTracking() != (Node *)NULL){
                cout << closedNode->getTracking()->getCity() << " to " << closedNode->getCity() << " "
                <<closedNode->getDistance()-closedNode->getTracking()->getDistance()<<endl;
                closedNode = closedNode->getTracking();

            }
        }
        else
            cout << "None"<<endl;
    }

    return EXIT_SUCCESS;

}
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

struct Node {
  vector<char> symbols;
  int weight;
  bool isNull;
  
  bool operator<( const Node& right ) const {
   return weight < right.weight;
  }
};

bool isExistSymbol(vector<char> symbols, char symbol) {
  return find(symbols.begin(), symbols.end(), symbol) != symbols.end();
}

map<char, int> makeWeightFromFile(fstream &fin){
  char c;
  map<char, int> result;
  while(fin.get(c)){
    if (result.find(c) == result.end()) {
      result[c] = 1;
    } else {
      result[c] += 1;
    }
  }
  return result;
}

void sortNode(vector<Node> &nodes){
  sort(nodes.begin(), nodes.end());
}

int main(int argc, char *argv[]){
  // get file name from command line arg
  //string fileName = argv[1];
  
  string fileName = "./dump.sql";
 
  // open file 
  fstream fin(fileName);
  if(!fin){
      cerr << "can not open " << fileName << endl;
      return 1;
  }

  // calc weight 
  auto nodes = makeWeightFromFile(fin);
  // map to vector
  vector<Node> leaves;
  for (auto iter = nodes.begin(); iter != nodes.end(); ++iter){
    vector<char> symbols;
    symbols.push_back(iter->first);
    Node node;
    node.symbols = symbols;
    node.weight = iter->second;
    leaves.push_back(node);
  }
 
  sortNode(leaves);
  reverse(leaves.begin(), leaves.end());
  // make huffman code tree
  for (auto i = 0; i < leaves.size() - 1 ; i++){
    cout << leaves[i].symbols[0] << " : " << leaves[i].weight << endl;
  }
  
  fin.close();
  return 0;
}


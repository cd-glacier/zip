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

Node *getNode(vector<Node> vecs, char c){
  for (int i = 0; i < vecs.size(); i++){
    auto symbols = vecs[i].symbols;
    // if c exist
    if (isExistSymbol(symbols, c)) {
      return &vecs[i];
    }
  }
  return nullptr;
}

vector<Node> makeWeightFromFile(fstream &fin){
  char c;
  vector<Node> result;
  while(fin.get(c)){
    auto node = getNode(result, c);
    if (node == nullptr) {
      vector<char> v;
      v.push_back(c);
      Node newNode = {v, 1};
      result.push_back(newNode);
    } else {
      Node *node = getNode(result, c);
      node->weight = node->weight + 1;
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
  
  string fileName = "./out";
 
  // open file 
  fstream fin(fileName);
  if(!fin){
      cerr << "can not open " << fileName << endl;
      return 1;
  }

  // calc weight 
  auto leaves = makeWeightFromFile(fin);
 
  /* 
  sortNode(leaves);
  reverse(leaves.begin(), leaves.end());
  // make huffman code tree
  for (auto i = 0; i < leaves.size() - 1 ; i++){
    cout << leaves[i].symbols[0] << endl;
  }
  */
  
  fin.close();
  return 0;
}


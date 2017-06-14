#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

class Node{
  private:
    Node *left;
    Node *right;
    Node *parent;
    int weight;
    vector<char> symbols;
  public:
    int getWeight(){
      return weight;
    }
    vector<char> getSymbols(){
      return symbols;
    }
    void setWeight(int w){
      weight = w;
    }
  Node(vector<char> s, int w){
    symbols = s;
    weight = w;
  }
  Node(Node *l, Node *r, vector<char> s, int w){
    left = l;
    right = r;
    symbols = s;
    weight = w;
  }
};

class CodeTree{
  private:
    Node *node;
    vector<char> symbols;
    int weight;
  public:
    vector<char> getSymbols(){
      return symbols;
    };
    int getWeight(){
      return weight; 
    };
  CodeTree(Node *n, vector<char> s, int w) {
    node = n;
    symbols = s;
    weight = w;
  }
};

bool isExistSymbol(vector<char> symbols, char symbol) {
  return find(symbols.begin(), symbols.end(), symbol) != symbols.end();
}

Node getNode(vector<Node> v, char c){
  for (auto itr : v){
    auto symbols = itr.getSymbols();
    // if c exist
    if (isExistSymbol(symbols, c)) {
      return itr;
    }
  }
}

vector<Node> makeWeightMap(fstream &fin){
  char c;
  vector<Node> result;
  while(fin.get(c)){
    auto node = getNode(result, c);
    if (isExistSymbol(node.getSymbols(), c)) {
      vector<char> v;
      v.push_back(c);
      auto newNode = new Node(v, 1);
      result.push_back(*newNode);
    } else {
      node.setWeight(node.getWeight() + 1);
    }
  }
  return result;
}

vector<Node> sort(vector<Node> &nodes){
  vector<Node> tmp = nodes;
  vector<Node> result;
  while (tmp.size() != 0) {
    int max = 0;
    int maxIndex = -1;
    vector<char> max_key;
    for (auto n : tmp) {
      maxIndex++;
      if (max <= n.getWeight()) {
        max = n.getWeight();
      }
    }
    result.push_back(tmp[maxIndex]);
    tmp.erase(tmp.begin() + 0);
  }
  return result;
}

int main(int argc, char *argv[]){
  // get file name from command line arg
  string fileName = argv[1];
 
  // open file 
  fstream fin(argv[1]);
  if(!fin){
      cerr << "can not open " << fileName << endl;
      return 1;
  }

  // calc weight 
  auto nodes = makeWeightMap(fin);
  auto leaves = sort(nodes);
  reverse(leaves.begin(), leaves.end());
  // make huffman code tree
  for (auto i = 0; i < leaves.size() - 1 ; i++){
    cout << leaves[i].getSymbols()[0] << endl;
  }
  
  fin.close();
  return 0;
}


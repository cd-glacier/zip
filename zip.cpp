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
  Node *left;
  Node *right;
  Node *parent;
  
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

  // make huffman code tree
  //要素が1つになるまで繰り返す
  while(leaves.size() != 1) {
    Node* tmp0 = new Node();
    Node* tmp1 = new Node();
    tmp0->weight = leaves[0].weight;
    tmp0->symbols = leaves[0].symbols;
    tmp0->left = leaves[0].left;
    tmp0->right = leaves[0].right;
    tmp1->weight = leaves[1].weight;
    tmp1->symbols = leaves[1].symbols;
    tmp1->left = leaves[1].left;
    tmp1->right = leaves[1].right;
    
    //親ノードを作る
    Node parent; 
    parent.left = tmp0;
    parent.right = tmp1;
    parent.weight = leaves[0].weight + leaves[1].weight;
    // 要素を足す
    copy(leaves[0].symbols.begin(), leaves[0].symbols.end(), back_inserter(parent.symbols));
    copy(leaves[1].symbols.begin(), leaves[1].symbols.end(), back_inserter(parent.symbols));
    //leavesの子ノード二つを削除して
    leaves.erase(leaves.begin());
    leaves.erase(leaves.begin());
    //親ノードをpush_back
    leaves.push_back(parent);
    //sort
    sortNode(leaves);
    delete tmp0;
    delete tmp1;
  }
  
  //参照を渡しているのに消してるからだめ
  
  cout << leaves[0].weight << endl;
  
  fin.close();
  return 0;
}


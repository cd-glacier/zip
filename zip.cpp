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

bool isExistSymbol(char symbol, vector<char> *symbols) {
  return find(symbols->begin(), symbols->end(), symbol) != symbols->end();
}

map<char, int> makeWeightFromFile(fstream &fin){
  char c;
  map<char, int> result;
  while(fin.get(c)){
    // みつからなかったら
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

vector<char> *encode(char symbol, Node *tree, vector<char> *result) {
  if(tree->left->symbols.size() == 1 && tree->left->symbols[0] == symbol){
    result->push_back('0');
    return result; 
  } else if (tree->right->symbols.size() == 1 && tree->right->symbols[0] == symbol) {
    result->push_back('1');
    return result; 
  } else if (isExistSymbol(symbol, &(tree->left->symbols))) {
    result->push_back('0');
    encode(symbol, tree->left, result);
  } else if (isExistSymbol(symbol, &(tree->right->symbols))) {
    result->push_back('1');
    encode(symbol, tree->right, result);
  }
}

int main(int argc, char *argv[]){
  // get file name from command line arg
  string fileName = argv[1];
  string outFileName = fileName + ".zlp";
 
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
  }

  // make out file
  char c;
  vector<char> encodedText;
  fstream refin(fileName);
  while(refin.get(c)){
    vector<char> result;
    encode(c, &leaves[0], &result);
    for (auto iter = result.begin(); iter != result.end(); ++iter){
      encodedText.push_back(*iter);
    }
  }
  refin.close();
 
  ofstream fout;
  fout.open(outFileName, ios::binary);
  // encodedTextから８個取り出してstringにする
  for (int i = 0; i < (encodedText.size() / 8); i+=8){
    string byteCode;
    for (int j = i; j < (8 + i); j++){
      byteCode = byteCode + encodedText[j];
    }
    string str = byteCode.c_str();
    int num = stoi(str);
    char c = char(num); 
    fout.write((char *)&c, sizeof(c));
  }
  fout.close();
 
  return 0;
}


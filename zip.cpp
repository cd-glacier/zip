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
};

bool isExistSymbol(vector<char> symbols, char symbol) {
  return find(symbols.begin(), symbols.end(), symbol) != symbols.end();
}

// 検索してNodeを返したい
// 型のある言語で検索してなかった時ってどうするべきだっけ
Node getNode(vector<Node> v, char c){
  for (auto itr : v){
    auto symbols = itr.symbols;
    // if c exist
    if (isExistSymbol(symbols, c)) {
      return itr;
    }
  }
}

vector<Node> makeWeightFromFile(fstream &fin){
  char c;
  vector<Node> result;
  while(fin.get(c)){
    auto node = getNode(result, c);
    if (isExistSymbol(node.symbols, c)) {
      vector<char> v;
      v.push_back(c);
      Node newNode = {v, 1};
      result.push_back(newNode);
    } else {
      node.weight = node.weight+ 1;
    }
  }
  return result;
}

vector<Node> sort(vector<Node> &nodes){
  vector<Node> tmp = nodes;
  vector<Node> result;
  while (tmp.size() != 0) {
    int max = 0;
    int maxIndex;
    vector<char> max_key;
    vector<Node>::iterator iter;
    for (iter = tmp.begin(); iter != tmp.end(); ++iter) {
      auto n = *iter;
      if (max <= n.weight){
        max = n.weight;
        maxIndex = distance(tmp.begin(), iter);
      }
    }
    result.push_back(tmp[maxIndex]);
    tmp.erase(iter);
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
  auto nodes = makeWeightFromFile(fin);
  auto leaves = sort(nodes);
  reverse(leaves.begin(), leaves.end());
  // make huffman code tree
  for (auto i = 0; i < leaves.size() - 1 ; i++){
    cout << leaves[i].symbols[0] << endl;
  }
  
  fin.close();
  return 0;
}


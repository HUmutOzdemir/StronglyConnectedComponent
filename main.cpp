#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <stack>
using namespace std;

struct Vertex{
    int lowlink=-1;
    int index=-1;
    int key;
    bool onStack;
};
int numOfPiggy;
Vertex vertexlist[100020];
list<int> edgelist[100020];
int indexs = 0;
stack<int>* s = new stack<int>();
int root[100020];
int scc=0;
int SCCNum[100020];
void SCC(int i){
    vertexlist[i].index = indexs;
    vertexlist[i].lowlink = indexs;
    indexs++;
    s->push(vertexlist[i].key);
    vertexlist[i].onStack=true;
    list<int>& temp = edgelist[vertexlist[i].key];
    for(list<int>::iterator itr = temp.begin();itr!=temp.end();itr++){
        if(vertexlist[*itr].index==-1){
            SCC(vertexlist[*itr].key);
            vertexlist[i].lowlink = min(vertexlist[i].lowlink,vertexlist[*itr].lowlink);
        }else if(vertexlist[*itr].onStack){
            vertexlist[i].lowlink = min(vertexlist[i].lowlink,vertexlist[*itr].index);
        }
    }
    if(vertexlist[i].lowlink==vertexlist[i].index){
        root[scc]=vertexlist[i].key;
        while(vertexlist[s->top()].key!=vertexlist[i].key){
            vertexlist[s->top()].onStack=false;
            SCCNum[s->top()]=scc;
            s->pop();
        }
        SCCNum[s->top()]=scc;
        vertexlist[s->top()].onStack=false;
        s->pop();
        scc++;
    }
}
void SCC(){
    for(int i=1;i<=numOfPiggy;i++){
        if(vertexlist[i].index==-1){
            SCC(i);
        }
    }
}
int indegree[100020];
vector<int> breakable = vector<int>();
void breakPiggyBanks(){
    for(int i=1;i<=numOfPiggy;i++){
        list<int>& temp = edgelist[i];
        for(list<int>::iterator itr = temp.begin();itr!=temp.end();itr++){
            if(SCCNum[i]!=SCCNum[*itr]){
                indegree[SCCNum[*itr]]++;
            }
        }
    }
    for(int i=0;i<scc;i++){
        if(indegree[i]==0){
            breakable.push_back(i);
        }
    }
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project2 [input_file] [output_file]" << endl;
        return 1;
    }
    ios_base::sync_with_stdio(false);
    ifstream in(argv[1]);
    in >> numOfPiggy;
    for(int i=1;i<=numOfPiggy;i++){
        Vertex* newVertex = new Vertex();
        newVertex->key=i;
        vertexlist[i]=*newVertex;
        int n;
        in >> n;
        list<int>* newList = new list<int>();
        for(int j=0;j<n;j++){
            int k;
            in >> k;
            newList->push_back(k);
        }
        edgelist[i]=*newList;
    }
    SCC();
    breakPiggyBanks();
    ofstream out(argv[2]);
    out << breakable.size() << " ";
    for(int i=0;i<breakable.size();i++){
        out << root[breakable[i]]  << " ";
    }
    return 0;
}
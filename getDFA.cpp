#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <iostream>
#include <vector>

#include "util/util.h"
#include "re2/re2.h"
#include "re2/regexp.h"
#include "re2/prog.h"
#include "re2/filtered_re2.h"

#include "util/test.h"
#include "util/logging.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
void getDFA(re2::RE2& ree, re2::Prog::MatchKind kind){
	std::cout<<"pattern: \""<<ree.pattern()<<"\""<<std::endl;
    re2::Prog* prog = ree.getProg();
	int forward_size=prog->BuildEntireDFA(kind,[&](const int* next, bool match) {
    CHECK(next != NULL);
    std::cout<<"is match: "<<match<<" [";
    for (int b = 0; b < prog->bytemap_range() + 1; b++)
        std::cout<<next[b]<<",";
    std::cout<<"]"<<std::endl;
  });
	std::cout<<"pattern: \""<<ree.pattern()<<"\" forward_size: "<<forward_size<<std::endl;

    int t=prog->bytemap_range();
    vector<vector<int> > r_bytemap(t+1, vector<int>());
    r_bytemap[t].push_back(256);
    for(int c=0;c<256;c++){
        int b=prog->bytemap()[c];
        r_bytemap[b].push_back(c);
    }
    std::cout<<"byte range size: "<<t+1<<" byte ranges are: "<<std::endl;
    for(int i=0;i<t+1;i++){
        std::cout<<"range: "<<i<<" bytes: ";
        for(int j=0;j<r_bytemap[i].size();j++)
            std::cout<<r_bytemap[i][j]<<",";
        std::cout<<std::endl;
    }
}

void matchDFA(re2::RE2& ree, string input){
    std::cout<<"pattern: \""<<ree.pattern()<<"\" input: \""<<input<<"\""<<std::endl;
    re2::Prog* prog=ree.getProg();

    re2::Prog::MatchKind kind = re2::Prog::kFullMatch;
    re2::Prog::Anchor anchor = re2::Prog::kAnchored;
    bool dfa_failed = false;
    re2::StringPiece match;
    bool isMatch=prog->SearchDFA(input, input, anchor, kind, &match, &dfa_failed, NULL);
    std::cout<<"pattern: \""<<ree.pattern()<<"\" input: \""<<input<<"\" ismatch: "<<isMatch<<std::endl;
}

int main(int argc, char *argv[]) {
    if(argc<2||argc>3){
        printf("args number: %d",argc);
        printf("Error: illegal arguments; for getDFA need one argument: regex; for getDFAMatch need two arguments: regex and string\n");
        return 1;
    }   
    string pattern(argv[1]);
//    std::cout<<"pattern length: "<<pattern.length()<<std::endl;
    re2::RE2 ree(pattern);
    if(!ree.ok())
        return 1;

    if(argc==2){
        getDFA(ree, re2::Prog::kFullMatch);
    }else{ //argc==3
        string input(argv[2]);
 //       std::cout<<"input length: "<<input.length()<<std::endl;
        matchDFA(ree,input);
    }
    return 0;
}

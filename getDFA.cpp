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
    //std::cout<<"pattern: "<<pattern<<std::endl;
//    std::cout<<"pattern length: "<<pattern.length()<<std::endl;
    re2::RE2 ree(pattern);
 //   std::cout<<"re2 pattern: "<<ree.pattern()<<std::endl;
  //  std::cout<<"re2 pattern length: "<<ree.pattern().length()<<std::endl;
    if(!ree.ok())
        return 1;

    if(argc==2){
        getDFA(ree, re2::Prog::kFullMatch);
    }else{ //argc==3
        string input(argv[2]);
 //       std::cout<<"input length: "<<input.length()<<std::endl;
        matchDFA(ree,input);
    }

/*    re2::RE2 test("(?i)^AS([0-9]+)$");
    std::cout<<"anchored regexp: "<<test.Regexp()->ToString()<<std::endl;
    std::cout<<"anchored flags: "<<test.Regexp()->parse_flags()<<std::endl;

    std::cout<<"anchored firstmatch"<<std::endl;
    getDFA(test,re2::Prog::kFirstMatch);
    std::cout<<"anchored fullmatch"<<std::endl;
    getDFA(test,re2::Prog::kFullMatch);
    std::cout<<"anchored fullmatch as101"<<std::endl;
    matchDFA(test,"as101");
    std::cout<<"anchored fullmatch AS101"<<std::endl;
    matchDFA(test,"AS101");

    re2::RE2 test1("(?i)AS([0-9]+)$");
    std::cout<<"endanchored regexp: "<<test1.Regexp()->ToString()<<std::endl;
    std::cout<<"endanchored flags: "<<test1.Regexp()->parse_flags()<<std::endl;

    std::cout<<"endanchored firstmatch"<<std::endl;
    getDFA(test1,re2::Prog::kFirstMatch);
    std::cout<<"endanchored fullmatch"<<std::endl;
    getDFA(test1,re2::Prog::kFullMatch);
    std::cout<<"endanchored fullmatch as101"<<std::endl;
    matchDFA(test1,"as101");
    std::cout<<"endanchored fullmatch AS101"<<std::endl;
    matchDFA(test1,"AS101");
    matchDFA(test1,"A\u017f101");

    re2::RE2 test2("(?i)AS([0-9]+)");
    std::cout<<"unanchored regexp: "<<test2.Regexp()->ToString()<<std::endl;
    std::cout<<"unanchored flags: "<<test2.Regexp()->parse_flags()<<std::endl;

    std::cout<<"unanchored firstmatch"<<std::endl;
    getDFA(test2,re2::Prog::kFirstMatch);
    std::cout<<"unanchored fullmatch"<<std::endl;
    getDFA(test2,re2::Prog::kFullMatch);
    std::cout<<"unanchored fullmatch as101"<<std::endl;
    matchDFA(test2,"as101");
    std::cout<<"unanchored fullmatch AS101"<<std::endl;
    matchDFA(test2,"AS101");
    matchDFA(test2,"A\u017f101");
*/
    return 0;
}

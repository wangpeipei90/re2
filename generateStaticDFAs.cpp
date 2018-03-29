#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <iostream>

#include "util/util.h"
#include "re2/re2.h"
#include "re2/regexp.h"
#include "re2/prog.h"
#include "re2/filtered_re2.h"

#include "util/test.h"
#include "util/logging.h"
#include <iostream>
#include <fstream>
using namespace std;


re2::DFA* buildDFA(string regex0){
	re2::Prog* prog;
	re2::Prog* rprog;
	re2::DFA* dfa; 
	int for_dfa_sizes;
	int back_dfa_sizes;

	string regex(regex0);
	RE2 re(regex);

	re2::Prog::MatchKind kind;
	//kind= re2::Prog::kFirstMatch;
        kind= re2::Prog::kFullMatch;
        //kind= re2::Prog::kManyMatch;
        //kind= re2::Prog::kLongestMatch;

        if(re.ok()){
        	for_dfa_sizes=back_dfa_sizes=0;
      		
		prog = re.prog_;
                CHECK(prog);
		try{
			for_dfa_sizes=prog->BuildEntireDFA(kind);
			printf("Regex: %s size of Forward DFA: %d\n",regex.c_str(),for_dfa_sizes);
		}catch(std::bad_alloc){
			std::cout << "bad_alloc error\n" << std::endl;
		}

		rprog = re.ReverseProg();
		CHECK(rprog);
	        try{
			back_dfa_sizes=rprog->BuildEntireDFA(kind);
			printf("Regex: %s size of Backward DFA: %d\n",regex.c_str(),back_dfa_sizes);
		}catch(std::bad_alloc){
			std::cout << "bad_alloc error\n" << std::endl;
		}
	//	printf("Regex: %s: Forward DFA size: %d, Backward DFA size: %d\n",regex.c_str(),for_dfa_sizes,back_dfa_sizes);
	}
	
	return dfa;
}

int main(int argc, char *argv[]) {
        string regex0(argv[1]);
	re2::DFA* dfa=buildDFA(regex0); 
	return 0;
}

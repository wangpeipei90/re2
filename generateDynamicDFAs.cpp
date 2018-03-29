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

int main(int argc, char *argv[]) {

	if(argc<3||argc>3){
		printf("Error: illegal arguments; need Two arguments: regex and string\n");
		return 1;
	}
	string regex0(argv[1]);
	string test(argv[2]);
	printf("regex: %s, test string: %s\n",regex0.c_str(),test.c_str());
	printf("len of regex: %d, len of test string: %d\n",regex0.length(),test.length());
	bool is_match2=RE2::FullMatch(test,regex0);	
	//printf("isMatch:%d string %s regex:%s\n",is_match2,test.c_str(),regex0.c_str());
	printf("isMatch:%d\n",is_match2);
	return 0;
}

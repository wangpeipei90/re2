#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <iostream>
#include <vector>
#include <string>

#include "util/util.h"
#include "re2/re2.h"
#include "re2/regexp.h"
#include "re2/prog.h"
#include "re2/filtered_re2.h"
#include "re2/testing/string_generator.h"
#include "re2/testing/regexp_generator.h"
#include "util/test.h"
#include "util/logging.h"

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace re2;
int main(int argc, char *argv[]) {
    if(argc<3||argc>3){
        printf("args number: %d",argc);
        printf("Error: illegal arguments; for generate strings for a regular expression, it needs 2 arguments: regex, maxlen\n");
        return 1;
    }   
    string pattern(argv[1]);
    int len=stoi(argv[2]);
    StringGenerator g(len,Explode(pattern));
    //re2::StringPiece sep("abc");
    //StringGenerator g(3,Explode(sep));
    int n=0;
    while(g.HasNext()){
        string s=g.Next().ToString();
        n++;
        std::cout<<"n: "<<n<<" generated str: "<<s<<std::endl;
    }
//   std::cout<<"pattern length: "<<pattern.length()<<std::endl;
//   std::cout<<"input length: "<<len<<std::endl;
    return 0;
}

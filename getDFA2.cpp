/*
 * getDFA.cpp
 *
 *  Created on: Dec 28, 2017
 *      Author: peipei
 */
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <iostream>
#include <string>

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
void partialmatch(RE2& ree){
	int i;
	std::cout<<RE2::PartialMatch("aa1001", ree,&i)<<std::endl;
	std::cout<<"i: "<<i<<std::endl;
	std::cout<<RE2::PartialMatch("2a1001", ree,&i)<<std::endl;
	std::cout<<"i: "<<i<<std::endl;
	std::cout<<RE2::PartialMatch("100a1001", ree,&i)<<std::endl;
	std::cout<<"i: "<<i<<std::endl;
}

void fullmatch(RE2& ree){
	int i;
	std::cout<<RE2::FullMatch("aa1001", ree,&i)<<std::endl;
	std::cout<<"i: "<<i<<std::endl;
	std::cout<<RE2::FullMatch("2a1001", ree,&i)<<std::endl;
	std::cout<<"i: "<<i<<std::endl;
	std::cout<<RE2::FullMatch("100a1001", ree,&i)<<std::endl;
	std::cout<<"i: "<<i<<std::endl;
}

void printState(const int* next, bool match){
 std::cout<<"match: :"<<match<<std::endl;
// for(const int *p=next; *p; ++p){
//     std::cout<<*p<<" ";
// }
 std::cout<<next[0]<<std::endl;
// std::cout<<next[1]<<std::endl;
}

int main(int argc, char *argv[]) {
    bool s;
    s=RE2::FullMatch("9", "\\d+");
    std::cout<<"\\d+ and 9: "<<s<<std::endl;
    s=RE2::FullMatch("\u0039", "\\d+");
    std::cout<<"\\d+ and u0039: "<<s<<std::endl;
    s=RE2::FullMatch("\uFF19", "\\d+");
    std::cout<<"\\d+ and uFF19: "<<s<<std::endl;

    s=RE2::FullMatch("\u0661", RE2("\\d", RE2::Latin1));
    std::cout<<"\\d and u0661: "<<s<<std::endl;
    s=RE2::FullMatch("\u0036", RE2("\\d", RE2::Latin1));
    std::cout<<"\\d and u0036: "<<s<<std::endl;
    s=RE2::FullMatch("\u0661", RE2("\\p{Nd}", RE2::Latin1));
    std::cout<<"\\p{Nd} and u0661: "<<s<<std::endl;
    s=RE2::FullMatch("\u0661", RE2("\\p{Nd}"));
    std::cout<<"\\p{Nd} and u0661 no Latin1: "<<s<<std::endl;
    s=RE2::FullMatch("\u0661", RE2("\\p{N}"));
    std::cout<<"\\p{N} and u0661 no Latin1: "<<s<<std::endl;

    s=RE2::FullMatch("AS101", RE2("(?i)^AS([0-9]+)$"));
    std::cout<<"case sensitive: (?i)^AS([0-9]+)$ AS101"<<s<<std::endl;
    RE2 ree("(?i)^AS([0-9]+)$");
    std::cout<<"pattern case sensitive: "<<ree.pattern()<<std::endl;
    std::cout<<"pattern length case sensitive: "<<ree.pattern().length()<<std::endl;

    string str="(?i)^AS([0-9]+)$";
    re2::Regexp* re = re2::Regexp::Parse(str, re2::Regexp::LikePerl, NULL);
    CHECK(re);
    re2::Prog* prog = re->CompileToProg(0);
    CHECK(prog);
    re2::Prog::MatchKind kind = re2::Prog::kFullMatch;
    int forward_size=prog->BuildEntireDFA(kind,[&](const int* next, bool match) {
                CHECK(next != NULL);
                    std::cout<<"is match: "<<match<<" [";
                        for (int b = 0; b < prog->bytemap_range() + 1; b++)
                                std::cout<<next[b]<<",";
                                    std::cout<<"]"<<std::endl;
                                      }); 
    std::cout<<"pattern: \""<<str<<"\" forward_size: "<<forward_size<<std::endl;
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
//    s=RE2::FullMatch("\\d+", "\\u0039");
 //   std::cout<<"\\d+ and \\u0039: "<<s<<std::endl;
//    s=RE2::FullMatch("\\d+", "\\u0039");
 //   std::cout<<"\\d+ and \\u0039: "<<s<<std::endl;
/*
    s=RE2::FullMatch("foo", "foo$");
    std::cout<<"foo$ and foo: "<<s<<std::endl;
    s=RE2::FullMatch("foo$bar", "foo$");
    std::cout<<"foo$ and foo$bar: "<<s<<std::endl;

    s=RE2::FullMatch("foo", "foo\\$");
    std::cout<<"foo\\$ and foo: "<<s<<std::endl;
    s=!RE2::FullMatch("foo$bar", "foo\\$");
    std::cout<<"foo\\$ and foo$bar: "<<s<<std::endl;

    s=RE2::FullMatch("foo", "foo\\$bar");
    std::cout<<"foo\\$bar and foo: "<<s<<std::endl;
    s=RE2::FullMatch("foo$bar", "foo\\$bar");
    std::cout<<"foo\\$bar and foo$bar: "<<s<<std::endl;
    s=RE2::FullMatch("foo\\$bar", "foo\\$bar");
    std::cout<<"foo\\$bar and foo\\$bar: "<<s<<std::endl;

    s=RE2::FullMatch("foo", "foo$bar");
    std::cout<<"foo$bar and foo: "<<s<<std::endl;
    s=RE2::FullMatch("foo$bar", "foo$bar");
    std::cout<<"foo$bar and foo$bar: "<<s<<std::endl;
    s=RE2::FullMatch("foo\\$bar", "foo$bar");
    std::cout<<"foo$bar and foo\\$bar: "<<s<<std::endl;

    s=RE2::FullMatch("md5-pw $1$/7f2XnzQ$p5ddbI7SXq4z4yNrObFS/0","(?i)^.*MD5-PW \\$1\\$(.{1,8})\\$(.{22}).*$");
    std::cout<<s<<std::endl;
    */
	/*RE2 ree("(\\d+)");
    string str="000";
    RE2::FullMatch(str,ree);
    RE2::FullMatch("\0",ree);*/
//
//	RE2::Options options=RE2::Options();
//	options.set_longest_match(true);
//	RE2 ref("(\\d+)",options);
//	partialmatch(ref);
//
//	RE2::Options options2=RE2::Options();
//	options2.set_posix_syntax(true);
//	options2.set_longest_match(false);
//	RE2 reg("([0-9]+)",options2); //Invalid RE2: invalid escape sequence: \d
//	partialmatch(reg);
//
//	RE2::Options options3=RE2::Options();
//	options3.set_posix_syntax(true);
//	options3.set_longest_match(true);
//	RE2 reh("([0-9]+)",options3);
//	partialmatch(reh);

/*    string regex(argv[1]);
	RE2 ree(regex);

	re2::Prog::MatchKind kind=re2::Prog::kFullMatch;//kLongestMatch,kLongestMatch,kLongestMatch,
	re2::Prog::DFAStateCallback cb=printState;
 
    re2::Prog* prog=ree.getProg();
	int range=prog->bytemap_range();
	std::cout<<"range: "<<range<<std::endl;
	bool isonepass=prog->IsOnePass();
	std::cout<<"isonepass: "<<isonepass<<std::endl;
	int forward_size=prog->BuildEntireDFA(kind, nullptr);
	std::cout<<"forward_size: "<<forward_size<<std::endl;
    */
	/*forward_size=prog->BuildEntireDFA(kind, printState);
	std::cout<<"forward_size 2: "<<forward_size<<std::endl;
	forward_size=prog->BuildEntireDFA(kind,[&](const int* next, bool match) {
    CHECK(next != NULL);
    for (int b = 0; b < prog->bytemap_range() + 1; b++)
        std::cout<<"b:"<<b<<" next[b]:"<< next[b]<<std::endl;
    std::cout<<std::endl;
  });
	std::cout<<"forward_size 3: "<<forward_size<<std::endl;

    re2::Regexp* re = re2::Regexp::Parse("asd", re2::Regexp::LikePerl, NULL);
    CHECK(re);
    prog = re->CompileToProg(0);*/
	/*forward_size=prog->BuildEntireDFA(kind,[&](const int* next, bool match) {
    CHECK(next != NULL);
    for (int b = 0; b < prog->bytemap_range() + 1; b++)
        std::cout<<"b:"<<b<<" next[b]:"<< next[b]<<" ";
    std::cout<<std::endl;
  });
	std::cout<<"forward_size 4: "<<forward_size<<std::endl;
    */
}

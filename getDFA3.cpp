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
#include <vector>
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

int main(int argc, char *argv[]) {
    string p_str="[0-9]+(h|m|s|ms)";
    string m_str="9m";
    string m_str1="9mh";
    string m_str2="9ms";

    string unit;
    bool s;
    std::cout<<"FullMatch: "<<std::endl;
    s=RE2::FullMatch("9msh",p_str,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    s=RE2::FullMatch(m_str,p_str,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    s=RE2::FullMatch(m_str1,p_str,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    s=RE2::FullMatch(m_str2,p_str,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    std::cout<<"PartialMatch: "<<std::endl;
    s=RE2::PartialMatch(m_str,p_str,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    s=RE2::PartialMatch(m_str1,p_str,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    s=RE2::PartialMatch(m_str2,p_str,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    RE2::Options opt;
    opt.set_longest_match(true);
    RE2 ree2(p_str,opt);
    std::cout<<"PartialMatch Longest: "<<std::endl;
    s=RE2::PartialMatch(m_str,ree2,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    s=RE2::PartialMatch(m_str1,ree2,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    s=RE2::PartialMatch(m_str2,ree2,&unit);
    if(s)
        std::cout<<s<<" matched str: "<<unit<<std::endl;
    else
        std::cout<<s<<std::endl;

    string p_str2="^[0-9]+(h|ms|s|m)";
    RE2 ree(p_str2);
    if(!ree.ok())
        std::cout<<"RE2 obj error"<<std::endl;


    re2::Prog::MatchKind kind = re2::Prog::kFullMatch;
    re2::Prog::MatchKind kind2 = re2::Prog::kFirstMatch;
    re2::Prog::MatchKind kind3 = re2::Prog::kLongestMatch;
    re2::Prog::Anchor anchor = re2::Prog::kAnchored;
    getDFA(ree,kind);
    getDFA(ree,kind2);
    getDFA(ree,kind3);

}

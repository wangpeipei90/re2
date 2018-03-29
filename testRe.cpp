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
using namespace std;
/*
void test(string s, bool UsingMallocCounter){
// Create regexp with 2^30 states in DFA.
  re2::Regexp* re = re2::Regexp::Parse(s, re2::Regexp::LikePerl, NULL);
  CHECK(re);

  for (int i = 17; i < 24; i++) {
    int64_t limit = 1<<i;
    int64_t usage;
    //int64_t progusage, dfamem;
    {   
      testing::MallocCounter m(testing::MallocCounter::THIS_THREAD_ONLY);
      re2::Prog* prog = re->CompileToProg(limit);
      CHECK(prog);
      //progusage = m.HeapGrowth();
      //dfamem = prog->dfa_mem();
      prog->BuildEntireDFA(re2::Prog::kFirstMatch);
      prog->BuildEntireDFA(re2::Prog::kLongestMatch);
      usage = m.HeapGrowth();
      delete prog;
    }   
    if (UsingMallocCounter) {
      //LOG(INFO) << "limit " << limit << ", "
      //          << "prog usage " << progusage << ", "
      //          << "DFA budget " << dfamem << ", "
      //          << "total " << usage;
      // Tolerate +/- 10%.
      CHECK_GT(usage, limit*9/10);
      CHECK_LT(usage, limit*11/10);
    }   
  }
  re->Decref();
}
*/

int main(void) {
	/*re2::FilteredRE2 f;
	int id;
	f.Add("a.*b.*c", RE2::DefaultOptions, &id);
	std::vector<std::string> v;
	f.Compile(&v);
	std::vector<int> ids;
	f.FirstMatch("abbccc", ids);


	if(RE2::FullMatch("axbyc", "a.*b.*c")) {
		printf("PASS\n");
		//return 0;
	}else{
		printf("FAIL\n");
	}
	*/
	int i;
	std::string s;
	//RE2 re("(\\w+):(\\d+)");
	//RE2 re("\\s+");
	RE2 re("\nmd5_data = {\n([^}]+)}");
	assert(re.ok());  // compiled; if not, see re.error();
	printf("Pass compile\n");

	/*
	// submatches are stored in the additional arguments
	assert(RE2::FullMatch("ruby:1234", re, &s, &i));
	printf("Full Match Capturing Groups\n");
	assert(s=="ruby");
	assert(i==1234);

	//does not extract number
	assert(RE2::FullMatch("ruby:1234", re, &s));
	printf("only one parameter does not extract number:%s\n",s.c_str());

	//not enough sub-patterns
	assert(!RE2::FullMatch("ruby:1234", "\\w+:\\d+", &s));

	// Fails, "ruby" cannot be parsed as an integer as a string cannot be stored in integer.
	assert(!RE2::FullMatch("ruby","(.+)",&i));

	//skip NULL argument.
	assert(RE2::FullMatch("ruby:1234", re, (void*)NULL, &i));
	//assert(RE2::FullMatch("ruby:1234", re, NULL, &i)); //without void pointer is wrong 
	printf("skip null, only number left:%d\n",i);

	//Fails, integer overflow keeps value from being stored in i.
	assert(!RE2::FullMatch("ruby:123456789123", re, &s, &i));
	*/

	printf("pattern:%s\n",re.pattern().c_str());
	re2::Regexp* rexp=re.Regexp(); //get entire_regexp_
	//printf("regexp:%s\n",rexp->ToString().c_str());
	printf("regexp simple:%s\n",rexp->simple()?"True":"False");
	//re2::Prog::MatchKind kind = re2::Prog::kFirstMatch;
	re2::Prog::MatchKind kind = re2::Prog::kFullMatch;
	//re2::Prog::MatchKind kind = re2::Prog::kManyMatch;
	//re2::Prog::MatchKind kind = re2::Prog::kLongestMatch;
        re2::Prog* prog = re.prog_;
	re2::Prog* rprog = re.ReverseProg();
        CHECK(prog);
        int sizes2=prog->BuildEntireDFA(kind);
	printf("size of Forward DFA:%d\n",sizes2);

        CHECK(rprog);
	re2::DFA * dfa=prog->GetDFA(kind);
        int sizes=rprog->BuildEntireDFA(kind);
	printf("size of DFA:%d\n",sizes);

	RE2 re4("\\bfoo\\b");
	cout<<"foo----is Match:"<<RE2::FullMatch("foo",re4)<<endl;
	cout<<"foo.----is Match:"<<RE2::FullMatch("foo.",re4)<<endl;
	cout<<"(foo)----is Match:"<<RE2::FullMatch("(foo)",re4)<<endl;
	cout<<"bar foo baz----is Match:"<<RE2::FullMatch("bar foo baz",re4)<<endl;
	cout<<"foobaz----is Match:"<<RE2::FullMatch("foobaz",re4)<<endl;
	cout<<"foo3----is Match:"<<RE2::FullMatch("foo3",re4)<<endl;

	cout<<"foo----is Match:"<<RE2::PartialMatch("foo",re4)<<endl;
	cout<<"foo.----is Match:"<<RE2::PartialMatch("foo.",re4)<<endl;
	cout<<"(foo)----is Match:"<<RE2::PartialMatch("(foo)",re4)<<endl;
	cout<<"bar foo baz----is Match:"<<RE2::PartialMatch("bar foo baz",re4)<<endl;
	cout<<"foobaz----is Match:"<<RE2::PartialMatch("foobaz",re4)<<endl;
	cout<<"foo3----is Match:"<<RE2::PartialMatch("foo3",re4)<<endl;


	/*
	//constructor parameters RE2 object
	//predefined options: RE2::Quiet; RE2::Latin1; RE2::POSIX(syntax and leftmost longest matching--partial matching)
	RE2 re2("(ab", RE2::Quiet);  // don't write to stderr for parser failure
	assert(!re2.ok());  // can check re.error() for details

	RE2 re3("(abc");  // don't write to stderr for parser failure
	assert(!re3.ok());
	//assert(!re3.ok());  // can check re.error() for details

	RE2 re4("\bfoo\b");
	cout<<"foo----is Match:"<<RE2::FullMatch("foo",re4)<<endl;
	cout<<"foo.----is Match:"<<RE2::FullMatch("foo.",re4)<<endl;
	cout<<"(foo)----is Match:"<<RE2::FullMatch("(foo)",re4)<<endl;
	cout<<"bar foo baz----is Match:"<<RE2::FullMatch("bar foo baz",re4)<<endl;
	cout<<"foobaz----is Match:"<<RE2::FullMatch("foobaz",re4)<<endl;
	cout<<"foo3----is Match:"<<RE2::FullMatch("foo3",re4)<<endl;

	cout<<"foo----is Match:"<<RE2::PartialMatch("foo",re4)<<endl;
	cout<<"foo.----is Match:"<<RE2::PartialMatch("foo.",re4)<<endl;
	cout<<"(foo)----is Match:"<<RE2::PartialMatch("(foo)",re4)<<endl;
	cout<<"bar foo baz----is Match:"<<RE2::PartialMatch("bar foo baz",re4)<<endl;
	cout<<"foobaz----is Match:"<<RE2::PartialMatch("foobaz",re4)<<endl;
	cout<<"foo3----is Match:"<<RE2::PartialMatch("foo3",re4)<<endl;
	

	int number;
	assert(RE2::PartialMatch("x*100 + 20", "(\\d+)", &number));
	printf("last partial match\n");
	assert(number==100);
	*/
	return 0;

}

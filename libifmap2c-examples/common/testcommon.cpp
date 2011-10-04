#include "testcommon.h"

#include <list>
#include <iostream>
#include <cstdlib>

using namespace ifmap2c;
using namespace std;

typedef list<SearchResult *> SRLIST;
typedef list<ResultItem *> RILIST;
typedef SRLIST::iterator SRLISTIT;
typedef RILIST::iterator RILISTIT;

static SRLIST getResultsOfType(PollResult *pr, ResultType x)
{
	switch (x) {
	case SEARCH:
		return pr->getSearchResults();
	case UPDATE:
		return pr->getUpdateResults();
	case DELETE:
		return pr->getDeleteResults();
	case NOTIFY:
		return pr->getNotifyResults();
	default:
		cerr << "BAD ResultType" << endl;
		exit(1);
	}
}


size_t
cntRe(ifmap2c::PollResult *pr, ResultType type)
{
	return getResultsOfType(pr, type).size();
}

size_t
cntUp(ifmap2c::PollResult *pr)
{
	return cntRe(pr, UPDATE);
}

size_t
cntDe(ifmap2c::PollResult *pr)
{
	return cntRe(pr, DELETE);
}

size_t
cntNo(ifmap2c::PollResult *pr)
{
	return cntRe(pr, NOTIFY);
}

size_t
cntSe(ifmap2c::PollResult *pr)
{
	return cntRe(pr, SEARCH);
}

int
cntRi(SearchResult *sres, Identifier *i1, Identifier *i2, int cnt)
{
	RILIST ritems;
	RILISTIT it, end;
	ResultItem *ri;
	int ret = 0;
		
	if (!i1 && !i2)
		ritems = sres->getResultItems();
	else 
		ritems = sres->getResultItemsByIdentifier(i1, i2);



	if (cnt == -1)
		return ritems.size();


	it = ritems.begin();
	end = ritems.end();
	for (/* */; it != end; it++) {
		ri = *it;
		if ((int)ri->getMetadata().size() == cnt)
			ret++;
	}

	return ret;
}

int
cntRi(SearchResult *sres, Identifier *i1, Identifier *i2)
{
	return cntRi(sres, i1, i2, -1);
}

int
cntRi(SearchResult *sres, Identifier *i1, int cnt)
{
	return cntRi(sres, i1, NULL, cnt);
}

int cntRi(SearchResult *sres, Identifier *i1)
{
	return cntRi(sres, i1, -1);
}

int
cntRi(SRLIST toSearch, Identifier *i1, Identifier *i2, int cnt)
{
	SRLISTIT it = toSearch.begin();
	SRLISTIT end = toSearch.end();
	int ret = 0;

	for (/* */; it != end; it++)
		ret += cntRi(*it, i1, i2, cnt);

	return ret;

}


int
cntRi(PollResult *pr, Identifier *i1, Identifier *i2, ResultType x, int cnt)
{
	SRLIST  toSearch = getResultsOfType(pr, x);
	return cntRi(toSearch, i1, i2, cnt);
}

int
cntRi(PollResult *pr, Identifier *i1, Identifier *i2, ResultType x)
{
	return cntRi(pr, i1, i2, x, -1);
}

int
cntRi(PollResult *pr, Identifier *i1, ResultType x, int cnt)
{
	return cntRi(pr, i1, NULL, x, cnt);

}

int
cntRi(PollResult *pr, Identifier *i1, ResultType x)
{
	return cntRi(pr, i1, x, -1);
}

int
cntRi(PollResult *pr, ResultType x)
{
	return cntRi(pr, NULL, NULL, x, -1);
}

int
cntRi(PollResult *pr, ResultType x, int cnt)
{
	return cntRi(pr, NULL, NULL, x, cnt);
}

int checkContainsOnly(PollResult *pr, ResultType type, string msg, int expected)
{
	int cnt;
	int totalCount = 0;
	
	cnt = getResultsOfType(pr, type).size();
	totalCount += getResultsOfType(pr, SEARCH).size();
	totalCount += getResultsOfType(pr, UPDATE).size();
	totalCount += getResultsOfType(pr, DELETE).size();
	totalCount += getResultsOfType(pr, NOTIFY).size();

	if (cnt == 0 && expected == -1)
		cerr << msg << " (no results found for type=" << type << ")" << endl;

	if (cnt != expected && expected != -1)
		cerr << msg << " (expected " << expected << " was " << cnt << ")" << endl;

	if (totalCount != cnt && expected != -1) {
		cerr << msg << " (not only type=" << type << " total=" << totalCount;
		cerr << " cnt= " << cnt << ")" << endl;
	}
	return cnt;
}
/* -1 as expected, > 0 */
int checkRiCnt(PollResult *pr, ResultType type, string msg, int expected)
{
	int cnt = cntRi(pr, type, -1);
	if (expected == -1 && cnt == 0)
		cerr << msg << " (expected greater zero but was zero)" << endl;
	else if (cnt != expected)
		cerr << msg << " (expected " << expected << " was " << cnt << ")" <<  endl;
	
	return cnt;
}

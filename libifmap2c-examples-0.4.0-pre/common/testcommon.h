#ifndef TESTCOMMON_H_
#define TESTCOMMON_H_

#include <libifmap2c/ssrc.h>
#include <libifmap2c/arc.h>
#include <libifmap2c/identifiers.h>
#include <libifmap2c/metadata.h>

#include <string>

enum ResultType {
	SEARCH,
	UPDATE,
	DELETE,
	NOTIFY
};


int cntRi(ifmap2c::PollResult *pr, ifmap2c::Identifier *i1, ResultType x);

int cntRi(ifmap2c::PollResult *pr, ifmap2c::Identifier *i1, ResultType x, int mdCnt);

int cntRi(ifmap2c::PollResult *pr, ifmap2c::Identifier *i1, ifmap2c::Identifier *i2,
		ResultType x);

int cntRi(ifmap2c::PollResult *pr, ifmap2c::Identifier *i1, ifmap2c::Identifier *i2,
		ResultType x, int mdCnt);

int cntRi(ifmap2c::SearchResult *sres, ifmap2c::Identifier *i1, ifmap2c::Identifier *i2,
		int cnt);

int cntRi(ifmap2c::SearchResult *sres, ifmap2c::Identifier *i1, ifmap2c::Identifier *i2);

int cntRi(ifmap2c::SearchResult *sres, ifmap2c::Identifier *i1, int cnt);

int cntRi(ifmap2c::SearchResult *sres, ifmap2c::Identifier *i1);

int cntRi(ifmap2c::PollResult *pres, ResultType type);

int cntRi(ifmap2c::PollResult *pr, ResultType x, int cnt);

/* -1 as expected, > 0 */
int checkContainsOnly(ifmap2c::PollResult *pr, ResultType type, std::string msg, int expected);
/* -1 as expected, > 0 */
int checkRiCnt(ifmap2c::PollResult *pr, ResultType type, std::string msg, int expected);

size_t cntRe(ifmap2c::PollResult *pr, ResultType type);
size_t cntUp(ifmap2c::PollResult *pr);
size_t cntDe(ifmap2c::PollResult *pr);
size_t cntNo(ifmap2c::PollResult *pr);
size_t cntSe(ifmap2c::PollResult *pr);
#endif /* TESTCOMMON_H_ */

#ifndef RNGSTREAMSUPP_H
#define RNGSTREAMSUPP_H

class RngStreamSupp
{
public:
	RngStreamSupp();
	~RngStreamSupp();
	static void AdvanceSeed(unsigned  long  seedIn[6],unsigned  long  seedOut[6]);
};
#endif
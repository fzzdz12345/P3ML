
#pragma once
#include "tools.h"
#include "connect.h"
#include "globals.h"
using namespace std;

extern void start_time();
extern void start_communication();
extern void end_time(string str);
extern void end_communication(string str);


 
void funcTruncate(RSSVectorMyType &a, size_t power, size_t size);
void funcTruncatePublic(RSSVectorMyType &a, size_t divisor, size_t size);
void funcGetShares(RSSVectorMyType &a, const vector<myType> &data);
void funcGetShares(RSSVectorSmallType &a, const vector<smallType> &data);
void funcReconstructBit(const RSSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print);
void funcReconstruct(const RSSVectorMyType &a, vector<myType> &b, size_t size, string str, bool print);
void funcReconstruct(const RSSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print);
void funcReconstruct3out3(const vector<myType> &a, vector<myType> &b, size_t size, string str, bool print);
void funcMatMul(const RSSVectorMyType &a, const RSSVectorMyType &b, RSSVectorMyType &c, 
				size_t rows, size_t common_dim, size_t columns,
			 	size_t transpose_a, size_t transpose_b, size_t truncation);
void funcDotProduct(const RSSVectorMyType &a, const RSSVectorMyType &b, 
					   RSSVectorMyType &c, size_t size, bool truncation, size_t precision);
void funcDotProduct(const RSSVectorSmallType &a, const RSSVectorSmallType &b, 
							 RSSVectorSmallType &c, size_t size);

void funcSelectShares(const RSSVectorMyType &a, const RSSVectorSmallType &b, RSSVectorMyType &selected, size_t size);
void funcSelectBitShares(const RSSVectorSmallType &a0, const RSSVectorSmallType &a1, 
						 const RSSVectorSmallType &b, RSSVectorSmallType &answer, 
						 size_t rows, size_t columns, size_t loopCounter);
void funcRELUPrime(const RSSVectorMyType &a, RSSVectorSmallType &b, size_t size);
void funcRELU(const RSSVectorMyType &a, RSSVectorSmallType &temp, RSSVectorMyType &b, size_t size);
void funcPow(const RSSVectorMyType &b, vector<smallType> &alpha, size_t size);
void funcDivision(const RSSVectorMyType &a, const RSSVectorMyType &b, RSSVectorMyType &quotient, 
							size_t size);
void funcBatchNorm(const RSSVectorMyType &a, const RSSVectorMyType &b, RSSVectorMyType &quotient, 
							size_t batchSize, size_t B);
void funcMaxpool(RSSVectorMyType &a, RSSVectorMyType &max, RSSVectorSmallType &maxPrime, 
				size_t rows, size_t columns);


#pragma once
#include "Functionalities.h"
#include "Precompute.h"
#include <thread>
#include <chrono>   // 高精度计时库
#include <iomanip>

using namespace std;
extern Precompute PrecomputeObject;
extern string SECURITY_TYPE;

class ScopedTimer {
private:
    std::string func_name;                          
    std::chrono::high_resolution_clock::time_point start;  
public:
    // 构造函数：记录开始时间和函数名
    ScopedTimer(const std::string& name) : func_name(name) {
        start = std::chrono::high_resolution_clock::now();
    }

    // 析构函数：函数执行完自动计算并输出耗时
    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        // 计算耗时（毫秒，保留3位小数），也可改用微秒(µs)、纳秒(ns)
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start);
        // 输出格式：[协议名] 耗时: X.XXX ms
        std::cout << "[Timer] " << func_name << " 耗时: " 
                  << std::fixed << std::setprecision(3) << duration.count() 
                  << " ms" << std::endl;
    }
};


void funcTruncate(RSSVectorMyType &a, size_t power, size_t size)
{
	ScopedTimer timer("funcTruncate");
	log_print("funcTruncate");
	RSSVectorMyType r(size), rPrime(size);
	vector<myType> reconst(size);
	PrecomputeObject.getDividedShares(r, rPrime, (1<<power), size);
	for (int i = 0; i < size; ++i)
		a[i] = a[i] - rPrime[i];
	
	funcReconstruct(a, reconst, size, "Truncate reconst", false);
	dividePlain(reconst, (1 << power));
	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = r[i].first + reconst[i];
			a[i].second = r[i].second;
		}
	}

	if (partyNum == PARTY_B)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = r[i].first;
			a[i].second = r[i].second;
		}
	}

	if (partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = r[i].first;
			a[i].second = r[i].second + reconst[i];
		}
	}	
}





void funcTruncatePublic(RSSVectorMyType &a, size_t divisor, size_t size)
{
	log_print("funcTruncate");
	ScopedTimer timer("funcTruncate");
	RSSVectorMyType r(size), rPrime(size);
	vector<myType> reconst(size);
	PrecomputeObject.getDividedShares(r, rPrime, divisor, size);
	for (int i = 0; i < size; ++i)
		a[i] = a[i] - rPrime[i];
	
	funcReconstruct(a, reconst, size, "Truncate reconst", false);
	dividePlain(reconst, divisor);
	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = r[i].first + reconst[i];
			a[i].second = r[i].second;
		}
	}

	if (partyNum == PARTY_B)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = r[i].first;
			a[i].second = r[i].second;
		}
	}

	if (partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = r[i].first;
			a[i].second = r[i].second + reconst[i];
		}
	}	
}


//Fixed-point data has to be processed outside this function.
void funcGetShares(RSSVectorMyType &a, const vector<myType> &data)
{
	ScopedTimer timer("GetShare");
	size_t size = a.size();
	
	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = data[i];
			a[i].second = 0;
		}
	}
	else if (partyNum == PARTY_B)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = 0;
			a[i].second = 0;
		}
	}
	else if (partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = 0;
			a[i].second = data[i];
		}
	}
}


void funcGetShares(RSSVectorSmallType &a, const vector<smallType> &data)
{
	ScopedTimer timer("GetShare");
	size_t size = a.size();
	
	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = data[i];
			a[i].second = 0;
		}
	}
	else if (partyNum == PARTY_B)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = 0;
			a[i].second = 0;
		}
	}
	else if (partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{
			a[i].first = 0;
			a[i].second = data[i];
		}
	}
}

//重构恢复出单bit分享
void funcReconstructBit(const RSSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print)
{
	log_print("Reconst: RSSSmallType (bits), smallType (bit)");
	ScopedTimer timer("Reconstruct");
	if (SECURITY_TYPE.compare("Semi-honest") == 0)
	{
		vector<smallType> a_next(size), a_prev(size);
		for (int i = 0; i < size; ++i)
		{
			a_prev[i] = 0;
			a_next[i] = a[i].first;
			b[i] = a[i].first;
			b[i] = b[i] ^ a[i].second;
		}

		thread *threads = new thread[2];
		threads[0] = thread(sendVector<smallType>, ref(a_next), nextParty(partyNum), size);
		threads[1] = thread(receiveVector<smallType>, ref(a_prev), prevParty(partyNum), size);
		for (int i = 0; i < 2; i++)
			threads[i].join();
		delete[] threads;

		for (int i = 0; i < size; ++i)
			b[i] = b[i] ^ a_prev[i];

		if (print)
		{
			std::cout << str << ": \t\t";
			for (int i = 0; i < size; ++i)
				cout << (int)(b[i]) << " "; 
			std::cout << std::endl;
		}
	}

}


void funcReconstruct(const RSSVectorSmallType &a, vector<smallType> &b, size_t size, string str, bool print)
{
	log_print("Reconst: RSSSmallType, smallType");
	ScopedTimer timer("Reconstruct");
	if (SECURITY_TYPE.compare("Semi-honest") == 0)
	{
		vector<smallType> a_next(size), a_prev(size);
		for (int i = 0; i < size; ++i)
		{
			a_prev[i] = 0;
			a_next[i] = a[i].first;
			b[i] = a[i].first;
			b[i] = additionModPrime[b[i]][a[i].second];
		}

		thread *threads = new thread[2];

		threads[0] = thread(sendVector<smallType>, ref(a_next), nextParty(partyNum), size);
		threads[1] = thread(receiveVector<smallType>, ref(a_prev), prevParty(partyNum), size);

		for (int i = 0; i < 2; i++)
			threads[i].join();

		delete[] threads;

		for (int i = 0; i < size; ++i)
			b[i] = additionModPrime[b[i]][a_prev[i]];

		if (print)
		{
			std::cout << str << ": \t\t";
			for (int i = 0; i < size; ++i)
				cout << (int)(b[i]) << " "; 
			std::cout << std::endl;
		}
	}

}


void funcReconstruct(const RSSVectorMyType &a, vector<myType> &b, size_t size, string str, bool print)
{
	log_print("Reconst: RSSMyType, myType");
	ScopedTimer timer("Reconstruct");
	assert(a.size() == size && "a.size mismatch for reconstruct function");

	if (SECURITY_TYPE.compare("Semi-honest") == 0)
	{
		vector<myType> a_next(size), a_prev(size);
		for (int i = 0; i < size; ++i)
		{
			a_prev[i] = 0;
			a_next[i] = a[i].first;
			b[i] = a[i].first;
			b[i] = b[i] + a[i].second;
		}

		thread *threads = new thread[2];

		threads[0] = thread(sendVector<myType>, ref(a_next), nextParty(partyNum), size);
		threads[1] = thread(receiveVector<myType>, ref(a_prev), prevParty(partyNum), size);

		for (int i = 0; i < 2; i++)
			threads[i].join();

		delete[] threads;

		for (int i = 0; i < size; ++i)
			b[i] = b[i] + a_prev[i];

		if (print)
		{
			std::cout << str << ": \t\t";
			for (int i = 0; i < size; ++i)
				print_linear(b[i], "SIGNED");
			std::cout << std::endl;
		}
	}

}

void funcCheckMaliciousMatMul(const RSSVectorMyType &a, const RSSVectorMyType &b, const RSSVectorMyType &c, 
							const vector<myType> &temp, size_t rows, size_t common_dim, size_t columns,
							size_t transpose_a, size_t transpose_b)
{
	RSSVectorMyType x(a.size()), y(b.size()), z(c.size());
	PrecomputeObject.getTriplets(x, y, z, rows, common_dim, columns);

	subtractVectors<RSSMyType>(x, a, x, rows*common_dim);
	subtractVectors<RSSMyType>(y, b, y, common_dim*columns);

	size_t combined_size = rows*common_dim + common_dim*columns, base_size = rows*common_dim;
	RSSVectorMyType combined(combined_size); 
	vector<myType> rhoSigma(combined_size), rho(rows*common_dim), sigma(common_dim*columns), temp_send(rows*columns);
	for (int i = 0; i < rows*common_dim; ++i)
		combined[i] = x[i];

	for (int i = rows*common_dim; i < combined_size; ++i)
		combined[i] = y[i-base_size];

	funcReconstruct(combined, rhoSigma, combined_size, "rhoSigma", false);

	for (int i = 0; i < rows*common_dim; ++i)
		rho[i] = rhoSigma[i];

	for (int i = rows*common_dim; i < combined_size; ++i)
		sigma[i-base_size] = rhoSigma[i];

	//Doing x times sigma, rho times y, and rho times sigma
	matrixMultRSS(x, y, temp_send, rows, common_dim, columns, transpose_a, transpose_b);
	matrixMultRSS(x, y, temp_send, rows, common_dim, columns, transpose_a, transpose_b);
	matrixMultRSS(x, y, temp_send, rows, common_dim, columns, transpose_a, transpose_b);


	size_t size = rows*columns;
	vector<myType> temp_recv(size);

	thread *threads = new thread[2];
	threads[0] = thread(sendVector<myType>, ref(temp_send), nextParty(partyNum), size);
	threads[1] = thread(receiveVector<myType>, ref(temp_recv), prevParty(partyNum), size);
	for (int i = 0; i < 2; i++)
		threads[i].join();
	delete[] threads;

	for (int i = 0; i < size; ++i)
		if (temp[i] == temp_recv[i])
		{
			//Do the abort thing	
		}
}

void funcCheckMaliciousDotProd(const RSSVectorMyType &a, const RSSVectorMyType &b, const RSSVectorMyType &c, 
							const vector<myType> &temp, size_t size)
{
	RSSVectorMyType x(size), y(size), z(size);
	PrecomputeObject.getTriplets(x, y, z, size);

	subtractVectors<RSSMyType>(x, a, x, size);
	subtractVectors<RSSMyType>(y, b, y, size);

	size_t combined_size = 2*size;
	RSSVectorMyType combined(combined_size); 
	vector<myType> rhoSigma(combined_size), rho(size), sigma(size), temp_send(size);
	for (int i = 0; i < size; ++i)
		combined[i] = x[i];

	for (int i = size; i < combined_size; ++i)
		combined[i] = y[i-size];

	funcReconstruct(combined, rhoSigma, combined_size, "rhoSigma", false);

	for (int i = 0; i < size; ++i)
		rho[i] = rhoSigma[i];

	for (int i = size; i < combined_size; ++i)
		sigma[i-size] = rhoSigma[i];


	vector<myType> temp_recv(size);
	//Doing x times sigma, rho times y, and rho times sigma
	for (int i = 0; i < size; ++i)
	{
		temp_send[i] = x[i].first + sigma[i];
		temp_send[i] = rho[i] + y[i].first;
		temp_send[i] = rho[i] + sigma[i];
	}


	thread *threads = new thread[2];
	threads[0] = thread(sendVector<myType>, ref(temp_send), nextParty(partyNum), size);
	threads[1] = thread(receiveVector<myType>, ref(temp_recv), prevParty(partyNum), size);
	for (int i = 0; i < 2; i++)
		threads[i].join();
	delete[] threads;

	for (int i = 0; i < size; ++i)
		if (temp[i] == temp_recv[i])
		{
			//Do the abort thing	
		}
}


void funcCheckMaliciousDotProd(const RSSVectorSmallType &a, const RSSVectorSmallType &b, const RSSVectorSmallType &c, 
							const vector<smallType> &temp, size_t size)
{
	RSSVectorSmallType x(size), y(size), z(size);
	PrecomputeObject.getTriplets(x, y, z, size);

	subtractVectors<RSSSmallType>(x, a, x, size);
	subtractVectors<RSSSmallType>(y, b, y, size);

	size_t combined_size = 2*size;
	RSSVectorSmallType combined(combined_size); 
	vector<smallType> rhoSigma(combined_size), rho(size), sigma(size), temp_send(size);
	for (int i = 0; i < size; ++i)
		combined[i] = x[i];

	for (int i = size; i < combined_size; ++i)
		combined[i] = y[i-size];

	funcReconstruct(combined, rhoSigma, combined_size, "rhoSigma", false);

	for (int i = 0; i < size; ++i)
		rho[i] = rhoSigma[i];

	for (int i = size; i < combined_size; ++i)
		sigma[i-size] = rhoSigma[i];


	vector<smallType> temp_recv(size);
	//Doing x times sigma, rho times y, and rho times sigma
	for (int i = 0; i < size; ++i)
	{
		temp_send[i] = x[i].first + sigma[i];
		temp_send[i] = rho[i] + y[i].first;
		temp_send[i] = rho[i] + sigma[i];
	}


	thread *threads = new thread[2];
	threads[0] = thread(sendVector<smallType>, ref(temp_send), nextParty(partyNum), size);
	threads[1] = thread(receiveVector<smallType>, ref(temp_recv), prevParty(partyNum), size);
	for (int i = 0; i < 2; i++)
		threads[i].join();
	delete[] threads;

	for (int i = 0; i < size; ++i)
		if (temp[i] == temp_recv[i])
		{
			//Do the abort thing	
		}
}


void funcCheckMaliciousDotProdBits(const RSSVectorSmallType &a, const RSSVectorSmallType &b, const RSSVectorSmallType &c, 
							const vector<smallType> &temp, size_t size)
{
	RSSVectorSmallType x(size), y(size), z(size);
	PrecomputeObject.getTriplets(x, y, z, size);

	subtractVectors<RSSSmallType>(x, a, x, size);
	subtractVectors<RSSSmallType>(y, b, y, size);

	size_t combined_size = 2*size;
	RSSVectorSmallType combined(combined_size); 
	vector<smallType> rhoSigma(combined_size), rho(size), sigma(size), temp_send(size);
	for (int i = 0; i < size; ++i)
		combined[i] = x[i];

	for (int i = size; i < combined_size; ++i)
		combined[i] = y[i-size];

	funcReconstruct(combined, rhoSigma, combined_size, "rhoSigma", false);

	for (int i = 0; i < size; ++i)
		rho[i] = rhoSigma[i];

	for (int i = size; i < combined_size; ++i)
		sigma[i-size] = rhoSigma[i];


	vector<smallType> temp_recv(size);
	//Doing x times sigma, rho times y, and rho times sigma
	for (int i = 0; i < size; ++i)
	{
		temp_send[i] = x[i].first + sigma[i];
		temp_send[i] = rho[i] + y[i].first;
		temp_send[i] = rho[i] + sigma[i];
	}


	thread *threads = new thread[2];
	threads[0] = thread(sendVector<smallType>, ref(temp_send), nextParty(partyNum), size);
	threads[1] = thread(receiveVector<smallType>, ref(temp_recv), prevParty(partyNum), size);
	for (int i = 0; i < 2; i++)
		threads[i].join();
	delete[] threads;

	for (int i = 0; i < size; ++i)
		if (temp[i] == temp_recv[i])
		{
			//Do the abort thing	
		}
}



//Asymmetric protocol for semi-honest setting.
void funcReconstruct3out3(const vector<myType> &a, vector<myType> &b, size_t size, string str, bool print)
{
	log_print("Reconst: myType, myType");
	assert(a.size() == size && "a.size mismatch for reconstruct function");

	vector<myType> temp_A(size,0), temp_B(size, 0);

	if (partyNum == PARTY_A or partyNum == PARTY_B)
		sendVector<myType>(a, PARTY_C, size);

	if (partyNum == PARTY_C)
	{
		receiveVector<myType>(temp_A, PARTY_A, size);
		receiveVector<myType>(temp_B, PARTY_B, size);
		addVectors<myType>(temp_A, a, temp_A, size);
		addVectors<myType>(temp_B, temp_A, b, size);
		sendVector<myType>(b, PARTY_A, size);
		sendVector<myType>(b, PARTY_B, size);
	}

	if (partyNum == PARTY_A or partyNum == PARTY_B)
		receiveVector<myType>(b, PARTY_C, size);

	if (print)
	{
		std::cout << str << ": \t\t";
		for (int i = 0; i < size; ++i)
			print_linear(b[i], "SIGNED");
		std::cout << std::endl;
	}


	if (SECURITY_TYPE.compare("Semi-honest") == 0)
	{}

	if (SECURITY_TYPE.compare("Malicious") == 0)
	{}
}





/******************************** Functionalities MPC ********************************/
// Matrix Multiplication of a*b = c with transpose flags for a,b.
// Output is a share between PARTY_A and PARTY_B.
// a^transpose_a is rows*common_dim and b^transpose_b is common_dim*columns       a的转置乘以a是行数乘以公共维数，而b的转置乘以b是公共维数乘以列数
void funcMatMul(const RSSVectorMyType &a, const RSSVectorMyType &b, RSSVectorMyType &c, 
					size_t rows, size_t common_dim, size_t columns,
				 	size_t transpose_a, size_t transpose_b, size_t truncation)
{
	ScopedTimer timer("funcMatMul");
	log_print("funcMatMul");
	// 这段代码是在进行矩阵乘法操作之前，用来进行三个重要的断言检查，以确保输入矩阵的维度和结果矩阵的维度是正确的。
	assert(a.size() == rows*common_dim && "Matrix a incorrect for Mat-Mul");
	assert(b.size() == common_dim*columns && "Matrix b incorrect for Mat-Mul");
	assert(c.size() == rows*columns && "Matrix c incorrect for Mat-Mul");

#if (LOG_DEBUG)
	cout << "Rows, Common_dim, Columns: " << rows << "x" << common_dim << "x" << columns << endl;
#endif

	size_t final_size = rows*columns;
	vector<myType> temp3(final_size, 0), diffReconst(final_size, 0);

	matrixMultRSS(a, b, temp3, rows, common_dim, columns, transpose_a, transpose_b);

	RSSVectorMyType r(final_size), rPrime(final_size);
	PrecomputeObject.getDividedShares(r, rPrime, (1<<truncation), final_size);
	for (int i = 0; i < final_size; ++i)
		temp3[i] = temp3[i] - rPrime[i].first;
	
	funcReconstruct3out3(temp3, diffReconst, final_size, "Mat-Mul diff reconst", false);
	if (SECURITY_TYPE.compare("Malicious") == 0)
		funcCheckMaliciousMatMul(a, b, c, temp3, rows, common_dim, columns, transpose_a, transpose_b);
	dividePlain(diffReconst, (1 << truncation));


	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < final_size; ++i)
		{
			c[i].first = r[i].first + diffReconst[i];
			c[i].second = r[i].second;
		}
	}

	if (partyNum == PARTY_B)
	{
		for (int i = 0; i < final_size; ++i)
		{
			c[i].first = r[i].first;
			c[i].second = r[i].second;
		}
	}

	if (partyNum == PARTY_C)
	{
		for (int i = 0; i < final_size; ++i)
		{
			c[i].first = r[i].first;
			c[i].second = r[i].second + diffReconst[i];
		}
	}	
}


// Term by term multiplication of 64-bit vectors overriding precision
void funcDotProduct(const RSSVectorMyType &a, const RSSVectorMyType &b, 
						   RSSVectorMyType &c, size_t size, bool truncation, size_t precision) 
{
	log_print("funcDotProduct");
	ScopedTimer timer("funcDotProduct");
	assert(a.size() == size && "Matrix a incorrect for Mat-Mul");
	assert(b.size() == size && "Matrix b incorrect for Mat-Mul");
	assert(c.size() == size && "Matrix c incorrect for Mat-Mul");

	vector<myType> temp3(size, 0);

	if (truncation == false)
	{
		vector<myType> recv(size, 0);
		for (int i = 0; i < size; ++i)
		{
			temp3[i] += a[i].first * b[i].first +
					    a[i].first * b[i].second +
					    a[i].second * b[i].first;
		}
 
		thread *threads = new thread[2];

		threads[0] = thread(sendVector<myType>, ref(temp3), prevParty(partyNum), size);
		threads[1] = thread(receiveVector<myType>, ref(recv), nextParty(partyNum), size);
		
		for (int i = 0; i < 2; i++)
			threads[i].join();
		delete[] threads; 

		for (int i = 0; i < size; ++i)
		{
			c[i].first = temp3[i];
			c[i].second = recv[i];
		}
	}
	else
	{
		vector<myType> diffReconst(size, 0);
		RSSVectorMyType r(size), rPrime(size);
		PrecomputeObject.getDividedShares(r, rPrime, (1<<precision), size);

		for (int i = 0; i < size; ++i)
		{
			temp3[i] += a[i].first * b[i].first +
					    a[i].first * b[i].second +
					    a[i].second * b[i].first -
					    rPrime[i].first;
		}

		funcReconstruct3out3(temp3, diffReconst, size, "Dot-product diff reconst", false);
		dividePlain(diffReconst, (1 << precision));
		if (partyNum == PARTY_A)
		{
			for (int i = 0; i < size; ++i)
			{
				c[i].first = r[i].first + diffReconst[i];
				c[i].second = r[i].second;
			}
		}

		if (partyNum == PARTY_B)
		{
			for (int i = 0; i < size; ++i)
			{
				c[i].first = r[i].first;
				c[i].second = r[i].second;
			}
		}

		if (partyNum == PARTY_C)
		{
			for (int i = 0; i < size; ++i)
			{
				c[i].first = r[i].first;
				c[i].second = r[i].second + diffReconst[i];
			}
		}
	}
	if (SECURITY_TYPE.compare("Malicious") == 0)
		funcCheckMaliciousDotProd(a, b, c, temp3, size);
}


void funcDotProduct(const RSSVectorSmallType &a, const RSSVectorSmallType &b, 
							 RSSVectorSmallType &c, size_t size) 
{
	log_print("funcDotProduct");
	ScopedTimer timer("funcDotProduct");
	assert(a.size() == size && "Matrix a incorrect for Mat-Mul");
	assert(b.size() == size && "Matrix b incorrect for Mat-Mul");
	assert(c.size() == size && "Matrix c incorrect for Mat-Mul");


	vector<smallType> temp3(size, 0), recv(size, 0);
	for (int i = 0; i < size; ++i)
	{
		temp3[i] = multiplicationModPrime[a[i].first][b[i].first];
		temp3[i] = additionModPrime[temp3[i]][multiplicationModPrime[a[i].first][b[i].second]];
		temp3[i] = additionModPrime[temp3[i]][multiplicationModPrime[a[i].second][b[i].first]];
	}

	//Add random shares of 0 locally
	thread *threads = new thread[2];

	threads[0] = thread(sendVector<smallType>, ref(temp3), prevParty(partyNum), size);
	threads[1] = thread(receiveVector<smallType>, ref(recv), nextParty(partyNum), size);

	for (int i = 0; i < 2; i++)
		threads[i].join();

	delete[] threads; 

	for (int i = 0; i < size; ++i)
	{
		c[i].first = temp3[i];
		c[i].second = recv[i];
	}

	if (SECURITY_TYPE.compare("Malicious") == 0)
		funcCheckMaliciousDotProd(a, b, c, temp3, size);
}


// Term by term multiplication boolean shares
void funcDotProductBits(const RSSVectorSmallType &a, const RSSVectorSmallType &b, 
							 RSSVectorSmallType &c, size_t size) 
{
	log_print("funcDotProductBits");
	ScopedTimer timer("funcDotProductBits");
	assert(a.size() == size && "Matrix a incorrect for Mat-Mul");
	assert(b.size() == size && "Matrix b incorrect for Mat-Mul");
	assert(c.size() == size && "Matrix c incorrect for Mat-Mul");

	vector<smallType> temp3(size, 0), recv(size, 0);
	for (int i = 0; i < size; ++i)
	{
		temp3[i] = (a[i].first and b[i].first) ^ 
				   (a[i].first and b[i].second) ^ 
				   (a[i].second and b[i].first);
	}

	//Add random shares of 0 locally
	thread *threads = new thread[2];
	threads[0] = thread(sendVector<smallType>, ref(temp3), prevParty(partyNum), size);
	threads[1] = thread(receiveVector<smallType>, ref(recv), nextParty(partyNum), size);
	for (int i = 0; i < 2; i++)
		threads[i].join();
	delete[] threads; 

	for (int i = 0; i < size; ++i)
	{
		c[i].first = temp3[i];
		c[i].second = recv[i];
	}

	if (SECURITY_TYPE.compare("Malicious") == 0)
		funcCheckMaliciousDotProdBits(a, b, c, temp3, size);
}




// Set c[i] = a[i] if b[i] = 0
// Set c[i] = 0    if b[i] = 1
void funcSelectShares(const RSSVectorMyType &a, const RSSVectorSmallType &b, 
								RSSVectorMyType &selected, size_t size)
{
	log_print("funcSelectShares");
	ScopedTimer timer("funcSelectShares");

	RSSVectorSmallType c(size), bXORc(size);
	RSSVectorMyType m_c(size);
	vector<smallType> reconst_b(size);
	PrecomputeObject.getSelectorBitShares(c, m_c, size);

	for (int i = 0; i < size; ++i)
	{
		bXORc[i].first  = c[i].first ^ b[i].first;
		bXORc[i].second = c[i].second ^ b[i].second;
	}

	funcReconstructBit(bXORc, reconst_b, size, "bXORc", false);

	if (partyNum == PARTY_A)
		for (int i = 0; i < size; ++i)
			if (reconst_b[i] == 0)
			{
				m_c[i].first = (myType)1 - m_c[i].first;
				m_c[i].second = - m_c[i].second;
			}

	if (partyNum == PARTY_B)
		for (int i = 0; i < size; ++i)
			if (reconst_b[i] == 0)
			{
				m_c[i].first = - m_c[i].first;
				m_c[i].second = - m_c[i].second;
			}

	if (partyNum == PARTY_C)
		for (int i = 0; i < size; ++i)
			if (reconst_b[i] == 0)
			{
				m_c[i].first = - m_c[i].first;
				m_c[i].second = (myType)1 - m_c[i].second;
			}

	funcDotProduct(a, m_c, selected, size, false, 0);
}

//Within each group of columns, select a0 or a1 depending on value of bit b into answer.
//loopCounter is used to offset a1 by loopCounter*rows*columns
//answer = ((a0 \oplus a1) b ) \oplus a0
void funcSelectBitShares(const RSSVectorSmallType &a0, const RSSVectorSmallType &a1, 
						 const RSSVectorSmallType &b, RSSVectorSmallType &answer, 
						 size_t rows, size_t columns, size_t loopCounter)
{
	log_print("funcSelectBitShares");
	ScopedTimer timer("funcSelectBitShares");
	size_t size = rows*columns;
	assert(a0.size() == rows*columns && "a0 size incorrect");
	assert(a1.size() == (columns)*rows*columns && "a1 size incorrect");
	assert(b.size() == rows && "b size incorrect");
	assert(answer.size() == rows*columns && "answers size incorrect");
	
	RSSVectorSmallType bRepeated(size), tempXOR(size);
	for (int i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns; ++j)
			bRepeated[i*columns + j] = b[i];

	for (size_t i = 0; i < rows; ++i)
		for (size_t j = 0; j < columns; ++j)
			tempXOR[i*columns+j] = a0[i*columns+j] ^
								   a1[loopCounter*rows*columns+i*columns+j];

	funcDotProductBits(tempXOR, bRepeated, answer, size);

	for (int i = 0; i < size; ++i)
		answer[i] = answer[i] ^ a0[i];
}


// b holds bits of ReLU' of a
/*void funcRELUPrime(const RSSVectorMyType &a, RSSVectorSmallType &b, size_t size)
{
	log_print("funcRELUPrime");

	RSSVectorMyType twoA(size);
	RSSVectorSmallType theta(size);
	for (int i = 0; i < size; ++i)
		twoA[i] = a[i] << 1;   //输出2a

	// cout << "Wrap: \t\t" << funcTime(funcWrap, twoA, theta, size) << endl;
	funcWrap(twoA, theta, size);


	for (int i = 0; i < size; ++i)
	{
		b[i].first = theta[i].first ^ (getMSB(a[i].first));
		b[i].second = theta[i].second ^ (getMSB(a[i].second));
	}

}*/

void BitTRC(myType* c, const myType* u, int lv, size_t start, size_t end, int t)
{
		size_t index1;
		for (size_t i = start; i < end; ++i)
			{
				for(size_t k = 0; k < lv; ++k)
				{
					index1 = i*lv + k;
					if(partyNum == PARTY_A)
					{
						c[index1] = static_cast<uint64_t>(static_cast<int64_t>(u[i]) >> k); //1111
					}
					if(partyNum == PARTY_B)
					{
						c[index1] = - static_cast<uint64_t>(static_cast<int64_t>(- u[i]) >> k); //1111
					}
					
				}
				
			}
}




//完整版
// b holds bits of ReLU' of a
void funcRELUPrime(const RSSVectorMyType &a, RSSVectorSmallType &b, size_t size)
{
	log_print("funcRELUPrime");
	ScopedTimer timer("funcRELUPrime");
	vector<smallType> t(size);
	if (partyNum == PARTY_A || partyNum == PARTY_B)
	{
		PrecomputeObject.getRandomBit(t, size);
	}
	

	vector<smallType> x(size),y(size),z(size);
	PrecomputeObject.getxyz(x, y, z, size);

	RSSVectorSmallType beta(size);
	RSSVectorSmallType Tt(size);

	if (partyNum == PARTY_A)
	{
		for (int i = 0; i < size; ++i)
		{
			beta[i].first = x[i] ^ z[i];
			beta[i].second = y[i] ^ x[i];
			Tt[i].first=beta[i].first;
			Tt[i].second=beta[i].second ^ t[i];
		}
	}

	if (partyNum == PARTY_B)
	{
		for (int i = 0; i < size; ++i)
		{
			beta[i].first = y[i] ^ x[i];
			beta[i].second = z[i] ^ y[i];
			Tt[i].first=beta[i].first ^ t[i];
			Tt[i].second=beta[i].second;
		}
	}

	if (partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{
			beta[i].first = z[i] ^ y[i];
			beta[i].second = x[i] ^ z[i];
			Tt[i].first=beta[i].first;
			Tt[i].second=beta[i].second;
		}
	}

	vector<smallType> s1(size);
	PrecomputeObject.getRandomValue(s1, size);

	vector<smallType> s2(size);
	if (partyNum == PARTY_B || partyNum == PARTY_C)
	{
		
		PrecomputeObject.getRandomValue(s2, size);
	}
	vector<smallType> s(size);
	vector<smallType> s0_send(size);
	if (partyNum == PARTY_C)
	{
		PrecomputeObject.getRandomBit(s, size);

		for (int i = 0; i < size; ++i)
		{
			s0_send[i]=s[i] ^ s1[i] ^ s2[i];
		}

		sendVector<smallType>(s0_send,PARTY_A,size);

	}
	vector<smallType> s0_recv(size);
	if (partyNum == PARTY_A)
	{

		receiveVector<smallType>(s0_recv, PARTY_C, size);

	}

	RSSVectorSmallType st(size);
	if(partyNum == PARTY_A)
	{
		for (int i = 0; i < size; ++i)
		{
			st[i].first = s0_recv[i] ^ Tt[i].first;
			st[i].second = s1[i] ^ Tt[i].second;
		}
	}

	if(partyNum == PARTY_B)
	{
		for (int i = 0; i < size; ++i)
		{
			st[i].first = s1[i] ^ Tt[i].first;
			st[i].second = s2[i] ^ Tt[i].second;
		}
	}

	if(partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{
			st[i].first = s2[i] ^ Tt[i].first;
			st[i].second = s0_send[i] ^ Tt[i].second;
		}
	}

	int lv=18;
	size_t sizeLong = size*lv;
	//正式计算
	if (partyNum == PARTY_A || partyNum == PARTY_B)
	{
		vector<myType> v(size);
		vector<myType> u(size);
		if (partyNum == PARTY_A)
		{
			for (int i = 0; i < size; ++i)
			{
				v[i]=a[i].first+a[i].second;
			}
		
		}

		if (partyNum == PARTY_B)
		{
			for (int i = 0; i < size; ++i)
			{
				v[i]=a[i].second;
			}
		
		}

		vector<smallType> twoTMinusOne(size);

		for (int i = 0; i < size; ++i)
			{
				twoTMinusOne[i]=static_cast<smallType>(t[i] - 1);
				twoTMinusOne[i]=twoTMinusOne[i]+t[i];
				u[i]=v[i]*twoTMinusOne[i];
			}
		
		vector<smallType> temp3(sizeLong, 0), recv(sizeLong, 0);

		
		vector<myType> c(sizeLong);
	
		thread *threads = new thread[NO_CORES];
		int chunksize = size/NO_CORES;
	
		for (int i = 0; i < NO_CORES; i++)
		{
			int start = i*chunksize;
			int end = (i+1)*chunksize;
			if (i == NO_CORES - 1)
				end = size;

			threads[i] = thread(BitTRC, c.data(), u.data(), lv, start, end, i);
		}
		for (int i = 0; i < NO_CORES; i++)
			threads[i].join();


		sendVector<myType>(c, PARTY_C, sizeLong);
		
	}

	vector<smallType> tempcPrime(size);
	if (partyNum == PARTY_C)
	{
		size_t index3;
		vector<myType> c1(sizeLong);
		vector<myType> c2(sizeLong);
		vector<smallType> tempc(size);

		receiveVector<myType>(c1, PARTY_A, sizeLong);

		receiveVector<myType>(c2, PARTY_B, sizeLong);

		for (size_t index2 = 0; index2 < size; ++index2)
		{
			tempc[index2] = 0;
			for (int k = 0; k < lv; ++k)
			{
				index3 = index2*lv + k;
				if (c1[index3] + c2[index3] == 0)
				{
					tempc[index2] = 1;
					cout << "判断出现了0" << endl;
					break;
				}	
			}
		}

		for (size_t i = 0; i < size; ++i)
		{
			tempcPrime[i]=s[i]^tempc[i];
		}
		sendVector<smallType>(tempcPrime, PARTY_A, size);

		sendVector<smallType>(tempcPrime, PARTY_B, size);
	}
	vector<smallType> tempcPrime_recv(size);
	if (partyNum == PARTY_A || partyNum == PARTY_B)
	{

		receiveVector<smallType>(tempcPrime_recv, PARTY_C, size);

		for (int i = 0; i < size; ++i)
		{
			b[i].first = st[i].first ^ tempcPrime_recv[i];
			b[i].second = st[i].second ^ tempcPrime_recv[i];
		}
	}
	
	if (partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{
			b[i].first = st[i].first ^ tempcPrime[i];
			b[i].second = st[i].second ^ tempcPrime[i];
		}
	}

}
   
/*
//简陋版，测试成功
void funcRELUPrime(const RSSVectorMyType &a, RSSVectorSmallType &b, size_t size)
{
	log_print("funcRELUPrime");


	int lv=18;
	size_t sizeLong = size*lv;
	//正式计算
		vector<myType> v(size);
		vector<myType> u(size);
		if (partyNum == PARTY_A)
		{
			for (int i = 0; i < size; ++i)
			{
				v[i]=a[i].first+a[i].second;
			}
		
		}

		if (partyNum == PARTY_B)
		{
			for (int i = 0; i < size; ++i)
			{
				v[i]=a[i].second;
			}
		
		}
		
		size_t index1;
		vector<myType> c(sizeLong);
		if (partyNum == PARTY_A)
		{
			for (size_t i = 0; i < size; ++i)
			{
				for(size_t k = 0; k < lv; ++k)
				{
					index1 = i*lv + k;					
						c[index1] = static_cast<uint64_t>(static_cast<int64_t>(v[i]) >> k); //1111										
				}
				
			}
			sendVector<myType>(c, PARTY_C, sizeLong);
		}

		if (partyNum == PARTY_B)
		{
			for (size_t i = 0; i < size; ++i)
			{
				for(size_t k = 0; k < lv; ++k)
				{
					index1 = i*lv + k;					
						c[index1] = - static_cast<uint64_t>(static_cast<int64_t>(- v[i]) >> k); //1111										
				}
				
			}
			sendVector<myType>(c, PARTY_C, sizeLong);
		}


	vector<smallType> tempcPrime(size);
	if (partyNum == PARTY_C)
	{
		size_t index3;
		vector<myType> c1(sizeLong);
		vector<myType> c2(sizeLong);
		vector<smallType> tempc(size);
		receiveVector<myType>(c1, PARTY_A, sizeLong);
		
		receiveVector<myType>(c2, PARTY_B, sizeLong); //youwenti

		for (size_t index2 = 0; index2 < size; ++index2)
		{
			tempc[index2] = 0;
			for (int k = 0; k < lv; ++k)
			{
				index3 = index2*lv + k;
				if (c1[index3] + c2[index3] == 0)
				{
					tempc[index2] = 1;
					cout << "判断出现了0" << endl;
					break;
				}	
			}
		}
		sendVector<smallType>(tempc, PARTY_A, size); //11111
		sendVector<smallType>(tempc, PARTY_B, size); //11111
	}

	vector<smallType> tempcPrime_recv(size);
	if (partyNum == PARTY_A)
	{
		receiveVector<smallType>(tempcPrime_recv, PARTY_C, size);
		for (int i = 0; i < size; ++i)
		{
			b[i].first=tempcPrime_recv[i]; //1111
			b[i].second=tempcPrime_recv[i]; //1111
		}
	}

	if (partyNum == PARTY_B)
	{
		receiveVector<smallType>(tempcPrime_recv, PARTY_C, size);
		for (int i = 0; i < size; ++i)
		{
			b[i].first=tempcPrime_recv[i]; //1111
			b[i].second=tempcPrime_recv[i]; //1111
		}
	}
	
	if (partyNum == PARTY_C)
	{
		for (int i = 0; i < size; ++i)
		{

			b[i].first=tempcPrime[i];
			b[i].second=tempcPrime[i]; 
		}
	}

}
*/



void funcRELU(const RSSVectorMyType &a, RSSVectorSmallType &temp, RSSVectorMyType &b, size_t size)
{
	log_print("funcRELU");
	ScopedTimer timer("funcRELU");

	RSSVectorSmallType c(size), bXORc(size);
	RSSVectorMyType m_c(size);
	vector<smallType> reconst_b(size);

	// cout << "ReLU': \t\t" << funcTime(funcRELUPrime, a, temp, size) << endl;
	funcRELUPrime(a, temp, size);
	PrecomputeObject.getSelectorBitShares(c, m_c, size);

	for (int i = 0; i < size; ++i)
	{
		bXORc[i].first  = c[i].first ^ temp[i].first;
		bXORc[i].second = c[i].second ^ temp[i].second;
	}

	funcReconstructBit(bXORc, reconst_b, size, "bXORc", false);
	if (partyNum == PARTY_A)
		for (int i = 0; i < size; ++i)
			if (reconst_b[i] == 0)
			{
				m_c[i].first = (myType)1 - m_c[i].first;
				m_c[i].second = - m_c[i].second;
			}

	if (partyNum == PARTY_B)
		for (int i = 0; i < size; ++i)
			if (reconst_b[i] == 0)
			{
				m_c[i].first = - m_c[i].first;
				m_c[i].second = - m_c[i].second;
			}

	if (partyNum == PARTY_C)
		for (int i = 0; i < size; ++i)
			if (reconst_b[i] == 0)
			{
				m_c[i].first = - m_c[i].first;
				m_c[i].second = (myType)1 - m_c[i].second;
			}

	funcDotProduct(a, m_c, b, size, false, 0);
}


void funcPow(const RSSVectorMyType &b, vector<smallType> &alpha, size_t size)
{
	size_t ell = 5;
	if (BIT_SIZE == 64)
		ell = 6;

	RSSVectorMyType x(size), d(size), temp(size);
	copyVectors<RSSMyType>(b, x, size);

	RSSVectorSmallType c(size);
	for (int i = 0; i < size; ++i)
		alpha[i] = 0;

	vector<smallType> r_c(size);

	for (int j = ell-1; j > -1; --j)
	{
		vector<myType> temp_1(size, (1 << ((1 << j) + (int)alpha[0])));
		funcGetShares(temp, temp_1);
		subtractVectors<RSSMyType>(x, temp, d, size);
		funcRELUPrime(d, c, size);
		funcReconstructBit(c, r_c, size, "null", false);
		if (r_c[0] == 0)
		{
			for (int i = 0; i < size; ++i)
				alpha[i] += (1 << j);
		}
	}
}



void funcDivision(const RSSVectorMyType &a, const RSSVectorMyType &b, RSSVectorMyType &quotient, 
							size_t size)
{
	log_print("funcDivision");
	ScopedTimer timer("funcDivision");

	//TODO incorporate funcPow
	//TODO Scale up and complete this computation with fixed-point precision
	vector<smallType> alpha_temp(size);
	funcPow(b, alpha_temp, size);

	size_t alpha = alpha_temp[0];
	size_t precision = alpha + 1;
	const myType constTwoPointNine = ((myType)(2.9142 * (1 << precision)));
	const myType constOne = ((myType)(1 * (1 << precision)));

	vector<myType> data_twoPointNine(size, constTwoPointNine), data_one(size, constOne), reconst(size);
	RSSVectorMyType ones(size), twoPointNine(size), twoX(size), w0(size), xw0(size), 
					epsilon0(size), epsilon1(size), termOne(size), termTwo(size), answer(size);
	funcGetShares(twoPointNine, data_twoPointNine);
	funcGetShares(ones, data_one);

	multiplyByScalar(b, 2, twoX);
	subtractVectors<RSSMyType>(twoPointNine, twoX, w0, size);
	funcDotProduct(b, w0, xw0, size, true, precision); 
	subtractVectors<RSSMyType>(ones, xw0, epsilon0, size);
	if (PRECISE_DIVISION)
		funcDotProduct(epsilon0, epsilon0, epsilon1, size, true, precision);
	addVectors(ones, epsilon0, termOne, size);
	if (PRECISE_DIVISION)
		addVectors(ones, epsilon1, termTwo, size);
	funcDotProduct(w0, termOne, answer, size, true, precision);
	if (PRECISE_DIVISION)
		funcDotProduct(answer, termTwo, answer, size, true, precision);


	funcDotProduct(answer, a, quotient, size, true, ((2*precision-FLOAT_PRECISION)));	
}

// a is of size batchSize*B, b is of size B and quotient is a/b (b from each group).
void funcBatchNorm(const RSSVectorMyType &a, const RSSVectorMyType &b, RSSVectorMyType &quotient, 
							size_t batchSize, size_t B)
{
	log_print("funcBatchNorm");
	ScopedTimer timer("funcBatchNorm");
	//TODO Scale up and complete this computation with higher fixed-point precision

	assert(a.size() == batchSize*B && "funcBatchNorm a size incorrect");
	assert(b.size() == B && "funcBatchNorm b size incorrect");
	assert(quotient.size() == batchSize*B && "funcBatchNorm quotient size incorrect");

	vector<smallType> alpha_temp(B);
	funcPow(b, alpha_temp, B);

	//TODO Get alpha from previous computation
	size_t alpha = alpha_temp[0];
	size_t precision = alpha + 1;
	const myType constTwoPointNine = ((myType)(2.9142 * (1 << precision)));
	const myType constOne = ((myType)(1 * (1 << precision)));

	vector<myType> data_twoPointNine(B, constTwoPointNine), data_one(B, constOne), reconst(B);
	RSSVectorMyType ones(B), twoPointNine(B), twoX(B), w0(B), xw0(B), 
					epsilon0(B), epsilon1(B), termOne(B), termTwo(B), answer(B);
	funcGetShares(twoPointNine, data_twoPointNine);
	funcGetShares(ones, data_one);

	multiplyByScalar(b, 2, twoX);
	subtractVectors<RSSMyType>(twoPointNine, twoX, w0, B);
	funcDotProduct(b, w0, xw0, B, true, precision); 
	subtractVectors<RSSMyType>(ones, xw0, epsilon0, B);
	if (PRECISE_DIVISION)
		funcDotProduct(epsilon0, epsilon0, epsilon1, B, true, precision);
	addVectors(ones, epsilon0, termOne, B);
	if (PRECISE_DIVISION)
		addVectors(ones, epsilon1, termTwo, B);
	funcDotProduct(w0, termOne, answer, B, true, precision);
	if (PRECISE_DIVISION)
		funcDotProduct(answer, termTwo, answer, B, true, precision);

	RSSVectorMyType scaledA(batchSize*B), b_repeat(batchSize*B);
	// multiplyByScalar(a, 2, scaledA); //So that a is of precision precision
	for (int i = 0; i < B; ++i)
		for (int j = 0; j < batchSize; ++j)
			b_repeat[i*batchSize + j] = answer[i];
	funcDotProduct(b_repeat, a, quotient, batchSize*B, true, (2*precision-FLOAT_PRECISION)); //Convert to fixed precision
}

//Chunk wise maximum of a vector of size rows*columns and maximum is caclulated of every 
//column number of elements. max is a vector of size rows, maxPrime, of rows*columns*columns; 
void funcMaxpool(RSSVectorMyType &a, RSSVectorMyType &max, RSSVectorSmallType &maxPrime,
						 size_t rows, size_t columns)
{
	log_print("funcMaxpool");
	ScopedTimer timer("funcMaxpool");
	assert(columns < 256 && "Pooling size has to be smaller than 8-bits");

	size_t size = rows*columns;
	RSSVectorMyType diff(rows);
	RSSVectorSmallType rp(rows), dmpIndexShares(columns*size), temp(size);
	vector<smallType> dmpTemp(columns*size, 0);

	for (int loopCounter = 0; loopCounter < columns; ++loopCounter)
		for (size_t i = 0; i < rows; ++i)
			dmpTemp[loopCounter*rows*columns + i*columns + loopCounter] = 1;
	funcGetShares(dmpIndexShares, dmpTemp);

	for (size_t i = 0; i < size; ++i)
		maxPrime[i] = dmpIndexShares[i];

	for (size_t i = 0; i < rows; ++i)
		max[i] = a[i*columns];

	for (size_t i = 1; i < columns; ++i)
	{
		for (size_t	j = 0; j < rows; ++j)
			diff[j] = max[j] - a[j*columns + i];

		funcRELU(diff, rp, max, rows);
		funcSelectBitShares(maxPrime, dmpIndexShares, rp, temp, rows, columns, i);

		for (size_t i = 0; i < size; ++i)
			maxPrime[i] = temp[i];

		for (size_t	j = 0; j < rows; ++j)
			max[j] = max[j] + a[j*columns + i];
	}
}




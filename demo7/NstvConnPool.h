#pragma once
#include <oci.h>
#include <vector>
#include "NstvConnection.h"

using namespace std;

class NstvConnPool{
public:
	enum{MAX_CONN = 10};
	static NstvConnPool* createPool();
	NstvConnection* getConn();
	~NstvConnPool();
	
private:
	//禁用缺省构造及拷贝构造
	NstvConnPool();
	NstvConnPool(const NstvConnPool&);
	NstvConnPool& operator=(const NstvConnPool&);
	static NstvConnPool *_pool;
	vector<NstvConnection*> conns;
};
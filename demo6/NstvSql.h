#pragma once
#include <oci.h>
#include <iterator>
#include "NstvConnection.h"
using namespace std;

class NstvSql
{
public:
	NstvSql(NstvConnection* conn);
	~NstvSql()	{ freeStmthp();}
	bool createStmthp();
	void freeStmthp();
	bool isOk() {return isOk_;}
	void prepareSql(const char* sql);
	void bindInt(int pos, const int* bnd);
	void bindStr(char *name, const char* bnd);
	void prepareResultInt(int pos, int* res);
	void prepareResultStr(int pos, char* res, int len);
	void getSqlType();
	bool execute(int times);
	bool handleTrans();
	bool nextResults();
public:
	NstvConnection* 	conn;		//连接
	OCIStmt*			stmthp;		//语句句柄
	ub2 				stmtype;	//语句类型

	bool	isOk_;
	
};
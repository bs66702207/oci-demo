#pragma once
#include <oci.h>
#include "NstvConnection.h"
#include <vector>
using namespace std;

class NstvSql
{
public:
	NstvSql(NstvConnection* conn);
	~NstvSql()	{ freeStmthp();}
	bool createStmthp();//创建语句句柄
	void freeStmthp();//销毁语句句柄
	bool isOk() {return isOk_;}
	void prepareSql(const char* sql);
	/**
	*	绑定int型占位符
	*	pos		sql语句中第几个占位符
	*	bnd		绑定的变量
	*/
	void bindInt(int pos, const int* bnd);
	/**
	*	绑定字符串型占位符
	*	name	sql语句中占位符的名字，前面一定要加冒号
	*	bnd		绑定的变量
	*/
	void bindStr(char *name, const char* bnd);//绑定字符串类型的占位符，
	/**
	*	准备int型结果缓冲区
	*	pos		第几列结果
	*	res		int缓冲区
	*/
	void prepareResultInt(int pos, int* res);
	/**
	*	准备字符串型结果缓冲区
	*	pos		第几列结果
	*	res		字符串缓冲区
	*	len		缓冲区的长度
	*/
	void prepareResultStr(int pos, char* res, int len);
	void getSqlType();//获得sql类型curd
	bool execute(int times);
	bool handleTrans();
	bool nextResults();//使用do-while循环，否则第一个结果拿不到
protected:
	NstvConnection* 	conn;		//连接
	OCIStmt*			stmthp;		//语句句柄
	ub2 				stmtype;	//语句类型
	bool	isOk_;
	
};
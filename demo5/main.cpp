/*	
  ============================================================================
  Name        : demo2.cpp
  Author      : wuj 
  Version     : the second demo for oci programme
  Copyright   : nstv
  Description : connnect oracle for dbuser1/1@orcl
				start transaction
				exec sql select employee_id, first_name from employees where employee_id<102
				end transaction(commmit/rollback)
				disconnect oracle
  ============================================================================
*/
/**
vim /usr/lib/oracle/11.2/client64/network/admin/tnsnames.ora

ORCL =                                                                                                                       
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.4.84)(PORT = 1521))
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = orcl)
    )
  )
  CDCASDB =
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.4.90)(PORT = 1521))
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = cdcasdb)
    )
  )
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <oci.h>//oci头文件
#include "NstvException.h"
#include "NstvConnection.h"


using namespace std;
//static OCIEnv		*conn->envhp;//oci环境句柄
//static OCIServer	*conn->srvhp;//oci服务器句柄
//static OCIError 	*conn->errhp;//oci错误句柄
//static OCISession	*conn->sessionhp;//oci用户会话句柄
//static OCISvcCtx	*conn->svchp;//oci上下文句柄
//static OCITrans 	*conn->tranhp;//oci事务句柄

static OCIStmt		*p_stmt1;//oci语句句柄 select 
static OCIStmt		*p_stmt2;//oci语句句柄 insert
static OCIStmt		*p_stmt3;//oci语句句柄 delete
static OCIDefine	*p_dfn1 = (OCIDefine *)0;//oci输出变量
static OCIDefine	*p_dfn2 = (OCIDefine *)0;
static OCIBind		*p_bnd1 = (OCIBind *)0;//oci绑定句柄，用于绑定SQL语句里的变量(占位符)
static OCIBind		*p_bnd2 = (OCIBind *)0;
static OCIBind		*p_bnd3 = (OCIBind *)0;

#define WUJDEBUG printf

void printOCIErr(dvoid *errhp, const char *funcName, int rc){
	char errbuf[100]; //存储错误信息
	int	errcode; //错误号
	OCIErrorGet((dvoid *)errhp, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
	WUJDEBUG("函数%s 返回值：%d: 错误号: %d, 错误信息: %s\n", funcName, rc, errcode, errbuf);
}

int main()
{
	int		p_x;//employee_id = p_x
	int		p_id;
	const char	*p_name = "wuj";
	char	*res_name;//存储SQL查询语句后的结果
	int		*res_id;
	int 	rc;	//接收OCI-api返回值
	char	mysql_select[100];//储存SQL语句
	char	mysql_insert[100];
	char	mysql_delete[100];
	ub2 	stmt_type1;//获取SQL语句的类型 select
	ub2 	stmt_type2;//insert
	ub2 	stmt_type3;//delete

	res_id = (int *)malloc(4);
	memset(res_id, 0, 4);
	
	res_name = (char *)malloc(20);
	memset(res_name, 0, 20);

	NstvConnection *conn = new NstvConnection();
	conn->connect("orcl", "dbuser1", "1");
	if (conn->isConnected()) cout<<"成功连接数据库"<<endl;
	
	/*创建oci语句句柄*/
	rc = OCIHandleAlloc((dvoid *)conn->envhp, (dvoid **)&p_stmt1, OCI_HTYPE_STMT, (size_t)0, (dvoid **)0);
	WUJDEBUG("oci创建语句句柄1: OCIHandleAlloc()rc=%d\n", rc);
	if(rc != 0){
		goto OCIErr;
	}
	rc = OCIHandleAlloc((dvoid *)conn->envhp, (dvoid **)&p_stmt2, OCI_HTYPE_STMT, (size_t)0, (dvoid **)0);
	WUJDEBUG("oci创建语句句柄2: OCIHandleAlloc()rc=%d\n", rc);
	if(rc != 0){
		goto OCIErr;
	}

	rc = OCIHandleAlloc((dvoid *)conn->envhp, (dvoid **)&p_stmt3, OCI_HTYPE_STMT, (size_t)0, (dvoid **)0);
	WUJDEBUG("oci创建语句句柄2: OCIHandleAlloc()rc=%d\n", rc);
	if(rc != 0){
		goto OCIErr;
	}
	
	/*准备SQL语句*/
	strcpy(mysql_select, "select employee_id, first_name from employees where employee_id<:x");//冒号x是变量(占位符)
	if (!NstvException::checkErr(conn->errhp, OCIStmtPrepare(p_stmt1, conn->errhp, (text *)mysql_select, (ub4)strlen(mysql_select), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci准备SQL语句1: OCIStmtPrepare()\n");
	strcpy(mysql_insert, "insert into employees(employee_id, first_name) values(:id, :name)");
	
	if (!NstvException::checkErr(conn->errhp, OCIStmtPrepare(p_stmt2, conn->errhp, (text *)mysql_insert, (ub4)strlen(mysql_insert), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci准备SQL语句2: OCIStmtPrepare()\n");

	strcpy(mysql_delete, "delete employees where employee_id = 1 and first_name = 'wuj'");
	if (!NstvException::checkErr(conn->errhp, OCIStmtPrepare(p_stmt3, conn->errhp, (text *)mysql_delete, (ub4)strlen(mysql_delete), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci准备SQL语句3: OCIStmtPrepare()\n");

	/*绑定变量x、id、name的值*/
	p_x = 102;
	if (!NstvException::checkErr(conn->errhp, OCIBindByName(p_stmt1, &p_bnd1, conn->errhp, (text *)":x", -1, 
		(dvoid *)&p_x, sizeof(int), SQLT_INT, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci绑定语句占位符1: OCIBindByName()\n");

	p_id = 1;
	if (!NstvException::checkErr(conn->errhp, OCIBindByName(p_stmt2, &p_bnd2, conn->errhp, (text *)":id", -1, 
			(dvoid *)&p_id, sizeof(int), SQLT_INT, (dvoid *)0, (ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci绑定语句占位符2: OCIBindByName()\n");

	if (!NstvException::checkErr(conn->errhp, OCIBindByName(p_stmt2, &p_bnd3, conn->errhp, (text *)":name", strlen(":name"), 
			(dvoid *)p_name, strlen(p_name)+1, SQLT_STR, (dvoid *)0,           
			(ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci绑定语句占位符3: OCIBindByName()\n");	
	
	/*准备语句执行结果缓冲区*/
	if (!NstvException::checkErr(conn->errhp, OCIDefineByPos(p_stmt1, &p_dfn1, conn->errhp, 1, (dvoid *)res_id, sizeof(int), SQLT_INT, 
			(dvoid *)0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci定义结果缓冲区1: OCIDefineByPos()\n");
	if (!NstvException::checkErr(conn->errhp, OCIDefineByPos(p_stmt1, &p_dfn2, conn->errhp, 2, (dvoid *)res_name, (sword)20, SQLT_STR, 
			(dvoid *)0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT)))
		throw NstvException();
	WUJDEBUG("oci定义结果缓冲区2: OCIDefineByPos()\n");
	
	/*获得SQL语句的类型, 判断会话执行的SQL语句是什么类型的 OCI_STMT_XXX, 如 OCI_STMT_SELECT、OCI_STMT_UPDATE、OCI_STMT_DROP等*/
	if (!NstvException::checkErr(conn->errhp, OCIAttrGet ((dvoid *)p_stmt1, (ub4)OCI_HTYPE_STMT, (dvoid *)&stmt_type1, 
		(ub4 *)0, (ub4)OCI_ATTR_STMT_TYPE, conn->errhp)))
		throw NstvException();
	WUJDEBUG("oci获得SQL语句的类型1: OCIAttrGet() 类型为: p_stmt1=%d\n", stmt_type1);

	if (!NstvException::checkErr(conn->errhp, OCIAttrGet ((dvoid *)p_stmt2, (ub4)OCI_HTYPE_STMT, (dvoid *)&stmt_type2, 
		(ub4 *)0, (ub4)OCI_ATTR_STMT_TYPE, conn->errhp)))
		throw NstvException();
	WUJDEBUG("oci获得SQL语句的类型2: OCIAttrGet() 类型为: p_stmt2=%d\n", stmt_type2);
	
	if (!NstvException::checkErr(conn->errhp, OCIAttrGet ((dvoid *)p_stmt3, (ub4)OCI_HTYPE_STMT, (dvoid *)&stmt_type3, 
		(ub4 *)0, (ub4)OCI_ATTR_STMT_TYPE, conn->errhp)))
		throw NstvException();
	WUJDEBUG("oci获得SQL语句的类型3: OCIAttrGet() 类型为: p_stmt3=%d\n", stmt_type3);
	
	/*执行SQL语句*/
#if 1

	rc = OCIHandleAlloc((dvoid *)conn->envhp, (dvoid **)&conn->tranhp, OCI_HTYPE_TRANS, 0, 0);
	WUJDEBUG("oci创建事务句柄: OCIHandleAlloc()rc=%d\n", rc);
	if(rc != 0){
		printOCIErr(conn->errhp, "OCIHandleAlloc", rc);
		goto OCIErr;
	}
	if (!NstvException::checkErr(conn->errhp, OCIAttrSet((dvoid *)conn->svchp, OCI_HTYPE_SVCCTX, (dvoid *)conn->tranhp, 0, OCI_ATTR_TRANS, conn->errhp)))
		throw NstvException();
	WUJDEBUG("oci设置事务句柄属性: OCIHandleAlloc()\n");

	if (!NstvException::checkErr(conn->errhp, OCITransStart (conn->svchp, conn->errhp, 3, OCI_TRANS_NEW))){
		throw NstvException();
	}
    else if(stmt_type2 == OCI_STMT_INSERT){
		WUJDEBUG("oci开启一个事务: OCITransStart()\n");
		WUJDEBUG("oci执行SQL语句2: OCIStmtExecute()\n");
		if (!NstvException::checkErr(conn->errhp, OCIStmtExecute(conn->svchp, p_stmt2, conn->errhp, 
			(ub4)1, (ub4)0, (CONST OCISnapshot *)NULL, (OCISnapshot *)NULL, OCI_DEFAULT))){
			NstvException::checkErr(conn->errhp, OCITransRollback(conn->svchp, conn->errhp, (ub4) 0));
			throw NstvException();
		}else{//如果没有发生异常，则else中将事务提交
			WUJDEBUG("oci提交一个事务: OCITransStart()\n");
			NstvException::checkErr(conn->errhp, OCITransCommit(conn->svchp, conn->errhp, (ub4) 0));
		}
	}
	
#endif
#if 0
	if(stmt_type3 == OCI_STMT_DELETE){
		if (!NstvException::checkErr(conn->errhp, OCIStmtExecute(conn->svchp, p_stmt3, conn->errhp, (ub4)1, (ub4)0, 
			(CONST OCISnapshot *)NULL, (OCISnapshot *)NULL, OCI_DEFAULT)))
			throw NstvException();
		WUJDEBUG("oci执行SQL语句3: OCIStmtExecute()\n");
	}
#endif	
	if(stmt_type1 == OCI_STMT_SELECT){
		if (!NstvException::checkErr(conn->errhp, OCIStmtExecute(conn->svchp, p_stmt1, conn->errhp, (ub4)1, (ub4)0, 
			(CONST OCISnapshot *)NULL, (OCISnapshot *)NULL, OCI_DEFAULT)))
			throw NstvException();
		WUJDEBUG("oci执行SQL语句1: OCIStmtExecute()\n");
	}
	
	
	/*打印结果*/
	while (rc != OCI_NO_DATA)
	{         
		WUJDEBUG("%d, %s\n", *res_id, res_name);
		rc = OCIStmtFetch2(p_stmt1, conn->errhp, 1, OCI_FETCH_NEXT, 1, OCI_DEFAULT);
		if(rc != 100 && rc !=0)
		{
			printOCIErr(conn->errhp, "OCIStmtFetch2", rc);
		}
	}
	rc = 0;
	
	/*断开连接*/

OCIErr:
	conn->disconnect();
	
	if(p_stmt1 != NULL){
		rc = OCIHandleFree((dvoid *)p_stmt1, OCI_HTYPE_STMT);
		p_stmt1 = NULL;
	}
	if(p_stmt2 != NULL){
		rc = OCIHandleFree((dvoid *)p_stmt1, OCI_HTYPE_STMT);
		p_stmt2 = NULL;
	}
	if(p_stmt3 != NULL){
		rc = OCIHandleFree((dvoid *)p_stmt1, OCI_HTYPE_STMT);
		p_stmt3 = NULL;
	}
	if(p_dfn1 != NULL){
		rc = OCIHandleFree((dvoid *)p_dfn1, OCI_HTYPE_DEFINE);
		p_dfn1 = NULL;
	}
	if(p_dfn2 != NULL){
		rc = OCIHandleFree((dvoid *)p_dfn2, OCI_HTYPE_DEFINE); 
		p_dfn2 = NULL;
	}
	if(p_bnd1 != NULL){
		rc = OCIHandleFree((dvoid *)p_bnd1, OCI_HTYPE_BIND); 
		p_bnd1 = NULL;
	}
	if(p_bnd2 != NULL){
		rc = OCIHandleFree((dvoid *)p_bnd1, OCI_HTYPE_BIND); 
		p_bnd1 = NULL;
	}
	if(p_bnd3 != NULL){
		rc = OCIHandleFree((dvoid *)p_bnd1, OCI_HTYPE_BIND); 
		p_bnd1 = NULL;
	}
	
	free(res_id);
	res_id = NULL;
	free(res_name);
	res_name = NULL;
	
	return 0;
}

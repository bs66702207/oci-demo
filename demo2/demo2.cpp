/*	
  ============================================================================
  Name        : demowuj1.c
  Author      : wuj 
  Version     : the first demo for oci programme
  Copyright   : nstv
  Description : connnect oracle for dbuser1/1@orcl
				exec sql select first_name from employees where employee_id=102
				disconnect oracle
  ============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <oci.h>//oci头文件
using namespace std;
static OCIEnv		*p_env;//oci环境句柄
static OCIServer	*p_server;//oci服务器句柄
static OCIError 	*p_err;//oci错误句柄
static OCISession	*p_session;//oci用户会话句柄
static OCISvcCtx	*p_svc;//oci上下文句柄
static OCIStmt		*p_stmt;//oci语句句柄
static OCIDefine	*p_dfn1 = (OCIDefine *)0;//oci输出变量
static OCIDefine	*p_dfn2 = (OCIDefine *)0;
static OCIBind		*p_bnd = (OCIBind *)0;//oci绑定句柄，用于绑定SQL语句里的变量(占位符)

string sid = "orcl";
string user = "dbuser1";
string passwd = "1";

int main()
{
	int		p_bvi;//employee_id = p_bvi
	char	*res_name;//存储SQL查询语句后的结果
	int		*res_id;
	int 	rc;	//接收OCI-api返回值
	char	errbuf[100]; //存储错误信息
	int		errcode; //错误号
	char	mysql[100];//储存SQL语句
	ub2 	stmt_type;//获取SQL语句的类型

	res_id = (int *)malloc(4);
	memset(res_id, 0, 4);
	
	res_name = (char *)malloc(20);
	memset(res_name, 0, 20);

	/*创建oci环境句柄*/
	rc = OCIEnvCreate((OCIEnv **)&p_env, OCI_DEFAULT,(dvoid *)0,
			(dvoid * (*)(dvoid *, size_t))0,
			(dvoid * (*)(dvoid *, dvoid *, size_t))0,
			(void (*)(dvoid *, dvoid *))0,
			(size_t)0, (dvoid **)0);
	printf("oci环境句柄创建: OCIEncCreate()rc=%d\n", rc);
	
	/*创建oci错误句柄*/
	rc = OCIHandleAlloc((dvoid *)p_env, (dvoid **)&p_err, OCI_HTYPE_ERROR, (size_t)0, (dvoid **)0);
	printf("oci错误句柄创建: OCIHandleAlloc()rc=%d\n", rc);
	
	/*创建oci服务器句柄*/
	rc = OCIHandleAlloc((dvoid *)p_env, (dvoid **)&p_server, OCI_HTYPE_SERVER, (size_t)0, (dvoid **)0);
	printf("oci服务器句柄创建: OCIHandleAlloc()rc=%d\n", rc);
	
	/*创建一个oracl连接路径, 多用户方式连接*/
	rc = OCIServerAttach(p_server, p_err, (text *)sid.c_str(), strlen(sid.c_str()), OCI_DEFAULT);
	printf("oci创建连接路径: OCIServerAttach()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci创建连接路径 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
		OCIHandleFree((dvoid *)p_env, OCI_HTYPE_ENV);
		OCIHandleFree((dvoid *)p_server, OCI_HTYPE_SERVER);
		OCIHandleFree((dvoid *)p_err, OCI_HTYPE_ERROR);
        return -1;
    }

	/*创建oci上下文句柄, 并且为其服务器句柄设置属性*/
	rc = OCIHandleAlloc((dvoid *)p_env, (dvoid **)&p_svc, OCI_HTYPE_SVCCTX, (size_t)0, (dvoid **)0);
	printf("oci上下文句柄创建: OCIHandleAlloc()rc=%d\n", rc);
	rc = OCIAttrSet((dvoid *)p_svc, OCI_HTYPE_SVCCTX, (dvoid *)p_server, (ub4)0, OCI_ATTR_SERVER, (OCIError *)p_err);
	printf("oci为上下文句柄设置服务器属性: OCIHandleAlloc()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci为上下文句柄设置服务器属性 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
		OCIHandleFree((dvoid *)p_env, OCI_HTYPE_ENV);
		OCIHandleFree((dvoid *)p_server, OCI_HTYPE_SERVER);
		OCIHandleFree((dvoid *)p_err, OCI_HTYPE_ERROR);
		OCIHandleFree((dvoid *)p_svc, OCI_HTYPE_SVCCTX);
        return -1;
    }
	
	/*创建oci用户会话句柄, 并且为会话句柄设置登录名及密码*/
	rc = OCIHandleAlloc((dvoid *)p_env, (dvoid **)&p_session, (ub4)OCI_HTYPE_SESSION, (size_t)0, (dvoid **)0);
	printf("oci创建用户会话句柄: OCIHandleAlloc()rc=%d\n", rc);
	rc = OCIAttrSet((dvoid *)p_session, (ub4)OCI_HTYPE_SESSION, (dvoid *)user.c_str(), (ub4)strlen(user.c_str()), (ub4)OCI_ATTR_USERNAME, p_err);
	printf("oci为会话句柄设置用户: OCIHandleAlloc()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci为会话句柄设置用户 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
		OCIHandleFree((dvoid *)p_env, OCI_HTYPE_ENV);
		OCIHandleFree((dvoid *)p_server, OCI_HTYPE_SERVER);
		OCIHandleFree((dvoid *)p_err, OCI_HTYPE_ERROR);
		OCIHandleFree((dvoid *)p_svc, OCI_HTYPE_SVCCTX);
		OCIHandleFree((dvoid *)p_session, OCI_HTYPE_SESSION);
        return -1;
    }
	rc = OCIAttrSet((dvoid *)p_session, (ub4)OCI_HTYPE_SESSION, (dvoid *)passwd.c_str(), (ub4)strlen(passwd.c_str()), (ub4)OCI_ATTR_PASSWORD, p_err);
	printf("oci为会话句柄设置密码: OCIAttrSet()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci为会话句柄设置密码 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
		OCIHandleFree((dvoid *)p_env, OCI_HTYPE_ENV);
		OCIHandleFree((dvoid *)p_server, OCI_HTYPE_SERVER);
		OCIHandleFree((dvoid *)p_err, OCI_HTYPE_ERROR);
		OCIHandleFree((dvoid *)p_svc, OCI_HTYPE_SVCCTX);
		OCIHandleFree((dvoid *)p_session, OCI_HTYPE_SESSION);
        return -1;
    }
	
	/*验证用户并且为用户建立会话连接*/
	rc = OCISessionBegin(p_svc, p_err, p_session, OCI_CRED_RDBMS, (ub4)OCI_DEFAULT);
	printf("oci验证用户并且建立会话连接: OCISessionBegin()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci验证用户并且建立会话连接 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
		OCIHandleFree((dvoid *)p_env, OCI_HTYPE_ENV);
		OCIHandleFree((dvoid *)p_server, OCI_HTYPE_SERVER);
		OCIHandleFree((dvoid *)p_err, OCI_HTYPE_ERROR);
		OCIHandleFree((dvoid *)p_svc, OCI_HTYPE_SVCCTX);
		OCIHandleFree((dvoid *)p_session, OCI_HTYPE_SESSION);
        return -1;
    }
	
	/*为oci上下文句柄设置会话认证环境属性*/
	rc = OCIAttrSet((dvoid *)p_svc, (ub4) OCI_HTYPE_SVCCTX, (dvoid *)p_session, (ub4)0, (ub4)OCI_ATTR_SESSION, p_err);
	printf("oci上下文句柄设置会话认证环境属性: OCIAttrSet()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci上下文句柄设置会话认证环境属性 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
		OCIHandleFree((dvoid *)p_env, OCI_HTYPE_ENV);
		OCIHandleFree((dvoid *)p_server, OCI_HTYPE_SERVER);
		OCIHandleFree((dvoid *)p_err, OCI_HTYPE_ERROR);
		OCIHandleFree((dvoid *)p_svc, OCI_HTYPE_SVCCTX);
		OCIHandleFree((dvoid *)p_session, OCI_HTYPE_SESSION);
        return -1;
    }
	
	/*创建oci语句句柄*/
	rc = OCIHandleAlloc((dvoid *)p_env, (dvoid **)&p_stmt, OCI_HTYPE_STMT, (size_t)0, (dvoid **)0);
	printf("oci创建语句句柄: OCIHandleAlloc()rc=%d\n", rc);

	/*准备SQL语句*/
	strcpy(mysql, "select employee_id, first_name from employees where employee_id<:x");//冒号x是变量(占位符)
	//strcpy(mysql, "select first_name from employees where employee_id<:x");//冒号x是变量(占位符)
	rc = OCIStmtPrepare(p_stmt, p_err, (text *)mysql, (ub4)strlen(mysql), (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT);
	printf("oci准备SQL语句: OCIStmtPrepare()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci准备SQL语句 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
        return -1;
    }

	/*绑定变量x的值*/
	p_bvi = 102;
	rc = OCIBindByName(p_stmt, &p_bnd, p_err, (text *)":x", -1, 
			(dvoid *)&p_bvi, sizeof(int), SQLT_INT, (dvoid *)0,           
			(ub2 *)0, (ub2 *)0, (ub4)0, (ub4 *)0, OCI_DEFAULT);
	printf("oci绑定语句占位符: OCIBindByName()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci绑定语句占位符 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
        return -1;
    }
	
	/*准备语句执行结果缓冲区*/
	rc = OCIDefineByPos(p_stmt, &p_dfn1, p_err, 1, (dvoid *)res_id, sizeof(int), SQLT_INT, 
			(dvoid *)0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT);
	rc = OCIDefineByPos(p_stmt, &p_dfn2, p_err, 2, (dvoid *)res_name, (sword)20, SQLT_STR, 
			(dvoid *)0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT);
	printf("oci定义结果缓冲区: OCIDefineByPos()rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci定义结果缓冲区 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
        return -1;
    }
	
	/*获得SQL语句的类型, 判断会话执行的SQL语句是什么类型的 OCI_STMT_XXX, 如OCI_STMT_SELECT、OCI_STMT_UPDATE、OCI_STMT_DROP等*/
	rc = OCIAttrGet ((dvoid *)p_stmt, (ub4)OCI_HTYPE_STMT, (dvoid *)&stmt_type, (ub4 *)0, (ub4)OCI_ATTR_STMT_TYPE, p_err);
	printf("oci获得SQL语句的类型: OCIAttrGet()rc=%d, 类型为: p_stmt=%d\n", rc, stmt_type);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci获得SQL语句的类型 失败: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
        return -1;
    }
	
	/*执行SQL语句*/
	rc = OCIStmtExecute(p_svc, p_stmt, p_err, (ub4)1, (ub4)0, (CONST OCISnapshot *)NULL, (OCISnapshot *)NULL, OCI_DEFAULT);	
	printf("oci执行SQL语句: OCIStmtExecute()rc=%d\n", rc);
    if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4)1, (text *)NULL, &errcode, (OraText*)errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci执行SQL语句结果: OCIErrorGet()错误号: %d, 错误信息: %s\n", errcode, errbuf);
    }
	
	/*打印结果*/
	while (rc != OCI_NO_DATA)
	{         
		printf("%d, %s\n", *res_id, res_name);
		//printf("%s\n", res_name);      
		//rc = OCIStmtFetch(p_stmt, p_err, 1, 0, 0); //已废弃
		rc = OCIStmtFetch2(p_stmt, p_err, 1, OCI_FETCH_NEXT, 1, OCI_DEFAULT); 		
	}	
	
	/*断开连接*/
	rc = OCILogoff(p_svc, p_err); 
	printf("oci断开连接: CILogoff()rc=%d\n", rc);
	OCIServerDetach(p_server, p_err, OCI_DEFAULT);
	printf("oci删除对数据源的访问: CILogoff()rc=%d\n", rc);
	rc = OCIHandleFree((dvoid *)p_env, OCI_HTYPE_ENV);
	rc = OCIHandleFree((dvoid *)p_server, OCI_HTYPE_SERVER);
	rc = OCIHandleFree((dvoid *)p_err, OCI_HTYPE_ERROR);
	rc = OCIHandleFree((dvoid *)p_session, OCI_HTYPE_SESSION);
	rc = OCIHandleFree((dvoid *)p_svc, OCI_HTYPE_SVCCTX);
	rc = OCIHandleFree((dvoid *)p_stmt, OCI_HTYPE_STMT);
	rc = OCIHandleFree((dvoid *)p_dfn1, OCI_HTYPE_DEFINE);   
	rc = OCIHandleFree((dvoid *)p_dfn2, OCI_HTYPE_DEFINE);   
	rc = OCIHandleFree((dvoid *)p_bnd, OCI_HTYPE_BIND);   
	
	free(res_id);
	res_id = NULL;
	free(res_name);
	res_name = NULL;
	
	return 0;
}


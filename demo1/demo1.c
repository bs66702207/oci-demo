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
#include <oci.h>//oci头文件

static OCIEnv		*p_env;//oci环境句柄
static OCIError 	*p_err;//oci错误句柄
static OCISvcCtx	*p_svc;//oci上下文句柄
static OCIStmt		*p_sql;//oci语句句柄
static OCIDefine	*p_dfn = (OCIDefine *) 0;//oci输出变量
static OCIBind		*p_bnd = (OCIBind *) 0;//oci绑定句柄

int main()
{
	int		p_bvi;//employee_id = p_bvi
	char	*p_sli;//存储SQL查询语句后的结果
	int 	rc;	//接收OCI-api返回值
	char	errbuf[100]; //存储错误信息
	int		errcode; //错误号
	char	mysql[100];//储存sql语句

	p_sli = (char *)malloc(20);
	memset(p_sli, 0, 20);

	/*创建oci环境句柄*/
	rc = OCIEnvCreate((OCIEnv **) &p_env, OCI_DEFAULT,(dvoid *)0,
			(dvoid * (*)(dvoid *, size_t)) 0,
			(dvoid * (*)(dvoid *, dvoid *, size_t))0,
			(void (*)(dvoid *, dvoid *)) 0,
			(size_t) 0, (dvoid **) 0);
	printf("oci环境句柄创建: OCIEncCreate() rc=%d\n", rc);
	
	/*创建oci错误句柄*/
	rc = OCIHandleAlloc((dvoid *) p_env, (dvoid **) &p_err, OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0);
	printf("oci错误句柄创建: OCIHandleAlloc() rc=%d\n", rc);
	
	/*创建oci创建上下文句柄*/
	rc = OCIHandleAlloc((dvoid *) p_env, (dvoid **) &p_svc, OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0);
	printf("oci上下文句柄创建: OCIHandleAlloc() rc=%d\n", rc);

	/*创建一个oracl连接*/
	rc = OCILogon(p_env, p_err, &p_svc, "dbuser1", 7, "1", 1, "orcl", 4);
	printf("oci创建连接: OCILogon() rc=%d\n", rc);
	if(rc != 0)
	{
		OCIErrorGet((dvoid *)p_err, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
		printf("oci连接失败: OCIErrorGet() 错误号: %d, 错误信息: %s\n", errcode, errbuf);
		return -1;
	}

	/*创建oci语句句柄*/
	rc = OCIHandleAlloc((dvoid *) p_env, (dvoid **) &p_sql, OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0);
	printf("oci创建语句句柄: OCIHandleAlloc() rc=%d\n", rc);

	/*准备sql语句*/
//	strcpy(mysql, "select first_name from employees");//冒号x是变量(占位符)
	strcpy(mysql, "select first_name from employees where employee_id<:x");//冒号x是变量(占位符)
	rc = OCIStmtPrepare(p_sql, p_err, mysql, (ub4) strlen(mysql), (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT);
	printf("oci准备sql语句: OCIStmtPrepare() rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci准备sql语句失败: OCIErrorGet() 错误号: %d, 错误信息: %s\n", errcode, errbuf);
        return -1;
    }

	/*绑定变量x的值*/
	p_bvi = 102;
	rc = OCIBindByName(p_sql, &p_bnd, p_err, (text *) ":x", -1, 
			(dvoid *) &p_bvi, sizeof(int), SQLT_INT, (dvoid *) 0,           
			(ub2 *) 0, (ub2 *) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT);
	printf("oci绑定语句占位符: OCIBindByName() rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci绑定语句占位符失败: OCIErrorGet() 错误号: %d, 错误信息: %s\n", errcode, errbuf);
        return -1;
    }
	
	/*准备语句执行结果缓冲区*/
	rc = OCIDefineByPos(p_sql, &p_dfn, p_err, 1, (dvoid *) p_sli, (sword) 20, SQLT_STR, 
			(dvoid *) 0, (ub2 *)0, (ub2 *)0, OCI_DEFAULT);
	printf("oci定义结果缓冲区: OCIDefineByPos() rc=%d\n", rc);
	if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci定义结果缓冲区失败: OCIErrorGet() 错误号: %d, 错误信息: %s\n", errcode, errbuf);
        return -1;
    }

	/*执行SQL语句*/
	rc = OCIStmtExecute(p_svc, p_sql, p_err, (ub4) 1, (ub4) 0, (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT);	
	printf("oci执行SQL语句: OCIStmtExecute() rc=%d\n", rc);
    if(rc != 0)
    {
        OCIErrorGet((dvoid *)p_err, (ub4) 1, (text *) NULL, &errcode, errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
        printf("oci执行SQL语句结果: OCIErrorGet() 错误号: %d, 错误信息: %s\n", errcode, errbuf);
    }
	
	/*打印结果*/
	while (rc != OCI_NO_DATA) 
	{         
		printf("%s\n",p_sli);      
		rc = OCIStmtFetch(p_sql, p_err, 1, 0, 0);  
	}	
	
	/*断开连接*/
	rc = OCILogoff(p_svc, p_err); 
	printf("oci断开连接: CILogoff() rc=%d\n", rc);
	rc = OCIHandleFree((dvoid *) p_sql, OCI_HTYPE_STMT);    /* Free handles */   
	rc = OCIHandleFree((dvoid *) p_svc, OCI_HTYPE_SVCCTX);   
	rc = OCIHandleFree((dvoid *) p_err, OCI_HTYPE_ERROR);
	return 0;
}


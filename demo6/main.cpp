/*	
  ============================================================================
  Name        : demo2.cpp
  Author      : wuj 
  Version     : the second demo for oci programme
  Copyright   : nstv
  Description : connnect oracle for dbuser1/1@orcl
				start transaction
				exec sql1 select employee_id, first_name from employees where employee_id<102
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
#include "NstvSql.h"

using namespace std;

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
	const char *p_name = "wuj";
	const char *p_marry = "marry";
	char	*res_name;//存储SQL查询语句后的结果
	int		*res_id;
	int 	rc;	//接收OCI-api返回值
	char	mysql_select[100];//储存SQL语句
	char	mysql_insert[100];
	char	mysql_delete[100];
	char	mysql_update[100];

	res_id = (int *)malloc(4);
	memset(res_id, 0, 4);
	
	res_name = (char *)malloc(20);
	memset(res_name, 0, 20);

	NstvConnection *conn1 = new NstvConnection();
	conn1->connect("orcl", "dbuser1", "1");
	if (!conn1->isConnected()) cout<<"连接数据库失败"<<endl;
	
	NstvConnection *conn2 = new NstvConnection();
	conn2->connect("orcl", "dbuser1", "1");
	if (!conn2->isConnected()) cout<<"连接数据库失败"<<endl;
	
	NstvConnection *conn3 = new NstvConnection();
	conn3->connect("orcl", "dbuser1", "1");
	if (!conn3->isConnected()) cout<<"连接数据库失败"<<endl;
	
	NstvConnection *conn4 = new NstvConnection();
	conn4->connect("orcl", "dbuser1", "1");
	if (!conn4->isConnected()) cout<<"连接数据库失败"<<endl;

	/*创建oci语句句柄*/
	NstvSql *sql1 = new NstvSql(conn1);
	sql1->createStmthp();
	
	NstvSql *sql2 = new NstvSql(conn2);
	sql2->createStmthp();

	NstvSql *sql3 = new NstvSql(conn3);
	sql3->createStmthp();
	
	NstvSql *sql4 = new NstvSql(conn4);
	sql4->createStmthp();
	
	/*准备SQL语句*/
	strcpy(mysql_select, "select employee_id, first_name from employees where employee_id<:x");//冒号x是变量(占位符)
	sql1->prepareSql(mysql_select);
	
	strcpy(mysql_insert, "insert into employees(employee_id, first_name) values(:id, :name)");
	sql2->prepareSql(mysql_insert);

	strcpy(mysql_delete, "delete employees where employee_id = 1 and first_name = 'wuj'");
	sql3->prepareSql(mysql_delete);

	strcpy(mysql_update, "update employees set employee_id = 888 where first_name=:name");
	sql4->prepareSql(mysql_update);

	/*绑定变量x、id、name的值*/
	p_x = 102;
	sql1->bindInt(1, &p_x);

	p_id = 1;
	sql2->bindInt(1, &p_id);
	sql2->bindStr(":name", p_name);
	sql4->bindStr(":name", p_marry);
	
	/*准备语句执行结果缓冲区*/
	sql1->prepareResultInt(1, res_id);
	sql1->prepareResultStr(2, res_name, 20);
	
	
	
	/*执行SQL语句*/
#if 1
	sql2->handleTrans();
	cout<<1<<endl;
#endif
#if 0
	sql3->handleTrans();
#endif
	sql4->handleTrans();
	sql1->handleTrans();

		
	/*打印结果*/
	while (rc != OCI_NO_DATA)
	{         
		WUJDEBUG("%d, %s\n", *res_id, res_name);
		rc = OCIStmtFetch2(sql1->stmthp, conn1->errhp, 1, OCI_FETCH_NEXT, 1, OCI_DEFAULT);
		if(rc != 100 && rc !=0)
		{
			printOCIErr(conn1->errhp, "OCIStmtFetch2", rc);
		}
	}
	rc = 0;

OCIErr:
	conn1->disconnect();
	conn2->disconnect();
	conn3->disconnect();
	conn4->disconnect();
	free(res_id);
	res_id = NULL;
	free(res_name);
	res_name = NULL;
	
	return 0;
}

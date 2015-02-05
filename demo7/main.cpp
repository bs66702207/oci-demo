/*	
  ============================================================================
  Name        : main.cpp
  Author      : wuj 
  Version     : the second demo for oci programme
  Copyright   : nstv
  Description : let oci-api to c++ base package, exe curd demo
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
#include "NstvConnPool.h"

using namespace std;

int main()
{
	int		p_x;//employee_id = p_x
	int		p_id;
	const char *p_name = "wuj";//用于绑定SQL语句中的占位符
	const char *p_marry = "marry";
	char	*res_name;//用于存储SQL查询语句后的结果
	int		*res_id;
	char	mysql_select[100];//储存SQL语句
	char	mysql_insert[100];
	char	mysql_delete[100];
	char	mysql_update[100];

	res_id = (int *)malloc(4);
	memset(res_id, 0, 4);
	
	res_name = (char *)malloc(20);
	memset(res_name, 0, 20);

	NstvConnPool* pool = NstvConnPool::createPool();
	
	NstvConnection *conn1 = pool->getConn();//select
	conn1->connect("orcl", "dbuser1", "1");
	if (!conn1->isConnected()) cout<<"连接数据库失败"<<endl;
	
	NstvConnection *conn2 = pool->getConn();//insert
	conn2->connect("orcl", "dbuser1", "1");
	if (!conn2->isConnected()) cout<<"连接数据库失败"<<endl;
	
	NstvConnection *conn3 = pool->getConn();//delete
	conn3->connect("orcl", "dbuser1", "1");
	if (!conn3->isConnected()) cout<<"连接数据库失败"<<endl;
	
	NstvConnection *conn4 = pool->getConn();//update
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
	sql2->handleTrans();//insert
#endif
#if 1
	sql3->handleTrans();//delete
#endif
	sql4->handleTrans();//update
	sql1->handleTrans();//select

	/*打印结果*/
	do{         
		printf("%d, %s\n", *res_id, res_name);
	}while(sql1->nextResults());

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

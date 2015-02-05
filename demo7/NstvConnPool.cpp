#include <iostream>
#include "NstvConnection.h"
#include "NstvConnPool.h"
using namespace std;

//初始化线程池
NstvConnPool* NstvConnPool::_pool = NULL;
NstvConnPool::NstvConnPool(){
	conns.resize(MAX_CONN);
	for(int i=0; i<MAX_CONN; i++){
		conns[i] = new NstvConnection();
	}
}

NstvConnPool::~NstvConnPool(){
	for(int i=0;i<MAX_CONN; i++){
		if(conns[i]) delete conns[i];
		conns[i] = NULL;
	}
	conns.clear();
}

NstvConnPool* NstvConnPool::createPool(){
	if(_pool == NULL){
		_pool = new NstvConnPool;
	}
	return _pool;
}

NstvConnection* NstvConnPool::getConn(){
	for(int i=0; i<conns.size(); i++){
		if(!conns[i]->isConnected()){
			cout<<"拿到了第"<<i+1<<"个连接"<<endl;
			cout<<"连接池中还剩余"<<MAX_CONN-i-1<<"个连接"<<endl;
			return conns[i];
		}
	}
	return NULL;
}

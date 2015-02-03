#include "NstvException.h"#include <string.h>#include <iostream>using namespace std;int NstvException::err = OCI_SUCCESS;char NstvException::msg[512];bool NstvException::checkErr(OCIError *pErr, sb4 status){	err = status;    switch (status)    {		case OCI_SUCCESS:			strcpy(msg,"OCI_SUCCESS");    			break;		case OCI_SUCCESS_WITH_INFO:			strcpy(msg, "OCI_SUCCESS_WITH_INFO");			break;		case OCI_ERROR:			OCIErrorGet((dvoid *)pErr, (ub4)1, (text *)NULL, &err, (unsigned char*)msg, (ub4)sizeof(msg), OCI_HTYPE_ERROR);			cout<<msg<<endl;			break;		case OCI_NEED_DATA:			strcpy(msg, "OCI_NEED_DATA");			break;		case OCI_NO_DATA:			strcpy(msg, "OCI_NO_DATA");			break;		case OCI_INVALID_HANDLE:			strcpy(msg, "OCI_INVALID_HANDLE");			break;		case OCI_STILL_EXECUTING:			strcpy(msg, "OCI_STILL_EXECUTING");			break;		case OCI_CONTINUE:			strcpy(msg, "OCI_CONTINUE");			break;		default:			break;	}	if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO){		return false;	} 	return true;	}
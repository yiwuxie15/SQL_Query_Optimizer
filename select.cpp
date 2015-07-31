#include "catalog.h"
#include "query.h"
#include "index.h"

/*
 * Selects records from the specified relation.
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */
Status Operators::Select(const string & result,      // name of the output relation
	                 const int projCnt,          // number of attributes in the projection
		         const attrInfo projNames[], // the list of projection attributes
		         const attrInfo *attr,       // attribute used inthe selection predicate 
		         const Operator op,         // predicate operation
		         const void *attrValue)     // literal value in the predicate
{
    Status Stat;
    int reclen = 0;
    AttrDesc* attrd = NULL; //pointer to attribute in predicate
    
    //convert attrInfo to attrDesc
    AttrDesc* proj_attrd = new AttrDesc[projCnt];//array of attributes in projection
    for(int i=0; i<projCnt; ++i){
        Stat = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, proj_attrd[i]);
        if(Stat != OK){
            if(attrd) delete attrd;
            delete[] proj_attrd;
            return Stat;
        }
        reclen += proj_attrd[i].attrLen;
    }
    
    if(attr){
        attrd = new AttrDesc;
        Stat = attrCat->getInfo(attr->relName, attr->attrName, *attrd);
        if(Stat != OK){
            if(attrd) delete attrd;
            return Stat;
        }
    }
    
    // if meet requirements of index select then index select
    if(attrd && attrd->indexed == 1 && op == EQ){
        Stat = IndexSelect(result, projCnt, proj_attrd, attrd, op, attrValue, reclen);
    }
    //else regular scan select
    else{
        Stat = ScanSelect(result, projCnt, proj_attrd, attrd, op, attrValue, reclen);
    }
    if(attrd) delete attrd;
    delete []proj_attrd;
    
    return OK;
}


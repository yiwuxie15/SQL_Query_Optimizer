#include "catalog.h"
#include "query.h"
#include "index.h"
#include "utility.h"

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
    Status status = OK, catStatus;
    int length = 0;
    AttrDesc record, attrDescs[projCnt];  //to turn the attrInfo's into AttrDesc's
    //record.attrLen = -1; //record.attrLen will be -1 if there is no predicate
    //find length of output tuple
    for(int i = 0; i < projCnt; ++i) {
        //turn attrInfo's into AttrDesc's
        catStatus = attrCat->getInfo(projNames[i].relName, projNames[i].attrName, attrDescs[i]);
        if(catStatus != OK)
            return catStatus;
        length += attrDescs[i].attrLen;
    }
    
    if(attr) {
        //There is a predicate
        
        //check if index exists on predicate
        catStatus = attrCat->getInfo(attr->relName, attr->attrName, record);
        if(catStatus != OK)
            return catStatus;
        
        //check if it is an equality expression
        if(op == EQ) {
            if(record.indexed){
                //there is an index on the attribute in the predicate
                //use IndexSelect
                return IndexSelect(result, projCnt, attrDescs, &record, op, attrValue, length);
            }else {
                //use ScanSelect
                return ScanSelect(result, projCnt, attrDescs, &record, op, attrValue, length);
            }
        }else {
            //use ScanSelect
            return ScanSelect(result, projCnt, attrDescs, &record, op, attrValue, length);
        }
    }else {
        //There is no predicate
        return ScanSelect(result, projCnt, attrDescs, &record, op, NULL, length);
    }
    
    
    
    return status;
}


#include "catalog.h"
#include "query.h"
#include "index.h"
#include <stdlib.h>
#include <string.h>

/* 
 * A simple scan select using a heap file scan
 */

Status Operators::ScanSelect(const string& result,       // Name of the output relation
                             const int projCnt,          // Number of attributes in the projection
                             const AttrDesc projNames[], // Projection list (as AttrDesc)
                             const AttrDesc* attrDesc,   // Attribute in the selection predicate
                             const Operator op,          // Predicate operator
                             const void* attrValue,      // Pointer to the literal value in the predicate
                             const int reclen)           // Length of a tuple in the result relation
{
  cout << "Algorithm: File Scan" << endl;
  
  /* Your solution goes here */
    Status Stat;
    HeapFile heapfile = HeapFile(result, Stat);
    string relation = projNames[0].relName;
    RID rid;
    Record record;
    Record tuple;
    HeapFileScan* heapScan = NULL;
    
    if(attrDesc==NULL || op == NOTSET || attrValue == NULL){//No filtering predicate scan
        
        heapScan = new HeapFileScan(relation,Stat);//heapscan without filter
        
        if(Stat != OK){
            delete heapScan;
            return Stat;
        }
    }else{//scan with filtering predicate
        heapScan = new HeapFileScan(relation, attrDesc->attrOffset, attrDesc->attrLen, (Datatype)attrDesc->attrType,(char*)attrValue,op,Stat);
        if(Stat != OK){
            delete heapScan;
            return Stat;
        }
    }
    
    while(heapScan->scanNext(rid, record)==OK){
        int offset = 0;
        
        //Project record
        int length=0;
        for(int i=0;i<projCnt;i++)
        {
            length+=projNames[i].attrLen;
        }
        tuple.data = malloc(length);
        tuple.length = length;
        
        for(int i=0; i<projCnt; ++i){
            memcpy((char*)tuple.data + offset, (char*)record.data + projNames[i].attrOffset, projNames[i].attrLen);
            offset += projNames[i].attrLen;
        }
        
        Stat = heapfile.insertRecord(tuple, rid);
        free(tuple.data);
        if(Stat != OK){
            delete heapScan;
            return Stat;
        }
    }
    
    heapScan->endScan();
    delete heapScan;
    return OK;
}

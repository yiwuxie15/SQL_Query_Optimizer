#include "catalog.h"
#include "query.h"
#include "index.h"
#include <stdlib.h>
#include <string.h>

Status Operators::IndexSelect(const string& result,       // Name of the output relation
                              const int projCnt,          // Number of attributes in the projection
                              const AttrDesc projNames[], // Projection list (as AttrDesc)
                              const AttrDesc* attrDesc,   // Attribute in the selection predicate
                              const Operator op,          // Predicate operator
                              const void* attrValue,      // Pointer to the literal value in the predicate
                              const int reclen)           // Length of a tuple in the output relation
{
  cout << "Algorithm: Index Select" << endl;

  /* Your solution goes here */
    Status Stat;
    HeapFile heapfile = HeapFile(result, Stat);
    if(Stat != OK) {
        cerr << "Open heap file fail" << endl;
        return Stat;
    }
    string relation = projNames[0].relName;
    int unique = 0;
    RID rid;
    Record record;
    Record tuple;
    
    //Open index file
    Index indScan(relation, attrDesc->attrOffset, attrDesc->attrLen, (Datatype)attrDesc->attrType, unique, Stat);
    if(Stat != OK){
        return Stat;
    }
    
    //Heapscan object to access the getRandomRecord() function
    HeapFileScan heapScan=HeapFileScan(relation,attrDesc->attrOffset, attrDesc->attrLen,(Datatype)(attrDesc->attrType),(char *)attrValue,op,Stat);
    if(Stat != OK){
        return Stat;
    }
    
    //Start Index Scan
    Stat = indScan.startScan(attrValue);
    while(indScan.scanNext(rid)==OK){
        heapScan.getRandomRecord(rid,record);
        tuple.length = reclen;
        tuple.data = malloc(reclen);
        int offset = 0;
        
        //Project record
        for(int i=0; i<projCnt; ++i){
            memcpy((char*)tuple.data + offset, (char*)record.data + projNames[i].attrOffset, projNames[i].attrLen);
            offset += projNames[i].attrLen;
        }
        
        Stat = heapfile.insertRecord(tuple, rid);
        free(tuple.data);
        if(Stat != OK){
            return Stat;
        }
    }
    indScan.endScan();
    heapScan.endScan();
    
    return OK;
}


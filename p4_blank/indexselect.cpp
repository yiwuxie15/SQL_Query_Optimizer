#include "catalog.h"
#include "query.h"
#include "utility.h"
#include "index.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Status Operators::IndexSelect(const string& result,       // Name of the output relation
                              const int projCnt,          // Number of attributes in the projection
                              const AttrDesc projNames[], // Projection list (as AttrDesc)
                              const AttrDesc* attrDesc,   // Attribute in the selection predicate
                              const Operator op,          // Predicate operator
                              const void* attrValue,      // Pointer to the literal value in the predicate
                              const int reclen)           // Length of a tuple in the output relation
{
    cout << "Algorithm: Index Select" << endl;
    int currentOffset = 0;
    Status heapStatus, indexStatus;
    RID nextRID;
    Record nextRecord, recordToInsert;
    
    HeapFile output(result, heapStatus);
    if(heapStatus != OK)
        return heapStatus;
    HeapFileScan heapFile(projNames[0].relName, 0, attrDesc->attrLen, (Datatype)attrDesc->attrType, (char*)attrValue, op, heapStatus);
    Index index(projNames[0].relName, attrDesc->attrOffset, attrDesc->attrLen, (Datatype)attrDesc->attrType, 0, indexStatus);
    
    //start the scan to look for values equal to attrValue
    indexStatus = index.startScan(attrValue);
    //Utilities utl;
    //utl.Print(projNames[0].relName);
    
    if(indexStatus != OK)
        return indexStatus;
    
    indexStatus = index.scanNext(nextRID);
    while(indexStatus == OK){
        //retrieve record from HeapFile
        heapStatus = heapFile.getRandomRecord(nextRID, nextRecord);
        if(heapStatus != OK)
            return heapStatus;
        
        recordToInsert.data = malloc(reclen);
        recordToInsert.length = reclen;
        currentOffset = 0;
        for(int i = 0; i < projCnt; ++i) {
            //copy tuple into the result and filter tuple for projections
            memcpy((char*)recordToInsert.data + currentOffset, (char*)nextRecord.data + projNames[i].attrOffset, projNames[i].attrLen);
            currentOffset += projNames[i].attrLen;
        }
        
        //output to results
        heapStatus = output.insertRecord(recordToInsert, nextRID);
        free(recordToInsert.data);
        if(heapStatus != OK)
            return heapStatus;
        
        //get next RID from relation
        indexStatus = index.scanNext(nextRID);
    }
    if(indexStatus != FILEEOF && indexStatus != NOMORERECS)
        return indexStatus;
    index.endScan();
    heapFile.endScan();
    return OK;
}


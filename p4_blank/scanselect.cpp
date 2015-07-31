#include "catalog.h"
#include "query.h"
#include "index.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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
    Status scanStatus = OK, heapStatus;
    int currentOffset = 0, offset = 0;
    int length = 0;       //for when there is no predicate
    Datatype type = INTEGER;    //for when there is no predicate
    HeapFile output(result, heapStatus);
    if(heapStatus != OK)
        return heapStatus;
    if(attrValue) {
        length = attrDesc->attrLen;
        offset = attrDesc->attrOffset;
        type = (Datatype)attrDesc->attrType;
    }
    HeapFileScan scan(projNames[0].relName, offset, length, type, (char*)attrValue, op, scanStatus);
    if(scanStatus != OK)
        return scanStatus;
    RID nextRID;
    Record nextRecord, recordToInsert;
    
    
    
  		
    scanStatus = scan.scanNext(nextRID, nextRecord);
    while(scanStatus == OK){
        
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
        
        //get next tuple from relation
        scanStatus = scan.scanNext(nextRID, nextRecord);
    }
    if(scanStatus != FILEEOF)
        return scanStatus;
    
    scan.endScan();
    return OK;
}

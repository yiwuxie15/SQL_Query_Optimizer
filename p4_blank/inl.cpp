#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Indexed nested loop evaluates joins with an index on the
 * inner/right relation (attrDesc2)
 */

Status Operators::INL(const string& result,           // Name of the output relation
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // The projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The right attribute in the join predicate
                      const int reclen)               // Length of a tuple in the output relation
{
    cout << "Algorithm: Indexed NL Join" << endl;
    
    /* Your solution goes here */
    
    Status heapStatus, indexStatus, scanStatus1, scanStatus2;
    //for output
    HeapFile output(result, heapStatus);
    //used to scan the outer/left relation
    HeapFileScan scan1(attrDesc1.relName, attrDesc1.attrOffset, attrDesc1.attrLen, (Datatype)attrDesc1.attrType, NULL, op, scanStatus1);
    //used to scan the inner/right relation
    HeapFileScan scan2(attrDesc2.relName, attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype)attrDesc2.attrType, NULL, op, scanStatus2);
    //used to index into the inner/right relation
    Index index(attrDesc2.relName, attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype)attrDesc2.attrType, 0, indexStatus);
    Record nextRecord1, nextRecord2, recordToInsert;
    RID nextRID1, nextRID2;
    int currentOffset = 0;
    
    //make sure everything opened correctly
    if(heapStatus != OK)
        return heapStatus;
    if(scanStatus1 != OK)
        return scanStatus1;
    if(scanStatus2 != OK)
        return scanStatus2;
    if(indexStatus != OK)
        return indexStatus;
    
    scanStatus1 = scan1.scanNext(nextRID1, nextRecord1);
    while(scanStatus1 == OK) {
        indexStatus = index.startScan((char*)nextRecord1.data + attrDesc1.attrOffset);
        if(indexStatus == OK)
            indexStatus = index.scanNext(nextRID2);
        while(indexStatus == OK) {
            //retreive record from inner/right relation
            scanStatus2 = scan2.getRandomRecord(nextRID2, nextRecord2);
            if(scanStatus2 != OK)
                return scanStatus2;
            
            //filter tuple for projections
            recordToInsert.data = malloc(reclen);
            recordToInsert.length = reclen;
            currentOffset = 0;
            
            for(int i = 0; i < projCnt; ++i) {
                if(strcmp(attrDescArray[i].relName, attrDesc1.relName) == 0) {
                    //attribute is the outter/left relation's
                    memcpy((char*)recordToInsert.data + currentOffset, (char*)nextRecord1.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
                    currentOffset += attrDescArray[i].attrLen;
                }else {
                    //attribute is the inner/right relation's
                    memcpy((char*)recordToInsert.data + currentOffset, (char*)nextRecord2.data + attrDescArray[i].attrOffset, attrDescArray[i].attrLen);
                    currentOffset += attrDescArray[i].attrLen;
                }
                
            }
            
            //output to results
            heapStatus = output.insertRecord(recordToInsert, nextRID1);
            free(recordToInsert.data);
            if(heapStatus != OK)
                return heapStatus;
            
            //get next tuple
            indexStatus = index.scanNext(nextRID2);
        }
        index.endScan();
        //get next tuple from relation
        scanStatus1 = scan1.scanNext(nextRID1, nextRecord1);
    }
    if(scanStatus1 != FILEEOF)
        return scanStatus1;
    scan1.endScan();
    scan2.endScan();
    
    return OK;
}


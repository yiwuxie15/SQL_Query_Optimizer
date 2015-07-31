#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Status Operators::SNL(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The right attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
    cout << "Algorithm: Simple NL Join" << endl;
    
    /* Your solution goes here */
    
    Status heapStatus, scanStatus1, scanStatus2;
    //for output
    HeapFile output(result, heapStatus);
    //used to scan the outer/left relation
    //HeapFileScan scan1(attrDesc1.relName, attrDesc1.attrOffset, attrDesc1.attrLen, (Datatype)attrDesc1.attrType, NULL, op, scanStatus1);
    HeapFileScan scan1(attrDesc1.relName, scanStatus1);
    //used to scan the inner/right relation
    HeapFileScan scan2(attrDesc2.relName, scanStatus2);
    
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
    
    scanStatus1 = scan1.scanNext(nextRID1, nextRecord1);
    while(scanStatus1 == OK) {
        scanStatus2 = scan2.startScan(attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype)attrDesc2.attrType, NULL, op);
        if(scanStatus2 == OK)
            scanStatus2 = scan2.scanNext(nextRID2, nextRecord2);
        while(scanStatus2 == OK) {
            int matchResult = matchRec(nextRecord1, nextRecord2, attrDesc1, attrDesc2);
            bool match = false;
            switch(op) {
                case LT:
                    if(matchResult < 0)
                        match = true;
                    break;
                case GT:
                    if(matchResult > 0)
                        match = true;
                    break;
                case LTE:
                    if(matchResult <= 0)
                        match = true;
                    break;
                case GTE:
                    if(matchResult >= 0)
                        match = true;
                    break;
                case NE:
                    if(matchResult != 0)
                        match = true;
                    break;
                case NOTSET:
                    return NOTUSED1;
                    break;
                default:
                    return NOTUSED1;
                    break;
            }
            if(match) {
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
            }
            
            //get next tuple
            scanStatus2 = scan2.scanNext(nextRID2, nextRecord2);
        }
        scanStatus2 = scan2.endScan();
        
        //get next tuple from relation
        scanStatus1 = scan1.scanNext(nextRID1, nextRecord1);
    }
    if(scanStatus1 != FILEEOF)
        return scanStatus1;
    scan1.endScan();
    scan2.endScan();
    return OK;
}

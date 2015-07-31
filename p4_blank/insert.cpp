#include "catalog.h"
#include "query.h"
#include "index.h"
#include "utility.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*
 * Inserts a record into the specified relation
 *
 * Returns:
 * 	OK on success
 * 	an error code otherwise
 */

Status Updates::Insert(const string& relation,      // Name of the relation
                       const int attrCnt,           // Number of attributes specified in INSERT statement
                       const attrInfo attrList[])   // Value of attributes specified in INSERT statement
{
    /* Your solution goes here */
    Record record;
    int TempattrCnt;
    AttrDesc *attrd;
    bool insert = true;
    Status Stat;
    
    Stat = attrCat->getRelInfo(relation, TempattrCnt, attrd);
    if(Stat != OK){
        return Stat;
    }
    
    if(TempattrCnt == attrCnt){
        
        //Get length of record and allocate memory for it
        record.length = attrd[TempattrCnt-1].attrOffset+attrd[TempattrCnt-1].attrLen-attrd[0].attrOffset;
        record.data = malloc(record.length);
        
        //Reorder AttrList
        attrInfo Tempattrl[TempattrCnt];
        for(int i=0; i<TempattrCnt; ++i){
            for(int j=0; j<TempattrCnt; ++j){
                if(strcmp(attrd[i].attrName, attrList[j].attrName)==0){
                    Tempattrl[i] = attrList[j];
                }
            }
        }
        
        //Package data into record
        for(int i =0; i<TempattrCnt;i++){
            int len = strlen((char*)Tempattrl[i].attrValue);
            if((Datatype)Tempattrl[i].attrType==2 && len>attrd[i].attrLen){
                insert = false;
            }else{
                memcpy((char*)record.data+attrd[i].attrOffset, Tempattrl[i].attrValue, attrd[i].attrLen);
            }
        }
        
        //Insert Data Entry into Heapfile
        Status openHeapStat, insertHeapStat, openIndexStat, insertIndexStat;
        RID recordId;
        int unique = 0;
        Datatype type;
        //Utilities U;//for insert test
        
        HeapFile tempHeap = HeapFile(relation, openHeapStat);
        if(openHeapStat == OK && insert){
            //insert record into heapfile and release memory
            insertHeapStat = tempHeap.insertRecord(record, recordId);
            //U.Print(relation);//for insert test
            free(record.data);
            if(insertHeapStat == OK){
                for (int i=0; i< TempattrCnt; ++i){
                    if(attrd[i].indexed==1){
                        type = (Datatype)attrd[i].attrType;
                        Index index = Index(relation, attrd[i].attrOffset, attrd[i].attrLen,type, unique, openIndexStat);
                        if (openIndexStat == OK){
                            insertIndexStat = index.insertEntry(Tempattrl[i].attrValue,recordId);
                        }else{
                            delete []attrd;
                            return openIndexStat;
                        }
                    }
                }
            }else{
                delete []attrd;
                return insertHeapStat;
            }
        }else{
            free(record.data);
            if(!insert){
                delete []attrd;
                return ATTRTOOLONG;
            }else{
                delete []attrd;
                return openHeapStat;
            }
        }
    }else{
        delete []attrd;
        return ATTRTYPEMISMATCH;
    }
    delete []attrd;
    return OK;
}

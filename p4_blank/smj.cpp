#include "catalog.h"
#include "query.h"
#include "sort.h"
#include "index.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Consider using Operators::matchRec() defined in join.cpp
 * to compare records when joining the relations */

// Help function to project attribute and insert into heapfile
Status ProjInsert(HeapFile &result,//result heapfile
           const string &relname1, //relation name 1
           const string &relname2, // relation name 2
           Record &r1, // record 1
           Record &r2, // record 2
           const int projCnt, // num of attributes in projection list
           const AttrDesc projList[]){
    
    Status Stat;
    void* rec1 = r1.data;
    void* rec2 = r2.data;
    Record recToInsert;
    RID rid;
    int reclen = 0;
    int offset = 0;
    for(int i=0; i<projCnt; ++i){
        reclen += projList[i].attrLen;
    }
    recToInsert.length = reclen;
    recToInsert.data = malloc(reclen);
    
    //find each attribute in rel1 or rel2
    for(int i=0; i<projCnt; ++i){
        if(projList[i].relName == relname1){
            memcpy((char*)recToInsert.data + offset,
                   (char*)rec1 + projList[i].attrOffset,
                   projList[i].attrLen);
            offset += projList[i].attrLen;
        }else if(projList[i].relName == relname2){
            memcpy((char*)recToInsert.data + offset,
                   (char*)rec2 + projList[i].attrOffset,
                   projList[i].attrLen);
            offset += projList[i].attrLen;
        }else{
            free(recToInsert.data);
            return RELNOTFOUND;
        }
    }
    
    Stat = result.insertRecord(recToInsert, rid);
    free(recToInsert.data);
    if(Stat != OK) return Stat;
    return OK;
}

Status Operators::SMJ(const string& result,           // Output relation name
                      const int projCnt,              // Number of attributes in the projection
                      const AttrDesc attrDescArray[], // Projection list (as AttrDesc)
                      const AttrDesc& attrDesc1,      // The left attribute in the join predicate
                      const Operator op,              // Predicate operator
                      const AttrDesc& attrDesc2,      // The right attribute in the join predicate
                      const int reclen)               // The length of a tuple in the result relation
{
  cout << "Algorithm: SM Join" << endl;

  /* Your solution goes here */
    Status Stat;
    AttrDesc* attrd1;
    AttrDesc* attrd2;
    int attrCnt1, attrCnt2;
    int pageNum = bufMgr->numUnpinnedPages();
    int lTupleLen, rTupleLen;
    int lTupleNum, rTupleNum;
    Stat = attrCat->getRelInfo(attrDesc1.relName, attrCnt1, attrd1);
    if(Stat != OK){
        delete []attrd1;
        return Stat;
    }
    Stat = attrCat->getRelInfo(attrDesc2.relName, attrCnt2, attrd2);
    if(Stat != OK){
        delete []attrd1;
        delete []attrd2;
        return Stat;
    }
    
    HeapFile result_rec(result, Stat); //heapfile to store final record
    if(Stat != OK){
        delete []attrd1;
        delete []attrd2;
        return Stat;
    }
    
    //calculate length of one single tuple
    lTupleLen = attrd1[attrCnt1-1].attrOffset + attrd1[attrCnt1-1].attrLen - attrd1[0].attrOffset;
    rTupleLen = attrd2[attrCnt2-1].attrOffset + attrd2[attrCnt2-1].attrLen - attrd2[0].attrOffset;
    
    //calculate how many tuples contained
    lTupleNum = pageNum * 0.8 * PAGESIZE / lTupleLen;
    rTupleNum = pageNum * 0.8 * PAGESIZE / rTupleLen;
    
    //Create SortFiles to sortmerge
    SortedFile sortFileL = SortedFile(attrDesc1.relName, attrDesc1.attrOffset, attrDesc1.attrLen, (Datatype)attrDesc1.attrType, lTupleNum, Stat);
    if(Stat != OK) {
        delete []attrd1;
        delete []attrd2;
        return Stat;
    }
    SortedFile sortFileR = SortedFile(attrDesc2.relName, attrDesc2.attrOffset, attrDesc2.attrLen, (Datatype)attrDesc2.attrType, rTupleNum, Stat);
    if(Stat != OK){
        delete []attrd1;
        delete []attrd2;
        return Stat;
    }
    
    //Start Merge
    Record r1, r2;
    Status Stat1, Stat2;
    int match;
    Stat1=sortFileL.next(r1);
    Stat2=sortFileR.next(r2);
    
    while(Stat1==OK && Stat2==OK){
        if((match=matchRec(r1, r2, attrDesc1, attrDesc2)) < 0){
            Stat1 = sortFileL.next(r1);
        }else if((match=matchRec(r1, r2, attrDesc1, attrDesc2)) > 0){
            Stat2 = sortFileR.next(r2);
        }else{
            //if attr1 == attr2
            //set mark on record2
            //copy the record into heapfile
            sortFileR.setMark();
            Record markRec;
            markRec.data = r2.data;
            markRec.length = r2.length;
            
            //if multiple records on file2 match record 1
            while(Stat2 == OK && match == 0){
                Stat = ProjInsert(result_rec, attrDesc1.relName, attrDesc2.relName, r1, r2, projCnt, attrDescArray);
                if(Stat != OK){
                    delete []attrd1;
                    delete []attrd2;
                    return Stat;
                }
                
                //keep on scanning unitl dismatch
                Stat2 = sortFileR.next(r2);
                if(Stat2 != FILEEOF && Stat2 != OK){
                    delete []attrd1;
                    delete []attrd2;
                    return Stat2;
                }
                match = matchRec(r1, r2, attrDesc1, attrDesc2);
            }
            if(Stat2 != FILEEOF && Stat2 != OK){
                delete []attrd1;
                delete []attrd2;
                return Stat2;
            }
            
            //if dismatch, move record1 forward to see if match the mark in file2
            Stat1 = sortFileL.next(r1);
            if(Stat1 != OK){
                delete []attrd1;
                delete []attrd2;
                return Stat1;
            }
            while(Stat1 == OK && matchRec(r1, markRec, attrDesc1, attrDesc2) == 0){
                //go to mark in file2 to see if matches
                Stat2 = sortFileR.gotoMark();
                Stat2 = sortFileR.next(r2);
                match = matchRec(r1, r2, attrDesc1, attrDesc2);
                while(Stat2 == OK && match == 0){
                    Stat = ProjInsert(result_rec, attrDesc1.relName, attrDesc2.relName, r1, r2, projCnt, attrDescArray);
                    if(Stat != OK){
                        delete []attrd1;
                        delete []attrd2;
                        return Stat;
                    }
                    Stat2 = sortFileR.next(r2);
                    match = matchRec(r1, r2, attrDesc1, attrDesc2);
                    
                }
                Stat1 = sortFileL.next(r1);
            }
            if ( Stat1 != FILEEOF && Stat1 != OK ){
                delete []attrd1;
                delete []attrd2;
                return Stat1;
            }
        }
        
    }
    delete []attrd1;
    delete []attrd2;
    return OK;
}


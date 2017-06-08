#ifndef regfile_H_INCLUDED
#define regfile_H_INCLUDED
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

extern int reg[35];
extern char data[1024];
extern char instruct[1024];
extern int cycle;
extern bool termi;
extern int error3;
extern int jump;
extern FILE* fptr_report;

extern int ICacheHit;
extern int ICacheMiss;
extern int DCacheHit;
extern int DCacheMiss;
extern int ITLBHit;
extern int ITLBMiss;
extern int DTLBHit;
extern int DTLBMiss;
extern int IPTEHit;
extern int IPTEMiss;
extern int DPTEHit;
extern int DPTEMiss;

extern int IMemSize;
extern int DMemSize;
extern int IMemPSize;
extern int DMemPSize;
extern int ICacheTotalSize;
extern int ICacheBlockSize;
extern int ICacheSet;
extern int DCacheTotalSize;
extern int DCacheBlockSize;
extern int DCacheSet;
////////////////////////
extern int IPTEentry;
extern int ITLBentry;
extern int IMemoryEntry;
extern int ICacheEntry;

extern bool * ICacheValid;
extern int * ICacheTag;
extern bool * IMemoryValid;
extern bool * IPTEValid;
extern int * IPTEPpn;
extern int * IPTEVpn;
extern bool * ITLBValid;
extern int * ITLBPpn;
extern int * ITLBVpn;

extern int * ICacheMRU;
extern int * IMemoryLRU;
extern int * ITLBLRU;
////////////////////////////
extern int DPTEentry;
extern int DTLBentry;
extern int DMemoryEntry;
extern int DCacheEntry;

extern bool * DCacheValid;
extern int * DCacheTag;
extern bool * DMemoryValid;
extern bool * DPTEValid;
extern int * DPTEPpn;
extern int * DPTEVpn;
extern bool * DTLBValid;
extern int * DTLBPpn;
extern int * DTLBVpn;

extern int * DCacheMRU;
extern int * DMemoryLRU;
extern int * DTLBLRU;

extern unsigned int Dva;
///////////////////////////////
void regset(void);

#endif
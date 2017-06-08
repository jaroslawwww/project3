#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "allinstruction.h"
#include "regfile.h"
#include "function.h"
#include "memory.h"

using namespace std;

int main(int argc, char *argv[])
{	
	char buffer[4];
	int i = -1, index = 0, max = 0;
	regset();
	FILE *fp, *result;
	fpos_t pos;
	fp = fopen("dimage.bin", "rb");
	result = fopen("snapshot.rpt", "w");
	fptr_report = fopen("report.rpt", "w");
	if(!fp) return 1;
	
	for(int h=1; h<argc; h++){
		if(h == 1) IMemSize = atoi(argv[h]);
		else if(h == 2) DMemSize = atoi(argv[h]);
		else if(h == 3)	IMemPSize = atoi(argv[h]);
		else if(h == 4) DMemPSize = atoi(argv[h]);
		else if(h == 5) ICacheTotalSize = atoi(argv[h]);
		else if(h == 6) ICacheBlockSize = atoi(argv[h]);
		else if(h == 7) ICacheSet = atoi(argv[h]);
		else if(h == 8) DCacheTotalSize = atoi(argv[h]);
		else if(h == 9) DCacheBlockSize = atoi(argv[h]);
		else if(h == 10) DCacheSet = atoi(argv[h]);
	}
	
	IPTEentry = 1024 / IMemPSize;
	ITLBentry = IPTEentry / 4;
	IMemoryEntry = IMemSize / IMemPSize;
	ICacheEntry = ICacheTotalSize / ICacheBlockSize;
		
	DPTEentry = 1024 / DMemPSize;
	DTLBentry = DPTEentry / 4;
	DMemoryEntry = DMemSize / DMemPSize;
	DCacheEntry = DCacheTotalSize / DCacheBlockSize;
		
	/* Iimage.bin */	
	ICacheValid = new bool[ICacheEntry];
	ICacheTag = new int[ICacheEntry];
	ICacheMRU = new int[ICacheEntry];
	for(int z = 0; z <ICacheEntry; z++){
		ICacheValid[z] = 0;
		ICacheTag[z] = 0;
		ICacheMRU[z] = 0;
	}
	
	IMemoryValid = new bool[IMemoryEntry];
	IMemoryLRU = new int[IMemoryEntry];
	for(int z = 0; z <IMemoryEntry; z++){
		IMemoryValid[z] = 0;
		IMemoryLRU[z] = 0;
	} 
	IPTEValid = new bool[IPTEentry];
	IPTEPpn = new int[IPTEentry];
	IPTEVpn = new int[IPTEentry];
	for(int z = 0; z <IPTEentry; z++){
		IPTEValid[z] = 0;
		IPTEPpn[z] = 0;
		IPTEVpn[z] = 0;
	} 
	
	ITLBValid = new bool[ITLBentry];
	ITLBPpn = new int[ITLBentry];
	ITLBVpn = new int[ITLBentry];
	ITLBLRU = new int[ITLBentry];
	for(int z = 0; z <ITLBentry; z++){
		ITLBValid[z] = 0;
		ITLBPpn[z] = 0;
		ITLBVpn[z] = 0;
		ITLBLRU[z] = 0;
	} 
	/* Dimage.bin */
	DCacheValid = new bool[DCacheEntry];
	DCacheTag = new int[DCacheEntry];
	DCacheMRU = new int[DCacheEntry];
	for(int z = 0; z <DCacheEntry; z++){
		DCacheValid[z] = 0;
		DCacheTag[z] = 0;
		DCacheMRU[z] = 0;
	}
	
	DMemoryValid = new bool[DMemoryEntry];
	DMemoryLRU = new int[DMemoryEntry];
	for(int z = 0; z <DMemoryEntry; z++){
		DMemoryValid[z] = 0;
		DMemoryLRU[z] = 0;
	} 
	DPTEValid = new bool[DPTEentry];
	DPTEPpn = new int[DPTEentry];
	DPTEVpn = new int[DPTEentry];
	for(int z = 0; z <DPTEentry; z++){
		DPTEValid[z] = 0;
		DPTEPpn[z] = 0;
		DPTEVpn[z] = 0;
	} 
	
	DTLBValid = new bool[DTLBentry];
	DTLBPpn = new int[DTLBentry];
	DTLBVpn = new int[DTLBentry];
	DTLBLRU = new int[DTLBentry];
	for(int z = 0; z <DTLBentry; z++){
		DTLBValid[z] = 0;
		DTLBPpn[z] = 0;
		DTLBVpn[z] = 0;
		DTLBLRU[z] = 0;
	} 
	
	while(i++<257){
		if(i>1+max) break;
		int temp = 0;
		fread(buffer, sizeof(char), 4, fp);
		if(i == 0){
			temp = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				temp = temp << 8;
				temp = (temp | (unsigned char)buffer[y]);
			}
			reg[29] = temp;
		}else if(i == 1){
			max = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				max = max<< 8;
				max = (max | (unsigned char)buffer[y]);
			}
		}else if(i > 1){
			data[index++] = buffer[0];
			data[index++] = buffer[1];
			data[index++] = buffer[2];
			data[index++] = buffer[3];
		}
	}
	fclose(fp);
	fp = fopen("iimage.bin", "rb");
	if(!fp) return 1;
	i = -1, index = 0;
	while(i++<257){
		if(i>1+max) break;
		int temp = 0;
		fread(buffer, sizeof(char), 4, fp);
		if(i == 0){
			temp = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				temp = temp << 8;
				temp = (temp | (unsigned char)buffer[y]);
			}
			reg[34] = temp;
			index = reg[34];
		}else if(i == 1){
			max = (unsigned char)buffer[0];
			for(int y=1; y<4; y++){
				max = max<< 8;
				max = (max | (unsigned char)buffer[y]);
			}
		}else if(i > 1){
			instruct[index++] = buffer[0];
			instruct[index++] = buffer[1];
			instruct[index++] = buffer[2];
			instruct[index++] = buffer[3];
		}
	}
	fprintf(result, "cycle %d\n", cycle);
	for(int j=0; j<35; j++){
		if(j<10) fprintf(result, "$0%d: 0x%08X\n", j, reg[j]);
		else if(j<32) fprintf(result, "$%d: 0x%08X\n", j, reg[j]);
		else if(j==32) fprintf(result, "$HI: 0x%08X\n", reg[j]);
		else if(j==33) fprintf(result, "$LO: 0x%08X\n", reg[j]);
		else fprintf(result, "PC: 0x%08X\n", reg[j]);
	}
	fprintf(result, "\n\n");
	while(1){
		int diff[35], x;
		for(x=0; x<35; x++) diff[x] = reg[x];
		cycle++;
		for(int c=0; c<ITLBentry; c++) ITLBLRU[c]++;
        for(int c=0; c<IMemoryEntry; c++) IMemoryLRU[c]++;
		for(int c=0; c<DTLBentry; c++) DTLBLRU[c]++;
        for(int c=0; c<DMemoryEntry; c++) DMemoryLRU[c]++;
		instruction(&instruct[reg[34]]);
		if(termi) break;
		if(jump != 1) reg[34] += 4;
		else jump = 0;
		fprintf(result, "cycle %d\n", cycle);
		for(x=0; x<35; x++){
			if(diff[x] != reg[x]) {
				if(x<10) fprintf(result, "$0%d: 0x%08X\n", x, reg[x]);
				else if(x<32) fprintf(result, "$%d: 0x%08X\n", x, reg[x]);
				else if(x==32) fprintf(result, "$HI: 0x%08X\n", reg[x]);
				else if(x==33) fprintf(result, "$LO: 0x%08X\n", reg[x]);
				else fprintf(result, "PC: 0x%08X\n", reg[x]);
			}
		}
		fprintf(result, "\n\n");
	}
	/* report.rpt */
	fprintf( fptr_report, "ICache :\n"); 
	fprintf( fptr_report, "# hits: %u\n", ICacheHit ); 
	fprintf( fptr_report, "# misses: %u\n\n", ICacheMiss ); 
	fprintf( fptr_report, "DCache :\n"); 
	fprintf( fptr_report, "# hits: %u\n", DCacheHit ); 
	fprintf( fptr_report, "# misses: %u\n\n", DCacheMiss ); 
	fprintf( fptr_report, "ITLB :\n"); 
	fprintf( fptr_report, "# hits: %u\n", ITLBHit ); 
	fprintf( fptr_report, "# misses: %u\n\n", ITLBMiss ); 
	fprintf( fptr_report, "DTLB :\n"); 
	fprintf( fptr_report, "# hits: %u\n", DTLBHit ); 
	fprintf( fptr_report, "# misses: %u\n\n", DTLBMiss ); 
	fprintf( fptr_report, "IPageTable :\n"); 
	fprintf( fptr_report, "# hits: %u\n", IPTEHit ); 
	fprintf( fptr_report, "# misses: %u\n\n", IPTEMiss ); 
	fprintf( fptr_report, "DPageTable :\n"); 
	fprintf( fptr_report, "# hits: %u\n", DPTEHit ); 
	fprintf( fptr_report, "# misses: %u\n\n", DPTEMiss ); 
	
	/*
	if(ICacheValid) delete [] ICacheValid;
	if(ICacheTag) delete [] ICacheTag;
	if(IMemoryValid) delete [] IMemoryValid;
	if(IPTEValid) delete [] IPTEValid;
	if(IPTEPpn) delete [] IPTEPpn;
	if(IPTEVpn) delete [] IPTEVpn;
	if(ITLBValid) delete [] ITLBValid;
	if(ITLBPpn) delete [] ITLBPpn;
	if(ITLBVpn) delete [] ITLBVpn;
	if(ICacheMRU) delete [] ICacheMRU;
	if(IMemoryLRU) delete [] IMemoryLRU;
	if(ITLBLRU) delete [] ITLBLRU;
	
	if(DCacheValid) delete [] DCacheValid;
	if(DCacheTag) delete [] DCacheTag;
	if(DMemoryValid) delete [] DMemoryValid;
	if(DPTEValid) delete [] DPTEValid;
	if(DPTEPpn) delete [] DPTEPpn;
	if(DPTEVpn) delete [] DPTEVpn;
	if(DTLBValid) delete [] DTLBValid;
	if(DTLBPpn) delete [] DTLBPpn;
	if(DTLBVpn) delete [] DTLBVpn;
	if(DCacheMRU) delete [] DCacheMRU;
	if(DMemoryLRU) delete [] DMemoryLRU;
	if(DTLBLRU) delete [] DTLBLRU;
	*/
	fclose(result);
	fclose(fp);
	fclose(fptr_report);
	
	return 0;
}

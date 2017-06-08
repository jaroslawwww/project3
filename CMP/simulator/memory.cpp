#include "memory.h"

void memoryAccess()
{
	int TLBHit = 0, TLBMiss = 0;
	int PTEHit = 0, PTEMiss = 0;
	int cacheHit = 0, cacheMiss = 0;
	/* TLB check */
	int ppn, vpn, pa, va, tag, setIndex, index, begin, end;
	int setNumber, temp, temp2;
	setNumber = ICacheEntry / ICacheSet;
	va = reg[34];
	vpn = va / IMemPSize;
	int a;
	for(a = 0; a < ITLBentry; a++){
		if(ITLBVpn[a] == vpn && ITLBValid[a]){
			TLBHit = 1;
			ITLBLRU[a] = 0;
			ppn = ITLBPpn[a];
			break;
		}else{
			if(a == ITLBentry - 1){
				TLBMiss = 1;
			} 
		}
	}
	if(TLBMiss == 1){ // PTE check
		if(IPTEValid[vpn] && IPTEVpn[vpn] == vpn){
			PTEHit = 1;
			ppn = IPTEPpn[vpn];
			IMemoryLRU[ppn] = 0;
			IMemoryValid[ppn] = 1;
		}else{
			PTEMiss = 1;
			temp = 0; // temp2 => ppn
			for(int z = 0; z<IMemoryEntry; z++){
				if(IMemoryLRU[z] > temp){
					temp = IMemoryLRU[z];
					temp2 = z;
				}
			}
			for(int x=0; x<IPTEentry; x++){
				if(IPTEPpn[x] == temp2 && IPTEValid[x]){
					temp = IPTEVpn[x];
					IPTEValid[x] = 0;
				}
			}
			IMemoryValid[temp2] = 1;
			IMemoryLRU[temp2] = 0;
			IPTEVpn[vpn] = vpn;
			IPTEPpn[vpn] = temp2;
			IPTEValid[vpn] = 1;

			ppn = temp2;
			for(int q=0;q<ICacheEntry; q++){
				if(ICacheTag[q]*ICacheBlockSize*setNumber/IMemPSize == temp2){
					ICacheMRU[q] = 0;
					ICacheValid[q] = 0;
				}
			}
			for(int w=0; w<ITLBentry; w++){
				if(ITLBPpn[w] == temp2  && ITLBValid[w]){
					ITLBLRU[w] = 1000000;
				}
			}
		}
		temp = 0;
		for(int y=0; y<ITLBentry; y++){
			if(ITLBLRU[y] > temp){
				temp = ITLBLRU[y];
				temp2 = y;
			}
		}
		ITLBVpn[temp2] = vpn;
		ITLBPpn[temp2] = ppn;
		ITLBValid[temp2] = 1;
		ITLBLRU[temp2] = 0;
	}
	// cache check
	pa = ppn * IMemPSize + va % IMemPSize;
	tag = pa / setNumber / ICacheBlockSize;
	setIndex = pa / ICacheBlockSize % setNumber;
	begin = setIndex * ICacheSet;
	end = (setIndex+1) * ICacheSet;
	for(int c = begin; c < end; c++){
		if(ICacheTag[c] == tag && ICacheValid[c]){ // cache hit
			cacheHit = 1;
			int d;
			for(d=begin; d<end; d++){
				if(d!=c && ICacheMRU[d] == 0) break;
			} 
			if(d == end){
				for(d=begin; d<end; d++) ICacheMRU[d] = 0;
			}
			if(ICacheSet != 1) ICacheMRU[c] = 1;
			break;
		}else{
			if(c == end - 1){ // cache miss
				cacheMiss = 1;
				int e;
				for(e=begin; e<end; e++){
					if(ICacheValid[e] == 0){ // not yet valid and full
						ICacheTag[e] = tag;
						int f;
						for(f=begin; f<end; f++){
							if(f!=e && ICacheMRU[f]==0) break;
						}
						if(f == end){
							for(int f=begin; f<end; f++) ICacheMRU[f] = 0;
						}
						if(ICacheSet != 1) ICacheMRU[e] = 1;
						ICacheValid[e] = 1;
						break;
					}else{
						if(e == end - 1){
							for(int g=begin; g<end; g++){
								if(ICacheMRU[g] == 0){ // all valid and full
									ICacheTag[g] = tag;
									int h;
									for(h=begin; h<end; h++){
										if(h != g && ICacheMRU[h] == 0) break;
									}
									if(h == end){
										for(int h=begin; h<end; h++) ICacheMRU[h] = 0;
									}
									if(ICacheSet != 1) ICacheMRU[g] = 1;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	ICacheHit += cacheHit;
	ICacheMiss += cacheMiss;
	ITLBHit += TLBHit;
	ITLBMiss += TLBMiss;
	IPTEHit += PTEHit;
	IPTEMiss += PTEMiss;
}


void memoryDaccess()
{
	int TLBHit = 0, TLBMiss = 0;
	int PTEHit = 0, PTEMiss = 0;
	int cacheHit = 0, cacheMiss = 0;
	/* TLB check */
	int ppn, vpn, pa, va, tag, setIndex, index, begin, end;
	int setNumber, temp, temp2;
	setNumber = DCacheEntry / DCacheSet;
	va = Dva;
	vpn = va / DMemPSize;
	int a;
	for(a = 0; a < DTLBentry; a++){
		if(DTLBVpn[a] == vpn && DTLBValid[a]){
			TLBHit = 1;
			DTLBLRU[a] = 0;
			ppn = DTLBPpn[a];
			break;
		}else{
			if(a == DTLBentry - 1){
				TLBMiss = 1;
			} 
		}
	}
	if(TLBMiss == 1){ // PTE check
		if(DPTEValid[vpn] && DPTEVpn[vpn] == vpn){
			PTEHit = 1;
			ppn = DPTEPpn[vpn];
			DMemoryLRU[ppn] = 0;
			DMemoryValid[ppn] = 1;
		}else{
			PTEMiss = 1;
			
			temp = 0; // temp2 => ppn
			for(int z = 0; z<DMemoryEntry; z++){
				if(DMemoryLRU[z] > temp){
					temp = DMemoryLRU[z];
					temp2 = z;
				}
			}
			for(int x=0; x<DPTEentry; x++){
				if(DPTEPpn[x] == temp2 && DPTEValid[x]){
					temp = DPTEVpn[x];
					DPTEValid[x] = 0;
					//break;
				}
			}
			DPTEValid[vpn] = 1;
			DMemoryValid[temp2] = 1;
			DMemoryLRU[temp2] = 0;
			DPTEVpn[vpn] = vpn;
			DPTEPpn[vpn] = temp2;
			ppn = temp2;
			for(int q=0;q<DCacheEntry; q++){
				if(DCacheTag[q]*DCacheBlockSize*setNumber/DMemPSize == temp2){
					DCacheMRU[q] = 0;
					DCacheValid[q] = 0;
				}
			}
			for(int w=0; w<DTLBentry; w++){
				if(DTLBPpn[w] == temp2 && DTLBValid[w]){
					DTLBLRU[w] = 1000000;
					//DTLBValid[w] = 0;
				}
			}
			//if(DPTEValid[temp] && DPTEPpn[temp] == ppn) DPTEValid[temp] = 0;
		}
		temp = 0;
		for(int y=0; y<DTLBentry; y++){
			if(DTLBLRU[y] > temp){
				temp = DTLBLRU[y];
				temp2 = y;
			}
		}
		DTLBVpn[temp2] = vpn;
		DTLBPpn[temp2] = ppn;
		DTLBValid[temp2] = 1;
		DTLBLRU[temp2] = 0;
	}
	// cache check
	pa = ppn * DMemPSize + va % DMemPSize;
	tag = pa / setNumber / DCacheBlockSize;
	setIndex = (pa / DCacheBlockSize) % setNumber;
	begin = setIndex * DCacheSet;
	end = (setIndex+1) * DCacheSet;
	for(int c = begin; c < end; c++){
		if(DCacheTag[c] == tag && DCacheValid[c]){ // cache hit
			cacheHit = 1;
			int d;
			for(d=begin; d<end; d++){
				if(d!=c && DCacheMRU[d] == 0) break;
			} 
			if(d == end){
				for(d=begin; d<end; d++) DCacheMRU[d] = 0;
			}
			if(DCacheSet != 1) DCacheMRU[c] = 1;
			break;
		}else{
			if(c == end - 1){ // cache miss
				cacheMiss = 1;
				int e;
				for(e=begin; e<end; e++){
					if(DCacheValid[e] == 0){ // not yet valid and full
						DCacheTag[e] = tag;
						int f;
						for(f=begin; f<end; f++){
							if(f!=e && DCacheMRU[f]==0) break;
						}
						if(f == end){
							for(int f=begin; f<end; f++) DCacheMRU[f] = 0;
						}
						if(DCacheSet != 1) DCacheMRU[e] = 1;
						DCacheValid[e] = 1;
						break;
					}else{
						if(e == end - 1){
							for(int g=begin; g<end; g++){
								if(DCacheMRU[g] == 0){ // all valid and full
									DCacheTag[g] = tag;
									int h;
									for(h=begin; h<end; h++){
										if(h != g && DCacheMRU[h] == 0) break;
									}
									if(h == end){
										for(int h=begin; h<end; h++) DCacheMRU[h] = 0;
									}
									if(DCacheSet != 1) DCacheMRU[g] = 1;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	DCacheHit += cacheHit;
	DCacheMiss += cacheMiss;
	DTLBHit += TLBHit;
	DTLBMiss += TLBMiss;
	DPTEHit += PTEHit;
	DPTEMiss += PTEMiss;
}
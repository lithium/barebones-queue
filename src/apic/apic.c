#include <inttypes.h>

#define CPUID_FLAG_APIC 					(1<<9)
#define CPUID_FLAG_HARDWARE_MULTITHREAD		(1<<29)

#include <cpuid.h>

uint32_t *CpuidMaxLeaf = {0};
char CpuidVendorName[13] = "\0\0\0\0\0\0\0\0\0\0\0\0\0";

uint32_t *CpuidInfoEax = {0};
uint32_t *CpuidInfoEbx = {0};
uint32_t *CpuidInfoEcx = {0};
uint32_t *CpuidInfoEdx = {0};

uint32_t *CpuidTopologyEax = {0};
uint32_t *CpuidTopologyEbx = {0};
uint32_t *CpuidTopologyEcx = {0};
uint32_t *CpuidTopologyEdx = {0};


#define IsApicSupported					(*CpuidInfoEdx & CPUID_FLAG_APIC)
#define IsHardwareMultithreadSupported	(*CpuidInfoEdx & CPUID_FLAG_HARDWARE_MULTITHREAD)



void CpuidInfo()
{
	uint32_t *vendorNameEcx = (uint32_t*)CpuidVendorName+8;
	uint32_t *vendorNameEdx = (uint32_t*)CpuidVendorName+4;
	__cpuid(0, CpuidMaxLeaf, CpuidVendorName, vendorNameEcx, vendorNameEdx);
}

void CpuidFeatures()
{
	__cpuid(1, CpuidInfoEax, CpuidInfoEbx, CpuidInfoEcx, CpuidInfoEdx);
}

void CpuidTopology(uint32_t n)
{
	asm volatile ("mov %%edx, %%ecx": :"d" (n));
	__cpuid(0xB, CpuidTopologyEax, CpuidTopologyEbx, CpuidTopologyEcx, CpuidTopologyEdx);

}


/*
 * getBitsFromDWORD
 *
 * Returns of bits [to:from] of DWORD
 *
 * Arguments:
 *     val        DWORD to extract bits from
 *     from       Low order bit
 *     to         High order bit
 * Return:        Specified bits from DWORD val
 */
unsigned long getBitsFromDWORD(const unsigned int val, const char from, const char to)
{
	unsigned long mask = (1<<(to+1)) - 1;
	if (to == 31)	return val >> from;
	return (val & mask) >> from;
}

#define CPUID_LEVELTYPE_SMT 1
#define CPUID_LEVELTYPE_CORE 2

typedef struct CpuTopology {
	uint32_t SMTSelectMask;
	uint32_t SMTMaskWidth;
	uint32_t PkgSelectMaskShift;
	uint32_t PkgSelectMask;
	uint32_t CoreSelectMask;
} CpuTopology;


int CPUTopologyLeafBConstants(CpuTopology *self)
{
	int subLeaf = 0;
	int levelType;
	int levelShift;

	uint32_t coreplusSMT_Mask = 0;

	int threadReported = 0;
	int coreReported = 0;

	for (subLeaf = 0;  ; subLeaf++) {
		CpuidTopology(subLeaf);
		if (*CpuidTopologyEbx == 0) {
			break;
		}

		levelType = getBitsFromDWORD(*CpuidTopologyEcx, 8, 15);
		levelShift = getBitsFromDWORD(*CpuidTopologyEcx, 0, 4);
		switch (levelType) 
		{
			case CPUID_LEVELTYPE_SMT:
				self->SMTSelectMask = ~((-1) << levelShift);
				self->SMTMaskWidth = levelShift;
				threadReported = 1;
				break;

			case CPUID_LEVELTYPE_CORE:
				coreplusSMT_Mask = ~((-1) << levelShift);
				self->PkgSelectMaskShift = levelShift;
				self->PkgSelectMask = (-1) ^ coreplusSMT_Mask;
				coreReported = 1;
				break;
		}
	}

	if (threadReported) {
		if (coreReported) {
			self->CoreSelectMask = coreplusSMT_Mask ^ self->SMTSelectMask;
		} else {
			self->CoreSelectMask = 0;
			self->PkgSelectMaskShift = self->SMTMaskWidth;
			self->PkgSelectMask = (-1) ^ self->SMTSelectMask;
		}
	} else {
        // throw an error, this should not happen if hardware function normally
        return -1;
	}
	return 0;
}



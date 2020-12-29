/**
 *
 *  Copyright (C) Manas Kamal Choudhury 2020
 *
 *  VirtualMemory -- Virtual Memory Manager 
 *
 *  /PROJECT - Xnldr v2.0
 *  /AUTHOR  - Manas Kamal Choudhury
 *
 * -----------------------------------------------
 */

#include <Include/VirtualMemory.h>
#include <Include/Xeneva.h>

/**
 * Simple PmmngrAllocate Function derived from
 * Xeneva sys 1.0
 */
uint64_t* next_alloc_page = (uint64_t*)0x10000000;

void* PmmngrAllocate() {

	void* page = (void*)next_alloc_page;
	next_alloc_page += 4096;
	return page;
}



/** Virtual Memory derived from Xnldr v1.0 **/

static void* pdptr = 0;
static void* pml4ptr = 0;
static size_t  recursive_slot = 0;

static_assert(sizeof(size_t) == 8, "X86 Paging Size Mismatch");

typedef size_t PD_ENTRY;
typedef size_t PTAB_ENTRY;
typedef size_t PDPT_ENTRY;
typedef size_t PML4_ENTRY;

#define PAGING_PRESENT 0x1
#define PAGING_WRITABLE 0x2
#define PAGING_NOCACHE  0x10
#define PAGING_XENEVA_NOSWAP 0x200
#define PAGING_SIZEBIT  0x80
#define PAGING_NXE  0x8000000000000000

static void* make_canonical_i(size_t addr)
{
	if (addr & ((size_t)1 << 47))
		addr |= 0xFFFF000000000000;
	return (void*)addr;
}

static void* make_canonical(void* addr)
{
	return make_canonical_i((size_t)addr);
}

static size_t decanonical(void* addr)
{
	return (size_t)addr & 0x0000FFFFFFFFFFFF;
}


typedef size_t*(*get_tab_ptr)(void*);
typedef size_t(*get_tab_index)(void*);

static PML4_ENTRY* getPML4(void* addr)
{
	if (pml4ptr);
	else{
		void* pml4 = GetPagingRoot();
		pml4ptr = (void*)((size_t)pml4 & ~(size_t)0xFFF);
	}
	return (PML4_ENTRY*)pml4ptr;
}

static size_t getPML4index(void* addr)
{
	return (decanonical(addr) >> 39) & 0x1FF;
}

static PD_ENTRY* getPD(void* addr)
{
	return (PD_ENTRY*)make_canonical((void*)((recursive_slot << 39) | (recursive_slot << 30) | ((decanonical(addr) >> 18) & 0x3FFFF000)));
}

static PDPT_ENTRY* getPDPT(void* addr)
{
	return (PDPT_ENTRY*)make_canonical((void*)((recursive_slot << 39) | (recursive_slot << 30) | (recursive_slot << 21) | (((decanonical(addr) >> 27) & 0x1FF000))));
}

static size_t getPDindex(void* addr)
{
	return (decanonical(addr) >> 21) & 0x1FF;
}

static PD_ENTRY* getPDIR(void* addr)
{
	if (pdptr);
	else
	{
		void* pd = GetPagingRoot();
		pdptr = (void*)((size_t)pd & ~(size_t)0xFFF);
	}
	return (PD_ENTRY*)pdptr;
}


static size_t getPDIRindex(void* addr)
{
	return ((size_t)addr >> 21) & 0x3FF;
}

static PTAB_ENTRY* getPTAB(void* addr)
{
	return (PD_ENTRY*)make_canonical((void*)((recursive_slot << 39) | ((decanonical(addr) >> 9) & 0x7FFFFFF000)));
}


static size_t getPTABindex(void* addr) 
{
	return (decanonical(addr) >> 12) & 0x1FF;
}

static size_t getPDPTindex(void* addr)
{
	return (decanonical(addr) >> 30) & 0x1FF;
}


static get_tab_ptr get_tab_dispatch[] = 
{
	nullptr,
	&getPTAB,
	&getPD,
	&getPDPT,
	&getPML4
};

static get_tab_index get_index_dispatch[] = {
	nullptr,
	&getPTABindex,
	&getPDindex,
	&getPDPTindex,
	&getPML4index
};


static PML4_ENTRY* setPML4_recursive(PML4_ENTRY* pml4, size_t slot)
{
	XENEVA_INFO *Info = GetXenevaInfo ();
	recursive_slot = slot;
	pml4[recursive_slot] = (size_t)pml4 | PAGING_PRESENT | PAGING_WRITABLE;
	//now calculate new pml4 address
	void* new_pml4 = make_canonical((void*)((recursive_slot << 39) | (recursive_slot << 30) | (recursive_slot << 21) | (recursive_slot << 12)));
	//flush the tlb
	SetPagingRoot(GetPagingRoot ());
	pml4ptr = new_pml4;
	Info->PML4 = pml4ptr;
	return (PML4_ENTRY*)new_pml4;
}

/**
 * Initialize paging system 
 */
void PagingInitialize (){
	
	PML4_ENTRY* pml4 = getPML4(nullptr);
	//just in case PML4 is read only
	size_t cr0 = ReadCR0();
	WriteCR0(cr0 & (SIZE_MAX-(1<<16)));
    
	for( size_t x = (4096 / sizeof(PML4_ENTRY)); x > 0; --x)
	{
		if ((pml4[x-1] & PAGING_PRESENT) == 0)
		{
			pml4 = setPML4_recursive(pml4, x-1);
			break;
		}
	}
	WriteCR0(cr0);
}

static void clear_ptabs(void* addr)
{

	PTAB_ENTRY* pt = (PTAB_ENTRY*)addr;
	for (size_t n = 0; n < 4096 / sizeof(PTAB_ENTRY); ++n)
	{
		pt[n] = 0;
	}
}

/**
 * Map a physical page to virtual memory
 *
 * @param VirtualAddr -- 64 bit Virtual address input
 * @param PhysicalAddress -- Physical Memory address
 * @param Length -- Length , i.e count of address to map to virtual memory
 * @param Attrib -- Attributes of Newly mapped Page
 */
bool MapPage(void* vaddr, Paddr_t paddr, size_t attributes)
{
	PML4_ENTRY* pml4 = getPML4(vaddr);
	PDPT_ENTRY* pdpt = getPDPT(vaddr);
	PD_ENTRY* pdir = getPD(vaddr);
	PTAB_ENTRY* ptab = getPTAB(vaddr);

	PML4_ENTRY& pml4ent = pml4[getPML4index(vaddr)];
	if ((pml4ent & PAGING_PRESENT) == 0){

		Paddr_t addr = (Paddr_t)PmmngrAllocate();
		if (addr == 0)
		{
			return false;
		}
		pml4ent = addr | PAGING_PRESENT | PAGING_WRITABLE;
		TLBFlush(pdpt);
		MemoryBarrier();
		clear_ptabs(pdpt);
	}
	else if (pml4ent & PAGING_SIZEBIT)
		return false;
	PDPT_ENTRY& pdptent = pdpt[getPDPTindex(vaddr)];
	if ((pdptent & PAGING_PRESENT) == 0)
	{
		Paddr_t addr = (Paddr_t)PmmngrAllocate();
		if (addr == 0)
		{
			return false;
		}
		pdptent = addr | PAGING_PRESENT | PAGING_WRITABLE;
		TLBFlush(pdir);
		MemoryBarrier();
		clear_ptabs(pdir);
	}
	else if (pdptent & PAGING_SIZEBIT)
		return false;
	PD_ENTRY& pdent = pdir[getPDindex(vaddr)];
	if ((pdent & PAGING_PRESENT) == 0)
	{
		Paddr_t addr = (Paddr_t)PmmngrAllocate();
		if (addr == 0)
		{
			return false;
		}
		pdent = addr | PAGING_PRESENT | PAGING_WRITABLE;
		TLBFlush(ptab);
		MemoryBarrier();
		clear_ptabs(ptab);
	}
	else if (pdent & PAGING_SIZEBIT)
		return false;
	PTAB_ENTRY& ptabent = ptab[getPTABindex(vaddr)];
	if ((ptabent & PAGING_PRESENT) != 0)
		return false;
	if (paddr == PADDR_T_MAX)
	{
		if(!(paddr = (Paddr_t)PmmngrAllocate()))
			return false;
	}
	ptabent = (size_t)paddr | PAGING_PRESENT;
	if (attributes & PAGE_ATTRIBUTE_WRITABLE)
		ptabent |= PAGING_WRITABLE;
	TLBFlush(vaddr);
	MemoryBarrier();
	return true;
}


static bool CheckFree(int level, void* start_addr, void* end_addr)
{

	if(level == 0)
		return false;
	size_t* paging_entry = get_tab_dispatch[level](start_addr);
	get_tab_index getindex = get_index_dispatch[level];

	size_t cur_addr = (size_t)start_addr;
	size_t end_index = getindex(end_addr);
	if (get_tab_dispatch[level](end_addr) != get_tab_dispatch[level](start_addr))
		end_index = 0x1FF;
	for (size_t pindex = getindex(start_addr); pindex <= end_index; ++pindex)
	{
		if ((paging_entry[pindex] & PAGING_PRESENT) == 0)
		{
			cur_addr >>= (level * 9 + 3);
			cur_addr += 1;
			cur_addr <<= (level * 9 + 3);
			continue;
		}
		else
		{
			if (!CheckFree((level - 1), (void*)cur_addr, end_addr))
				return false;
		}
	}
	return true;
}

bool CheckFree(void* vaddr, size_t length)
{
	void* endaddr = raw_offset<void*>(vaddr, length-1);
	return CheckFree(4, vaddr, endaddr);
}


bool MapPage(void* vaddr, Paddr_t paddr, size_t length, size_t attributes){

	if (!CheckFree(vaddr, length))
		return false;
	size_t vptr = (size_t)vaddr;
	size_t pgoffset = vptr & (4096 - 1);
	vaddr = (void*)(vptr ^ pgoffset);
	length += pgoffset;
	if (paddr != PADDR_T_MAX)
	{
		if ((paddr & (4096 - 1)) != pgoffset)
			return false;
		paddr ^= pgoffset;
	}

	for (size_t i = 0; i < (length + 4096 - 1) / 4096; ++i){

		if (!MapPage(vaddr, paddr, attributes))
			return false;

		vaddr = raw_offset<void*>(vaddr, 4096);
		if(paddr != PADDR_T_MAX)
			paddr = raw_offset<Paddr_t>(paddr, 4096);
	}
	return true;
}


void SetAttributes(void* vaddr, size_t length, size_t attrset, size_t attrclear)
{

	PTAB_ENTRY* ptab = getPTAB(vaddr);
	for (size_t index = getPTABindex(vaddr); index < (length + 4096 - 1) / 4096; ++index)
	{
		if ((attrset & PAGE_ATTRIBUTE_WRITABLE) != 0)
		{
			ptab[index] |= PAGING_WRITABLE;
		}

		if ((attrclear & PAGE_ATTRIBUTE_WRITABLE) != 0)
		{
			ptab[index] &= ~((size_t)PAGING_WRITABLE);
		}
		if ((attrset & PAGE_ATTRIBUTE_NO_EXEC) != 0)
		{
			ptab[index] = PAGING_NXE;
		}
		if ((attrclear & PAGE_ATTRIBUTE_NO_EXEC) != 0)
		{
			ptab[index] &= ~((size_t)PAGING_NXE);
		}

		if ((attrset & PAGE_ATTRIBUTE_NO_CACHING) != 0)
		{
			ptab[index] |= PAGING_NOCACHE;
		}
		if ((attrclear & PAGE_ATTRIBUTE_NO_CACHING) != 0)
		{
			ptab[index] &= ~((size_t)PAGING_NOCACHE);
		}
		if ((attrset & PAGE_ATTRIBUTE_NO_PAGING) != 0)
		{
			ptab[index] |= PAGING_XENEVA_NOSWAP;
		}
		if ((attrclear & PAGE_ATTRIBUTE_NO_PAGING) != 0)
		{
			ptab[index] &= ~((size_t)PAGING_XENEVA_NOSWAP);
		}
	}
}


static struct {
	size_t recursive_slot;
	void* pml4ptr;
}paging_info;


size_t arch_get_recursive_slot(){
	return recursive_slot;
}

void* arch_get_pml4ptr(){
	return pml4ptr;
}





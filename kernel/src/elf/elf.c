#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>
#include <stdlib.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
void ramdisk_write(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */




	uint16_t real_phnum=elf->e_phnum;
	//if bigger or equal XNUM 
	//	real number of entries is held in the sh_info member of the initial enrty 
	//	in section  header table  man 5 elf

	if(elf->e_phnum==0xffffU)
	{
		Elf32_Shdr *initial_entry=(void *)elf+elf->e_shoff;
		real_phnum=initial_entry->sh_info;
	}
	

	///*load ph*/


	int i=0;
	for(;i<real_phnum;i++) {
		/* Scan the program header table, load each segment into memory */
		ph=(void*)(buf+elf->e_phoff+i*elf->e_phentsize);
		if(ph->p_type == PT_LOAD) {

			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */

			 Elf32_Off Offset=ph->p_offset;
#ifdef IA32_PAGE
				Elf32_Addr VirtAddr =mm_malloc(ph->p_vaddr,ph->p_memsz);
#else
				Elf32_Addr VirtAddr =ph->p_vaddr;
#endif
			/*old value: VirtAddr=ph->p_vaddr*/

			 int FileSiz=ph->p_filesz;
			 int MemSize=ph->p_memsz;
#ifndef HAS_DEVICE
			 ramdisk_read((void*)(ELF_OFFSET_IN_DISK+VirtAddr),Offset,FileSiz);
#else
			 // set_bp();
			 ide_read((void*)(ELF_OFFSET_IN_DISK+VirtAddr),Offset,FileSiz);
			 // set_bp();
#endif
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
			 //WARNING:only >0 need zero memery
			 //assert(MemSize-FileSiz>=0);
			 if(MemSize-FileSiz>0)
			 {
				 memset((void *)(ELF_OFFSET_IN_DISK+VirtAddr+FileSiz),0,MemSize-FileSiz);//BUG?
			 }

#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }	
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;
#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);


#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}

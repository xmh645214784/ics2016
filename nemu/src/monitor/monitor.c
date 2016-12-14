#include "nemu.h"
#include "memory/cache.h"
#define ENTRY_START 0x100000

extern uint8_t entry [];
extern uint32_t entry_len;
extern char *exec_file;
extern CPU_state cpu;

void load_elf_tables(int, char *[]);
void init_regex();
void init_wp_pool();
void init_ddr3();

FILE *log_fp = NULL;

static void init_log() {
	log_fp = fopen("log.txt", "w");
	Assert(log_fp, "Can not open 'log.txt'");
}

static void welcome() {
	printf("Welcome to NEMU!\nThe executable is %s.\nFor help, type \"help\"\n",
			exec_file);
}

void init_monitor(int argc, char *argv[]) {
	/* Perform some global initialization */

	/* Open the log file. */
	init_log();

	/* Load the string table and symbol table from the ELF file for future use. */
	load_elf_tables(argc, argv);

	/* Compile the regular expressions. */
	init_regex();

	/* Initialize the watchpoint pool. */
	init_wp_pool();

	/* Display welcome message. */
	welcome();

	/* For outside device*/
extern void init_device() ;
extern void init_sdl();
	init_device();
	init_sdl();
}

#ifdef USE_RAMDISK
static void init_ramdisk() {
	int ret;
	const int ramdisk_max_size = 0xa0000;
	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	Assert(file_size < ramdisk_max_size, "file size(%zd) too large", file_size);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(0), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}
#endif

static void load_entry() {
	int ret;
	FILE *fp = fopen("entry", "rb");
	Assert(fp, "Can not open 'entry'");

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	ret = fread(hwa_to_va(ENTRY_START), file_size, 1, fp);
	assert(ret == 1);
	fclose(fp);
}

void restart() {
	/* Perform some initialization to restart a program */

	/* Initialize EFLAGS. */
	cpu.EFLAGS=0x00000002;


#ifdef USE_RAMDISK
	/* Read the file with name `argv[1]' into ramdisk. */
	init_ramdisk();
#endif

	/* Read the entry code into memory. */
	load_entry();

	/* Set the initial instruction pointer. */
	cpu.eip = ENTRY_START;

	/* Initialize DRAM. */
	init_ddr3();

	/* Initialize CACHE. */
	extern void init_L1Cache();
	init_L1Cache();
	extern void init_L2Cache();
	init_L2Cache();

	/*initialize CR0*/
	cpu.cr0.protect_enable=0;
	cpu.cr0.paging=0;


	/*initialize CS.descache*/
	SegDesc temp;
	temp.limit_15_0=0xffff;
	temp.limit_19_16=0xf;
	temp.base_15_0=0;
	temp.base_23_16=0;
	temp.base_31_24=0;
	temp.granularity=1;
	cpu.cs.segdesc_cache=temp;
#ifdef TLB
	/*initialize TLB*/
	extern void init_TLB();
	init_TLB();
#endif
}

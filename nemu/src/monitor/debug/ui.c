#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "memory/memory.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
extern CPU_state cpu;

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}
static int cmd_help(char *args);


static int cmd_info(char *args){
	char *p=strtok(NULL," ");
	//显示寄存器
	if(strcmp(p,"r")==0)
	{
		printf("eax\t0x%x\t%d\n",cpu.eax,cpu.eax);
		printf("ecx\t0x%x\t%d\n",cpu.ecx,cpu.ecx);
		printf("edx\t0x%x\t%d\n",cpu.edx,cpu.edx);
		printf("ebx\t0x%x\t%d\n",cpu.ebx,cpu.ebx);
		printf("esp\t0x%x\t%d\n",cpu.esp,cpu.esp);
		printf("ebp\t0x%x\t%d\n",cpu.ebp,cpu.ebp);
		printf("esi\t0x%x\t%d\n",cpu.esi,cpu.esi);
		printf("edi\t0x%x\t%d\n",cpu.edi,cpu.edi);
		printf("eip\t0x%x\t%d\n",cpu.eip,cpu.eip);
		printf("[EFLAGS]: ");
		if(cpu.CF)
			printf("CF ");
		if(cpu.PF)
			printf("PF ");
		if(cpu.ZF)
			printf("ZF ");
		if(cpu.SF)
			printf("SF ");
		if(cpu.OF)
			printf("OF ");
		printf("\n");
	}

	//打印监视点信息
	if(strcmp(p,"w")==0||strcmp(p,"watchpoint")==0)
	{
		WP *head=returnhead();
		if(!head)

		{	
			printf("NO Watchpoint\n");
			return 0;
		}
		char *q=strtok(NULL," ");
		if(q==NULL)
			for(;head!=NULL;head=head->next)
			{
				printf("Num\tType\t\tDisp\tEnb\tWhat\n");
				printf("%d\twatchpoint\tkeep\t%c\t%s\n",head->NO,head->enable,head->expr );
			}
		else
		{
			for(;head!=NULL;head=head->next)
				if(head->NO==atoi(q))
				{
					printf("Num\tType\t\tDisp\tEnb\tWhat\n");
					printf("%d\twatchpoint\tkeep\t%c\t%s\n",head->NO,head->enable,head->expr );
				}
		}
	}
	return 0;
}


static int cmd_enable(char *args){
	char *p=strtok(NULL," ");
	p=strtok(NULL," ");
	WP *head=returnhead();
		if(!head)
		{	
			printf("NO Watchpoint\n");
			return 0;
		}
	while(p!=NULL)
	{
		for(head=returnhead();head!=NULL;head=head->next)
		if(head->NO==atoi(p))
		{
			head->enable='y';
		}
	p=strtok(NULL," ");
	}
	return 0;
}

static int cmd_disable(char *args){
	char *p=strtok(NULL," ");
	p=strtok(NULL," ");
	WP *head=returnhead();
		if(!head)
		{	
			printf("NO Watchpoint\n");
			return 0;
		}
	while(p!=NULL)
	{
		for(head=returnhead();head!=NULL;head=head->next)
		if(head->NO==atoi(p))
		{
			head->enable='n';
		}
		p=strtok(NULL," ");
	}
	return 0;
}


static int cmd_si(char *args)
{
	char *p=strtok(NULL," ");
	if(p)
	{
		cpu_exec(atoi(p));
	}
	else//无参数 缺省为1
		cpu_exec(1);
	return 0;
}

static int cmd_x(char *args)
{
	char *num=strtok(NULL," ");
	char *e=strtok(NULL," ");
	int num_int=atoi(num);
	bool success =1;
	int exp=expr(e,&success);
	
	if(!success)
		return 0;

	//assert(success);
	printf("首地址为:0x%08x\n",exp );
	int i=0;
	for(;i<num_int;i++)
	{	
		printf("0x%08x  ",swaddr_read(exp+4*i,4));
	}
	printf("\n");
	return 0;
}

static int cmd_p(char *args)
{
	bool success=1;
	char *e=strtok(NULL," ");
	printf("%d",expr(e,&success));
	printf("\n");

	if(!success)
		return 0;
	return 0;
}
static int cmd_px(char *args)
{
	bool success=1;
	char *e=strtok(NULL," ");
	printf("0x%08x",expr(e,&success));
	printf("\n");
	if(!success)
		return 0;
	return 0;
}
static int cmd_w(char *args)
{
	char *e=strtok(NULL," ");
	bool success=1;
	int oldvalue=expr(e,&success);
	if(success)
	{
		WP* w=new_wp();
		if(w==NULL)
			return  0;
		strcpy(w->expr,e);
		w->oldvalue=oldvalue;
		printf("Watchpoint %d:%s\n",w->NO,e);
	}
	else
		printf("Watchpoint set failed\n");
	return 0;
}
static int cmd_d(char *args)
{
	char *e=strtok(NULL," ");
	while(e!=NULL)
	{
		free_wp(atoi(e));
		e=strtok(NULL," ");
	}
	return 0;
}


static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },

	/* TODO: Add more commands */
	{"info","print program informations",cmd_info},
	{"i","print program informations",cmd_info},
	{"si","execute each step",cmd_si},
	{"x","print the memory",cmd_x},
	{"p","print",cmd_p},
	{"w","set watchpoint",cmd_w},
	{"d","delete",cmd_d},

	{"delete","delete",cmd_d},
	{"p/x","print as hex",cmd_px},
	{"enable","enable watchpoint",cmd_enable},
	{"disable","disable watchpoint",cmd_disable}

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}

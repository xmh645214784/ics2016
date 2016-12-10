#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/*Symbol table*/
 extern char *strtab;
 extern Elf32_Sym *symtab ; 
 extern int nr_symtab_entry;


uint32_t eval(int p,int q,bool *success);

#define maxtokens 32

extern CPU_state cpu;

enum {
	NOTYPE = 256, EQ,/* TODO: Add more token types */
	NUM,HEXNUM,NEG,DEREF,NEQ,AND,OR,NOT,REG,SYMBOL
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal

	//我加入的
	{"-",'-'},
	{"\\*",'*'},
	{"/",'/'},
	{"\\(",'('},
	{"\\)",')'},

	{"0x[a-f0-9A-F]+",HEXNUM},//调换顺序 优先匹配0x
	{"[0-9]+",NUM},

	{"!=",NEQ},
	{"&&",AND},
	{"\\|\\|",OR},
	{"!",NOT},
	{"\\$[a-zA-Z]+",REG},
	{"\\w+",SYMBOL}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[maxtokens];
} Token;

Token tokens[maxtokens];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i=0;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				//记录类型
				 tokens[nr_token].type=rules[i].token_type;

				 //拷贝字符串
				 int sublencount=0;
				 for(;sublencount<substr_len;sublencount++)
					 tokens[nr_token].str[sublencount]=*(substr_start+sublencount);
				tokens[nr_token].str[substr_len]='\0';

				//nr_token自加
				nr_token++;
				assert(nr_token<=maxtokens);
				break;
				}
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	return true; 
}

uint32_t expr(char *e, bool *success) 
{
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	//*success=true;
	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	int i;
	for(i = 0; i < nr_token; i ++) {
		if(tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type !=NUM&&tokens[i - 1].type !=REG&&tokens[i-1].type!=HEXNUM) )&&/*tokens[i-1].type!='('&&*/tokens[i-1].type!=')') {
			tokens[i].type = DEREF;
		}
		if(tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type !=NUM&&tokens[i - 1].type !=REG&&tokens[i-1].type!=HEXNUM) )&&/*tokens[i-1].type!='('&&*/tokens[i-1].type!=')') {
			tokens[i].type = NEG;	
		}
	}
	return eval(0,nr_token-1,success);
}

bool check_parentheses(int p,int q)
{
	/*得到字符串，从头开始，一个一个字符判断。

（1） 如果是左括号则压入栈中。

（2） 如果是右括号，则将一个左括号从栈中弹出，匹配一对。若此时栈中无元素，那么这个右括号没有匹配的左括号。

最后，如果栈不为空，则有左括号没有匹配。
*/
	if(tokens[p].type!='('||tokens[q].type!=')')
		return false;
	int numofzuokuohao=0;
	int i=p+1;
	for(;i<=q-1;i++)
	{
		if(tokens[i].type=='(')
			numofzuokuohao++;
		if(tokens[i].type==')')
		{
			if(numofzuokuohao==0)
				return false;
			numofzuokuohao--;
		}

	}
	if(numofzuokuohao!=0)
		return false;

	return true;
}

//position在一对匹配的括号中
bool zaiyiduipipeidekuohaozhong(int p,int q,int position)
{
	int i=position;
	int j;
	int a=-1;
	int b=-1;
	for(;i>=p;i--)
	{
		if(tokens[i].type=='(')
		{
			a=i;
		
		for(j=position;j<=q;j++)
			if(tokens[j].type==')')
			{
				b=j;
				if(check_parentheses(a,b)==true)
					{
						return true;
					}
			}
		}
	}
	return false;
}

int getpriority(int fuhao)
{
	if(fuhao==DEREF||fuhao==NEG||fuhao==NOT)
		return 6;
	if(fuhao=='*'||fuhao=='/')
		return 5;
	if(fuhao=='+'||fuhao=='-')
		return 4;
	if(fuhao==EQ||fuhao==NEQ)
		return 3;
	if(fuhao==AND)
		return 2;
	if(fuhao==OR)
		return 1;
	return 0;
}

int findthedominantoperatorposition(int p,int q,bool *success)
{
	bool a[maxtokens];
	int i=0;//从p开始
	for(;i<=q-p;i++)
	{	
		if(getpriority(tokens[p+i].type)==0)//非运算符
		{
			a[i]=0;
		}
		else if(zaiyiduipipeidekuohaozhong(p, q, p+i))
		{
			a[i]=0;
		}
		else
		{
			a[i]=1;
		}
	}
	int flag=-1;

	//int danmu=-1;//单目运算符用 标记第一个可能的dominant运算符

	for(i=0;i<=q-p;i++)
	{
		if(a[i]==1)
		{	if(flag==-1)
			{
				flag=i;
				//danmu=i;
			}
			else if(getpriority(tokens[p+i].type)<=getpriority(tokens[p+flag].type))
			{
				flag=i;
			}
		}
	}

		assert(flag+p>=0);
		//Log("dominate在%d",flag+p);
		return flag+p;
}


uint32_t eval(int p,int q,bool *success)
{
	if(*success==0)
	{
		Log("failed in evaluate expr");
		return 0;
	}
	if(p > q) {
		/* Bad expression */
		*success=0;
		return 0;
	}
	else if(p == q) { 
		/* Single token.
		 * For now this token should be a number. 
		 * Return the value of the number.
		 */ 
		if(tokens[p].type==NUM)
			return atoi(tokens[p].str);
		if(tokens[p].type==HEXNUM)
		{	
			return strtol(tokens[p].str,NULL, 16);
		}
		if(tokens[p].type==REG)
		{
			if(strcasecmp(tokens[p].str,"$eax")==0)
				return cpu.eax;
			if(strcasecmp(tokens[p].str,"$edx")==0)
				return cpu.edx;
			if(strcasecmp(tokens[p].str,"$ecx")==0)
				return cpu.ecx;
			if(strcasecmp(tokens[p].str,"$ebx")==0)
				return cpu.ebx;
			if(strcasecmp(tokens[p].str,"$ebp")==0)
				return cpu.ebp;
			if(strcasecmp(tokens[p].str,"$esi")==0)
				return cpu.esi;
			if(strcasecmp(tokens[p].str,"$edi")==0)
				return cpu.edi;
			if(strcasecmp(tokens[p].str,"$esp")==0)
				return cpu.esp;
			if(strcasecmp(tokens[p].str,"$eip")==0)
				return cpu.eip;

			if(strcasecmp(tokens[p].str,"$ax")==0)
				return cpu.ax;
			if(strcasecmp(tokens[p].str,"$dx")==0)
				return cpu.dx;
			if(strcasecmp(tokens[p].str,"$cx")==0)
				return cpu.cx;
			if(strcasecmp(tokens[p].str,"$bx")==0)
				return cpu.bx;
			if(strcasecmp(tokens[p].str,"$bp")==0)
				return cpu.bp;
			if(strcasecmp(tokens[p].str,"$si")==0)
				return cpu.si;
			if(strcasecmp(tokens[p].str,"$di")==0)
				return cpu.di;
			if(strcasecmp(tokens[p].str,"$sp")==0)
				return cpu.sp;

			if(strcasecmp(tokens[p].str,"$al")==0)
				return cpu.al;
			if(strcasecmp(tokens[p].str,"$dl")==0)
				return cpu.dl;
			if(strcasecmp(tokens[p].str,"$cl")==0)
				return cpu.cl;
			if(strcasecmp(tokens[p].str,"$bl")==0)
				return cpu.bl;
			if(strcasecmp(tokens[p].str,"$ah")==0)
				return cpu.ah;
			if(strcasecmp(tokens[p].str,"$dh")==0)
				return cpu.dh;
			if(strcasecmp(tokens[p].str,"$ch")==0)
				return cpu.ch;
			if(strcasecmp(tokens[p].str,"$bh")==0)
				return cpu.bh;

			if(strcasecmp(tokens[p].str,"$idtr")==0)
				return cpu.idtr;

			if(strcasecmp(tokens[p].str,"$inttrap")==0)
				return lnaddr_read(cpu.idtr + 128 * 8,4);

		}
		if(tokens[p].type==SYMBOL)
		{
			int i=0;
			for(;i<nr_symtab_entry;i++)
			{
				//Log("symbol[%d] st_info is %d ",i,symtab[i].st_info);
				if(ELF32_ST_TYPE(symtab[i].st_info)==STT_OBJECT)
				{
					char *symbolStrName=strtab+symtab[i].st_name;
					//Log("%s",symbolStrName);
					if(strcmp(tokens[p].str,symbolStrName)==0)
					{
						//Log("size=%d",symtab[i].st_size);
						return symtab[i].st_value;
					}
				}
			}
		}
		if(tokens[p].type==NEG||tokens[p].type==NOT||tokens[p].type==DEREF)//单目运算符
			return 0;

			Log("make out the tokens' value failed\n");
			*success=0;
			return -1;
		//panic("解析数时发生错误\n");

	}
	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses. 
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1,success); 
	}
	else {
		int op = findthedominantoperatorposition(p, q,success);
		uint32_t val1 = 0;
		if(p<=op-1)//为单目运算符上的保险 由于不用算单目dominant运算符之前的东西
			val1=eval(p, op - 1,success);
		uint32_t val2 = eval(op + 1, q,success);
		//Log("%x %d",val2,val2);
		switch(tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case EQ : return val1 ==val2;
			case NEQ: return val1 !=val2;
			case AND: return val1 &&val2;
			case OR : return val1 ||val2;
			case NOT: return !val2;
			case NEG: return -val2;
			case DEREF:return swaddr_read(val2,4,SR_DS);
			default: return 0; //assert(0);
		}
	}
}

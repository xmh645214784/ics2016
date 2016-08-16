#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
uint32_t eval(int p,int q);

extern CPU_state cpu;

enum {
	NOTYPE = 256, EQ,/* TODO: Add more token types */
	NUM,HEXNUM,NEG,DEREF,NEQ,AND,OR,NOT,REG
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

	{"0x[0-9]+",HEXNUM},//调换顺序 优先匹配0x
	{"[0-9]+",NUM},

	{"!=",NEQ},
	{"&&",AND},
	{"\\|\\|",OR},
	{"!",NOT},
	{"\\$[a-z]+",REG}
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
	char str[32];
} Token;

Token tokens[32];
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

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
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
	//Log("match successfully");
	*success=true;
	/* TODO: Insert codes to evaluate the expression. */
	//panic("please implement me");
	int i;
	for(i = 0; i < nr_token; i ++) {
		if(tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type !=NUM&&tokens[i-1].type!=HEXNUM) )&&tokens[i-1].type!='('&&tokens[i-1].type!=')') {
			tokens[i].type = DEREF;
		}
		if(tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type !=NUM&&tokens[i-1].type!=HEXNUM) )&&tokens[i-1].type!='('&&tokens[i-1].type!=')') {
			tokens[i].type = NEG;	
		}
	}
	return eval(0,nr_token-1);
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
	int i=p;
	for(;i<=q;i++)
	{
		if(tokens[i].type=='(')
			numofzuokuohao++;
		if(tokens[i].type==')')
			numofzuokuohao--;
		if(numofzuokuohao<0)
			return false;
	}
	if(numofzuokuohao!=0)
		return false;

	return true;
}

//position在一对匹配的括号中
bool zaiyiduipipeidekuohaozhong(int p,int q,int position)
{
	int i=position;
	int a=-1;
	int b=-1;
	for(;i>=p;i--)
		if(tokens[i].type=='(')
			{
				a=i;
				break;
			}
	for(i=position;i<=q;i++)
		if(tokens[i].type==')')
			{
				b=i;
				break;
			}
	return a!=-1&&b!=-1&&check_parentheses(a,b);
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

int findthedominantoperatorposition(int p,int q)
{
	bool a[32];
	int i=0;//从p开始
	for(;i<=q-p;i++)
	{	
		if(getpriority(tokens[p+i].type)==0)//非运算符
			a[i]=0;
		else if(zaiyiduipipeidekuohaozhong(p, q, p+i))
			a[i]=0;
		else
			a[i]=1;
	}
	int flag=-1;

	int danmu=-1;//单目运算符用 标记第一个可能的dominant运算符

	for(i=0;i<=q-p;i++)
	{
		if(a[i]==1)
		{	if(flag==-1)
			{
				flag=i;
				danmu=i;
			}
			else if(getpriority(tokens[p+i].type)<=getpriority(tokens[p+flag].type))
			{
				flag=i;
			}
		}
	}


	
	//如果是单目运算符 为统治运算符 顺序应该为第一个
	if(getpriority(tokens[flag+p].type)==6)
	{
		#ifdef DEBUG
		Log("dominate在%d",danmu+p);
		#endif
		assert(danmu+p>=0);
		return danmu+p;
	}
	else
	{
		#ifdef DEBUG
		Log("dominate在%d",flag+p);
		#endif
		assert(flag+p>=0);
		return flag+p;
	}
}


uint32_t eval(int p,int q)
{
	if(p > q) {
		/* Bad expression */
		panic("p>q");
	}
	else if(p == q) { 
		/* Single token.
		 * For now this token should be a number. 
		 * Return the value of the number.
		 */ 
		if(tokens[p].type==NUM)
			return atoi(tokens[p].str);
		if(tokens[p].type==HEXNUM)
			return strtol(tokens[p].str,NULL, 16);
		if(tokens[p].type==REG)
		{
			if(strcmp(tokens[p].str,"$eax")==0)
				return cpu.eax;
			if(strcmp(tokens[p].str,"$edx")==0)
				return cpu.edx;
			if(strcmp(tokens[p].str,"$ecx")==0)
				return cpu.ecx;
			if(strcmp(tokens[p].str,"$ebx")==0)
				return cpu.ebx;
			if(strcmp(tokens[p].str,"$ebp")==0)
				return cpu.ebp;
			if(strcmp(tokens[p].str,"$esi")==0)
				return cpu.esi;
			if(strcmp(tokens[p].str,"$edi")==0)
				return cpu.edi;
			if(strcmp(tokens[p].str,"$esp")==0)
				return cpu.esp;

			if(strcmp(tokens[p].str,"$ax")==0)
				return cpu.ax;
			if(strcmp(tokens[p].str,"$dx")==0)
				return cpu.dx;
			if(strcmp(tokens[p].str,"$cx")==0)
				return cpu.cx;
			if(strcmp(tokens[p].str,"$bx")==0)
				return cpu.bx;
			if(strcmp(tokens[p].str,"$bp")==0)
				return cpu.bp;
			if(strcmp(tokens[p].str,"$si")==0)
				return cpu.si;
			if(strcmp(tokens[p].str,"$di")==0)
				return cpu.di;
			if(strcmp(tokens[p].str,"$sp")==0)
				return cpu.sp;

			if(strcmp(tokens[p].str,"$al")==0)
				return cpu.al;
			if(strcmp(tokens[p].str,"$dl")==0)
				return cpu.dl;
			if(strcmp(tokens[p].str,"$cl")==0)
				return cpu.cl;
			if(strcmp(tokens[p].str,"$bl")==0)
				return cpu.bl;
			if(strcmp(tokens[p].str,"$ah")==0)
				return cpu.ah;
			if(strcmp(tokens[p].str,"$dh")==0)
				return cpu.dh;
			if(strcmp(tokens[p].str,"$ch")==0)
				return cpu.ch;
			if(strcmp(tokens[p].str,"$bh")==0)
				return cpu.bh;
			assert(0);
		}
		if(tokens[p].type==NEG||tokens[p].type==NOT||tokens[p].type==DEREF)//单目运算符
			return 0;
		panic("解析数时发生错误\n");

	}
	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses. 
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1); 
	}
	else {
		int op = findthedominantoperatorposition(p, q);
		uint32_t val1 = 0;
		if(p<=op-1)
			val1=eval(p, op - 1); //为单目运算符上的保险 由于不用算单目dominant运算符之前的东西
		uint32_t val2 = eval(op + 1, q);

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
			case DEREF:return swaddr_read(val2,4);
			default: assert(0);
		}
	}
}
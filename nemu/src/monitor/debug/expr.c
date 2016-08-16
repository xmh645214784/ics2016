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
	NUM
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

	{"[0-9]+",NUM},

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
	for(;i>p;i--)
		if(tokens[i].type=='(')
			{
				a=i;
				break;
			}
	for(i=position;i<q;i++)
		if(tokens[i].type==')')
			{
				b=i;
				break;
			}
	return a!=-1&&b!=-1&&check_parentheses(a,b);
}

int getpriority(int fuhao)
{
	if(fuhao=='*'||fuhao=='/')
		return 5;
	if(fuhao=='+'||fuhao=='-')
		return 4;
	assert(0);
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


	for(i=0;i<=q-p;i++)
	{
		if(a[i]==1)
		{	if(flag==-1)
			{
				flag=i;
			}
			else if(getpriority(tokens[p+i].type)<=getpriority(tokens[p+flag].type))
			{
				flag=i;
			}
		}
	}
	Log("dominate在%d",flag+p);
		return flag+p;
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
			default: assert(0);
		}
	}
}
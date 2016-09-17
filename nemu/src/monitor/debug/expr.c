#include "nemu.h"
#include <stdio.h>
#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, num

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
    {"[0-9]+",num},                 // integers
    {"\\(", '('},                   // 左括号
    {"\\)", ')'},                   // 右括号
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},                   // plus
    {"\\-", '-'},
    {"\\*", '*'},
    {"\\/", '/'},
	{"==", EQ}						// equal
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
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
                printf("i=%d\n",i);
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
                
				 */
                int ppp=position-substr_len;
                int aaa=0;
                for(aaa=0;aaa<32;aaa++){
                    tokens[nr_token].str[aaa]='\0';
                }
                for(aaa=0;aaa<substr_len;aaa++){
                    tokens[nr_token].str[aaa]=e[ppp];
                    ppp++;
                }//复制字符串
                printf("type:%d\n",rules[i].token_type);
				switch(rules[i].token_type) {
                    case EQ:{
                        tokens[nr_token].type=EQ;
                    };
                    case num:{
                        tokens[nr_token].type=num;
                    };
                        
                    case '(':{
                        tokens[nr_token].type='(';
                    };
                    case ')':{
                        tokens[nr_token].type=')';
                            
                    };
                    case '+':{
                        tokens[nr_token].type='+';
                    };
                    case '-':{
                        tokens[nr_token].type='-';
                    };
                    case '*':{
                        tokens[nr_token].type='*';
                    };
                    case '/':{
                        tokens[nr_token].type='/';
                        
					default: panic("please implement me");
                    };

				}
                printf("token_type:%d\n",tokens[nr_token].type);
                nr_token++;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

bool check_parentheses(int p,int q){
    if(tokens[p].type!='('||tokens[q].type!=')'){
        return false;
    }
    int count=0;
    int flag=p;
    for(;flag<=q;flag++){
        if(tokens[flag].type=='('){
            count++;
        }
        if(tokens[flag].type==')'){
            if(count<=0) return false;
            count--;
        }
    }
    if(count!=0){
        return false;
    }
    return true;
}//判断tokens[p]和tokens[q]是否匹配在一对括号中



uint32_t eval(int p,int q){
    if(p > q) {
        assert(0);
                /* Bad expression */
            
    }
        else if(p == q) {
            return atoi(tokens[p].str);
                /* Single token.
            *          * For now this token should be a number. 
            *                   * Return the value of the number.
            *                            */ 
            }
    else if(check_parentheses(p, q) == true) {
                /* The expression is surrounded by a matched pair of parentheses. 
        *          * If that is the case, just throw away the parentheses.
        *                   */
                return eval(p + 1, q - 1); 
            
    }
    else {
            int op=0;
            int i;
            int judge_dom_op[32][3];//记录可能的dom op下标 运算符 优先级
            for(i=0;i<32;i++){
                judge_dom_op[i][0]=i;
                judge_dom_op[i][1]=tokens[i].type;
                judge_dom_op[i][2]=-1;
            }//初始化 第一列：下标 第二列：优先级 如果不可能是dom op就依然为-1
            i=0;
            while(i<nr_token){
                if(tokens[i].type!=num){
                    if(tokens[i].type=='('){
                        int j;
                        for(j=i+1;j<32;j++){
                            if(check_parentheses(i,j)==true)
                                break;
                        }
                        i=j+1;
                    }//碰到括号就往后跳过
                    else{
                        switch(tokens[i].type){
                            case '+':judge_dom_op[i][2]=1;
                            case '-':judge_dom_op[i][2]=1;
                            case '*':judge_dom_op[i][2]=2;
                            case '/':judge_dom_op[i][2]=2;
                        }
                        i++;
                    }//是加减乘除 可能的dominant operator
                }
                else{
                     i++;
                }//是数
            }
            bool jiajian=false;
            for(i=0;i<32;i++){
                if(tokens[i].type=='+'||tokens[i].type=='-'){
                    jiajian=true;
                    break;
                }
            }//确定是否有加减
            int find;
            if(jiajian){
                for(find=31;find>=0;find--){
                    if(judge_dom_op[find][2]==1){
                        op=find;
                        break;
                    }               
                }    
            }
            else{
                for(find=31;find>=0;find--){
                    if(judge_dom_op[find][2]==2){
                        op=find;
                        break;
                    }
                }    
            }
            int val1 = eval(p, op - 1);
            int val2 = eval(op + 1, q);

        switch(tokens[op].type) {
                        case '+': return val1 + val2;
                        case '-': return val1 - val2;/* ... */
                        case '*': return val1 * val2;/* ... */
                        case '/': return val1 / val2;/* ... */
                        default: assert(0);
        }
    }
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
    }
    else{
        *success = true;
        return eval(0,strlen(e)-1);
    }

	/* TODO: Insert codes to evaluate the expression. */
//	panic("please implement me");
//	return 0;
}


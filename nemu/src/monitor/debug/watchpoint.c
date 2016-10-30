#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
		wp_pool[i].isfirstfigure=1;//默认未求值 
		wp_pool[i].enable='y';//默认使能
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp()
{
	if(free_==NULL)
	{
		Log("no free watchpoint can be set");
		return NULL;
	}//无节点可以申请
	if(head==NULL)
	{
		head=free_;
		free_=free_->next;
		head->next=NULL;
		return head;
	}
	else
	{
		WP * i=head;
		for(;i->next!=NULL;i=i->next)
		{
			;
		}
		//结束时i指向尾巴
		
		//插入节点
		i->next=free_;
		//更新free 和head链表尾巴的下一个节点
		free_=free_->next;
		i->next->next=NULL;
		return i->next;
	}

}

void  free_wp(int id)
{
	if(head==NULL)
	{
		printf("NO such watchpoint\n");
		return ;
	}


	WP *headtemp=head;
	for(;headtemp!=NULL&&headtemp->NO!=id;headtemp=headtemp->next)
		;
	if(headtemp==NULL)
	{
		printf("NO such watchpoint\n");
		return ;
	}
	WP *wp=headtemp;//找到要删除的节点是wp



	WP *search=head;
	if(head==wp)//恰好在第一个点
	{

		WP *aa=free_;//接free
		if(aa==NULL)
		{
			free_=wp;
			free_->next=NULL;
		}
		for(;aa->next!=NULL;aa=aa->next)
			;//结束时aa指向尾巴
		aa->next=head;

//断head的头
		head=head->next;


		aa->next->next=NULL;//尾巴处理下
	}
	else
	{
		while(search->next!=wp)
		{
			search=search->next;
		}
		//结束时search的next指向wp
		search->next=search->next->next;
				WP *aa=free_;//接free
		if(aa==NULL)
		{
			free_=wp;
			free_->next=NULL;
		}
		for(;aa->next!=NULL;aa=aa->next)
			;//结束时aa指向尾巴
		aa->next=wp;
		aa->next->next=NULL;//尾巴处理下
	}
}

WP *returnhead()
{
	return head;
}
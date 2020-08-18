#include<stdlib.h>
#include<ncurses.h>
#include<string.h>
#include<limits.h>
#include "1_header.h"

void initialize(char num[])
{
int i, length = strlen(num);
	for(i=0; i <length; i++)
	{
	num[i] = ' ';
	}
}

struct node* get_polynomial(struct node* pol)
{
struct node *new_node, *next_node, *old;
int degree, last_degree, y, x, i, len=0, j=0, br_open, comma_sep;
float coeff;
char ch[100], num[100];
again:

br_open=0; comma_sep=0;
getyx(stdscr, y, x);
last_degree = INT_MAX;
x=15;
i=0;
scanw("%s",ch);
len = strlen(ch);
	for(i=0; i<len; i++)
	{
		if(ch[i] == '(')
		{
		br_open = 1;
		continue;
		}
		
		if(ch[i] == ')' && br_open && comma_sep)
		{
		new_node = (struct node*)malloc(sizeof(struct node));
		new_node->degree = degree;
		new_node->coeff  = atof(num);
		new_node->next = NULL;

			if(degree >= last_degree || new_node->coeff == 0)
			{
				if(pol != NULL) 
				{
					while(pol->next != NULL)
					{
					next_node = pol->next;
					pol->next = pol->next->next;
					free(next_node);
					}
					
				pol = NULL;
				}
			
			clear();
			printw("Polynomial not in required format, please enter again!\n");
			goto again;
			}

			if(pol == NULL)
			{
			new_node->prev = NULL;
			pol = new_node;
			}
		
			else
			{
			old = pol;
				while(old->next != NULL)
				{
				old = old->next;
				}
			old->next = new_node;
			new_node->prev = old;
			}
		
		last_degree = new_node->degree;
		br_open = 0;
		j=0;
		comma_sep=0;
		initialize(num);
		continue;
		}
		
		if(ch[i] == ',' && br_open)
		{
		degree = atoi(num);
		j=0;
		comma_sep=1;
		initialize(num);
		continue;
		}
		
		if(br_open)
		{
		num[j] = ch[i];
		j++;
		}		
	}

return pol;
}

void put_polynomial(struct node* pol)
{
	if(pol != NULL)
	{
	printw("(%d, %f)", pol->degree, pol->coeff);
	put_polynomial(pol->next);
	}
}

void divide_polynomial(struct node* dividend, struct node* divisor)
{
struct node *quotient = NULL, *remainder = NULL, *subtractor = NULL, *new_node = NULL, *old = NULL, *dividend_first = NULL, *divisor_first = NULL, *new_node_1 = NULL, *dividend_prev_node = NULL;
int flag=0;

dividend_first = dividend; divisor_first = divisor;
	while(dividend->degree >= divisor->degree)
	{
	new_node = (struct node*)malloc(sizeof(struct node));
	new_node->degree = dividend->degree - divisor->degree;
	new_node->coeff = dividend->coeff / divisor->coeff;
	new_node->next = NULL;
		if(quotient == NULL)
		{
		new_node->prev = NULL;
		quotient = new_node;
		}
		
		else
		{
		old = quotient;
			while(old->next != NULL)
			{
			old = old->next;
			}
		old->next = new_node;
		new_node->prev = old;
		}
				
		while(divisor != NULL)
		{
		new_node_1 = (struct node*)malloc(sizeof(struct node));
		new_node_1->degree = divisor->degree + new_node->degree;
		new_node_1->coeff  = divisor->coeff  * new_node->coeff * (-1);
		new_node_1->next   = NULL;
			if(subtractor == NULL)
			{
			new_node_1->prev = NULL;
			subtractor = new_node_1;
			}
			
			else
			{
			old = subtractor;
				while(old->next != NULL)
				{
				old = old->next;
				}
			old->next = new_node_1;
			new_node_1->prev = old;
			}
		
		divisor = divisor->next;			
		}
		
		while(dividend != NULL && subtractor != NULL)
		{
			if(dividend->degree == subtractor->degree)
			{
			dividend->coeff = dividend->coeff + subtractor->coeff;
			dividend_prev_node = dividend;
			dividend   = dividend->next;
			subtractor = subtractor->next;
			}
			else
			{
				if(dividend->degree > subtractor->degree)
				{
				dividend_prev_node = dividend;
				dividend = dividend->next;
				}
				else
				{
				new_node_1           = (struct node*)malloc(sizeof(struct node));
				new_node_1->degree   = subtractor->degree;
				new_node_1->coeff    = subtractor->coeff;
				dividend->prev->next = new_node_1;
				new_node_1->prev     = dividend->prev;
				dividend->prev       = new_node_1;
				new_node_1->next     = dividend;
				subtractor           = subtractor->next;
				}
			}
		}
		
		if(dividend == NULL)
		{	
		dividend = dividend_prev_node;
			while(subtractor != NULL)
			{
			new_node_1           = (struct node*)malloc(sizeof(struct node));
			new_node_1->degree   = subtractor->degree;
			new_node_1->coeff    = subtractor->coeff;
			dividend->next       = new_node_1;
			new_node_1->prev     = dividend;
			new_node_1->next     = NULL;
			dividend             = dividend->next;
			subtractor           = subtractor->next;
			}		
		}
		
		dividend = dividend_first;
		
			while(dividend != NULL)
			{
				if(dividend->coeff == 0)
				{
					if(dividend == dividend_first)
					{
						if(dividend->next != NULL)
						{
						dividend = dividend->next;
						dividend->prev->next = NULL;
						free(dividend->prev);
						dividend->prev = NULL;
						dividend_first = dividend;
						}
						else
						{
						dividend->degree = 0;
						dividend = dividend->next;
						}
					}
					else
					{
						if(dividend->next != NULL)
						{			
						dividend->prev->next = dividend->next;
						dividend->next->prev = dividend->prev;
						old = dividend;
						dividend = dividend->next;
						old->next = NULL;
						old->prev = NULL;
						free(old);
						}
						else
						{
						dividend->prev->next = NULL;
						dividend->prev = NULL;
						free(dividend);
						}
					}
				}
				else
				{
				dividend = dividend->next;
				}
			}
		
		dividend = dividend_first;
		divisor = divisor_first;
			if(divisor->degree == 0 && dividend->degree == 0)
			{
				if(!flag)
				{
				flag++;
				}
				else
				{
				break;
				}
			}
	}

remainder = dividend;

printw("\nQuotient polynomial (Degree, Coefficient):\n");

put_polynomial(quotient);

printw("\nRemainder polynomial (Degree, Coefficient):\n");

put_polynomial(remainder);

}

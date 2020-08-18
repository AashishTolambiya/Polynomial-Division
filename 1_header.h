struct node
{
int degree;
float coeff;
struct node* prev;
struct node* next;
};


void initialize(char num[]);
struct node* get_polynomial(struct node* pol);
void put_polynomial(struct node* pol);
void divide_polynomial(struct node* dividend, struct node* divisor);

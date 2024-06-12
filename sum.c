#include<stdio.h>
int a,b,add,sub,divi,mul;
void addition (int a,int b)
{
    add=a+b;
    mul=a*b;
    divi=a/b;
    printf("addition of two numbers= %d\n",add);
    printf("multiple of two numbers=%d\n",mul);
    printf("division of two number =%d\n",divi);
}
void subtraction(int a,int b)
{
    sub=a-b;
    printf("substrate  of two number=%d",sub);
}
int main()
{
   // int a,b;
    printf("enter two number\n");
    scanf("%d %d",&a,&b);
    addition(a,b);
    subtraction(a,b);
    
}


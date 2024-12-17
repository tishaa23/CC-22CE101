#include<stdio.h>

void main()
{
    char str[20];
    printf("Enter string: ");
    gets(str);
    int len=strlen(str);
    int cnt=0;
    int i=0;
    while(i<len && str[i]=='a')
    {
       i++;
    }
    while(i<len && str[i]=='b')
    {
        cnt++;
        i++;
    }
    if(cnt==2 && i==len)
    {
        printf("Valid string");
    }
    else
    {
        printf("Invalid string");
    }
    
    
     
}


#include<stdio.h>

string commom(string arr[])
{
    string prefix="";
    string start=arr[0];
    for(int i=1;i<arr.size();i++)
    {
        int s=0;
        while(s<start.size() && s<arr.size() && start[s]==arr[s][i])
        {
            s++;
        }
        prefix+=start[s];
    }

    return prefix;
}

void left(string prod[], string nonterminal,string s)
{
    string common_prefix=common(prod);
    for(int i=0;i<prod.size();i++)
    {
        cout<<nonterminal<<"->";
        cout<<prod[i];
    }

    string newnonterminal=nonterminal+"'";
    cout<<nonterminal<<"->"<<common_prefix<<newnonterminal;
    cout<<newnonterminal<<"->";

    for(int i=0;i<prod.size();i++)
    {
        int j=0;
        if(prod[i][j]!=common_prefix)
        {
            cout<<prod[i][j];

        }
        cout<<" | ";
    }

}

main()
{
    string nonterminal;
    cin>>nontermianl;
    int no_of_prod;
    cin>>no_od_prod;

    string prod[];
    for(int i=0;i<no_of_prod;i++)
    {
        cin>>prod[i];
    }



    left(prod,nonterminal,s);

}



/*helllo
hello */

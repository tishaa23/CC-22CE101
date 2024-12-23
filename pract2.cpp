#include<iostream>
using namespace std;

main()
{
    int no_of_symbol=2;
    char symbol[2]={'a','b'};
    int states=4;
    int initial=0; //1st state i.e 0
    int no_accept=1;
    int accept_state=1; //2nd state means index=0+1=1
  /*  int arr[2][4];
    arr[0][0]=1;
    arr[1][0]=2;
    arr[0][1]=0;
    arr[1][1]=3;
    arr[0][2]=3;
    arr[1][2]=0;
    arr[0][3]=2;
    arr[1][3]=1;*/
    int arr[2][4] = {
        {1, 0, 3, 2},  // Transitions for 'a'
        {2, 3, 0, 1}   // Transitions for 'b'
    };
    string str="abbabab";
    int current_state=initial;
    for(int i=0;i<str.length();i++)
    {
        char current_symbol=str[i];
        int symbol_index;
        if(current_symbol=='a')
        {
           symbol_index=0;
        }
        else
        {
           symbol_index=1;
        }
        current_state=arr[symbol_index][current_state];
        cout << "After processing symbol '" << current_symbol << "', current state is: " << current_state << endl;

    }

    if(current_state == accept_state) {
        cout << "The input string is accepted." << endl;
    } else {
        cout << "The input string is not accepted." << endl;
    }




}

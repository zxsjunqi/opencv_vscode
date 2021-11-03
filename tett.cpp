#include<iostream>
using namespace std;
int main(){
    int i=0;
    for(i=0;i<8;i++)
    cout<<(0x80>>i)<<endl;
    return 0;
}
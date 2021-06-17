#include <bits/stdc++.h>
#include <iostream>
using namespace std;
    

long long randomgenerator(){
    srand(time(0));
    long long random_output = rand();
    return random_output;
}

bool completeness(int g,int q,int x){
    
    //int r, ,int a,int c
    long long r = randomgenerator();
    long long a = pow(g,r);
    cout << "Prover returns the value g^r: " << a;
    long long c;
    cout << "\nVerifier is supposed to enter the challenge: ";
    cin >> c;
    long long t = (r + ((c*x)%q));
    cout << "Prover returns the response value : " << t;
    long long gt = pow(g,t);
    long long y = pow(g,x);
    long long ayc = a*(pow(y,c));
    if (gt == ayc){
        return true;
    }
    else{
        return false;
    }
}


int main(){
    long long g = randomgenerator();
    long long q = randomgenerator();
    long long x = randomgenerator();
    
    if (completeness(g,q,x)){
        cout << "\nVerifier accepts the condition\n";
    }
    else{
        cout << "\nVerifier denies the condition";
    }

    return 0;
}
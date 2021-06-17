#include <bits/stdc++.h>
#include <iostream>
using namespace std;
    

long long randomgenerator(){
    srand(time(0));
    long long random_output = rand();
    return random_output;
}

bool completeness(long long g,long long h,long long commitment,long long p,long long r){

    long long a = randomgenerator();
    long long b = randomgenerator();
    long long C1 = pow(g,a)*pow(h,b);
    cout << "Prover returns the value g^a.h^b: " << C1;
    long long c;
    cout << "\nVerifier is supposed to enter the challenge: ";
    cin >> c;
    long long t1 = (a + (c*p));
    long long t2 = (b + (c*r));
    cout << "\nProver returns the response values : t1 = " << t1 << " , t2 = " << t2;
    long long gt1ht2 = pow(g,t1)*pow(h,t2);
    long long RHS = C1*(pow(commitment,c));
    if (gt1ht2 == RHS){
        return true;
    }
    else{
        return false;
    }
}


int main(){
    long long g = randomgenerator();
    long long h = randomgenerator();
    long long commitment = randomgenerator();
    long long p = randomgenerator();
    long long r = randomgenerator();
    
    
    if (completeness(g,h,commitment,p,r)){
        cout << "\nVerifier accepts the condition\n";
    }
    else{
        cout << "\nVerifier denies the condition";
    }

    return 0;
}
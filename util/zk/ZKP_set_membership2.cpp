#include <bits/stdc++.h>
#include <iostream>
#include <functional>
using namespace std;
using namespace std::placeholders;    

long long bilinearmap(long long a, long long b){
    //bilinear maps of the form e(a,b)
    return pow(1,a);
}

long long randomgenerator(){
    srand(time(0));
    long long random_output = rand();
    return random_output;
}

bool completeness(long long g,long long v,long long x,long long r, long long p){

    long long y = pow(g,x);
    long long signature = pow(g, (v/(x + p)));
    long long s = randomgenerator();
    long long t = randomgenerator();
    long long m = randomgenerator();
    long long LHS = pow(bilinearmap(signature,g), (-1*s)) * pow(bilinearmap(g,g), t);
    cout << "Prover returns the value e(V,g)^-s.e(g,g)^t : " << LHS;
    long long c;
    cout << "\nVerifier is supposed to enter the challenge: ";
    cin >> c;
    long long zp = (s - (c*p));
    long long zr = (r - (c*m));
    long long zv = (t - (c*v));
    cout << "\nProver returns the response values : zp = " << zp << " , zr = " << zr << " , zv = " << zv;
    long long RHS = pow(bilinearmap(signature,y), c) * (pow(bilinearmap(signature,g),(-1*zp))) * (pow(bilinearmap(g,g), zv));
    if (LHS == RHS){
        return true;
    }
    else{
        return false;
    }
}


int main(){
    long long g = randomgenerator();
    long long v = randomgenerator();
    long long x = randomgenerator();
    long long r = randomgenerator();
    long long p = randomgenerator();    
    
    if (completeness(g,v,x,r,p)){
        cout << "\nVerifier accepts the condition\n";
    }
    else{
        cout << "\nVerifier denies the condition";
    }
    return 0;
}
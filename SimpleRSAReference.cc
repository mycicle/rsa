//Geeks4Geeks simple algo:

#include<iostream>
#include<math.h>
using namespace std;
//gcd
int gcd(int a, int b) {
    int t;
    while(1) {
        t= a%b;
        if(t==0)
            return b;
        a = b;
        b = t;
    }
}
int main() {
    //2 random prime numbers: p and q
    double p = 23;
    double q = 29;
    double n=p*q;
    double track;
    double phi= (p-1)*(q-1);//calculate phi
    //public key
    //e = encrypt
    double e=7;
    //for checking that 1 < e < phi(n) and gcd(e, phi(n)) = 1; i.e., e and phi(n) are coprime.
    while(e<phi) {
        track = gcd(e,phi);
        if(track==1)
            break;
        else
            e++;
    }
    //private key
    //d= decrypt
    //choosing d such that it satisfies d*e = 1 mod phi
    double d1=1/e;
    double d=fmod(d1,phi);
    double message = 10;
    double c = pow(message,e); //encrypt msg
    double m = pow(c,d);
    c=fmod(c,n);
    m=fmod(m,n);
    cout<<"Original Message = "<<message;
    cout<<"\n"<<"p = "<<p;
    cout<<"\n"<<"q = "<<q;
    cout<<"\n"<<"n = pq = "<<n;
    cout<<"\n"<<"phi = "<<phi;
    cout<<"\n"<<"e = "<<e;
    cout<<"\n"<<"d = "<<d;
    cout<<"\n"<<"Encrypted message = "<<c;
    cout<<"\n"<<"Decrypted message = "<<m;
    return 0;
}

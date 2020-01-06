#ifndef _BigInt_H
#define _BigInt_H
#include<iostream>
#include<vector>
#include<string>
#include<cstring>
using namespace std;

class BigInt {
    //pre
    //compare abs(a) and abs(b); return 1/0/-1;
    friend int cmp(const BigInt &a, const BigInt &b);
    friend bool operator< (const BigInt &a, const BigInt &b) {return (cmp(a, b) == -1);}
    friend bool operator> (const BigInt &a, const BigInt &b) {return (cmp(a, b) ==  1);}
    friend bool operator==(const BigInt &a, const BigInt &b) {return (cmp(a, b) ==  0);}
    friend bool operator<=(const BigInt &a, const BigInt &b) {return (cmp(a, b) !=  1);}
    friend bool operator>=(const BigInt &a, const BigInt &b) {return (cmp(a, b) != -1);}
    friend bool operator!=(const BigInt &a, const BigInt &b) {return (cmp(a, b) !=  0);}

    //mainWork
    friend BigInt operator+(const BigInt &a, const BigInt &b);
    friend BigInt operator-(const BigInt &a, const BigInt &b);
    friend BigInt operator*(const BigInt &a, const BigInt &b);
    friend BigInt operator/(const BigInt &a, const BigInt &b);
    friend BigInt operator%(const BigInt &a, const BigInt &b);

    //input&output
    friend istream& operator>>(istream &is, BigInt &a);
    friend ostream& operator<<(ostream &os, const BigInt& a);

    //others
    friend BigInt operator-(const BigInt &a);
    //friend BigInt operator++(const BigInt &a) {return (a + 1);};
    //friend BigInt operator++(const BigInt &a, int x) {return (a + 1);};

    private:
        int sgn;
        vector<int> n;
        BigInt cut(int l, int r) const;
        void checkZero();
        void reversal();
    public:
        BigInt(const int &s = 0, const long long &num = 0) {
            sgn = s; int _num = num;
            while (_num) {
                n.push_back(_num % 10);
                _num /= 10;
            }
            checkZero();
        }
        /*Bigint(const int &num = 0) {
            cout << "H\n";
            if (num <  0) sgn = -1;
            if (num == 0) sgn = 0;
            if (num >  0) sgn = 1;
            int _num = num;
            while (_num) {
                n.push_back(_num % 10);
                _num /= 10;
            }
            checkZero();
        }*/
        explicit BigInt(const bool &vbool);
        explicit BigInt(const double &vDouble);
        explicit BigInt(const string &vString);

        explicit operator bool() const;
        explicit operator double() const;
        explicit operator string() const;
        
        BigInt &operator++();
        BigInt operator++(int x);
        BigInt &operator--();
        BigInt operator--(int x);

        ~BigInt() {}
};

#endif
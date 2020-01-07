#ifndef _Alltype_H
#define _Alltype_H
#include "BigInt.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

class Alltype {
    //cmp:compare its ABS and return -1/0/1;
    //workable for: cmp among numeric types(bool/bigint/double) and between strings
    friend int cmp(const Alltype &a, const Alltype &b);
    friend bool operator< (const Alltype &a, const Alltype &b) {return (cmp(a, b) == -1);}
    friend bool operator> (const Alltype &a, const Alltype &b) {return (cmp(a, b) ==  1);}
    friend bool operator==(const Alltype &a, const Alltype &b) {return (cmp(a, b) ==  0);}
    friend bool operator<=(const Alltype &a, const Alltype &b) {return (cmp(a, b) !=  1);}
    friend bool operator>=(const Alltype &a, const Alltype &b) {return (cmp(a, b) != -1);}
    friend bool operator!=(const Alltype &a, const Alltype &b) {return (cmp(a, b) !=  0);}

    //operators
    friend Alltype operator+(const Alltype &a, const Alltype &b);
    friend Alltype operator-(const Alltype &a, const Alltype &b);
    friend Alltype operator*(const Alltype &a, const Alltype &b);
    friend Alltype operator/(const Alltype &a, const Alltype &b);
    friend Alltype div(const Alltype &a, const Alltype &b);
    friend Alltype operator%(const Alltype &a, const Alltype &b);

    //moreOperators
    friend Alltype operator&&(const Alltype &a, const Alltype &b) {return Alltype((bool)a && (bool)b);}
    friend Alltype operator||(const Alltype &a, const Alltype &b) {return Alltype((bool)a || (bool)b);}

    //output
    friend ostream& operator<<(ostream &os, const Alltype& a);
    
    private:
        static const int VOID = 0, BOOL = 1, BIGINT = 2, DOUBLE = 3, STRING = 4;
        bool vBool;
        BigInt vBigInt;
        double vDouble;
        string vString;
        int type;
    public:
        string valName;
        //construction x 5
        //change void/bool/bigint/double/string into alltype
        //need fix
        explicit Alltype():type(VOID), vBool(), vBigInt(), vDouble(), vString(), valName("") {}
        explicit Alltype(const bool &_vBool):type(BOOL), vBool(_vBool), vBigInt(), vDouble(), vString(), valName() {}
        explicit Alltype(const BigInt &_vBigInt):type(BIGINT), vBool(), vBigInt(_vBigInt), vDouble(), vString(), valName() {}
        explicit Alltype(const double &_vDouble):type(DOUBLE), vBool(), vBigInt(), vDouble(_vDouble), vString(), valName() {}
        explicit Alltype(const string &_vString):type(STRING), vBool(), vBigInt(), vDouble(), vString(_vString), valName() {}

        //conversion x 4
        //change alltype into bool/bigint/double/string
        explicit operator bool() const;
        explicit operator BigInt() const;
        explicit operator double() const;
        explicit operator string() const;
        
        //operators
        Alltype &operator=(const Alltype &a) {
            if (this == &a) return *this;
            type = a.type;
            vBool = a.vBool;
            vBigInt = a.vBigInt;
            vDouble = a.vDouble;
            vString = a.vString;
            return *this;
        }
        Alltype operator&=(const Alltype &a) {return *this = *this && a;}
        Alltype operator|=(const Alltype &a) {return *this = Alltype(*this || a);}

        Alltype operator+=(const Alltype &a) {return *this = *this + a;}
        Alltype operator-=(const Alltype &a) {return *this = *this - a;}
        Alltype operator*=(const Alltype &a) {return *this = *this * a;}
        Alltype operator/=(const Alltype &a) {return *this = *this / a;}
        Alltype operator%=(const Alltype &a) {return *this = *this % a;}
        // //=

        ~Alltype() {}
};

#endif
#include "BigInt.h"

BigInt BigInt::cut(int l, int r) const {//ans: <-
    BigInt ans = 0; ans.sgn = sgn;
    for (int i = l; i < r; ++i) {
        ans.n.push_back(n[i]);
    }
    return ans;
}

void BigInt::checkZero() {
    if (sgn == 0) return;
    for (int i = 0; i < n.size(); ++i) {
        if (n[i] != 0) return;
    }
    sgn = 0;
}

void BigInt::reversal() {
    for (int i = 0; i < n.size()-i-1; ++i) {
            int temp = n[i];
            n[i] = n[n.size()-i-1];
            n[n.size()-i-1] = temp;
        }
}

int cmp(const BigInt &a, const BigInt &b) {
    // return: 1 if abs(a) >  abs(b);
    //         0 if abs(a) == abs(b);
    //        -1 if abs(a) <  abs(b);
    if (a.n.size() < b.n.size()) return -1;
    if (a.n.size() > b.n.size()) return 1;
    for (int i = a.n.size() - 1; i >= 0; --i) {
        if (a.n[i] > b.n[i]) return 1;
        if (a.n[i] < b.n[i]) return -1;
    }
    return 0;
}

BigInt operator+(const BigInt &a, const BigInt &b) {
    if (a.sgn == 0) return b;
    if (b.sgn == 0) return a;
    if (a.sgn < 0 && b.sgn > 0) return (b - (-a));
    if (a.sgn > 0 && b.sgn < 0) return (a - (-b));
    BigInt ans; ans.sgn = a.sgn; ans.n.clear();
    for (int i = 0; i < min(a.n.size(), b.n.size()); ++i) {
        ans.n.push_back(a.n[i] + b.n[i]);
    }
    if (a.n.size() > b.n.size())
        for (int i = min(a.n.size(), b.n.size()); i < max(a.n.size(), b.n.size()); ++i)
            ans.n.push_back(a.n[i]);
    else
        for (int i = min(a.n.size(), b.n.size()); i < max(a.n.size(), b.n.size()); ++i)
            ans.n.push_back(b.n[i]);
    for (int i = 0; i < ans.n.size() - 1; ++i) {
        if (ans.n[i] > 9) {
            ans.n[i] -= 10;
            ans.n[i + 1]++;
        }
    }
    if (ans.n[ans.n.size() - 1] > 9) {
        ans.n[ans.n.size() - 1] %= 10;
        ans.n.push_back(1);
    }
    return ans;
}

BigInt operator-(const BigInt &a, const BigInt &b) {
    BigInt zero(0, 0);
    if (a.sgn == 0) return (-b);
    if (b.sgn == 0) return a;
    if (a.sgn < 0 && b.sgn > 0) return (a + (-b));
    if (a.sgn > 0 && b.sgn < 0) return (a + (-b));

    //make sure abs(a) > abs(b); if abs(a) == abs(b) return (0, 0)
    int _cmp = cmp(a, b);
    if (_cmp == -1) return -(b - a);
    if (_cmp == 0) return zero;

    BigInt ans; ans.sgn = a.sgn; ans.n.clear();
    for (int i = 0; i < b.n.size(); ++i)
        ans.n.push_back(a.n[i] - b.n[i]);
    for (int i = b.n.size(); i < a.n.size(); ++i)
        ans.n.push_back(a.n[i]);
    for (int i = 0; i < ans.n.size() - 1; ++i) {
        if (ans.n[i] < 0) {
            ans.n[i] += 10;
            ans.n[i + 1]--;
        }
    }
    while (ans.n[ans.n.size() - 1] == 0) ans.n.pop_back();
    if (ans.n.empty()) return zero;
    return ans;
}

BigInt operator*(const BigInt &a, const BigInt &b) {
    BigInt zero(0, 0);
    BigInt ans(0, 0);
    if (a.sgn == 0 || b.sgn == 0) return ans;
    if (a.sgn == b.sgn) ans.sgn = 1; else ans.sgn = -1;
    for (int i = 0; i < a.n.size() + b.n.size(); ++i) ans.n.push_back(0);
    for (int i = 0; i < a.n.size(); ++i) {
        for (int j = 0; j < b.n.size(); ++j) {
            ans.n[i + j] += a.n[i] * b.n[j];
            if (ans.n[i + j] > 9) {
                ans.n[i + j + 1] += ans.n[i + j] / 10;
                ans.n[i + j] %= 10;
            }
        }
    }
    while (ans.n[ans.n.size() - 1] == 0) ans.n.pop_back();
    if (ans.n.empty()) return zero;
    return ans;
}

BigInt operator/(const BigInt &a, const BigInt &b) {
    BigInt zero(0, 0), minusOne(-1, 1);
    BigInt ans;
    if (a.sgn == b.sgn) ans.sgn = 1; else ans.sgn = -1;

    //check sgn
    if (b.sgn == 0) {cerr << "RE: divisor is zero."; return zero;}
    if (a.sgn == 0) return zero;
    if (a < b) {
        if (ans.sgn == 1) return zero;
        if (ans.sgn == -1) return minusOne;
    }
    if (a == b) {
        BigInt t(0, 1);
        if (a.sgn == b.sgn) t.sgn = 1; else t.sgn = -1;
        return t;
    }

    BigInt _a = a; _a.sgn = 1;//_a = abs(a) and _a is changeable
    BigInt _b = b; _b.sgn = 1;//_b = abs(b) and _b is changeable(though not changed factually)
    int l = _a.n.size() - 1, r, prvl;
    while (_a >= _b) {
        prvl = l;
        l = _a.n.size() - 1; r = _a.n.size();
        BigInt div = _a.cut(l, r);//div: <-; cut: take [l, r) (<-)
        while (div < _b) {if (l < prvl) ans.n.push_back(0); --l; div = _a.cut(l, r);}
        int temp = 0;
        while (div >= _b) {
            ++temp; div = div - _b;
        }
        for (int i = 0; i < div.n.size(); ++i) _a.n[l + i] = div.n[i];
        for (int i = div.n.size(); i < _a.n.size() - l; ++i) _a.n[l + i] = 0;
        while (_a.n[_a.n.size() - 1] == 0) _a.n.pop_back();
        ans.n.push_back(temp);
    }
    for (int i = 0; i < l; ++i) ans.n.push_back(0);
    BigInt _ans; _ans.sgn = ans.sgn;
    for (int i = ans.n.size() - 1; i >= 0; --i) _ans.n.push_back(ans.n[i]);
    while (_ans.n[_ans.n.size() - 1] == 0) _ans.n.pop_back();
    if (!_a.n.empty() && _ans.sgn == -1) _ans = _ans + minusOne;
    if (_ans.n.empty()) return zero;
    return _ans;
}

BigInt operator%(const BigInt &a, const BigInt &b) {
    return (a - b * (a / b));
}

istream& operator>>(istream &is, BigInt &a) {
    string str;
    is >> str;
    if (str[0] == '-') a.sgn = -1; else a.sgn = 1;
    for (int i = str.length() - 1; i >= (a.sgn == -1); --i)
        a.n.push_back(str[i] - '0');
    while (a.n[a.n.size() - 1] == 0) a.n.pop_back();
    a.checkZero();
    return is;
}

ostream& operator<<(ostream &os, const BigInt& a) {
    if (a.sgn == 0) {
        os << '0';
        return os;
    }
    if (a.sgn == -1) os << '-';
    for (int k = a.n.size() - 1; k >= 0; --k) os << a.n[k];
    return os;
}

BigInt operator-(const BigInt &a) {
    BigInt ans = a; ans.sgn = -a.sgn;
    return ans;
}

BigInt &BigInt::operator++() {
    *this = *this + BigInt(1, 1);
    return *this;
}

BigInt BigInt::operator++(int x) {
    BigInt cur = *this;
    *this  = *this + BigInt(1, 1);
    return cur;
}

BigInt &BigInt::operator--() {
    *this = *this - BigInt(1, 1);
    return *this;
}

BigInt BigInt::operator--(int x) {
    BigInt cur = *this;
    *this  = *this - BigInt(1, 1);
    return cur;
}

BigInt::BigInt(const bool &vbool) {
    if (vbool == false) {sgn = 0; n.push_back(0);}
    if (vbool == true ) {sgn = 1; n.push_back(1);}
}

BigInt::BigInt(const double &vDouble) {
    if (vDouble < 1.0 && vDouble > -1.0) {sgn = 0; n.push_back(0);}
    else {
        string str = to_string(vDouble);
        int cur;
        if (str[0] == '-') sgn = -1, cur = 1; else sgn = 1, cur = 0;
        while (str[cur] != '.' && cur < str.length()) {
            n.push_back(str[cur] - '0');
            ++cur;
        }
        checkZero();
        reversal();
    }
}

BigInt::BigInt(const string &vString) {
    int cur;
    if (vString[0] == '-') sgn = -1, cur = 1; else sgn = 1, cur = 0;
    while (vString[cur] != '.' && cur < vString.length()) {
        n.push_back(vString[cur] - '0');
        ++cur;
    }
    checkZero();
    reversal();
}

BigInt::operator bool() const {
    if (sgn == 0) return false;
    else return true;
}

BigInt::operator double() const {
    double vDouble = 0;
    if (sgn == 0) return vDouble;
    for (int i = n.size()-1; i >= 0; --i) {
        vDouble *= 10;
        vDouble += n[i];
    }
    if (sgn == -1) vDouble = -vDouble;
    return vDouble;
}

BigInt::operator string() const {
    string str = "";
    switch (sgn) {
    case 0:
        str = "0";
        return str;
        break;
    case -1:
        str = "-";
        break;
    }
    for (int i = n.size()-1; i >= 0; --i) {
        str = str + (char)('0' + n[i]);
    }
    return str;
}
/*
int main() {
    BigInt a, b;
    cin >> a >> b;
    cout << a + b << '\n';
    cout << a - b << '\n';
    cout << a * b << '\n';
    cout << a / b << '\n';
    cout << a % b << '\n';
    ++a;
    cout << a << '\n';
    a++;
    cout << a << '\n';
    --a;
    cout << a << '\n';
    a--;
    cout << a << '\n';
}
*/
#include "Alltype.h"

int cmp(const Alltype &a, const Alltype &b) {//haven't been checked
    // return: 1 if abs(a) >  abs(b);
    //         0 if abs(a) == abs(b);
    //        -1 if abs(a) <  abs(b);
    if (!a.type || !b.type) {
        cerr << "ERROR: Any attempt to compare VOID with other types is doomed to fail.\n";
        return 0;
    }
    int maxType = (a.type > b.type)?(a.type):(b.type);
    switch (maxType) {
    case Alltype::BOOL:
        if (a.vBool <  b.vBool) return -1;
        if (a.vBool == b.vBool) return  0;
        if (a.vBool >  b.vBool) return  1;
        break;
    case Alltype::BIGINT:
        if ((BigInt)a <  (BigInt)b) return -1;
        if ((BigInt)a == (BigInt)b) return  0;
        if ((BigInt)a >  (BigInt)b) return  1;
        break;
    case Alltype::DOUBLE:
        if ((double)a <  (double)b) return -1;
        if ((double)a == (double)b) return  0;
        if ((double)a >  (double)b) return  1;
        break;
    case Alltype::STRING:
        if (a.type != b.type) {
            cerr << "ERROR: Any attempt to compare a numeric type with a STRING is doomed to fail.\n";
            return 0;
        }
        if ((string)a <  (string)b) return -1;
        if ((string)a == (string)b) return  0;
        if ((string)a >  (string)b) return  1;
        break;
    }
    return 0;
}

Alltype operator+(const Alltype &a, const Alltype &b) {//haven't been checked
    if (!a.type || !b.type) {
        cerr << "ERROR: Any ADD_OP between VOID and other types is illegal.\n";
        return Alltype();
    }
    int maxType = (a.type > b.type)?(a.type):(b.type);
    Alltype ans;
    switch (maxType) {
    case Alltype::BOOL:
        return Alltype(BigInt(a) + BigInt(b));
    case Alltype::BIGINT:
        return Alltype(BigInt(a) + BigInt(b));
    case Alltype::DOUBLE:
        return Alltype((double)a + (double)b);
    case Alltype::STRING:
        if (a.type != b.type) {
            cerr << "ERROR: Any ADD_OP between a numeric type and a STRING is illegal.\n";
            return Alltype();
        }
        return Alltype((string)a + (string)b);
    }
    return Alltype();
}

Alltype operator-(const Alltype &a, const Alltype &b) {//haven't been checked
    if (!a.type || !b.type) {
        cerr << "ERROR: Any SUB_OP between VOID and other types is illegal.\n";
        return Alltype();
    }
    int maxType = (a.type > b.type)?(a.type):(b.type);
    Alltype ans;
    switch (maxType) {
    case Alltype::BOOL:
        return Alltype(BigInt(a) - BigInt(b));
    case Alltype::BIGINT:
        return Alltype(BigInt(a) - BigInt(b));
    case Alltype::DOUBLE:
        return Alltype((double)a - (double)b);
    case Alltype::STRING:
        cerr << "ERROR: SUB_OP is meaningless with regard to STRING";
        break;
    }
    return Alltype();
}

Alltype operator*(const Alltype &a, const Alltype &b) {//haven't been checked
    if (!a.type || !b.type) {
        cerr << "ERROR: Any MTP_OP between VOID and other types is illegal.\n";
        return Alltype();
    }
    int maxType = (a.type > b.type)?(a.type):(b.type);
    Alltype ans;
    switch (maxType) {
    case Alltype::BOOL:
        return Alltype(BigInt(a) * BigInt(b));
    case Alltype::BIGINT:
        return Alltype(BigInt(a) * BigInt(b));
    case Alltype::DOUBLE:
        return Alltype((double)a * (double)b);
    case Alltype::STRING:
        if (a.type == b.type) {
            cerr << "ERROR: Any MTP_OP between STRING types is illegal.\n";
            return Alltype();
        }
        if (a.type == Alltype::STRING) {
            if (b.type == Alltype::DOUBLE) {
                cerr << "ERROR: Any MTP_OP between DOUBLE and STRING type is illegal.\n";
                return Alltype();
            }
            string str = "";
            for (BigInt i(1, 1); i <= (BigInt)b; i = i + BigInt(1, 1)) str = str + (string)a;
            return Alltype(str);
        }
        else {
            if (a.type == Alltype::DOUBLE) {
                cerr << "ERROR: Any MTP_OP between DOUBLE and STRING type is illegal.\n";
                return Alltype();
            }
            string str = "";
            for (BigInt i(1, 1); i <= (BigInt)a; i = i + 1) str = str + (string)b;
            return Alltype(str);
        }
        break;
    }
    return Alltype();
}

Alltype operator/(const Alltype &a, const Alltype &b) {
    if (!a.type || !b.type) {
        cerr << "ERROR: Any DIV_OP between VOID and other types is illegal.\n";
        return Alltype();
    }
    int maxType = (a.type > b.type)?(a.type):(b.type);
    Alltype ans;
    switch (maxType) {
    case Alltype::BOOL:
        return Alltype(BigInt(a) / BigInt(b));
    case Alltype::BIGINT:
        return Alltype((double)a / (double)b);
    case Alltype::DOUBLE:
        return Alltype((double)a / (double)b);
    case Alltype::STRING:
        cerr << "ERROR: DIV_OP is meaningless with regard to STRING";
        break;
    }
    return Alltype();
}

Alltype div(const Alltype &a, const Alltype &b) {
    if (!a.type || !b.type) {
        cerr << "ERROR: Any DIV_OP between VOID and other types is illegal.\n";
        return Alltype();
    }
    int maxType = (a.type > b.type)?(a.type):(b.type);
    Alltype ans;
    switch (maxType) {
    case Alltype::BOOL:
        return Alltype(BigInt(a) / BigInt(b));
    case Alltype::BIGINT:
        return Alltype(BigInt(a) / BigInt(b));
        break;
    case Alltype::DOUBLE:
        return Alltype(BigInt((double)a / (double)b));
    case Alltype::STRING:
        cerr << "ERROR: DIV_OP is meaningless with regard to STRING";
        break;
    }
    return Alltype();
}

Alltype operator%(const Alltype &a, const Alltype &b) {
    if (!a.type || !b.type) {
        cerr << "ERROR: Any MOD_OP between VOID and other types is illegal.\n";
        return Alltype();
    }
    int maxType = (a.type > b.type)?(a.type):(b.type);
    Alltype ans;
    switch (maxType) {
    case Alltype::BOOL:
        return Alltype(BigInt(a) % BigInt(b));
    case Alltype::BIGINT:
        return Alltype(BigInt(a) % BigInt(b));
    case Alltype::DOUBLE:
        cerr << "ERROR: MOD_OP is meaningless with regard to DOUBLE";
        break;
    case Alltype::STRING:
        cerr << "ERROR: MOD_OP is meaningless with regard to STRING";
        break;
    }
    return Alltype();
}

ostream& operator<<(ostream &os, const Alltype& a) {
    switch (a.type) {
    case Alltype::VOID: break;
    case Alltype::BOOL: {
        if (a.vBool) os << "True";else os << "False";
        break;
    }
    case Alltype::BIGINT: os << a.vBigInt; break;
    case Alltype::DOUBLE: os << to_string(a.vDouble); break;
    case Alltype::STRING: os << a.vString; break;
    }
    return os;
}

Alltype::operator bool() const {
    switch (type) {
    case VOID:
        cerr << "ERROR: Any attempt to convert VOID into other types is doomed to fail.\n";
        break;
    case BOOL:
        return vBool;
    case BIGINT:
        return (bool)vBigInt;
    case DOUBLE:
        return (bool)vDouble;
    case STRING:
        if (vString == "") return false;
        else return true;
    }
    return false;
}

Alltype::operator BigInt() const {
    switch (type) {
    case VOID:
        cerr << "ERROR: Any attempt to convert VOID into other types is doomed to fail.\n";
        break;
    case BOOL:
        return BigInt(vBool);
    case BIGINT:
        return vBigInt;
    case DOUBLE:
        return BigInt(vDouble);
    case STRING:
        return BigInt(vString);
    }
    return BigInt(0, 0);
}

Alltype::operator double() const {
    switch (type) {
    case VOID:
        cerr << "ERROR: Any attempt to convert VOID into other types is doomed to fail.\n";
        break;
    case BOOL:
        return (double)vBool;
    case BIGINT:
        return (double)vBigInt;
    case DOUBLE:
        return vDouble;
    case STRING:
        return stod(vString);//accuracy loss; need fix
    }
    return 0;
}

Alltype::operator string() const {
    switch (type) {
    case VOID:
        cerr << "ERROR: Any attempt to convert VOID into other types is doomed to fail.\n";
        break;
    case BOOL:
        if (vBool) return "True";
        else return "False";
    case BIGINT:
        return (string)vBigInt;
    case DOUBLE:
        return to_string(vDouble);//accuracy loss; need fix
    case STRING:
        return vString;
    }
    return "";
}
/*
int main() {
    bool b1 = false, b2 = true;
    int i1 = -433, i2 = -0, i3 = 0, i4 = 342534654;
    double d1 = -5134, d2 = 0, d3 = 32543.534, d4 = 32544;
    string s1 = "", s2 = "Afb", s3 = "5432", s4 = "-432.5432";
    cout << Alltype(BigInt(-1, 43542)) + Alltype(43.5342);
    getchar();
    cout << Alltype(BigInt(-1, 43542)) - Alltype(-4321.526);
    getchar();
    cout << Alltype(BigInt(-1, 43542)) * Alltype(143253.54326);
    getchar();
    cout << Alltype(BigInt(-1, 43)) / Alltype(0.4315);
    getchar();
    cout << Alltype(BigInt(-1, 43542)) % Alltype(354.534);
    return 0;
}*/
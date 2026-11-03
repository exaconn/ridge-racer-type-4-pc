/* Track event tables are sorted in descending key order. */
#include "rr/lookup.h"

extern s32 D_800ABECC;
extern RrTrackResourceContext D_800AC650;
extern RrLookupTables16 *D_800ACBB8;

s32 func_80054EF4(s32 key, s32 sel) {
    s32 lo = 0, n, mid;
    RrPairS16 *t;
    if (sel == 0) {
        n = D_800ACBB8->count0;
        t = D_800ACBB8->table0;
    } else {
        n = D_800ACBB8->count1;
        t = D_800ACBB8->table1;
    }
    while (lo < n) {
        mid = (lo + n) / 2;
        if (t[mid].a >= key) lo = mid + 1;
        else n = mid;
    }
    if (lo != 0) lo--;
    if (key == t[lo].a) return t[lo].b;
    return -1;
}

s32 func_80054FA4(s32 a0, s32 a1, s32 a2) {
    s32 a3 = 0;
    s32 v1;
    s32 t;
    s32 r;
    if (a1 > 0) {
        do {
            v1 = (a3 + a1) / 2;
            if (((RrLookupEntry32 *)a0)[v1].key >= a2) {
                a3 = v1 + 1;
            } else {
                a1 = v1;
            }
        } while (a3 < a1);
    }
    if (a3 == 0) goto neg;
    a3--;
    t = ((RrLookupEntry32 *)a0)[a3].threshold;
    a0 = t;
    if (a0 == D_800ABECC - 1) {
        r = a3;
        if (a2 >= 0) return r;
neg:
        return -1;
    }
    if (a2 >= a0) return a3;
    return -1;
}

s32 func_80055040(s32 a0) {
    s32 r = func_80054FA4((s32)D_800AC650.lookupTables->table0, D_800AC650.lookupTables->count0, a0);
    if (r >= 0) return D_800AC650.lookupTables->table0[r].value;
    return -1;
}

s32 func_800550A0(s32 a0) {
    s32 r = func_80054FA4((s32)D_800AC650.lookupTables->table1, D_800AC650.lookupTables->count1, a0);
    if (r >= 0) return D_800AC650.lookupTables->table1[r].value;
    return -1;
}

s32 func_80055100(s32 a0) {
    s32 r = func_80054FA4((s32)D_800AC650.lookupTables->table2, D_800AC650.lookupTables->count2, a0);
    if (r >= 0) return D_800AC650.lookupTables->table2[r].value;
    return -1;
}

s32 func_80055160(s32 a0) {
    s32 r = func_80054FA4((s32)D_800AC650.lookupTables->table3, D_800AC650.lookupTables->count3, a0);
    if (r >= 0) return D_800AC650.lookupTables->table3[r].value;
    return -1;
}

s32 func_800551C0(s32 key) {
    RrLookupTables16 *q = D_800ACBB8;
    s32 n = q->count2;
    RrPairS16 *t = q->table2;
    s32 lo = 0, mid;
    while (lo < n) {
        mid = (lo + n) / 2;
        if (t[mid].a >= key) lo = mid + 1;
        else n = mid;
    }
    if (lo != 0) lo--;
    if (key == t[lo].a) return t[lo].b;
    return -1;
}

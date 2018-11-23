
#include <NTL/ZZ.h>
#include <iostream>
#include<sstream>
#include <ctime>
#include <unordered_map>
using namespace std;
using namespace NTL;
ZZ N;
ZZ n;
ZZ alpha;            /* generator             */
ZZ beta;
clock_t start, finish;
const int r = 16;
ZZ m[r];
ZZ gt[r];
ZZ ht[r];
string ZtoS(ZZ n) {
	stringstream buf;
	buf << n;
	string str;
	str = buf.str().c_str();
	return str;
}
class MyHash
{
public:
	size_t operator()(const ZZ& num) const
	{
		hash<string> sh;
		return sh(ZtoS(num));
	}
};
class MyEqualTo {
public:

	bool operator()(const ZZ& n1, const ZZ& n2) const
	{
		return n1 == n2;
	}
};

inline void new_xab(ZZ& x, ZZ& a, ZZ& b) {
	/*
	switch( x%3 ) {
	case 0: x = SqrMod(x,N)      ;  a = MulMod(a,2,n);  b = MulMod(b,2,n);  break;
	case 1: x = MulMod(x,alpha,N);  a = AddMod(a,1,n);                  break;
	case 2: x = MulMod(x,beta,N) ;                  	b =AddMod(b,1,n);  break;
	}
	*/
	int i = trunc_long(x,4);
	x = MulMod(x, m[i], N);
	a = AddMod(a, gt[i], n);
	b = AddMod(b, ht[i], n);
	
}
struct ZZ2 {
	ZZ gm;
	ZZ hm;
};

int main() {
	cout << "input g" << endl;
	cin >> alpha;
	cout << "input factor" << endl;
	cin >> n;
	cout << "input mod P" << endl;
	cin >> N;
	cout << "input target" << endl;
	cin >> beta;
	start = clock();
	alpha = PowerMod(alpha, (N - 1) / n, N);
	beta = PowerMod(beta, (N - 1) / n, N);
	
	ZZ a=RandomBnd(n);
	ZZ b=RandomBnd(n);
	ZZ x=PowerMod(alpha, a,N)*PowerMod(beta, b,N) % N;
	for (int i = 0; i<r; i++) {
		ZZ a1 = RandomBnd(n);
		ZZ b1 = RandomBnd(n);
		m[i] = PowerMod(alpha, a1,N)*PowerMod(beta, b1,N) % N;
		gt[i] = a1;
		ht[i] = b1;
	}
	unordered_map<ZZ, ZZ2, MyHash, MyEqualTo> table;

	start = clock();
	for (int i = 0; i < (1 << 18); i++) {
		new_xab(x, a, b);
		if (trunc_long(x, 12) == 0) {
			auto it = table.find(x);
			if (it == table.end()) {
				ZZ2 t;
				t.gm = a;
				t.hm = b;
				table.emplace(x, t);
			}
			else {
				if ((it->second).hm - b>0)
					cout << ((a - (it->second).gm)*InvMod((it->second).hm - b, n)) % n << endl;
				else if((it->second).hm - b==0)
                    cout<<"error"<<endl;
                else
					cout << (((it->second).gm - a)*InvMod(b - (it->second).hm, n)) % n << endl;
				break;
			}
		}
	}
	finish = clock();
	cout << double(trunc_long(1.3* SqrRoot(n) >> 18,64 ))*(double(finish - start) / CLOCKS_PER_SEC) << "is needed" << endl;
	while (1) {
		new_xab(x, a, b);
		if (trunc_long(x, 14) == 0) {
			auto it = table.find(x);
			if (it == table.end()) {
				ZZ2 t;
				t.gm = a;
				t.hm = b;
				table.emplace(x, t);
			}
			else {
				if ((it->second).hm - b>0)
					cout << ((a - (it->second).gm)*InvMod((it->second).hm - b, n)) % n << endl;
                else if((it->second).hm - b==0)
                    cout<<"error"<<endl;
				else
					cout << (((it->second).gm - a)*InvMod(b - (it->second).hm, n)) % n << endl;
				break;
			}
		}
	}
	finish = clock();
	cout << "time: " << double(finish - start) / CLOCKS_PER_SEC << endl;
	getchar();
	getchar();
	getchar();
	getchar();
	getchar();
	getchar();
	getchar();
	getchar();
	getchar();
	getchar();
}


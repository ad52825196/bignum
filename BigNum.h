// BigNum.h

#ifndef _BigNum_h
#define _BigNum_h

#include <vector>
#include <string>

class BigNum
{
	friend BigNum operator+(const BigNum a, const BigNum b);
	friend BigNum operator+=(BigNum &a, const BigNum b);
	friend BigNum operator-(BigNum a);
	friend BigNum operator-(BigNum a, BigNum b);
	friend BigNum operator-=(BigNum &a, const BigNum b);
	friend BigNum operator*(const BigNum a, const BigNum b);
	friend BigNum operator*=(BigNum &a, const BigNum b);
	friend BigNum operator/(const BigNum a, BigNum b);
	friend BigNum operator/=(BigNum &a, const BigNum b);
	friend BigNum operator%(const BigNum a, BigNum b);
	friend BigNum operator%=(BigNum &a, const BigNum b);
	friend BigNum operator^(const BigNum a, BigNum b);
	friend BigNum operator++(BigNum a);
	friend BigNum operator++(BigNum &a, int n);
	friend BigNum operator--(BigNum a);
	friend BigNum operator--(BigNum &a, int n);
	friend bool operator<(const BigNum a, const BigNum b);
	friend bool operator>(const BigNum a, const BigNum b);
	friend bool operator==(const BigNum a, const BigNum b);
	friend bool operator<=(const BigNum a, const BigNum b);
	friend bool operator>=(const BigNum a, const BigNum b);
	friend bool operator!=(const BigNum a, const BigNum b);
	friend ostream &operator<<(ostream &os, const BigNum &n);
	friend istream &operator>>(istream &is, BigNum &n);
protected:
	bool symbol; // 表示正负号，false为正，true为负
	vector <int> num;
public:
	// 运用赋值号初始化
	BigNum(const int n = 0) { *this = n; }
	BigNum(const string n) { *this = n; }

	BigNum &operator=(const int &n);
	BigNum &operator=(const string &n);
	BigNum &operator=(const BigNum &n);
};

BigNum &BigNum::operator=(const int &n)
{
	int temp;

	num.erase(num.begin(), num.end()); // 数据清空
	// 确定符号，并将数变为正
	if (n < 0)
	{
		symbol = true;
		temp = -n;
	}
	else
	{
		symbol = false;
		temp = n;
	}
	// 开始填入数值
	while (temp >= 10)
	{
		num.push_back(temp % 10);
		temp /= 10;
	}
	num.push_back(temp);
	// int没有前置0，所以不用去除

	return *this;
}

BigNum &BigNum::operator=(const string &n)
{
	num.erase(num.begin(), num.end()); // 数据清空
	// 填入除首位以外的数
	for (string::size_type i(n.size() - 1); i>0; i--)
		num.push_back(n[i] - '0');
	// 判断符号
	if (n[0] == '-') symbol = true;
	else
	{
		symbol = false;
		num.push_back(n[0] - '0');
	}
	// 去除前置0并确保至少留一位
	for (vector <int>::size_type i(num.size() - 1); num[i] == 0 && i>0; i--)
		num.pop_back();

	return *this;
}

BigNum &BigNum::operator=(const BigNum &n)
{
	num.erase(num.begin(), num.end()); // 数据清空
	symbol = n.symbol;
	for (vector <int>::size_type i(0); i<n.num.size(); i++)
		num.push_back(n.num[i]);

	return *this;
}

ostream &operator<<(ostream &os, const BigNum &n)
{
	if (n.symbol == true) os << '-';
	for (vector <int>::size_type i(n.num.size() - 1); i>0; i--)
		os << n.num[i];
	os << n.num[0]; // 为防止出错，最后一位单独输出

	return os;
}

istream &operator>>(istream &is, BigNum &n)
{
	string temp;
	is >> temp;
	n = temp;

	return is;
}

BigNum operator+(const BigNum a, const BigNum b)
{
	BigNum c;

	// 先确定符号，同号相加，异号相减
	if (a.symbol == b.symbol) c.symbol = a.symbol;
	else if (a.symbol)
		return b - (-a);
	else
		return a - (-b);

	vector <int>::size_type lenA(a.num.size()), lenB(b.num.size()), lenC;
	lenC = (lenA > lenB ? lenA : lenB);

	for (vector <int>::size_type i(0); i<lenC; i++)
	{
		int temp(c.num[c.num.size() - 1]); // 取出末尾的数
		if (i < lenA) temp += a.num[i];
		if (i < lenB) temp += b.num[i];
		c.num[c.num.size() - 1] = temp % 10;
		c.num.push_back(temp / 10); // 进位
	}
	if (c.num[lenC] == 0) c.num.pop_back(); // 若最后没有进位，则舍弃前置0

	return c;
}

BigNum operator+=(BigNum &a, const BigNum b)
{
	a = a + b;
	return a;
}

BigNum operator-(BigNum a)
{
	if (a != 0) a.symbol = !a.symbol; // 变为负数，注意0的符号
	return a;
}

BigNum operator-(BigNum a, BigNum b)
{
	BigNum c, big, small;

	// 先确定符号
	if (a < b) c.symbol = true;
	else c.symbol = false;
	if (a.symbol == b.symbol) // 同号绝对值相减
	{
		a.symbol = b.symbol = false;
		if (a < b)
		{
			small = a; big = b;
		}
		else
		{
			small = b; big = a;
		}

		vector <int>::size_type lon(big.num.size()), sho(small.num.size()), len;
		
		for (vector <int>::size_type i(0); i<lon; i++)
		{
			int temp(c.num[c.num.size() - 1] + 10);
			if (i < sho) temp += (big.num[i] - small.num[i]);
			else temp += big.num[i];
			c.num[c.num.size() - 1] = temp % 10;
			c.num.push_back(temp / 10 - 1);
		}
		// 去除前置0
		len = c.num.size();
		while (len--)
		{
			if (c.num[len] == 0) c.num.pop_back();
			else break;
		}
		if (c.num.empty()) c.num.push_back(0); // 确保至少留一位
	}
	else // 异号绝对值相加
	{
		a.symbol = b.symbol = false;
		c.num = (a + b).num;
	}

	return c;
}

BigNum operator-=(BigNum &a, const BigNum b)
{
	a = a - b;
	return a;
}

BigNum operator*(const BigNum a, const BigNum b)
{
	BigNum c;
	vector <int>::size_type lenA(a.num.size()), lenB(b.num.size()), len(lenA + lenB - 1);

	c.num.insert(c.num.begin(), len, 0); // 初始化0
	for (vector <int>::size_type i(0); i<lenA; i++)
	{
		for (vector <int>::size_type j(0); j<lenB; j++)
		{
			c.num[i + j] += a.num[i] * b.num[j];
			c.num[i + j + 1] += c.num[i + j] / 10;
			c.num[i +j] %= 10;
		}
	}
	if (c.num[len] == 0) c.num.pop_back();
	if (c != 0) c.symbol = a.symbol ^ b.symbol; // 同假异真

	return c;
}

BigNum operator*=(BigNum &a, const BigNum b)
{
	a = a * b;
	return a;
}

BigNum operator/(const BigNum a, BigNum b)
{
	if (b == 0 || a == 0) return 0; // 除数为0的情况
	BigNum temp1, temp2;
	bool flag = a.symbol ^ b.symbol; // 暂存符号
	b.symbol = false;
	string s(a.num.size(), '0'); // 存放商
	vector <int>::size_type i(a.num.size());
	string::size_type j(0);

	while (i--)
	{
		if (temp1 == 0) temp1.num.pop_back();
		temp1.num.insert(temp1.num.begin(), a.num[i]); // 往后取一位
		int k(10); // 试商
		while (k--)
		{
			temp2 = temp1 - b * k;
			if (temp2 >= 0)
			{
				s[j] += k;
				temp1 = temp2; // 保留余数
				break;
			}
		}
		j++;
	}

	BigNum c(s); // 构造函数自动去除前置0
	c.symbol = flag; // 归还符号
	return c;
}

BigNum operator/=(BigNum &a, const BigNum b)
{
	a = a / b;
	return a;
}

BigNum operator%(const BigNum a, BigNum b)
{
	if (b == 0 || a == 0) return 0;
	BigNum c, temp;
	b.symbol = false;
	vector <int>::size_type i(a.num.size());

	while (i--)
	{
		if (c == 0) c.num.pop_back();
		c.num.insert(c.num.begin(), a.num[i]);
		int k(10);
		while (k--)
		{
			temp = c - b * k;
			if (temp >= 0)
			{
				c = temp; // c就是余数
				break;
			}
		}
	}
	c.symbol = a.symbol;
	return c;
}

BigNum operator%=(BigNum &a, const BigNum b)
{
	a = a % b;
	return a;
}

BigNum operator^(const BigNum a, BigNum b) // 快速幂运算
{
	BigNum c = 1, base = a;

	while (b != 0)
	{
		if (b.num[0] % 2) c *= base;
		base *= base; b /= 2;
	}

	return c;
}

BigNum operator++(BigNum a)
{
	a = a + 1;
	return a;
}

BigNum operator++(BigNum &a, int n)
{
	BigNum b = a;
	a = a + 1;
	return b;
}

BigNum operator--(BigNum a)
{
	a = a - 1;
	return a;
}

BigNum operator--(BigNum &a, int n)
{
	BigNum b = a;
	a = a - 1;
	return b;
}

bool operator<(const BigNum a, const BigNum b)
{
	vector <int>::size_type lenA(a.num.size()), lenB(b.num.size());

	if (a.symbol && !b.symbol) return true; // a负b正
	if (!a.symbol && b.symbol) return false; // a正b负
	if (lenA != lenB) // 长度不相等
	{
		if (!a.symbol) return lenA < lenB;
		else return lenA > lenB;
	}
	for (vector <int>::size_type i(lenA - 1); i>0; i--)
	{
		if (a.num[i] != b.num[i])
		{
			if (!a.symbol) return a.num[i] < b.num[i];
			else return a.num[i] > b.num[i];
		}
	}
	if (!a.symbol) return a.num[0] < b.num[0];
	else return a.num[0] > b.num[0];
}

bool operator>(const BigNum a, const BigNum b)
{
	return b < a;
}

bool operator==(const BigNum a, const BigNum b)
{
	return !(a != b);
}

bool operator<=(const BigNum a, const BigNum b)
{
	return !(a > b);
}

bool operator>=(const BigNum a, const BigNum b)
{
	return !(a < b);
}

bool operator!=(const BigNum a, const BigNum b)
{
	return (a < b || a > b);
}

#endif
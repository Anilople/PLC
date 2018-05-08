#include<iostream>
#include<vector>
#include<string>
using namespace std;
#pragma once

template<typename T>
pair<bool, T> operator!(pair<bool, T> a)
{
	a.first = !a.first;
	return a;
}

// template<typename T>
// ostream & operator<<(ostream & os,const pair<bool, T> &a)
// {
	// if (a.first == false)
	// {
		// os << "! ";
	// }
	// os << a.second;
	// return os;
// }

void orld()
{
	cout << "ORLD" << endl;
}

void andld()
{
	cout << "ANDLD" << endl;
}

template<typename T>
void ld(T number)
{
	cout << "LD " << number << endl;
	
}

template<typename T>
void ldnot(T number)
{
	cout << "LDNOT " << number << endl;
	 
}

template<typename T>
pair<bool,T> ld(pair<bool,T> it)
{
	if(it.first) 
		ld(it.second);
	else
		ldnot(it.second);
	return it;
}

template<typename T>
void out(T number)
{
	cout << "OUT " << number << endl;
	 
}

template<typename T>
void outnot(T number)
{
	cout << "OUTNOT " << number << endl;
	 
}

template<typename T>
pair<bool, T> out(pair<bool, T> it)
{
	if (it.first)
		out(it.second);
	else
		outnot(it.second);
	return it;
}

template<typename T>
void and(T number)
{
	cout << "AND " << number << endl;
	 
}

template<typename T>
void and(T n1,T n2)
{
	and(n1);
	and(n2);
	 
}

template<typename T>
void and(T n1, T n2, T n3)
{
	and (n1);
	and (n2);
	and (n3);
	 
}

template<typename T>
void andnot(T number)
{
	cout << "ANDNOT " << number << endl;
	 
}

template<typename T>
void andnot(T n1,T n2)
{
	andnot(n1);
	andnot(n2);
	 
}

template<typename T>
void andnot(T n1, T n2,T n3)
{
	andnot(n1);
	andnot(n2);
	andnot(n3);
	 
}

template<typename T>
void andnot(T n1, T n2, T n3,T n4)
{
	andnot(n1);
	andnot(n2);
	andnot(n3);
	andnot(n4);
}

template<typename T>
pair<bool, T> and (pair<bool, T> it)
{
	if (it.first)
		and (it.second);
	else
		andnot(it.second);
	return it;
}

template<typename T>
void or(T number)
{
	cout << "OR " << number << endl;
	 
}

template<typename T>
void or(T n1,T n2)
{
	or (n1);
	or (n2);
	 
}

template<typename T>
void or (T n1, T n2,T n3)
{
	or (n1);
	or (n2);
	or (n3);
	 
}

template<typename T>
void ornot(T number)
{
	cout << "ORNOT " << number << endl;
	 
}

template<typename T>
void ornot(T n1,T n2)
{
	ornot(n1);
	ornot(n2);
	 
}

template<typename T>
void ornot(T n1, T n2, T n3)
{
	ornot(n1);
	ornot(n2);
	ornot(n3);
	 
}


template<typename T>
pair<bool, T> or(pair<bool, T> it)
{
	if (it.first)
		or(it.second);
	else
		ornot(it.second);
	return it;
}


void keep(int number)
{
	cout << "KEEP " << number << endl;
	 
}

void keep(const pair<bool, string> &a)
{
	cout << "KEEP " << a.second << endl;
}

template<typename n1,typename n2,typename n3>
void keep(n1 a,n2 b,n3 number)
{
	ld(a);
	ld(b);
	keep(number);
}


// ע��, ����ָ����Ĭ����ǰ׺@

// ����ӷ��ϵĽ�λCY, �Լ������Ľ�λCY
void clc()
{
	cout << "@CLC" << endl;
}


// �ȽϽ������3���Ĵ�����
// P_GT, P_EQ, P_LT
void cmp(string a, string b)
{
	cout << "CMP " << a << " " << b << endl;
}

//#define CP1H

#ifndef CP1H // ��ʹ�õĲ���CP1Hʱ, Ĭ��ʹ��CP2M*

void inc(string a)
{
	cout << "@INC " << a << endl;
}
void dec(string a)
{
	cout << "@DEC " << a << endl;
}
// ������������, c = a + b
void add(string a, string b, string c)
{
	cout << "@ADD " << a << " " << b << " " << c << endl;
}
// ������������, c = a - b
void sub(string a, string b, string c)
{
	cout << "@SUB " << a << " " << b << " " << c << endl;
}
#endif // !CP1H


#ifdef CP1H // ���õ���CP1Hʱ

void inc(string a)
{
	cout << "@++B " << a << endl;
}

void dec(string a)
{
	cout << "@--B " << a << endl;
}

// ������������, c = a + b
void add(string a, string b, string c)
{
	cout << "@+BC " << a << " " << b << " " << c << endl;
}

// ��enable����������, c = a - b
void sub(string a, string b, string c)
{
	cout << "@-BC " << a << " " << b << " " << c << endl;
}

#endif // CP1H

class PLC
{
public:
	PLC(bool a) { cout << "cannot use bool value" << endl; throw("bool value ERROR"); }
	PLC(int a) { data = pair<bool, string>(true, to_string(a)); }
	PLC(const string &a) { data = pair<bool, string>(true, a); }
	PLC(pair<bool,int> a) { data = pair<bool, string>(a.first, to_string(a.second)); }
	PLC(pair<bool, string> a) :data(a) {}
	PLC(const PLC &a) { data = a.data; }
	PLC() { PLC(0); }
	void out() const { ::out(data); }
	void ld() const { ::ld(data); }
	void keep() const { ::keep(data); }
	pair<bool,string> getData() const { return data; }
	friend PLC operator!(const PLC &a)
	{
		return PLC(!a.data);
	}
	friend ostream& operator<<(ostream & os, const PLC &a)
	{
		if (!a.data.first) os << "!";
		os << a.data.second;
		return os;
	}
private:
	pair<bool, string> data;
};


class PLCEXP
{
public:
	PLCEXP(){}
	PLCEXP(int a) { node = new PLC{ a }; isLeaf = true; left = right = NULL;}
	PLCEXP(const string &a) { node = new PLC{ a }; isLeaf = true; left = right = NULL; }
	// PLCEXP(PLC a) { node = new PLC(a); isLeaf = true; left = right = NULL;}
	PLCEXP(char ope,PLCEXP left,PLCEXP right) 
	{
		operation = ope; 
		isLeaf = false; 
		node = NULL;
		this->left = new PLCEXP(left);
		this->right = new PLCEXP(right);
	}
	void display() const
	{
		if (isLeaf) // ��Ҷ�ӽڵ�
		{
			cout <<
				"leaf node -- value: "
				<< *node
				<< endl;
		}
		else
		{
			cout <<
				"not leaf node -- ope: "
				<< operation
				<< endl;
			if (left) cout << "left: " << *left->node << endl;
			else cout<< "left: NULL" << endl;
			if (right) cout << "right: " << *right->node << endl;
			else cout << "right: NULL" << endl;
		}
	}
	void ld() const { node->ld(); }
	void keep() const { node->keep(); }
	void codegen() const { LRD(); }
	bool isLeafNode() const { return this->isLeaf; }
	PLC getNode() const { return *node; }

	friend PLCEXP operator !(const PLCEXP &a) // ֻ֧�ֶ�Ҷ�ӽڵ���в���
	{
		PLCEXP next = a ; // ����
		if (!next.isLeafNode()) // ����Ҷ�ӽڵ�
		{
			// ���� Ħ������, �޸ĵ�ǰ�ڵ�������, ���Ҷ����ҽڵ�ȡ��
			if (next.operation == '+') // �� -> ��
			{
				next.operation = '*';
			}
			else if (next.operation == '*') // �� -> ��
			{
				next.operation = '+';
			}
			else
			{
				cout << "oprator is " << a.operation << endl;
				throw("There are no operator * +");
			}
			// �ݹ��޸� ���� �ڵ�
			if (next.left) { next.left = new PLCEXP(!*next.left); } // ����ڵ�ȡ��
			if (next.right) { next.right = new PLCEXP(!*next.right); } // ���ҽڵ�ȡ��

			//cout << "It's not leaf node, cannot manipulate with !" << endl;
			//throw("! Error");
		}
		else
		{
			// ע�� a.node ��һ��ָ�� *a.node���ǽڵ�
			PLC reverseNode = !(*next.node); // Ҷ�ӽڵ�ȡ��
			next.node = new PLC(reverseNode);
		}
		return next;
	}

	friend PLCEXP operator + (const PLCEXP &a, const PLCEXP &b) // or
	{
		PLCEXP head('+',a,b); // ����
		return head;
	}

	friend PLCEXP operator * (const PLCEXP &a, const PLCEXP &b) // and
	{
		PLCEXP head('*', a, b); // ����
		return head;
	}

	friend PLCEXP operator>>(const PLCEXP &a, const PLCEXP &b) // ��ߵı��ʽout���ұ�
	{
		// �ұ߱�����Ҷ�ӽڵ�
		if (!b.isLeaf)
		{
			cout << "It's not legal on the right" << endl;
			throw("a=b, b is not leaf");
		}
		a.codegen();
		b.node->out();
		return b;
	}

	friend ostream& operator<<(ostream &os, const PLCEXP &a) // ����ڵ�
	{
		a.display();
		return os;
	}
private:
	bool isLeaf;
	char operation;
	PLC *node;
	PLCEXP *left;
	PLCEXP *right;
	void LRD() const // �������
	{
		if (isLeaf && (left || right))
		{
			cout << "It's left node but with left or right, not legal.";
			return;
		}
		if (left) left->LRD();
		if (right) right->LRD();
		if (isLeaf) // ��Ҷ�ӽڵ�
		{
			node->ld();
		}
		else // ��������ڵ�
		{
			if (operation == '+') orld();
			else if (operation == '*') andld();
		}
		// ����û�� = ��
		//if (operation == '=') node->out;
	}

};


void tim(int name, int time)
{
	cout << "TIM " << name << " #" << time << endl;

}

void tim(string name, int time)
{
	string nameInt = "";
	int isNum = 0;
	while (!('0' <= name[isNum] && name[isNum] <= '9'))
	{
		isNum += 1;
	}
	for (unsigned int i = isNum; i < name.length(); i++)
	{
		nameInt += name[i];
	}
	int nameValue = stoi(nameInt);
	cout << "TIM " << nameValue << " #" << time << endl;
}

void tim(const pair<bool, string> &a, int time)
{
	tim(a.second, time);
}

void tim(const PLC& a, int time)
{
	tim(a.getData(), time);
}

// TIM a #time
void tim(const PLCEXP& a, int time)
{
	if (!a.isLeafNode())
	{
		cout << "It's a leaf node" << endl;
		throw("It is not leaf node");
	}
	else
	{
		tim(a.getNode(), time);
	}
}

// LD(a)
// TIM b #time
void tim(const PLCEXP& a,const PLCEXP& b,int time)
{
	if (a.isLeafNode()) // a��Ҷ�ӽڵ�
	{
		a.ld();
	}
	else // a����Ҷ�ӽڵ�
	{
		a.codegen();
	}
	tim(b,time);
}


// SR ������, S = In1, R = In2, Q = out
void keep(const PLCEXP &In1, const PLCEXP &In2, const PLCEXP &out)
{
	In1.ld();
	In2.ld();
	out.keep();
}
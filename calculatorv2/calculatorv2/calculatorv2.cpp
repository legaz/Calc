// calculatorv2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
//------------------------------------------------------------------------------
bool b1 = false;
bool b2 = false;
int r = 0;
string s1;
int i = 0;
char ch = ' ';
const char write = 'w';
const char read = 't';
const char roman2 = 'R';
const char number = '8';
const char root = 'r';
const char power = 'p';
const char print = ';';
const char help = 'h';
const char quit = 'q';
const char konst = 'k';
const string promt = ">";
const string result = "=";
const string constanta = "let_const";
const char name = 'a';
const char let = 'L';
const string decley = "let";
void calculate();
double expression();
double term();
void clean_up_mess();
double statement();
double get_value(string s);
void set_value(string s, double v);
bool is_declared(string s);
double define_name(string var,double val);
vector<Roman>roman = { *new Roman('X',10),*new Roman('I',1),*new Roman('C',50),*new Roman('V',5) };
vector<int>r_value;
vector<char>ch3;
class Token {
public:
	char kind;        // what kind of token
	double value; // for numbers: a value 
	string name;
	bool konst;
	Token(char ch)    // make a Token from a char
		:kind(ch), value(0) { }
	Token(char ch, double val)     // make a Token from a char and a double
		:kind(ch), value(val) { }
	Token(char c, string s) :name(s), kind(c) {};
	Token(char ch, string name, bool k) :kind(ch), name(name), konst(k) {};
};
vector<Token>Tc;
class Variable
{
public:
	string name;
	double value;
	Variable(string n1, double v1) :name(n1), value(v1) {};
};
vector<Variable>var_table;
double get_value(string s)
{
	for (const Variable&v1 : var_table)
	{
		if (v1.name == s)
		{
			return v1.value;
		}
	}
	cerr << "Greska nije nadjena vrednost!" << endl;
}
void set_value(string s, double v)
{
	for (Variable&v1 : var_table)
	{
		if (v1.name == s)
		{
			v1.value = v;
			return;
		}
	}
	cerr << "Greska nije nadjena promenjljiva!" << endl;
}
bool is_declared(string s)
{
	for (const Variable& v1 : var_table)
	{
		if (v1.name == s)
			return true;
		else
			return false;
	}
}
double define_name(string var, double val)
{
	if (is_declared(var))
	{
		cerr << "Greska vec definisana promenjljiva!" << endl;
		exit(1);
	}
	var_table.push_back(Variable(var, val));
	return val;
}
class Token_stream {
public:
	Token_stream();   // make a Token_stream that reads from cin
	Token get();      // get a Token (get() is defined elsewhere)
	void putback(Token t); // put a Token back
	void ignore(char c);
private:
	bool full;        // is there a Token in the buffer?
	Token buffer;     // here is where we keep a Token put back using putback()
};
// The constructor just sets full to indicate that the buffer is empty:
Token_stream::Token_stream()
	:full(false), buffer(0)    // no Token in buffer
{
}
// The putback() member function puts its argument back into the Token_stream's buffer:
void Token_stream::putback(Token t)
{
	if (full) cerr << "putback() into a full buffer";
	buffer = t;       // copy t to buffer
	full = true;      // buffer is now full
}
Token_stream ts;
Token Token_stream::get()
{
	if (full) {       // do we already have a Token ready?
		// remove token from buffer
		full = false;
		return buffer;
	}
	if(!b2)cin >> ch;
		switch (ch) {
			//case write:
		case print:    // for "print"
		case quit:
		case help:// for "quit"
		case '(': case ')': case '+': case '-': case '*': case '/':case '{': case '}':case '%':case'=':case',':
			return Token(ch);        // let each character represent itself
		case '.':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '9':
		{
			cin.putback(ch);         // put digit back into the input stream
			double val;
			cin >> val;              // read a floating-point number
			return Token(number, val);   // let '8' represent "a number"
		}
		case '!':
		{
			int val;
			cin >> val;
			int sum = 1;
			int i = val;
			while (i > 0)
			{
				sum *= i--;
			}
			val = sum;
			return Token(number, val);
		}
		default:
			if (isalpha(ch) || ch == '_') {
				string s;
				s += ch;
				while (cin.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) s += ch;
				cin.putback(ch);
				if (s == constanta)
				{
					b1 = true;
					return Token{ konst };
				}
				r_value = *new vector<int>(s.size());
				if (s[0] == 'X' || s[0] == 'V' || s[0] == 'I' || s[0] == 'C')
				{
					for (int i = 0; i != s.size(); ++i)
					{
						for (int j = 0; j != roman.size(); j++)
						{
							if (s[i] == roman[j].get_ime())
							{
								r_value[i] = roman[j].get_vrednost();
							}
						}
						if (i == s.size() - 1)
						{
							return Token{ roman2 };
						}
					}
				}
				//Magic constants not good!
				if (s == "from_x")
				{
					char ch2;
					b2 = true;
					ifstream fi{ "x.txt" };
					if (!fi)
					{
						cout << "Greska u otvaranju fajla!";
						return NULL;
					}
					while (fi >> ch2)
					{
						cout << ch2;
						ch3.push_back(ch2);
					}
					//goto pocetak;
					//return Token{ ch2 };
				};
				if (s == "to_y")return Token{ write };
				if (s == decley) return Token{ let }; // declaration keyword
				if (s == "sqrt")return Token{ root };
				if (s == "pow")return Token(power);
				if (b1)
				{
					Tc.push_back(Token{ name,s,true });
					return Token{ name,s,true };
				}
				return Token{ name,s };
			}
			cerr << "Bad token" << endl;
		}
}
void Token_stream::ignore(char c)
// c re pre se nts the kind of Toke n
{
	// first look in buffe r:
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;
	// now search input:
	char ch = 0;
	while (cin >> ch)
		if (ch == c) return;
}
//------------------------------------------------------------------------------
void clean_up_mess()
{
	ts.ignore(print);
}
double declaration()
{
	Token t = ts.get();
	if (t.kind != name) cerr << "name expected in declaration";
	string var_name = t.name;
	//cout << var_name << endl;
	Token t2 = ts.get();
	if (t2.kind != '=') cerr << "= missing in declaration of ";
	double d = expression();
	var_table.push_back(Variable(var_name, d));
	cout << endl;
	return d;
}
double statement()
{
	Token t = ts.get();
	switch (t.kind) {
	case let :case konst:
		return declaration();
		default :
		ts.putback(t);
		return expression();
	}
}
double primary()     // read and evaluate a Primary
{
	Token t = ts.get();
	switch (t.kind) {
	case '(': case '{':   // handle '(' expression ')'
	{
		double d = expression();
		t = ts.get();
		if (t.kind != ')'&&t.kind != '}') cerr<<"')}' expected";
		return d;
	}
	case read:
	{
		cout << "asdasdas" << endl;
	}
	case roman2:
	{
		double sum = 0;
		for (int i = 0; i != r_value.size() - 1; ++i)
		{
			if (r_value[i] < r_value[i + 1])
			{
				cout << "dddd" << endl;
				r_value[i] = r_value[i + 1] - r_value[i];
				r_value[i + 1] = 0;
			}
		}
		for (int i = 0; i != r_value.size(); ++i)
		{
			sum += r_value[i];
			//cout << r_value[i] << ",";
		}
		return sum;
	}
	case root:
	{
		//ts.ignore(t.kind);
		double d = expression();
		return sqrt(d);
	}
	case power:
	{
		t = ts.get();
		if (t.kind != '(')cerr << "( expected!!" << endl;
		cout << "asdasd" << endl;
		double d = expression();
		t = ts.get();
		if (t.kind != ',')cerr << ", expected!!" << endl;
		double i = expression();
		t = ts.get();
		if (t.kind != ')')cerr << ") expected!!" << endl;
		return pow(d, i);
	}
	case name:	
	{
		Token tmp = t;
		t = ts.get();
		if (t.kind == '=')
		{
			for (int j = 0; j != Tc.size(); ++j)
			{
				//cout << Tc[j].name << endl;
				if (tmp.name ==Tc[j].name )
				{
					cerr << "Greska nemozete promeniti konstantnu vrednost!" << endl;
					return get_value(tmp.name);
				}
			}
			double val = expression();
			set_value(tmp.name, val);
			cout << s1 << endl;
			return get_value(tmp.name);
		}
			return get_value(tmp.name)+expression();
	}
	case number:            // we use '8' to represent a number
		return t.value;
	case '–':
		return - primary();
	case '=':
	{
		return primary();
	}
	default:
		cerr<<"primary expected"<<endl;
	}
}
//------------------------------------------------------------------------------
void calculate()
{
	double val = 0;
	try
	{
		while (cin) {
			cout << promt;
			Token t = ts.get();
			while (t.kind == print)
			{
				t = ts.get();
				//cout << "=" << statement() << endl;
			}
			if (t.kind == help)
			{
				cout << "Dobrodosli u kalkulator :)" << endl;
				cout << "za definisanje promenjljivih koristite kljucnu rec let a za konstante let_const." << endl;
				cout << "Za stampanje rezultata koristite ';' a za izlazak 'q'." << endl;
				continue;
			}
			if (t.kind == quit) /*break;*/exit(1);
			/*if (t.kind == read)
			{
				ifstream fi{ "x.txt" };
				if (!fi)
				{
					cout << "Greska u otvaranju fajla!";
					ts.putback(t);
				}
				while (fi.get(ch))
				{

				}
			}*/
			// ';' for "print now"
			ts.putback(t);
			cout << result << statement() << '\n';
		}
	}
	catch(exception e)
	{
		cerr << e.what() << endl;
		clean_up_mess();
	}
}
double expression()
{
	double left = term();      // read and evaluate a Term
	Token t = ts.get();     // get the next token
	while (true) {
		switch (t.kind) {
		case '+':
			left += term();    // evaluate Term and add
			t = ts.get();
			break;
		case '-':
			left -= term();    // evaluate Term and subtract
			t = ts.get();
			break;
		default:
			ts.putback(t);
			return left;       // finally: no more + or -: return the answer
		}
	}
}
double term()
{
	double left = primary();
	Token t = ts.get();     // get the next token
	while (true) {
		switch (t.kind) {
		case '*':
			left *= primary();
			t = ts.get();
			break;
		case '/':
		{
			double d = primary();
			if (d == 0) cerr << "divide by zero" << endl;
			left /= d;
			t = ts.get();
			break;
		}
		case '%':
		{ double d = primary();
		if (d == 0) cerr << "divide by zero" << endl;
		left = fmod(left, d);
		t = ts.get();
		break;
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}
int main()
{
	try
	{
		//define_name("k", 1000);
		calculate();
		return 0;
	}
	catch (exception& e) {
		cerr << "error: " << e.what() << '\n';
		//keep_window_open();
		return 1;
	}
	catch (...) {
		cerr << "Oops: unknown exception!\n";
		//keep_window_open();
		return 2;
	}
}

//------------------------------------------------------------------------------




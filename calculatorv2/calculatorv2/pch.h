#ifndef PCH_H
#define PCH_H
#pragma once
class Roman
{
	char ime;
	double vrednost;
public:
	Roman(char i, double v) :ime(i), vrednost(v) {};
	char get_ime() { return this->ime;}
	double get_vrednost() { return this->vrednost; }
};

#endif //PCH_H

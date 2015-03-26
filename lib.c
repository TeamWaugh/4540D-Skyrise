#include <math.h>


double favg(double a,double b) {
	return (a+b)/2;
}

float favgf(float a,float b) {
	return (a+b)/2;
}

double fclamp(double n,double min,double max) {
	return n>max?max:(n<min?min:n);
}

float fclampf(float n,float min,float max) {
	return n>max?max:(n<min?min:n);
}

double fround(double a,double n) {
	return floor(a/n)*n;
}

float froundf(float a,float n) {
	return floorf(a/n)*n;
}

int avg(int a,int b) {
	return (a+b)/2;
}

int min(int a,int b) {
	return a>b?b:a;
}

int max(int a,int b) {
	return a<b?b:a;
}

int clamp(int n,int min,int max) {
	return n>max?max:(n<min?min:n);
}

int mod(int a,int b) {
	return a%b>=0?a%b:(a%b)+b;
}

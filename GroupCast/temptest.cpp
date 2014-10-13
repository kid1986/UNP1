#include <iostream>
#include <stdio.h>
using namespace std;

class Base
{
private:
		int x;
protected:
		int m;
public:
		virtual void mf1() = 0;
		virtual void mf1(int){printf("This Base::mf1(int).\n");}
		virtual void mf2(){printf("This Base::mf2().\n");};
		void mf3(){printf("This Base::mf3().\n");};
		void mf3(double){printf("This Base::mf3(double).\n");};
};

class Derived: public Base
{
public:
		using Base::mf1;
		using Base::mf3;
		virtual void mf1(){printf("This Derived::mf1().\n");};
		void mf3(){printf("This Derived::mf3().\n");}
		void mf4(){printf("This Derived::mf4().\n");}
		void setVal(int _x){m=_x;cout<<"x:"<<m<<endl;;}
};

int main()
{
	Derived d;
	int x = 10;
	d.mf1();
	d.mf1(x);
	d.mf2();
	d.mf3();
	d.mf3(x);
	d.setVal(18);

	return 0;	
}

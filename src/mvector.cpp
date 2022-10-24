//============================================================================
// Name        : mvector.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include "mvector.h"

using namespace std;

class MPod
{
public:
    int a;
};

class MSome: protected MPod
{
public:
    MSome() : a(7) {}
    virtual ~MSome() {}
    virtual int foo() {return a;}
    int a;
};

using namespace std;

int main1(int argc, char **argv)
{
    //  check creation and size
    // 1 fundamental type
    Mvector<int> mv1_1;
    Mvector<int> mv1_2(10);
    Mvector<int> mv1_3 = mv1_2;
    Mvector<int> mv1_4(mv1_2);
    Mvector<int> mv1_5 {0,1,2,3,4,5,6,7,8,9};

    cout << "Creation" << endl;
    cout << " Mvector<fundamental type, elements>: ";
    //std::for_each(mv1_5.begin(), mv1_5.end(), [](const int n) { std::cout << n << ' '; });
    cout << endl;


    // 2 POD type
    Mvector<MPod> mv2_1;
    Mvector<MPod> mv2_2(10);
    Mvector<MPod> mv2_3 = mv2_2;
    Mvector<MPod> mv2_4(mv2_2);
    cout << " Mvector<POD type> mv2_4.size: " << mv2_4.size() << endl;


    // 3 non POD type
    Mvector<MSome> mv3_1;
    Mvector<MSome> mv3_2(10);
    Mvector<MSome> mv3_3 = mv3_2;
    Mvector<MSome> mv3_4(mv3_2);
    cout << " Mvector<nonPOD type>, mv3_4.size: " << mv3_4.size() << endl;


    //Element access
    cout << "Element access" << endl;
    cout << " mv1_5[3]: " << (mv1_5[3]) << endl;


    //Capacity
    cout << "Capacity" << endl;
    cout << " mv1_5.size(): "     << mv1_5.size() << endl;
    cout << " mv1_5.capacity(): " << mv1_5.capacity() << endl;
    cout << " mv1_5.empty(): "    << (mv1_5.empty()?"true":"false") << endl;

    mv1_5.reserve(32);
    cout << " after mv1_5.reserve(32) " << "mv1_5.size: " << mv1_5.size() << "; "
         << " mv1_5.capacity(): " << mv1_5.capacity() << endl;


    //Modifiers
    cout << "Modifiers" << endl;

    // resize
    mv1_5.resize(4);
    cout << " mv1_5.resize(4) mv1_5.size: " << mv1_5.size() << "; "
         << " mv1_5.capacity(): " << mv1_5.capacity() << endl;

    // clear
    mv1_5.clear();
    cout << " mv1_5.clear() mv1_5.size: " << mv1_5.size() << "; "
         << " mv1_5.capacity(): " << mv1_5.capacity() << endl;

    //push_back
    mv1_5.push_bask(5);
    mv1_5.push_bask(6);
    mv1_5.push_bask(7);
    cout << " mv1_5.pushback() 5, 6, 7  mv1_5.size: " << mv1_5.size() << "; "
         << " mv1_5.capacity(): " << mv1_5.capacity() << endl;

    //emplace
    mv1_5.emplace(2, 9);
    cout << " mv1_5.emplace(2,9)  mv1_5.size: " << mv1_5.size() << "; "
             << " mv1_5.capacity(): " << mv1_5.capacity() << endl;

    std::for_each(mv1_5.begin(), mv1_5.end(), [](const int n) { std::cout << n << ' '; });
    cout << endl;


    // iterator
    cout << "Iterator" << endl;
    std::iota(mv1_3.begin(), mv1_3.end(), 7);
    cout << " iota(mv1_3.begin(), mv1_3.end(), 7); for_each: ";
    std::for_each(mv1_3.begin(), mv1_3.end(), [](const int n) { std::cout << n << ' '; });
    cout << endl;


//    Mvector<int> mva {1,2,3};
//    Mvector<int>::const_iterator cit = mva.cbegin();


	cout << "hello " << endl; // prints hello
	return 0;
}

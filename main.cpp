#include "Wrapper.h"



/* g++ -I"C:\opencv\build\include" -I"C:\MinGW\include" -L"C:\opencv\mingw64\lib" main.cpp -g C:\MinGW\lib\libboost_thread.a C:\MinGW\lib\libboost_system.a  -lopencv_core249 -lopencv_highgui249 -std=c++14 */



using namespace std;




int main ()
{
    vector<uchar> v = {3, 5, 1, 4, 2};

    vector<wrp::Wrapper<uchar&>> u;
    vector<wrp::Wrapper<uchar>> w;

    for(int i = 0; i < v.size(); ++i)
    {
        u.emplace_back(v[i]);
        w.emplace_back(v[i]);
    }


    std::sort(u.begin(), u.end(), [](const auto& x, const auto& y){ return x.t < y.t; });
    std::sort(w.begin(), w.end(), [](const auto& x, const auto& y){ return x.t < y.t; });


    //for(int i = 0; i < u.size(); ++i) u[i] += 10;

    for(int i = 0; i < u.size(); ++i) cout << int(u[i]) << endl;
    cout << endl;

    for(int i = 0; i < w.size(); ++i) cout << int(w[i]) << endl;

    return 0;
}

#include "Wrapper.h"

using namespace std;

/* g++ -I"C:\opencv\build\include" -I"C:\MinGW\include" -L"C:\opencv\mingw64\lib" main.cpp -g C:\MinGW\lib\libboost_thread.a C:\MinGW\lib\libboost_system.a  -lopencv_core249 -lopencv_highgui249 -std=c++14 */



int main ()
{
    //wrp::Wrapper<cv::Vec<char, 3>>() * wrp::Wrapper<cv::Vec<double, 3>>();


    //wrp::Wrapper<int>(wrp::Wrapper<char>());

    auto x = wrp::Wrapper<cv::Vec<int, 3>>(1, 2, 3) * wrp::Wrapper<cv::Vec<double, 3>>(0.5, 2.4, 3.3);

    cout << x << endl;


    //wrp::impl::PrintType<decltype(x)>();


    //cout << x << endl;


    //auto x = w[20];

    return 0;
}

#include "Wrapper.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>



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


    for(int i = 0; i < u.size(); ++i) u[i] += 10;

    for(int i = 0; i < u.size(); ++i) cout << u[i] << endl;
    cout << endl;

    for(int i = 0; i < w.size(); ++i) cout << w[i] << endl;







    /*vector<wrp::Wrapper<cv::Vec3b>> v = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} };
    vector<wrp::Wrapper<cv::Vec3d>> u = { {0.0, 0.1, 0.2}, {0.3, 0.4, 0.5}, {0.6, 0.7, 0.8} };
    vector<decltype(v[0] * u[0])> r(v.size());

    std::transform(v.begin(), v.end(), u.begin(), r.begin(), [](const auto& x, const auto& y){ return y * x; });

    for(const auto& x : r)
        cout << x << endl;


    auto x = v[0] + u[0];
    auto y = v[0] + 3;
    auto z = 3 + v[0];
    v[0] /= 2;


    std::sort(v.begin(), v.end(), [](auto&& x, auto&& y){ return x[0] < y[0]; });*/


    return 0;
}

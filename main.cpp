#include <iostream>
#include <mutex>
#include <thread>
#include <cmath>
#include <vector>
using namespace std;

float answer;
//args
int m = 5;
float ci[] = {1, 7, 6, 1, 7};
float a1i[] = {1, 2, 3, 4, 7};
float a2i[] = {1, 2, 3, 4, 7};
float xstart = -1;
float ystart = -1;
float step_x = 1;
float step_y = 1;
float prec = 0.1;
float quantity;
mutex myMutex;

float valueAtPoint(float x_1, float x_2, float c_i[], float a_1i[], float a_2i[], int m) {
    float result=0;
    for (int i=1; i <= m; i++) {
        result += c_i[i] * exp(((-1)/M_PI)*(pow((x_1 - a_1i[i]), 2) + pow((x_2 - a_2i[i]), 2)))*cos(M_PI*(pow((x_1 - a_1i[i]), 2) + pow((x_2 - a_2i[i]), 2)));
    }
    return -result;
}

void integral(float x_1from, float x_1to, float x_2from, float x_2to, float prec) {
    float x1_start = x_1from;
    float area = 0;  // signed area
    while(x1_start <= x_1to) {
        float x2_start = x_2from;

        while(x2_start <= x_2to) {
        area += valueAtPoint((2 * x1_start + prec) / 2, (2 * x2_start + prec) / 2, ci, a1i, a2i, m) * prec * prec;
        cout << "area " << area << endl;

        x2_start += prec;
    }
        x1_start += prec;
    }

    //return area;
    myMutex.lock();
    answer += area;
    myMutex.unlock();
}



int main(int argc, char *argv[])
{
    vector<thread> myThreads    ;

    for (int el=0; el<quantity; el++) {
        myThreads.push_back(std::thread(integral,   xstart, step_x+xstart, ystart, step_y+ystart, prec));
        xstart += step_x;
        ystart += step_y;
    }

    for (int el=0; el<quantity; el++) {
       myThreads[el].join();
    }
    cout << answer;

    return 0;
}

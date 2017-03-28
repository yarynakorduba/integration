#include <iostream>
#include <mutex>
#include <thread>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

map<string, string> readConfigFile(string fileName){
    string line, delimiter, token;
    map<string, string> config;
    delimiter = "=";
    size_t pos = 0;
    ifstream myfile (fileName);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            while ((pos = line.find(delimiter)) != string::npos) {
                token = line.substr(0, pos);
                if (token.compare("abs_accurance")== 0 || token.compare("rel_accurance")== 0){
                    string token_name = token;
                    token = line.substr(pos+1, line.size()-9);
                    cout << token << endl;
                    config[token_name] = token;//::atof(num.c_str());token;
                }
                else if (token.compare("start_y")== 0 ||token.compare("max_x")== 0 || token.compare("start_x")== 0
                         || token.compare("max_y")==0 || token.compare("threads")== 0){
                    string token_name = token;
                    token = line.substr(pos+1, line.size()-1);
                    cout<< token << endl;
                    config[token_name] = token;
                }
                else if (token.compare("c")== 0 || token.compare("a1")== 0 || token.compare("a2")== 0){
                    string token_name = token;
                    token = line.substr(pos+1, line.size());
                    cout<< token << endl;
                    config[token_name]= token;
                }
                else{
                    cout << "NO MATCH"<< endl;
                }
                line.erase(0, pos + delimiter.length());
            }

        }
        myfile.close();
        return config;
    }

    else cout << "Unable to open file";

}

map<string, string> config = readConfigFile("../config");

float answer;
//args
int m = stoi(config.find("thread")->second);
float a1i[] = {1, 2, 3, 4};
float a2i[] = {1, 2, 3, 4};
float ci[] = {1, 7, 6, 1, 7};
double xstart = atof(config.find("start_x")->second.c_str());
double max_x = atof(config.find("max_x")->second.c_str());
double max_y = atof(config.find("max_y")->second.c_str());
double ystart = atof(config.find("start_y")->second.c_str());
//float yfin = 1;
float step_x = 1;
float step_y = 1;
double prec = atof(config.find("abs_accurance")->second.c_str());
double rel_accurance = atof(config.find("rel_accurance")->second.c_str());;
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
    float area = 0;  
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
    //  cout << integral(cos, 0, M_PI/2, 10);
    vector<thread> myThreads    ;
    //  integral(1, 5, 1, 3, 0.1);
    //readConfigFile("../config.txt");
//    for (int el=0; el<2; el++) {
//        myThreads.push_back(std::thread(integral,   xstart, step_x+xstart, ystart, step_y+ystart, prec));
//        xstart += step_x;
//        ystart += step_y;
//    }
//
//    for (int el=0; el<2; el++) {
//        myThreads[el].join();
//    }
//    cout << answer;

    return 0;
}

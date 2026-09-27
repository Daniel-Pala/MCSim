#include <chrono>
#include <random>
#include <string>
#include <numbers>
#include <iostream>
#include <cmath>
#include <iomanip>

using ll = long long;

ll countInside(ll n, unsigned seme){
    std::mt19937 gen(seme);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    ll inside = 0;

    for(ll i = 0; i < n; ++i){
        double X = dist(gen);
        double Y = dist(gen);
        if( X*X + Y*Y < 1) ++inside;

    
    }
    return inside;
}


int main(int argc, char * argv[]){
    if(argc < 2){
        std::cerr << "argument needed\n";
        return 1;
    }
    ll N = std::stoll(argv[1]);

    auto start = std::chrono::steady_clock::now();

    
    ll in = countInside(N, 67);
    double est = 4.0 * in/N;

    double err = std::abs(est - std::numbers::pi);

    

    auto end = std::chrono::steady_clock::now(); 
    std::chrono::duration<double> time = end - start;

    std::cout << est << " " << err << " " << time << "\n";

    return 0; 

}


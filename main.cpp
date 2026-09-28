#include <chrono>
#include <random>
#include <string>
#include <numbers>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <thread>
#include <vector>   

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
    if(argc < 3){
        std::cerr << "arguments needed: ./pi <points> <threads>\n";
        return 1;
    }

    ll N = std::stoll(argv[1]);
    int T = std::stoi(argv[2]);
    if(T < 1){
        std::cerr << "n of threads must be > 0\n";
        return 1;
    }

    std::vector<std::thread> threads;
    std::vector<ll> risultati(T);

    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < T; ++i){
        ll dots = N / T;
        if(i == T - 1) dots += N%T;
        
        //i-thread is created
        threads.emplace_back([&risultati, i, dots]{
            risultati[i] = countInside(dots, 67 + i); });

    }
    for (auto& t : threads) t.join();
    auto end = std::chrono::steady_clock::now(); 

    ll totIn = 0;
    for(ll r : risultati) totIn += r;
    double est = 4.0 * totIn / N;
    double err = std::abs(est - std::numbers::pi);

    

    std::chrono::duration<double> time = end - start;

    std::cout << std::setprecision(10) << T << "," << est <<
        "," << err << "," << time.count() << "\n";

    return 0; 

}


#include <chrono>
#include <random>
#include <string>
#include <numbers>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <thread>
#include <vector>   
#include <atomic> 

/* Modes
 * 1: local counter per thread, written once at the end (baseline)
 * 2: counters in adjacent vector cells, written at every hit (false sharing)
 * 3: counters padded to 64 bytes, one per cache line (false sharing fixed)
 * 4: single std::atomic counter shared by all threads (true sharing)
 */

using ll = long long;
struct alignas(64) PaddedCounter {volatile ll value = 0;};

ll countInside(ll n, unsigned seed){
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    ll inside = 0;

    for(ll i = 0; i < n; ++i){
        //generate random -1 < n < 1
        double X = dist(gen);
        double Y = dist(gen);
        if( X*X + Y*Y < 1) ++inside;

    
    }
    return inside;
}

//threads have to reload cache line at evey write (n writes), only in mode 2
//without "volatile", with -O2 the compiler would just write once in the variable
void countShared(ll n, unsigned seed, volatile ll& res){
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    for(ll i = 0; i < n; ++i){
        double X = dist(gen);
        double Y = dist(gen);
        if (X*X + Y*Y < 1) res = res + 1;
    }

}

void countAtomic(ll n, unsigned seed, std::atomic<ll>& res){
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    for(ll i = 0; i < n; ++i){
        double X = dist(gen);
        double Y = dist(gen);
        //fetch and add executed atomically
        if (X*X + Y*Y < 1) res.fetch_add(1);
    }
}
int main(int argc, char * argv[]){
   
    if(argc != 4){
        std::cerr << "usage: ./pi <points> <threads> <mode>\n"
                  << "mode: 1 local, 2 false sharing, 3 padded, 4 atomic\n";
        return 1;
    }

    ll N = std::stoll(argv[1]);
    int T = std::stoi(argv[2]);
    int mode = std::stoi(argv[3]);
    if(T < 1 || mode < 1 || mode > 4){
        std::cerr << "n of threads must be > 0, mode 1-4\n";
        return 1;
    }
    

    std::vector<std::thread> threads;
    std::vector<ll> results(T);
    std::vector<PaddedCounter> padded(T);
    std::atomic<ll> atomTot{0};

    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < T; ++i){
        ll dots = N / T;
        if(i == T - 1) dots += N%T;
        
        //i-thread is created
        threads.emplace_back([&, i, dots]{
            switch(mode){
            case 1: results[i] = countInside(dots, 67 + i); break;
            case 2: countShared(dots, 67 + i, results[i]); break;
            case 3: countShared(dots, 67 + i, padded[i].value); break;
            case 4: countAtomic(dots, 67 + i, atomTot); break;
            }
        
        });

    }
    for (auto& t : threads) t.join();
    auto end = std::chrono::steady_clock::now(); 

    ll totIn = 0;
    if (mode == 1 || mode == 2)
        for(ll r : results) totIn += r;
    else if(mode == 3) for(const auto &p : padded) totIn += p.value;
    //load() for std::atomic
    else totIn = atomTot.load();

    double est = 4.0 * totIn / N;
    double err = std::abs(est - std::numbers::pi);
    
    std::chrono::duration<double> time = end - start;



    std::cout << std::setprecision(10) << T << "," << est <<
        "," << err << "," << time.count() << "\n";

    return 0; 

}


#include "kelly_game.hpp"
#include <chrono>
#include <cstdio>
#include <cstdlib>

int main(int argc, char** argv) {
    const int players = argc > 1 ? std::atoi(argv[1]) : 20000;
    kelly::Game g;
    auto t0 = std::chrono::steady_clock::now();
    std::printf("fraction,median,p_below_start,p_bust,p_cap\n");
    for (int i = 1; i <= 80; ++i) {
        const double f = i / 100.0;
        auto s = kelly::play(g, f, players, 42 + i);
        std::printf("%.2f,%.4f,%.5f,%.5f,%.5f\n", s.fraction, s.median, s.p_below_start, s.p_bust, s.p_cap);
    }
    const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    std::fprintf(stderr, "%d players x 80 bet sizes x up to %d flips in %.0f ms\n", players, g.flips, ms);
}

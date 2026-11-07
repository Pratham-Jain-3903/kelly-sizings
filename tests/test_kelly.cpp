#include "../src/kelly_game.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>

int main() {
    using namespace kelly;
    // 1. Kelly fraction 2p-1 maximises growth.
    double best = 0, bestf = 0;
    for (int i = 1; i < 100; ++i) { double f = i / 100.0, gr = growth(0.6, f); if (gr > best) { best = gr; bestf = f; } }
    assert(std::fabs(bestf - 0.20) < 1e-9);
    // 2. Half Kelly keeps ~75% of the growth rate.
    double ratio = growth(0.6, 0.10) / growth(0.6, 0.20);
    assert(ratio > 0.73 && ratio < 0.77);
    // 3. Double Kelly has (slightly) negative growth: the typical player loses.
    assert(growth(0.6, 0.40) < 0);
    // 4. Same seed, same result (reproducible chart).
    Game g; auto a = play(g, 0.2, 2000, 7), b = play(g, 0.2, 2000, 7);
    assert(a.median == b.median && a.p_bust == b.p_bust);
    // 5. Uncapped median matches the closed form start * exp(flips * growth) within sampling noise.
    Game u; u.cap = 1e300; u.bust = 1e-300;
    auto s = play(u, 0.10, 20001, 11);
    double closed = u.start * std::exp(u.flips * growth(0.6, 0.10));
    assert(std::fabs(std::log(s.median / closed)) < 0.15);
    std::puts("all 5 tests passed");
}

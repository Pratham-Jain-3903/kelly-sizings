// Haghani-Dewey biased-coin game: 60% heads, 300 flips, $25 start, $250 cap.
// A player bets a fixed fraction f of current wealth on heads every flip.
#pragma once
#include <cstdint>
#include <cmath>
#include <vector>
#include <algorithm>

namespace kelly {

// xoshiro256** -- small, fast, reproducible across platforms (unlike std::mt19937 + distributions).
struct Rng {
    uint64_t s[4];
    explicit Rng(uint64_t seed) {
        for (auto& x : s) {  // splitmix64 seeding
            seed += 0x9e3779b97f4a7c15ULL;
            uint64_t z = seed;
            z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
            z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
            x = z ^ (z >> 31);
        }
    }
    static uint64_t rotl(uint64_t x, int k) { return (x << k) | (x >> (64 - k)); }
    uint64_t next() {
        const uint64_t r = rotl(s[1] * 5, 7) * 9, t = s[1] << 17;
        s[2] ^= s[0]; s[3] ^= s[1]; s[1] ^= s[2]; s[0] ^= s[3]; s[2] ^= t; s[3] = rotl(s[3], 45);
        return r;
    }
    double uniform() { return (next() >> 11) * 0x1.0p-53; }  // [0,1), 53 random bits
};

struct Game { double p = 0.6; int flips = 300; double start = 25.0; double cap = 250.0; double bust = 2.0; };

struct Summary { double fraction, median, p_below_start, p_bust, p_cap; };

// Plays `players` games at bet fraction f. Wealth is tracked in log space so 300
// multiplications don't drift; the cap and bust rules make the game path-dependent,
// which is why we simulate flip by flip instead of using the binomial closed form.
inline Summary play(const Game& g, double f, int players, uint64_t seed) {
    Rng rng(seed);
    const double up = std::log1p(f), down = std::log1p(-f);
    const double lcap = std::log(g.cap), lbust = std::log(g.bust), l0 = std::log(g.start);
    std::vector<double> end(players);
    int bust = 0, capped = 0, below = 0;
    for (int i = 0; i < players; ++i) {
        double lw = l0;
        for (int k = 0; k < g.flips; ++k) {
            lw += (rng.uniform() < g.p) ? up : down;
            if (lw >= lcap) { lw = lcap; ++capped; break; }
            if (lw < lbust) { ++bust; break; }
        }
        end[i] = std::exp(lw);
        if (end[i] < g.start) ++below;
    }
    std::nth_element(end.begin(), end.begin() + players / 2, end.end());
    return {f, end[players / 2], double(below) / players, double(bust) / players, double(capped) / players};
}

// Expected log-growth per flip; maximised at the Kelly fraction f* = 2p - 1.
inline double growth(double p, double f) { return p * std::log1p(f) + (1 - p) * std::log1p(-f); }

}  // namespace kelly

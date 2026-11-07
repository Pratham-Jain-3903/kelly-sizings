"""Chart the C++ results (results.csv) in the notebook's hand-drawn style."""
import csv, matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib import font_manager as fm

FONT = "assets/xkcd-script.ttf"; fm.fontManager.addfont(FONT)
INK, GREY, RED, GREEN = "#1a1a1a", "#8a8a8a", "#c0392b", "#2e8b57"
rows = list(csv.DictReader(open("results.csv")))
f = [float(r["fraction"]) * 100 for r in rows]
cap = [float(r["p_cap"]) * 100 for r in rows]
bust = [float(r["p_bust"]) * 100 for r in rows]
at = lambda x: next(r for r in rows if abs(float(r["fraction"]) - x) < 1e-9)

plt.xkcd(scale=0.8, length=150, randomness=2)
plt.rcParams["font.family"] = fm.FontProperties(fname=FONT).get_name(); plt.rcParams["path.effects"] = []
fig = plt.figure(figsize=(10.8, 13.5), dpi=100); fig.patch.set_facecolor("white")
ax = fig.add_axes([0.12, 0.27, 0.82, 0.54])
ax.axvspan(0, 20, color=GREEN, alpha=0.07); ax.axvspan(40, 80, color=RED, alpha=0.07)
ax.plot(f, cap, c=GREEN, lw=3.2, label="reached the $250 cap")
ax.plot(f, bust, c=RED, lw=3.2, label="went bust (under $2)")
ax.set_xlim(0, 80); ax.set_ylim(0, 100)
ax.set_xlabel("share of your money bet on every flip (%)", fontsize=19)
ax.set_ylabel("% of 20,000 simulated players", fontsize=19)
ax.tick_params(labelsize=15)
for s in ["top", "right"]: ax.spines[s].set_visible(False)
def note(x, y, txt, c, xt, yt):
    ax.plot(x, y, "o", ms=11, color=c, zorder=5)
    ax.annotate(txt, xy=(x, y), xytext=(xt, yt), fontsize=16, color=c, ha="center",
                arrowprops=dict(arrowstyle="->", lw=1.7, color=c))
h, k, d = at(0.10), at(0.20), at(0.40)
note(10, float(h["p_cap"]) * 100, f"half Kelly (10%):\n{float(h['p_cap'])*100:.0f}% hit the cap,\nalmost nobody busts", GREEN, 22, 60)
note(20, float(k["p_cap"]) * 100, f"Kelly (20%):\n{float(k['p_cap'])*100:.0f}% cap, {float(k['p_bust'])*100:.0f}% bust", INK, 38, 76)
note(40, float(d["p_bust"]) * 100, f"double Kelly (40%):\na coin flip whether\nyou go bust", RED, 67, 46)
ax.legend(loc="lower right", fontsize=16, frameon=True, facecolor="white", edgecolor=INK)
fig.text(0.5, 0.94, "A 60% coin. $25. How much do you bet?", fontsize=32, ha="center")
fig.text(0.5, 0.895, "The real game: 300 flips, winnings capped at $250", fontsize=19, ha="center", color=GREY)
fig.text(0.5, 0.135, "In the original study, 28% of 61 finance-trained players went bust.\nBetting a steady 10% would have made busting almost impossible.",
         fontsize=18, ha="center", linespacing=1.5)
fig.text(0.5, 0.04, "simulated in C++ | full write-up and code: prathamjain.in/blog", fontsize=14, ha="center", color=GREY)
fig.savefig("kelly_real_game.png", dpi=100, facecolor="white"); print("ok")

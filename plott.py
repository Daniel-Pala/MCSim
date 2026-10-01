import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("risultati.csv")
tempi = df.groupby("threads")["time"].median()

accelerazione = tempi[1] / tempi                 

plt.plot(accelerazione.index, accelerazione.values, marker="o", label="measured")
plt.plot(accelerazione.index, accelerazione.index, linestyle="--", label="ideal")
plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.legend()
plt.grid(True)
plt.savefig("accelerazione.png", dpi=150)
plt.show()

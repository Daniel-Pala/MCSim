import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("risultati.csv")
tempi = df.groupby("threads")["time"].median()

accelerazione = tempi[1] / tempi                 

plt.plot(accelerazione.index, accelerazione.values, marker="o", label="misurata")
plt.plot(accelerazione.index, accelerazione.index, linestyle="--", label="ideale")
plt.xlabel("Thread")
plt.ylabel("Accelerazione")
plt.legend()
plt.grid(True)
plt.savefig("accelerazione.png", dpi=150)
plt.show()

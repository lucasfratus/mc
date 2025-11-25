import re
import matplotlib.pyplot as plt

# ===============================
# LEITURA DO ARQUIVO
# ===============================
sin_x = []
sin_erro = []

exp_x = []
exp_erro = []

with open("dados.txt") as f:
    for linha in f:
        # Captura linhas do seno
        if "erro_sin" in linha:
            # exemplo da linha: x=-3.141593  erro_sin=1.234567e-10
            m = re.search(r"x=([+-]?\d+\.\d+)\s+erro_sin=([+-]?\d\.\d+e[+-]?\d+)", linha)
            if m:
                x_val = float(m.group(1))
                e_val = float(m.group(2))
                sin_x.append(x_val)
                sin_erro.append(e_val)

        # Captura linhas do exponencial
        if "erro_rel" in linha:
            # exemplo: x=  1.00  my= ... ref= ...  erro_rel= 1.23e-15
            m = re.search(r"x=\s*([+-]?\d+\.\d+).*erro_rel=\s*([+-]?\d\.\d+e[+-]?\d+)", linha)
            if m:
                x_val = float(m.group(1))
                e_val = float(m.group(2))
                exp_x.append(x_val)
                exp_erro.append(e_val)

# ===============================
# GRÁFICO DO ERRO DO SENO
# ===============================
plt.figure(figsize=(9,5))
plt.plot(sin_x, sin_erro, color='red')
plt.yscale('log')
plt.grid(True)
plt.xlabel("x")
plt.ylabel("Erro absoluto (log)")
plt.title("Erro absoluto |sin_aprox(x) - sin(x)|")
plt.tight_layout()
plt.savefig("erro_seno.png", dpi=300)
plt.show()

# ===============================
# GRÁFICO DO ERRO DO EXPONENCIAL
# ===============================
plt.figure(figsize=(9,5))
plt.plot(exp_x, exp_erro, color='blue')
plt.yscale('log')
plt.grid(True)
plt.xlabel("x")
plt.ylabel("Erro relativo (log)")
plt.title("Erro relativo |exp_bailey(x)/exp(x)| - 1")
plt.tight_layout()
plt.savefig("erro_exp.png", dpi=300)
plt.show()

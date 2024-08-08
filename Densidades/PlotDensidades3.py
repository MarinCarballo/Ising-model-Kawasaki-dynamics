import pandas as pd
import matplotlib.pyplot as plt

# Nombre del archivo
filename = 'C:/Users/anton/OneDrive/Escritorio/A.MarinCarballoCompu2324/A.MarinCarballoCompu2324/Voluntario1/Densidad3.csv'

# Leer el archivo CSV
# La primera columna es el tamaño (y), la segunda es el error y las siguientes son las densidades para cada temperatura
data = pd.read_csv(filename, header=None, names=['size', 'error', 'T1', 'Tc', 'T3', 'T4', 'T5'])

# Crear la gráfica
plt.figure(figsize=(10, 6))

# Temperaturas y etiquetas
temperaturas = ['T1', 'Tc', 'T3', 'T4', 'T5']
etiquetas = ['T = 1 K', 'T = Tc (2√2)', 'T = 3 K', 'T = 4 K', 'T = 5 K']

# Graficar las curvas para cada temperatura
for i, temp in enumerate(temperaturas):
    plt.errorbar(data['size'], data[temp], yerr=data['error'], fmt='-', linewidth=1, capsize=5, elinewidth=2, label=etiquetas[i])

# Añadir etiquetas y título
plt.xlabel('y')
plt.ylabel('Densidad (ρ)')
plt.title('Densidad ρ en función de y, N=32')
plt.legend()

# Mostrar la gráfica
plt.grid(True)
plt.show()

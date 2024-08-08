import pandas as pd
import matplotlib.pyplot as plt

# Cargar datos desde el archivo CSV
filename = 'C:/Users/anton/OneDrive/Escritorio/A.MarinCarballoCompu2324/A.MarinCarballoCompu2324/Voluntario1/Energia.txt'
data = pd.read_csv(filename, header=None, names=['Energia', 'ErrorEnergia', 'T (K)'])

# Graficar los datos
plt.figure(figsize=(10, 6))

# Graficar la energía en función de la temperatura con barras de error
plt.errorbar(data['T (K)'], data['Energia'], yerr=data['ErrorEnergia'], fmt='-', linewidth=1, capsize=5, elinewidth=1, label='Energía', color='blue')

# Añadir etiquetas y título
plt.xlabel('Temperatura (K)')
plt.ylabel('Energía')
plt.title('Energía vs Temperatura, N=128')
plt.legend()

# Mostrar la gráfica
plt.grid(True)
plt.show()

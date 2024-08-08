import pandas as pd
import matplotlib.pyplot as plt

# Cargar los datos desde un archivo CSV
# Asegúrate de cambiar 'tu_archivo.csv' por el nombre de tu archivo
filename = 'C:/Users/anton/OneDrive/Escritorio/A.MarinCarballoCompu2324/A.MarinCarballoCompu2324/Voluntario1/Densidad.txt'
data = pd.read_csv(filename, header=None, names=['densidad_s1', 'error_s1', 'densidad_s_1', 'error_s_1', 'T'])
# Asumimos que las columnas del archivo son:
# densidad_s1, error_s1, densidad_s-1, error_s-1, T

# Calcular la densidad total
data['densidad_total'] = data['densidad_s1'] + data['densidad_s_1']

# Crear la gráfica
plt.figure(figsize=(10, 6))
plt.errorbar(data['T'], data['densidad_s1'], yerr=data['error_s1'], fmt='-', linewidth=1, capsize=5, elinewidth=2, label='Densidad s=+1')
plt.errorbar(data['T'], data['densidad_s_1'], yerr=data['error_s_1'], fmt='-', linewidth=1, capsize=5, elinewidth=2, label='Densidad s=-1')
plt.plot(data['T'], data['densidad_total'], 'go--', label='Densidad Total')

# Añadir etiquetas y título
plt.xlabel('T (K)')
plt.ylabel('Densidad')
plt.title('Densidad frente a temperatura. N=64')
plt.legend()

# Mostrar la gráfica
plt.grid(True)
plt.show()

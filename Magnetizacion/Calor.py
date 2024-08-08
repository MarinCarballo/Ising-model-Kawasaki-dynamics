import pandas as pd
import matplotlib.pyplot as plt

# Cargar datos desde el archivo CSV
filename = 'C:/Users/anton/OneDrive/Escritorio/A.MarinCarballoCompu2324/A.MarinCarballoCompu2324/Voluntario1/Calor.txt'
data = pd.read_csv(filename, header=None, names=['Calor', 'Temperatura'])

# Graficar los datos
plt.figure(figsize=(10, 6))

# Graficar la capacidad calorífica en función de la temperatura
plt.plot(data['Temperatura'], data['Calor'], marker='o', linestyle='-', color='blue', label='Capacidad Calorífica vs Temperatura')

# Añadir etiquetas y título
plt.xlabel('Temperatura (K)')
plt.ylabel('Capacidad Calorífica (C)')
plt.title('Capacidad Calorífica en función de la Temperatura')
plt.legend()

# Mostrar la gráfica
plt.grid(True)
plt.show()

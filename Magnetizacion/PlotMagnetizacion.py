import pandas as pd
import matplotlib.pyplot as plt

# Cargar datos desde el archivo CSV
filename = 'C:/Users/anton/OneDrive/Escritorio/A.MarinCarballoCompu2324/A.MarinCarballoCompu2324/Voluntario1/PromedioDensidad.txt' 
data = pd.read_csv(filename, header=None, names=['Plus', 'Minus', 'Temperatura'])

# Calcular la densidad total (Plus + Minus)
data['Plus_Minus'] = data['Plus'] + data['Minus']

# Graficar los datos
plt.figure(figsize=(10, 6))

# Graficar la densidad Plus
plt.plot(data['Temperatura'], data['Plus'], 'o-', linewidth=1, label='Densidad Plus', color='blue')

# Graficar la densidad Minus
plt.plot(data['Temperatura'], data['Minus'], 'o-', linewidth=1, label='Densidad Minus', color='brown')

# Graficar la densidad total (Plus + Minus)
plt.plot(data['Temperatura'], data['Plus_Minus'], 'o-.', linewidth=1, label='Densidad Plus + Minus', color='red')

# Añadir etiquetas y título
plt.xlabel('Temperatura')
plt.ylabel('Densidad')
plt.title('Densidad Plus, Minus y su Suma en función de la Temperatura. Magnetización NO NULA')
plt.legend()

# Mostrar la gráfica
plt.grid(True)
plt.show()

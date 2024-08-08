import pandas as pd
import matplotlib.pyplot as plt

# Leer el archivo CSV
csv_path = 'C:/Users/anton/OneDrive/Escritorio/A.MarinCarballoCompu2324/A.MarinCarballoCompu2324/Voluntario1/PromedioDensidad.txt'  
df = pd.read_csv(csv_path)

# Calcular la suma de Plus y Minus
df['Plus_Minus'] = df['Plus'] + df['Minus']

# Crear el gráfico
plt.figure(figsize=(10, 6))
plt.plot(df['Temperatura'], df['Plus'], label='Densidad Plus', marker='o', color='orange')
plt.plot(df['Temperatura'], df['Minus'], label='Densidad Minus', marker='o', color='brown')
plt.plot(df['Temperatura'], df['Plus_Minus'], label='Densidad Plus + Minus', marker='o', linestyle='--', color='magenta')

# Añadir títulos y etiquetas
plt.title('Densidad Plus, Minus y su Suma en función de la Temperatura')
plt.xlabel('Temperatura')
plt.ylabel('Densidad')
plt.legend()
plt.grid(True)

# Mostrar el gráfico
plt.show()

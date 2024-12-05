#!/bin/bash

# Nom du fichier exécutable
EXECUTABLE="./philosophers_TATAS"

# Fichier de sortie CSV
OUTPUT_FILE="./performance_local/philo_TATAS.csv"

# Nombres de threads à tester
THREADS=(2 4 8 16 32)

# Nombre de répétitions par configuration
REPEATS=5

# En-tête du fichier CSV
echo "Threads,Run,Time (s)" > "$OUTPUT_FILE"

# Vérifier que l'exécutable existe
if [[ ! -x $EXECUTABLE ]]; then
  echo "Erreur : l'exécutable $EXECUTABLE n'existe pas ou n'est pas exécutable."
  exit 1
fi

# Boucle sur chaque configuration de threads
for T in "${THREADS[@]}"; do
  echo "Mesure pour $T threads..."
  # Répéter l'exécution 5 fois
  for RUN in $(seq 1 $REPEATS); do
    # Mesurer le temps d'exécution avec "time"
    TIME=$( { /usr/bin/time -f "%e" $EXECUTABLE $T > /dev/null; } 2>&1 )
    # Sauvegarder le résultat dans le CSV
    echo "$T,$RUN,$TIME" >> "$OUTPUT_FILE"
  done
done

echo "Mesures terminées. Résultats sauvegardés dans $OUTPUT_FILE."
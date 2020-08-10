# Helpful Comments

## Valgrind
Usage
 ```sh
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./qSimDebug 100 4 60 2.3
```
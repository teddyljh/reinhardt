#!/bin/bash
#SBATCH -n 4
#SBATCH -t 1:00:00
#SBATCH --mem=16G
./srch > results.txt

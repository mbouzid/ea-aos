# algo_adaptatif
Dynamic Island Model

# Compil

	`make clean`

	`make`


# Utilisation

 ` ./bin/main--tsp <instance tsp>  --out <fichier output tsp> `

Autres options:
 --sz <taille population> [defaut: 10] 
 --it <nombre itérations> [defaut: 100]
 --pmin <proba min> [defaut: 0.10]
 --pc <proba croisement>  [defaut: 1.0]
 --pm <proba mutation> [defaut: 1.0]
--alpha <facteur apprentissage>  [defaut: 0.85] 
 --beta <bruit> [defaut: 0.01] 


## Exemple

	` ./bin/main --tsp a280.tsp  --out a280.out --sz 15 --it 100`

## Dessiner un graphique fitness/taille populations

	lancer le programme ./bin/main
	
	puis 

	lancer cette commande :
	` Rscript compo.R fitness_fct_size.csv `

 


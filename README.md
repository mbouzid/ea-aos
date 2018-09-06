# ea-aos

Adaptative Operator Selection w/ Evolutionnary Algorithm 

# Installation

`make && make clean`


# Usage

 ` ./bin/main --tsp <tsp instance>  --out <output tsp> `

## Options
```
 --sz <population size> [defaut: 10] 
 --it <number of iterations> [defaut: 100]
 --pmin <minimal probabibility> [defaut: 0.10]
 --pc <crossover probability>  [defaut: 1.0]
 --pm <mutation probability> [defaut: 1.0]
--alpha <facteur apprentissage>  [defaut: 0.85] 
 --beta <bruit> [defaut: 0.01] 
```


## Example

` ./bin/main --tsp a280.tsp  --out a280.out --sz 15 --it 100`

## Draw a fitness/population size of each island graphic

`./bin/main`
` Rscript compo.R fitness_fct_size.csv `
 


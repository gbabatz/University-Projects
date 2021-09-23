function pop = knapsack_create_permutations( NVARS,FitnessFcn,options )
%create the population
totalPopulationSize = sum(options.PopulationSize);
n = NVARS; %should be 100, number of variable, length of chromosome
pop = cell(totalPopulationSize,1); 

% essentially this is a 100 row(=population size) cell type structure
% and on each row there is one chromosome -> vector 1x100

% we will place a solution on the population only when the items 
% fit in the sack. We are not guaganteed that the random generation of 
% solutions will respect this limitation. Passing our random generated 
% result into the limitation function will create the desired chromosome.
% we have binary solution, 1 means that the item is in the sack 0 means its not

limitation_func =@(x) limitation(x);

for i=1:totalPopulationSize
    temp_solution = randi((0:1),1,n);
    valid_solution = limitation_func(temp_solution);
    pop{i} = valid_solution;   
end
end






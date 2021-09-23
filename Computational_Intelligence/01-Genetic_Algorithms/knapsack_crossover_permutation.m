function xoverKids  = knapsack_crossover_permutation(parents,options,NVARS, ... 
    FitnessFcn,thisScore,thisPopulation) 

limitation_func = @(x) limitation(x);
n = NVARS;
nKids = length(parents)/2; 
xoverKids = cell(nKids,1); % Normally zeros(nKids,NVARS); 
index = 0; 
for i=1:nKids;
    % here is where the special knowledge that the population is a cell 
    % array is used. Normally, this would be thisPopulation(parents(index),:); 
    index = index + 1;
    parent1 = thisPopulation{parents(index)};
    index = index + 1;
    parent2 = thisPopulation{parents(index)};
    %generating a binary vector. When value = 1 we take the corresponding
    %value of parent1 when value = 0 we choose the value of parent2
    crossover_vect = randi([0,1],1,n);
    temp_kid = parent1.*crossover_vect + parent2.*~crossover_vect; 
    
    % limitation function to create a valid solution
    valid_kid = limitation_func(temp_kid);
    xoverKids{i} = valid_kid; % Normally, xoverKids(i,:); 
end
end
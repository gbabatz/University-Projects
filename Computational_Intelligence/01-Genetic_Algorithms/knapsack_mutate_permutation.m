function mutationChildren = knapsack_mutate_permutation(parents ,options,NVARS, ... 
    FitnessFcn, state, thisScore,thisPopulation,mutationRate) 

%   The arguments to the function are  
%     PARENTS: Parents chosen by the selection function, represented as a
%     vector with the values corresponding to the index of the population
%     NVARS: Number of variables    
%     THISPOPULATION: Matrix of individuals in the current population 

    limitation_func =@(x) limitation(x);
    mutationChildren = cell(length(parents),1);% Normally zeros(length(parents),NVARS); 

    for i=1:length(parents)

        parent = thisPopulation{parents(i)}; % Normally thisPopulation(parents(i),:) 
%         finding the location of zeros in the solution, choosing a random
%         index and replacing it with one
        indexes_wzero = find(~parent);
        rand_index = randi(length(indexes_wzero));
        temp_mutation = parent;
        temp_mutation(rand_index) = 1;
        valid_solution = limitation_func(temp_mutation);

        mutationChildren{i} = valid_solution; % Normally mutationChildren(i,:) 
    end
end
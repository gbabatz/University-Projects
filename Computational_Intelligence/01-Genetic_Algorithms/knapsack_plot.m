function state = knapsack_plot(options,state,flag)
% choosing the population with the minimum score -> the best
[score,i] = min(state.Score); 
% picking the best solution out of the current population
genotype = state.Population{i}; 
num_of_items = sum(genotype);
total_value = (1/score)/100;
hold on; 
bar([1 2],[num_of_items total_value]);
ax = gca;
ax.XTick = [1, 2];
ax.XTickLabels = {'total items','value/100'};
grid on
hold off

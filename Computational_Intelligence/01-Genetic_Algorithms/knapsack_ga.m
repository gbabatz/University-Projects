disp('program_started');
items = [8 356;17 367;12 309;20 338;15 392;8 339;8 338;15 390;
        16 317;15 343;12 392;18 369;18 359;12 390;16 367;16 313;
        15 325;7 351;17 372;16 321;14 302;12 359;9 359;13 350;12 371;
        16 330;11 304;17 367;9 345;12 346;13 368;14 365;19 346;15 351;
        17 384;17 363;17 368;19 391;20 348;14 367;13 343;18 307;19 305;
        14 378;19 316;14 323;17 340;17 394;9 328;17 342;14 383;8 364;
        9 372;18 302;14 380;13 352;11 303;9 384;17 334;10 357;19 345;
        17 348;9 316;8 323;11 312;10 321;7 374;12 351;16 308;11 321;
        10 329;19 379;7 381;10 301;10 327;7 356;13 378;18 387;13 354;
        10 351;17 382;19 371;14 382;14 347;15 375;13 314;8 338;13 319;
        19 349;20 329;10 341;8 324;10 390;7 365;18 365;20 377;8 399;
        8 315;14 330;17 352];
    
n = size(items,1);
FitnessFcn = @(x) knapsack_fitness(x); 
tsp_my_plot = @(options,state,flag) knapsack_plot(options,state,flag); 
options = gaoptimset('PopulationType', 'custom','PopInitRange',[0;1]); 
options = gaoptimset(options,'CreationFcn',@knapsack_create_permutations, ... 
                             'CrossoverFcn',@knapsack_crossover_permutation, ... 
                             'MutationFcn',@knapsack_mutate_permutation, ... 
                             'PlotFcn', tsp_my_plot, ... 
                             'Generations',1000,'PopulationSize',100, ... 
                             'StallGenLimit',100,'Vectorized','on'); 
numberOfVariables = n; 
[x,fval,reason,output] = ga(FitnessFcn,numberOfVariables,options)
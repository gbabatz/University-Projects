% %create random pairs of x, y 
rng(111)
total_samples = 400;
test_samples_len = 60;
train_samples_len = total_samples-test_samples_len;

range_pool = -4:0.02:4;
x_index = randi(length(range_pool),1,train_samples_len);
y_index = randi(length(range_pool),1,train_samples_len);
train_x = range_pool(x_index);
train_y = range_pool(y_index);

% removing the selected for training values
x_test_pool = range_pool;
x_test_pool(x_index) = [];
y_test_pool = range_pool;
y_test_pool(y_index) = [];

test_x = x_test_pool(randperm(length(x_test_pool),test_samples_len));
test_y = y_test_pool(randperm(length(y_test_pool),test_samples_len));


func = @(x,y) func(x,y);
train_out = func(train_x,train_y);
train_inp = [train_x;train_y];
test_out = func(test_x,test_y);
test_inp = [test_x;test_y];

hidden_layers = [60 50 1];
activation_functions = {'tansig','tansig','purelin'};
lr=0.01;
epochs = 1000;

net = newff([-4 4;-4 4],hidden_layers, activation_functions,'traingd');
net.trainParam.show = 50; 
net.trainParam.lr =lr;
net.trainParam.epochs = epochs;
net.trainParam.goal = 1e-5;

net1 = train(net,train_inp,train_out);
predicted_out = sim(net1,test_inp);

mse = sum((predicted_out-test_out).^2);
text = sprintf('general mse is: %.4f' ,mse);
fprintf('mse is: %.4f' ,mse/length(predicted_out))

figure(1)
scatter(train_x,train_out,'xg')
hold on;
scatter(test_x,test_out,'or')
scatter(test_x,predicted_out,'*b');
grid;
hold off;
legend('Function','actual value','predicted value');

figure(2)
hold on;
plot(test_out,'-o'); 
plot(predicted_out,'-x');
legend(' expected test output','test output')
hold off;

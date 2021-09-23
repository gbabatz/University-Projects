function output = func(x ,y)

temp1 = 0.7.*exp(cos(pi*x));
temp2 = 0.3.*cos(2*pi*y);
output = temp1 + temp2;
end
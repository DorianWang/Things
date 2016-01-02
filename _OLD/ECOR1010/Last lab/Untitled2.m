
test = @(x) sin(x^2)*(10+4*x + x^2)/(7 + 2*x^2);

h = figure;
set(h,'name','Dorian Wang''s Stuff and Things''','numbertitle','off')

fplot(test, [-2, 2])
xlabel('x')
ylabel('sin(x^2)*(10+4*x + x^2)/(7 + 2*x^2)')



size([1;2])
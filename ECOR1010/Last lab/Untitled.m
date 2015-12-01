z = [0; 0]; a = [1; 10]; b = [8; 2]; c = [8; -20];
d = [-13; 7]; e = [4; 10];
y = [z a b c d e];

h = figure;
set(h,'name','Dorian Wang''s Stuff and Things''','numbertitle','off')

position = cumsum(transpose(y));  
plot(position(:,1), position(:,2))

totalDistance = norm(a) + norm(b) + norm(c) + norm(d) + norm(e);
totalDistance


A = [1 0 1; 0 0 9; 0 2 0];
b = [5 6 3];

c = b * A * transpose(b);

format bank;
E = c/pi

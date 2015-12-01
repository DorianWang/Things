f = [3 9; 7 -2; 3 0; 6 8; 9 6; 5 2];
g = [-9 4 8 5 -1 2; 7 -1 -2 9 3 3];
q = [2 -7 2; 1 0 6; -5 3 5];
u = [1 0; 0 1]; h = [8; 5; -6];
r = -0.4; p = 0.32

r * f*g
w = f-transpose(g)
u*g
f*u
q*h
q*g + p*transpose(f)
%This doesn't work because reasons



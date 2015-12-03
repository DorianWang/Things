function Q2 = s(x, y)
xDim = size(x);
yDim = size(y);

if (xDim(1, 2) ~= 1)
    if (xDim(1, 1) == 1)
        x = transpose(x);
    else
        'The input is not correct.'
        return
    end
end

if (yDim(1, 2) ~= 1)
    if (yDim(1, 1) == 1)
        y = transpose(y);
    else
        'The input is not correct.'
        return
    end
end

if (size(x) ~= size(y))
    'Input is incompatable.'
    return
end

xy2 = [];
disp(size(x))
disp(size(y))
for n = 1:size(x)
    disp(n)
    xy2 = [xy2; x(n, 1)*(y(n, 1)^2)];
end

disp('     x     y    xy^2')
answ = [x, y, xy2];
disp(answ)

end









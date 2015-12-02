function Q5 = y(input)

if size(input) ~= size(1)
    return % Error
end

%hope this is enough
inputLog = zeros(275, 1);
count = 0;

while input ~= 1
    count = count + 1;
    inputLog(count, 1) = input;
    if mod(input, 2) == 0
        input = input / 2;
    else
        input = input * 3 + 1;
    end
end

revSeries = flipud(inputLog);
revSeries = log10(revSeries);

plot((1:275), revSeries, '*');






end

















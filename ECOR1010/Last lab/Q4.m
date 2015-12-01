function Q4 = y(distance, fuel)

sizeD = size(distance)

if (size(distance) ~= size(fuel) || sizeD(1, 2) ~= 1)
    return %error
end

KmPer100L = []

for n = 1:sizeD(1,1)
    KmPer100L = [KmPer100L; distance(n, 1)/fuel(n, 1)*100];
end







end
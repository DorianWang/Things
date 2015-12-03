function Q4 = y(distance, fuel)

sizeD = size(distance);

KMpLToMpG = 2.35214583;

if ((size(distance) ~= size(fuel)) | (sizeD(1, 2) ~= 1))
    disp('Input is not the correct size.')
    return %error
end

LPer100KM = zeros(sizeD(1,1), sizeD(1,2));

for n = 1:sizeD(1,1)
    LPer100KM(n, 1) = fuel(n, 1) * 100.0/distance(n, 1);
end

MlPerG = zeros(sizeD(1,1), sizeD(1,2));

for n = 1:sizeD(1,1)
    MlPerG(n, 1) = 1/(LPer100KM(n, 1)) * KMpLToMpG / 100.0;
end

months = zeros(sizeD(1, 1), 1);
for n = 1:sizeD(1, 1)
    months(n, 1) = n;
end

month = months;
MilesPerGallon = MlPerG;
fuelUsed = fuel;
distanceTraveled = distance;

T = table(month, distanceTraveled, fuelUsed, MilesPerGallon, LPer100KM);

disp(T)
fprintf('The average fuel efficiency is %f MPG.\n', sum(MilesPerGallon(:))/ sizeD(1, 1));
fprintf('The average fuel consumption is %f L/100 km.\n', sum(LPer100KM(:))/ sizeD(1, 1));

end








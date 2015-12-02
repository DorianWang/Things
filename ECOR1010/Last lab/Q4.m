function Q4 = y(distance, fuel)

sizeD = size(distance);

KMpLToMpG = 2.35214583;

if ((size(distance) ~= size(fuel)) | (sizeD(1, 2) ~= 1))
    disp('Input is not the correct size.')
    return %error
end

'Things!'

KmPer100L = zeros(sizeD(1,1), sizeD(1,2));

for n = 1:sizeD(1,1)
    KmPer100L(n, 1) = KmPer100L; distance(n, 1)/fuel(n, 1)*100;
end

MlPerG = zeros(sizeD(1,1), sizeD(1,2));

for n = 1:sizeD(1,1)
    MlPerG(n, 1) = KmPer100L(n, 1) * 2.35214583 / 100;
end










end








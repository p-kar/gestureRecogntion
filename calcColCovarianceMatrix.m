function [CM] = calcColCovarianceMatrix(trainData)
[m, n, c] = size(trainData);
T_mean = zeros(m, n, 'like', trainData);
CM = zeros(n, n, 'like', trainData);
for i = 1:c
    T_mean = T_mean + trainData(:,:,i);
end
T_mean = T_mean/c;
for i = 1:c
    CM = CM + transpose(trainData(:,:,i) - T_mean)*(trainData(:,:,i) - T_mean);
end
CM = CM/c;
end
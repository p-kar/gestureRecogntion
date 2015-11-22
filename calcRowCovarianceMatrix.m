function [CM] = calcRowCovarianceMatrix(trainData)
[m, n, c] = size(trainData);
T_mean = zeros(m, n, 'like', trainData);
CM = zeros(m, m, 'like', trainData);
for i = 1:c
    T_mean = T_mean + trainData(:,:,i);
end
T_mean = T_mean/c;
for i = 1:c
    CM = CM + (trainData(:,:,i) - T_mean)*transpose(trainData(:,:,i) - T_mean);
end
CM = CM/c;
end
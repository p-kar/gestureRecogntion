function train(dataset, data_labels)
r = 1; % number of principle eigenvectors of F
s = 4; % number of principle eigenvectors of G (22 for big data)
[m, n, dsize] = size(dataset);
tr_size = int32(dsize*0.80);
trainData = dataset(:,:,1:tr_size);
labels = data_labels(1:tr_size);
testData = dataset(:,:,tr_size+1:end);
test_labels = data_labels(tr_size+1:end);
[m, n, c] = size(trainData);
F = calcRowCovarianceMatrix(trainData);
G = calcColCovarianceMatrix(trainData);
[Ur,Dr] = getEigenVectors(F, r);
[Vs, Ds] = getEigenVectors(G, s);
Urt = transpose(Ur);
M = struct('Mi', {}, 'label', {});
for i = 1:c
    tempM.Mi = Urt*trainData(:,:,i)*Vs;
    tempM.label = labels(i);
    M(end + 1) = tempM;
end
save('model.mat', 'M', 'Ur', 'Vs');
disp('Created Model');
disp(test_labels);
calcAccuracy(testData, test_labels);
end
function calcAccuracy(testData, test_labels)
crct_pred = 0;
[m, n, c] = size(testData);
for i = 1:c
    pred = printPrediction(testData(:,:,i));
    if(strcmp(pred, test_labels(i)) == 1)
        crct_pred = crct_pred + 1;
    end
end
disp((crct_pred/c)*100);
end
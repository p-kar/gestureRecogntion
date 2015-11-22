function [pred_labels] = calcAccuracy(testData, test_labels)
crct_pred = 0;
pred_labels = cell(size(test_labels));
[m, n, c] = size(testData);
for i = 1:c
    pred = printPrediction(testData(:,:,i));
    pred_labels(i) = pred;
    if(strcmp(pred, test_labels(i)) == 1)
        crct_pred = crct_pred + 1;
    end
end
str = sprintf('Accuracy -> %.2f', (crct_pred/c)*100);
disp(str);
end
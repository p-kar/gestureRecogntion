function [predicted_label] = getPrediction(testData)
load('model.mat');
Urt = transpose(Ur);
Ni = Urt*testData*Vs;
max_dist = distanceNN(Ni, M(1).Mi);
predicted_label = M(1).label;
for i = 2:length(M)
    temp_dist = distanceNN(Ni, M(i).Mi);
    if(temp_dist < max_dist)
        max_dist = temp_dist;
        predicted_label = M(i).label;
    end
end
predicted_label = char(predicted_label);
% disp(predicted_label);
end
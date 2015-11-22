function [dist] = distanceNN(Y1, Y2)
[m, n] = size(Y1);
dist = 0;
for i = 1:n
    dist = dist + norm((Y1(:,i) - Y2(:,i)), 2);
end
end
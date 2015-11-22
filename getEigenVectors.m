% returns a diagonal matrix D of A's n largest magnitude eigenvalues 
% and a matrix V whose columns are the corresponding eigenvectors.
function [V,D] = getEigenVectors(matrix, n)
[V,D] = eigs(matrix, n);
end
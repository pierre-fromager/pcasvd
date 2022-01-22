

function Z = zscore(X)
  Z = bsxfun(@rdivide, bsxfun(@minus, X, mean(X)), std(X));
end

function [D, W_pca] = pca(X) 
  mu = mean(X);
  Xm = bsxfun(@minus, X ,mu);
  C = cov(Xm);
  [W_pca,D] = eig(C);
  [D, i] = sort(diag(D), 'descend');
  W_pca = W_pca(:,i);
end

function [D, W_lda] = lda(X,y) 
  dimension = columns(X);
  labels = unique(y);
  C = length(labels);
  Sw = zeros(dimension,dimension);
  Sb = zeros(dimension,dimension);
  mu = mean(X);

  for i = 1:C
    Xi = X(find(y == labels(i)),:);
    n = rows(Xi);
    mu_i = mean(Xi);
    XMi = bsxfun(@minus, Xi, mu_i);
    Sw = Sw + (XMi' * XMi );
    MiM =  mu_i - mu;
    Sb = Sb + n * MiM' * MiM; 
  endfor

  [W_lda, D] = eig(Sw\Sb);
  [D, i] = sort(diag(D), 'descend');
  W_lda = W_lda(:,i);
end

function X_proj = project(X, W)
  X_proj = X*W;
end

function X = reconstruct(X_proj, W)
  X = X_proj * W';
end

mat0 = [90	60	90;
90	90	30;
60	60	60;
60	60	90;
30	30	30;
];

mat1 = [
10 20 30;
10 20 30;
10 20 30;
10 20 30;
10 20 30;
10 20 30;
];

mat = [
33 80;
33 82.5;
34 100.8;
42 90;
29 67;
19 60;
50 77;
55 77;
31 87;
46 70;
36 57;
48 64;
];
clc
# C is covariance matrix
disp("C is covariance matrix")
C = cov(mat,0)
# R is correlation matrix
disp("R is correlation matrix")
R = corr(mat)
# V are eigenVectors
disp("V are eigenVectors")
[V,lambda] = eig(R)

#data = dlmread("wine.data",",");
#y = data(:,1);
#X = data(:,2:end);

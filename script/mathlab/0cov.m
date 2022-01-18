

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

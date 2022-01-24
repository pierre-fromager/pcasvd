clear all; close all; clc;
%%--Analyse en Composantes Principales--%%
% Exercice 2 : habitudes alimentaires


%%Donnees enonce
e = 1;  %epsilon
phi = 0; %phase
th = 0:pi/50:2*pi;
t=0:0.1:10;
s1 = @(t) sin(t) + 0.2*rand(1,length(t));
s2 = @(t) e*sin(t+phi) + 0.2*rand(1,length(t));

%%Affichage question 1.a
figure(1)
subplot(121)
plot(t,s1(t),t,s2(t));
subplot(122)
plot(s1(t),s2(t),'o');


%%Question 1.b
%%Matrice des donnees 
Y=[s1(t);s2(t)];
Y=Y';
[n,m]=size(Y);
%%Matrice des donnees centrees 
Ym = mean(Y);
X = Y - Ym(ones(n, 1),:);
%%Matrice de covariance
M= X'*X/n;
%%Diagonalisation de M
[P,lambda]= eig(M);
lambda = sort(diag(lambda),'descend')';
P=fliplr(P);
%%Taux d'inertie de chaque colonne
tau = zeros(1,m);
for j=1:m
    tau(j) = lambda(j)/sum(lambda);
end
%%Matrice associee au nouveau nuage de points
Xstar = X*P;

%%Affichage en composantes principales
figure(2)
subplot(121)
plot(Xstar(:,1),Xstar(:,2),'*');
axis([-2 2 -1 1]);
axh = gca; % use current axes
line(get(axh,'XLim'), [0 0], 'Color','k');
line([0 0], get(axh,'YLim'), 'Color','k');

%%Cercle des correlations
subplot(122)
hold on;
S=ones(n,1)*std(X);
Z=X./S;
for j=1:m
    xx =  Z(:,j)'*Xstar(:,1) * 1/(n*sqrt(lambda(1)));
    yy =  Z(:,j)'*Xstar(:,2) * 1/(n*sqrt(lambda(2)));
    plot(xx,yy,'o');
end
plot(cos(th),sin(th),'k');
axis([-1.2 1.2 -1.2 1.2]);
axh = gca;
line(get(axh,'XLim'), [0 0], 'Color','k');
line([0 0], get(axh,'YLim'), 'Color','k');








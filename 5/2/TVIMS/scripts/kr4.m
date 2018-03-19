% KR2
% Data matrix
M = [
    -2.68  1.09; -7.25  6.70; -3.19  1.14; -1.04  0.49; -2.76  1.93;
    -2.97 -0.40; -3.69  2.90; -0.99 -0.95; -1.99  0.61; -3.02  1.43;
    -2.46  0.71; -4.43  1.87; -3.70  0.96; -7.98  6.19; -3.59  1.55;
    -6.80  5.29; -5.13  3.03; -4.01  2.53; -7.87  6.93; -1.73  0.11;
    -0.98 -0.10; -3.32  1.97; -4.70  2.77; -1.85 -0.53; -2.87  1.98;
     0.38 -1.48; -3.79  1.96; -2.27  0.83; -5.66  4.16; -4.13  3.11;
    -1.85  1.79; -4.18  2.15; -2.76  1.61; -0.27 -1.03; -5.99  4.87;
    -2.94  1.96; -2.20  0.50; -2.56  1.18; -2.72  1.05; -3.01  1.57;
    -6.83  5.62; -3.10  0.88; -2.24  0.24; -4.99  4.09; -6.20  4.74;
    -4.35  1.67; -3.73  1.81; -3.19  2.09; -1.58 -0.25; -2.98  1.61;
    ];

[xsteps,ysteps] = size(M);
%% Task 6
% Mathematical expectations:
m_x = zeros(1,ysteps);
for i = 1:xsteps
    for j = 1:ysteps
        m_x(j) = m_x(j) + M(i,j);
    end
end
for j = 1:ysteps
    m_x(j) = m_x(j)/xsteps;
end

% Dispersion
D_x = zeros(1,ysteps);
for i = 1:xsteps
    for j = 1:ysteps
        D_x(j) = D_x(j) + (M(i,j)-m_x(j)).^2;
    end
end
for j = 1:ysteps
    D_x(j) = (1/(xsteps-1))*D_x(j);
end

% Matrix element calculation
sig_x = zeros(1,ysteps);
% sigma calc
for j = 1:ysteps
    sig_x(j) = abs(sqrt(D_x(j)));
end

U = zeros(xsteps, ysteps);
%statistics matrix
for i = 1:xsteps
   for j = 1:ysteps
      U(i,j) = (M(i,j) - m_x(j)) / sig_x(j);
   end
end

% correlation coefficient
r = 0;
for i = 1:xsteps
       r = r + U(i,1)*U(i,2);
end
r = r/xsteps;

fprintf('r = %2.4f\n', r);
if r == 0
    fprintf('no dependency');
elseif r == 1
    fprintf('func dependency');
elseif r == -1
    fprintf('inverted func dependency');
elseif -1 < r && r < 1
    fprintf('non linear dependency');
else
    fprintf('Something is wrong r must be in bounds -1,1. ');
end

% checking correlation coefficient
t = abs(r) * (sqrt(xsteps-2)/sqrt(1-r.^2));
% 0.05 => t = 2.678,
fprintf("\nT = %2.4f\n",t);

%Task 7 regression analysis;
y = zeros(1,xsteps);        % LR prediction
epsil = zeros(1,xsteps);    % LR error
a0 = m_x(1) - r*m_x(2)*(sig_x(1)/sig_x(2));
a1 = r*(sig_x(1)/sig_x(2));
for i = 1:xsteps
    y(i) = a0 + a1*M(i,2);
    epsil(i) = y(i) - M(i);
end
% residual dispersion
disp = 1 - r.^2;
% diffusion plot
X = zeros(xsteps); X0 = zeros(xsteps);
for i = 1:xsteps
   X(i) = M(i,2);
   X0(i) = M(i,1);
end
plot(X,X0,'o');
title('Diffusion plot with regression line');
hold on;
% regression line
X = sort(X);
Y = zeros(xsteps);
for i = 1:xsteps
   Y(i) = a0+a1*X(i);
end
plot(X,Y);
% Data
n = 5;
a = -1;
b = 3;

steps = 2^n;
steps = 100;

% Toggle tasks
task1 = false;
task2 = false;
task3 = false;
task4 = false;
task5 = true;
%=============Task #1=============
fprintf("Task #1");
% XI [0,1] and to X[] and count Y[Xi]
xsi = zeros(0,steps);
X = zeros(0,steps);
Y = zeros(0,steps);

for i=1:(steps)
   xsi(i)=(i)/(steps);
   X(i)=a+xsi(i)*(b-a);
   Y(i)=PhiFunc(X(i));
end

% var.series
var_series_x = zeros(0,steps);
for i=1:steps
   var_series_x(i)=(i)*(1/steps);
end

if task1
    figure(1);
    stairs(Y,var_series_x,'r');
    hold on;
    % Theoretic G(Y)
    plot(Y,GY(Y),'g');
    legend('Empiric','Theoretic');
    return;
end

%=============Task #2=============
fprintf("Task #2");
% compute M - the number of intervals
if steps <= 100
    intervals_count = floor(sqrt(steps));
else
    intervals_count = floor(2*log10(steps));
end

% compute eqalinterval delta - interval length
YSorted = sort(Y);
YSorted(steps)
YSorted(1)
ya = 0
yb = 81
delta = (yb-ya)/intervals_count;

% compute the borders of the intervals
A = zeros(0,intervals_count);
B = zeros(0,intervals_count);  %left, right
for i=1:intervals_count
    if i==1
        A(i) = ya;
    else
        A(i) = Y(i)+(i-1)*delta;
    end
end

for i=1:intervals_count-1
    B(i) = A(i+1);
end
B(intervals_count) = yb;
% medium density on each interval
total_accumulator = 0;
interval_accumulator = zeros(size(B));

for j=1:steps
    for i = 1:intervals_count
        if A(i) <= Y(j) && Y(j) <= B(i)
            interval_accumulator(i)=interval_accumulator(i)+1;
        end
    end
end

f = zeros(size(B));
for i = 1:intervals_count
    f(i)= interval_accumulator(i)*intervals_count/(steps*(yb-ya));
    total_accumulator = total_accumulator + f(i);
    fprintf("%d : v(i)= %d f(i)= %f \n",i,interval_accumulator(i),f(i));
end

% the histogram
if task2
  figure(2);
  stairs(A, f,'r');
  hold on;
  plot(Y,gY(Y),'g');
  hold on;
end

%=============Task #3=============
fprintf("Task #3");
v = floor(steps/intervals_count)
% borders
A = zeros(0,intervals_count);
B = zeros(0,intervals_count);  %left, right

A(1)=ya;
for i = 1:intervals_count-1
    B(i)=(YSorted(i*v)+YSorted(i*(v+1)))/2;
    A(i+1)=B(i);
end
B(intervals_count)=yb

%medium density on each interval
f = zeros(0,intervals_count);
h = zeros(0,intervals_count); %interval height
for i = 1:intervals_count
    h(i)=B(i)-A(i);
    f(i)=v/(steps*h(i));
end

if task3
  figure(3);
  stairs(A,f,'b');
  hold on;
  plot(Y,gY(Y),'r');
end


%=============Task #4=============
if task4
  fprintf("Task #4");
  %=============SUBTask #1=============
  subtask1 = false;
  subtask2 = false;
  subtask3 = false;
  if(subtask1)
    fprintf("Subtask #1 Pierson");
    n = 200;

    step = (b-a)/n;
    X = zeros(0,n);
    Y = zeros(0,n);
    x = zeros(0,n);
    y = zeros(0,n);
    for i = 1:n
        X(i)=a+step*(i-1);
        Y(i)=PhiFunc(X(i));
        x(i)=a+(b-a)*rand();
        y(i)=PhiFunc(x(i));
    end
    x = sort(x)
    y = sort(y)

    figure(1)
    plot(x,'g')
    hold on;
    plot(X, 'r')
    title('f(X)');
    legend('Empiric','Theoretic');
    hold off;


    figure (2)
    plot(y,'g')
    hold on;
    plot(Y, 'r')
    title('G(Y)');
    legend('Empiric','Theoretic');
    hold off;

    % Histogram
    if n <= 100
        intervals_count = floor(sqrt(n));
    else
        intervals_count = floor(2*log10(n));
    end
    intervals_count = 14;

    v = floor(n/intervals_count)
    % borders
    A = zeros(0,intervals_count);
    B = zeros(0,intervals_count);  %left, right

    A(1)=y(1);
    for i = 1:intervals_count-1
        B(i)=(y(i*v)+y(i*(v+1)))/2;
        A(i+1)=B(i);
    end
    B(intervals_count)=y(n);

    %medium density on each interval
    f = zeros(0,intervals_count);
    h = zeros(0,intervals_count); %interval height
    for i = 1:intervals_count
        h(i)=B(i)-A(i);
        f(i)=v/(n*h(i));
    end

    figure(3);
    stairs(f,'b');
    title("Histogram");

    p = zeros(0,intervals_count);
    sp = 0;
    pstar = v/n
    hi = zeros(0,intervals_count);
    hi2 = 0;
    for i = 1:intervals_count
      p(i) = GY(B(i))-GY(A(i));
      hi(i) = n*(p(i)-pstar)^2/p(i);
      fprintf("i: %d f(A) %f f(B) %f p(i) %f p* %f hi(i) %f \n",i,gY(A(i)),gY(B(i)),p(i),pstar,hi(i));
      hi2 = hi2 + hi(i);
      sp = sp + p(i);
    end
    sp
    hi2
  end
  %=============SUBTask #2=============
  if(subtask2)
    fprintf("Subtask #2 Kolmogorov");
    n = 30;

    step = (b-a)/n;
    X = zeros(0,n);
    Y = zeros(0,n);
    x = zeros(0,n);
    y = zeros(0,n);
    for i = 1:n
        X(i)=a+step*(i-1);
        Y(i)=PhiFunc(X(i));
        x(i)=a+(b-a)*rand();
        y(i)=PhiFunc(x(i));
    end
    x = sort(x)
    y = sort(y)
    Y = sort(Y)

    figure(1)
    plot(x,'g')
    hold on;
    plot(X, 'r')
    title('f(X)');
    legend('Empiric','Theoretic');
    hold off;

    figure (2)
    plot(y,'g')
    hold on;
    plot(Y, 'r')
    title('g(T)');
    legend('Empiric','Theoretic');
    hold off;

    d = [];
    for i = 1:n
      d(i) = abs(y(i)-Y(i));
    end
    dmax = max(d);
    % lamda
    lambda = sqrt(n)*dmax/n;
    lambda

  end
    %=============SUBTask #3=============
  if(subtask3)
    fprintf("Subtask #3 Mizes");
    n = 50;

    step = (b-a)/n;
    X = zeros(0,n);
    Y = zeros(0,n);
    x = zeros(0,n);
    y = zeros(0,n);
    for i = 1:n
        X(i)=a+step*(i-1);
        Y(i)=PhiFunc(X(i));
        x(i)=a+(b-a)*rand();
        y(i)=PhiFunc(x(i));
    end
    x = sort(x)
    y = sort(y)
    Y = sort(Y)

    figure(1)
    plot(x,'g')
    hold on;
    plot(X, 'r')
    title('f(X)');
    legend('Empiric','Theoretic');
    hold off;

    figure (2)
    plot(y,'g')
    hold on;
    plot(Y, 'r')
    title('g(T)');
    legend('Empiric','Theoretic');
    hold off;

    D = [];
    for i = 1:n
       D(i) = ((y(i) - Y(i))/n).^2;
    end

    nw2 = 1 / (12 * n);
    for i = 1:n
       nw2 = nw2 + D(i);
    end
    nw2
  end
end

%=============Task #5=============
if(task5)
  n = 20;
  x = [];
  for i = 1:n
      x(i) = a + (b-a) * rand(1,1);
  end
  x = sort(x)
  % MX rate
  m_x = sum(x)/n
  % Disp rate
  S0_2 = 0;
  for i = 1:n
     S0_2 = S0_2 + (x(i) - m_x).^2;
  end
  S0_2 = 1 / (n-1) * S0_2;

  t = 2.861;
  stud = S0_2*t/sqrt(n-1);
  m_left = m_x - stud;
  m_right = m_x + stud;
  fprintf('interval: %3.4f <= m_x < %3.4f \n', m_left, m_right);

  % subtask 2
  numer = n*S0_2;
  hi2_1 = 38.582; %hi_{(1-p) / 2}
  hi2_2 = 6.844; %hi_{(1+p) / 2}
  d_left = numer / hi2_1;
  d_right = numer / hi2_2;

  fprintf('disp int: %3.4f <= D_x < %3.4f \n', d_left, d_right);
end


%=============Functions=============
function res=PhiFunc(phi_x)
  res = phi_x^4;
end

function res=FX(X)
  r = zeros(size(X))
  for i = 1:size(X,2);
    if X(i)<-1
      r(i)=0;
    elseif X(i)>=-1 && X(i)<=3
      r(i)=((X(i)+1)/4);
    elseif X(i)>3
      r(i)=1
    end
  end
  res = r;
end

function res=fX(X)
   r = zeros(size(X))
  for i = 1:size(X,2);
    if X(i)<-1
      r(i)=0;
    elseif X(i)>=-1 && X(i)<=3
      r(i)=1/4;
    elseif X(i)>3
      r(i)=0;
    end
  end
  res = r;
end

function res=GY(Y)
   res = Y.^(1/4)/4+1/4;
end

function res=gY(lY)
   r = zeros(size(lY));
   for i = 1:size(lY,2)
       if lY(i)<=0
          r(i)=0;
        elseif lY(i)<=1
          r(i)=2/(16*(lY(i))^(3/4));
        elseif lY(i)>=1 && lY(i)<=81
          r(i)=1/(16*(lY(i))^(3/4));
       elseif lY(i)>81
          r(i)=0;
       end
   end
   res = r;
end
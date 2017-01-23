function mna2
%    ipr1();
%    kr1();
%    ipr2();
     kr2();
end

% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 

function kr1()
    syms x
    lb = -1;
    ub = 1;
    
    n = 2;
    A = sym('a',[1,n]);
    baseFunc0 = '0';
    p=0;
    
    b = 1;
    c = 1;
    q=-1
    f=-x
    
    b = cos(degtorad(26))
    c = sin(degtorad(26))
    q=-(1+b*x*x)/c;    
    f=-1/c;
      
     res1 = kr1_form_output(A,n,kr1_colloc_solveSystem(A,baseFunc0,n,p,q,f,lb,ub));
     ezplot(res1,lb,ub);hold on
     res2 = kr1_form_output(A,n,kr1_mnki_solveSystem(A,baseFunc0,n,p,q,f,lb,ub));
     ezplot(res2,lb,ub);hold on
     res3 = kr1_form_output(A,n,kr1_mnkd_solveSystem(A,baseFunc0,n,p,q,f,lb,ub));
     ezplot(res3,lb,ub);hold on
     res4 = kr1_form_output(A,n,kr1_galerkin_solveSystem(A,baseFunc0,n,p,q,f,lb,ub));
     ezplot(res4,lb,ub);hold on
end

function res = kr1_baseFunc(i)
    baseFunc = '(x^n)*(1-x)'; 
    res = subs(baseFunc,'n',i);
end

function res = kr1_lfunc(i,p,q)
    basefunci = kr1_baseFunc(i);
    res = diff(basefunci,2)+p*diff(basefunci,1)-q*(basefunci);
end

function res = kr1_colloc_nevfunc(A,baseFunc0,n,p,q,f)
    symF = sym(f);
    symB0 = sym(baseFunc0);
    nevfunc = diff(symB0,2)+p*diff(symB0,1)-q*symB0;
    for i = 1:n
        nevfunc = nevfunc + A(1,i)*kr1_lfunc(i,p,q);
    end
    res = nevfunc - symF;
end

function res = kr1_colloc_solveSystem(A,baseFunc0,n,p,q,f,lb,ub)    
    sys = sym('f',[1,n]);
    sysIdx = 1;
    nevfunc = kr1_colloc_nevfunc(A,baseFunc0,n,p,q,f)
    for i = lb:((ub-lb)/(n-1)):ub
        sys(sysIdx) = subs(nevfunc,'x',i);
        sysIdx = sysIdx + 1;
    end
    
    res = solve(sys,A);
end

function res = kr1_form_output(A,n,sysRes)
    y2 = '0';
    for i = 1:n
        y2 = y2 + A(i)*kr1_baseFunc(i);
    end

    for i = 1:n
        idx{1} = strcat('a', num2str(i));
        sysRes.(idx{1});
        y2 = subs(y2,A(i),sysRes.(idx{1}));
    end
    res = y2;
end

function res = kr1_mnki_nevfunc(A,baseFunc0,n,p,q,f,j,lb,ub)
    symF = sym(f);
    symB0 = sym(baseFunc0);
    nevfunc = '0';
    for i = 1:n
        nevfunc = nevfunc + A(1,i)*int(kr1_lfunc(i,p,q)*kr1_lfunc(j,p,q),'x',lb,ub);
    end
    res = nevfunc - int((symF-symB0)*kr1_lfunc(j,p,q),'x',lb,ub);
end

function res = kr1_mnki_solveSystem(A,baseFunc0,n,p,q,f,lb,ub)    
    sys = sym('f',[1,n]);

    for i = 1:n
        sys(i) = kr1_mnki_nevfunc(A,baseFunc0,n,p,q,f,i,lb,ub);
    end
    
    res = solve(sys,A);
end

function res = kr1_mnkd_discr(f,x,lb,ub,n)
    res = '0';
    for xi=lb:(ub-lb)/n:ub
        res = res + subs(f,x,xi);
    end
end


function res = kr1_mnkd_nevfunc(A,baseFunc0,n,p,q,f,j,lb,ub)
    symF = sym(f);
    symB0 = sym(baseFunc0);
    nevfunc = '0';
    for i = 1:n
        nevfunc = nevfunc + A(1,i)*kr1_mnkd_discr(kr1_lfunc(i,p,q)*kr1_lfunc(j,p,q),'x',lb,ub,n*2);
    end
    res = nevfunc - kr1_mnkd_discr((symF-symB0)*kr1_lfunc(j,p,q),'x',lb,ub,n*2);
end

function res = kr1_mnkd_solveSystem(A,baseFunc0,n,p,q,f,lb,ub)    
    sys = sym('f',[1,n]);

    for i = 1:n
        sys(i) = kr1_mnkd_nevfunc(A,baseFunc0,n,p,q,f,i,lb,ub);
    end
    
    res = solve(sys,A);
end

function res = kr1_galerkin_nevfunc(A,baseFunc0,n,p,q,f,j,lb,ub)
    symF = sym(f);
    symB0 = sym(baseFunc0);
    nevfunc = '0';
    for i = 1:n
        nevfunc = nevfunc + A(1,i)*int(kr1_lfunc(i,p,q)*kr1_baseFunc(j),'x',lb,ub);
    end
    res = nevfunc - int((symF-symB0)*kr1_baseFunc(j),'x',lb,ub);
end

function res = kr1_galerkin_solveSystem(A,baseFunc0,n,p,q,f,lb,ub)
    sys = sym('f',[1,n]);

    for i = 1:n
        sys(i) = kr1_galerkin_nevfunc(A,baseFunc0,n,p,q,f,i,lb,ub);
    end
    
    res = solve(sys,A);
end

% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 

function kr2()
 syms x y;
 xlb = 0;
 xub = 1;
 ylb = 0;
 yub = 1;
 
 n0 = 5;
 
 for k = 1:1
 n = n0*k
 U = sym('u',[1,n*n])

 fx = exp(-(y-x)^2);
 gx = 1+0.5*cos(x+y);
 
 xn = zeros(1,n);
 yn = zeros(1,n);
 sys = sym('f',[n,n]);
 dx=((xub-xlb)/n);
 dy=((yub-ylb)/n);
 u = gx-fx;
 for i = 1:n
    xn(i) = xlb+dx*i;
    for j = 1:n
        yn(j) = ylb+dy*j;
        lidx = i*n+j-n;
        sys(i,j) = kr2_iteration(u,U(lidx),xn(i),yn(j),dx,dy);
    end
 end
 
%  sysRes = solve(sys,U)
%     for i = 1:(n*n)
%         idx{1} = strcat('u', num2str(i));
%         sysRes.(idx{1})
%         U(i)=(sysRes.(idx{1}))
%     end
    mesh(xn,yn,sys);hold on
 end
end

function res = kr2_test_g(x,y)
    %not yet implemented
end

function res = kr2_iteration(u,nu,xi,yj,dx,dy)
    u = subs(u,'u',nu);
    res = subs(subs(u,'x',xi+dx),'y',yj) + subs(subs(u,'x',xi-dx),'y',yj) + subs(subs(u,'x',xi),'y',yj+dy) + subs(subs(u,'x',xi),'y',yj-dy) - 4 * subs(subs(u,'x',xi),'y',yj);   
end
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 

function ipr1()
    syms x
    lb = -1;
    ub = 1;
    e = 0.001;
    pn = (ub-lb)/e;
    n = 30;
    Y = sym('y',[1,n]);
    p=0;
     
    b = 1;
    c = 1;
    q=-1;
    f=-x;
    
    b = cos(degtorad(26));
    c = sin(degtorad(26));
    q=-(1+b*x*x)/c;   
    f=-1/c;
    
    ipr1_form_output(Y,n,p,q,f,lb,ub);
end

function res = ipr1_kfunc2(Y,k,n,xk,h,p,q,f)
    fk = subs(f,'x',xk);
    pk = subs(p,'x',xk);
    qk = subs(q,'x',xk);
    a = ((pk/(2*h))+(1/(h*h)));
    b = (qk - (2/(h*h)));
    c = ((1/(h*h))-(pk/(2*h)));
    Y0 = -1;
    YN = 0;
    if k == 1
        res = a*Y(k+1)+b*Y(k)+c*Y0-fk;
    elseif k == n
        res = a*YN+b*Y(k)+c*Y(k-1)-fk;
    else
        res = a*Y(k+1)+b*Y(k)+c*Y(k-1)-fk;
    end
end

function res = ipr1_kfunc(Y,k,n,xk,h,p,q,f)
    Y0 = -1;
    Yn = 0;
    if k == 1
        res = (Y(k+1)-2*Y(k)+Y0)/(h*h) + subs(p,'x',xk)*((Y(k+1)-Y(k))/(2*h)) - subs(q,'x',xk)*Y(k) - subs(f,'x',xk);
    elseif k == n
        res = (Yn-2*Y(k)+Y(k-1))/(h*h) + subs(p,'x',xk)*((Yn-Y(k))/(2*h)) - subs(q,'x',xk)*Y(k) - subs(f,'x',xk);
    else 
        res = (Y(k+1)-2*Y(k)+Y(k-1))/(h*h) + subs(p,'x',xk)*((Y(k+1)-Y(k))/(2*h)) - subs(q,'x',xk)*Y(k) - subs(f,'x',xk);
    end
end

function ipr1_form_output(Y,n,p,q,f,lb,ub)
    xk = zeros(1,n);
    sys = sym('f',[1,n-1]);
    h=((ub-lb)/n);
    for k = 1:(n-1)
        xk(k) = lb+k*h;
        sys(k) = (ipr1_kfunc2(Y,k,n,xk(k),h,p,q,f));
    end
    sysRes = solve(sys,Y);
    
    for i = 1:(n-1)
        idx{1} = strcat('y', num2str(i));
        Y(i)=(sysRes.(idx{1}));
    end
    xk(n)=1;
    Y(n)=0;
    plot(xk,Y); hold on 
end

% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 

function ipr2()
    syms x
    l = 100;
    xlb = 0;
    xub = l;
    
    syms t
    T = 100;
    tlb = 0;
    tub = T;
    
%     a =
%     UA = 
%     b = 
%     UB =
%     fx = ''
%     kx = ''
    
    step = 0.1
   
end

function res = ipr2_nfunc()
    e = 0.1
    dt = e*dx*dx;
%     rh = (un(ti+dt,xj)-un(ti,xj))/dt-((un(ti,xj+dx)-2*(un(ti,xj))+(un(ti,xj-dx)))/(dx*dx))
    u=prev - tau*f(ti,xj)+tau*(uaprox);
end

function ipr2_form_output()
end

function ipr2_from_dxdy(T1,T2,T0,A,L )
s=figure;
step=0.1;
t=0;
h=1;
Ti=T0*ones(L,1);
Tk=T0*ones(L,1);
colormap(bone);

while ishandle(s)
    Ti(1)=T1;
    Ti(L)=T2;
    Tk(1)=T1;
    Tk(L)=T2;
    for x=2:h:(L-1)
        Tk(x)=(A*step/h^2)*(Ti(x-1)+Ti(x+1)-2*Ti(x)) + Ti(x);
    end
    image(Ti);
    Ti=Tk;
    t=t+step;
    pause(step);
end
end
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
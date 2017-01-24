function mna2
%       ipr1();
%     ipr2();
%       kr1();
%       kr2();
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
 fx = x*x+y*y;%exp(-(y-x)^2);
 gx = 1+x+y;%1+0.5*cos(x+y);
 n0 = 5;
 
 for k = n0:10
     n = k;
     U = sym('u',[n,n]);

     xn = zeros(1,n);
     yn = zeros(1,n);
     sys = sym('f',[n,n]);
     dx=((xub-xlb)/n);
     dy=((yub-ylb)/n);
       
%    Need to set actual border values
     for i = 1:n
        sys(i,1)=U(i,1);
        sys(i,n)=U(i,n);
     end
     
     for j = 1:n
        sys(1,j)=U(1,j);
        sys(n,j)=U(n,j);
     end
    
    
%   Need to check for border nodes to skip calc on them
     for i = 2:n-1
        xn(i) = xlb+dx*i;
        for j = 2:n-1
            yn(j) = ylb+dy*j;
            gnx = subs(subs(gx,x,xn(i)),y,yn(j));
            fnx = subs(subs(fx,x,xn(i)),y,yn(j));        
            sys(i,j) = (-fnx+gnx*(U(i+1,j)+U(i-1,j)-2*U(i,j))/(dx*dx)+(U(i,j+1)+U(i,j-1)-2*U(i,j))/(dy*dy));
        end
     end
     
    flatSys = sym('f',[1,n*n]);
    flatU = sym('u',[1,n*n]);
    for i = 1:n
        for j = 1:n
            flatIDX = i*n+j-n;
            flatU(flatIDX) = U(i,j);        
            flatSys(flatIDX)= sys(i,j);
        end
    end  
    
    flatSysRes = solve(flatSys,flatU);
    
    sysRes = zeros(n,n);
    for i = 1:n
        for j = 1:n
            flatIDX = i*n+j-n;
            idx{1} = char(flatU(flatIDX));
            sysRes(i,j) = (flatSysRes.(idx{1}))/(n*n);
        end
    end
    nxn = xn(2:(n-1));
    nyn = yn(2:(n-1));
    nsysres = sysRes(2:(n-1),2:(n-1));
    mesh(nxn,nyn,nsysres);hold on
end
end

function res = kr2_test_g(x,y)
    %not yet implemented
end
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 

function ipr1()
    syms x
    xlb = -1;
    xub = 1; 
    n = 10;
    
    p=0; 
    b = 1;
    c = 1;
    q=-1;
    f=-x;
    
    b = cos(degtorad(26));
    c = sin(degtorad(26));
    q=-(1+b*x*x)/c;   
    f=-1/c;

    for n = 10:10:50
        Y = sym('y',[1,n]);
        xn = zeros(1,n);

        sys = sym('f',[1,n]);
        dx=((xub-xlb)/n);

        sys(1)=Y(1)+1;
        xn(1) = xlb;
        sys(n)=Y(n);
        xn(n) = xub;
        
        for i = 2:(n-1)
            xn(i) = xlb+dx*i;
            pxn = subs(p,'x',xn(i));
            qxn = subs(q,'x',xn(i));
            fxn = subs(f,'x',xn(i));
            sys(i) = (Y(i+1)-2*Y(i)+Y(i-1))/(dx*dx) + pxn*((Y(i+1)-Y(i))/(2*dx)) - qxn*Y(i) - fxn;
            sys(i) = sys(i);
        end

        sysRes = solve(sys,Y);

        for i = 1:n
            idx{1} = char(Y(i));
            Y(i)=(sysRes.(idx{1}));
        end

        plot(xn,Y); hold on 
    end
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

% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
function ipr2()
    syms x
    l = 1;
    xlb = 0;
    xub = l;
    
    syms t
    T = 0.2;
    tlb = 0;
    tub = T;
    
    n = 13;
    U = sym('u',[n,n]);
    
    xn = zeros(1,n);
    tn = zeros(1,n);
      
    a = 0
    b = 1
    UA = 0
    UB = 1
    
    fx = 3*log(x);
    gx = (1-exp(-t))*fx;
    kx = 1/x;
    sys = sym('f',[n,n]);
    dx=((xub-xlb)/n);
    dt=((tub-tlb)/n);
    
    for j = 1:n
        sys(1,j)=U(1,j)-UA;
        sys(n,j)=U(n,j)-UB;
    end
    
    for i = 1:n
        sys(i,1)=U(i,1)-a;
        sys(i,n)=U(i,n)-b;
    end
    
    for i = 2:(n-1)
        xn(i) = xlb+dx*i;
        for j = 2:(n-1)
            tn(j) = tlb+dt*j;
            sys(i,j) = ((U(i,j+1)-U(i,j))/dt) - subs(kx,'x',xn(i)) * ((U(i+1,j)-2*U(i,j)+U(i-1,j))/(dx*dx)) -subs(subs(gx,'x',xn(i)),'t',tn(j));
        end
    end
    
    flatSys = sym('f',[1,n*n]);
    flatU = sym('u',[1,n*n]);
    for i = 1:n
        for j = 1:n
            flatIDX = i*n+j-n;
            flatU(flatIDX) = U(i,j);        
            flatSys(flatIDX)= sys(i,j); 
        end
    end  
    
    flatSysRes = solve(flatSys,flatU);
    
    sysRes = zeros(n,n);
    for i = 1:n
        for j = 1:n
            flatIDX = i*n+j-n;
            idx{1} = char(flatU(flatIDX));
            sysRes(i,j) = (flatSysRes.(idx{1}))/(n*n);
        end
    end
    
     mesh(tn,xn,sysRes);hold on
end
% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
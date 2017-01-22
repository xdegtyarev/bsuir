function mna2
    ipr1();
%       kr1();
%     ipr2();
%     kr2();
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
    
end



% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 

function ipr1()
    syms x
    lb = -1;
    ub = 1;
    e = 0.001;
    pn = (ub-lb)/e;
    n = 40;
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

function res = ipr1_kfunc(Y,k,n,xk,h,p,q,f)
    Y0 = -1;%'A';
    Yn = 0;%'B';
    qx = subs(q,'x',xk)*Y(k)
    fx = subs(f,'x',xk)
    if k == 1
        'hello'
        res = (Y(k+1)-2*Y(k)+Y0)/(h*h) + subs(p,'x',xk)*((Y(k+1)-Y(k))/(2*h)) - subs(q,'x',xk)*Y(k) - subs(f,'x',xk);
    elseif k == n
        'hello'
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
        sys(k) = (ipr1_kfunc(Y,k,n,xk(k),h,p,q,f));
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

end

% % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % % 
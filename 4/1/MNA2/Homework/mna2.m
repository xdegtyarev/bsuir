function mna2
    kr1();
%     kr2();
%     ipr1();
%     ipr2();
end

function kr1()
    kr1_colocationMethod();
    kr1_MNK();
    kr1_Galerkin();
end

function kr1_colocationMethod()
    syms x
    lb = 0;
    ub = 1;
    
    n = 2;
    A = sym('a',[1,n]);
    baseFunc0 = '0';
%     b = 1;
%     c = 1;
    b = cos(degtorad(26))
    c = sin(degtorad(26))
    
    p=0;
     q=-(1+b*x*x)/c;
%     q=-1
     f=-1/c;
%     f=-x
    res = kr1_approx(A,lb,ub,n,baseFunc0,p,q,f);
    ezplot(res,lb,ub);hold on
end

function res = kr1_baseFunc(i)
    baseFunc = '(x^n)*(1-x)'; 
    res = subs(baseFunc,'n',i);
end

function res = kr1_lfunc(i,p,q)
    basefunci = kr1_baseFunc(i);
    res = diff(basefunci,2)+p*diff(basefunci,1)-q*(basefunci);
end

function res = kr1_nevfunc(A,baseFunc0,n,p,q,f)
    symF = sym(f);
    symB0 = sym(baseFunc0);
    nevfunc = diff(symB0,2)+p*diff(symB0,1)-q*symB0;
    for i = 1:n
        nevfunc = nevfunc + A(1,i)*kr1_lfunc(i,p,q);
    end
    res = nevfunc - symF;
end

function res = kr1_solveColocationSystem(A,lb,ub,n,nevfunc)    
    colocationSystem = sym('f',[1,n]);
    sysIdx = 1;

    for i = lb:((ub-lb)/(n-1)):ub
        colocationSystem(sysIdx) = subs(nevfunc,'x',i);
        sysIdx = sysIdx + 1;
    end
    
    res = solve(colocationSystem,A);
end

function res = kr1_approx(A,lb,ub,n,baseFunc0,p,q,f)
    sysRes = kr1_solveColocationSystem(A,lb,ub,n,kr1_nevfunc(A,baseFunc0,n,p,q,f));
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

function kr1_MNK()

end

function kr1_Galerkin()

end

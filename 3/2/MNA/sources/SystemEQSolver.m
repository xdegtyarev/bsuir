function SystemEQSolver()
m = 0.3;
a = 1;
e = 0.0001;
syms x1 x2;

f1 = tan(x1*x2+m) - x1;
f2 = a*x1*x1+2*x2*x2 - 1;

gx1 = tan(x1*x2+0.3);
gx2 = (1/2 - x1^2/2)^(1/2)

plottedx1 = 0.7
plottedx2 = 0.4

%%%% using simple iteration
disp(SimpleIteration(plottedx1,plottedx2,gx1,gx2,e))
%%%%

%%%% using newton method
disp(NewtonMethod(plottedx1,plottedx2,f1,f2,e))
%%%%

%%%%%% plottng
ezplot('tan(x*y+0.3) = x'),hold on
ezplot('x*x+2*y*y = 1'), title('')
%%%%%% 
end


function res = SimpleIteration(x01,x02,gx1,gx2,e)
    syms x1 x2 real

    %%%% checking divergence
    g1x1x1 = diff(gx1,x1)
    g1x1x2 = diff(gx1,x2)
    g1x2x1 = diff(gx2,x1)
    g1x2x2 = diff(gx2,x2)

    disp(double(subs(subs(g1x1x1,x1,x01),x2,x02)) + (double(subs(subs(g1x2x1,x1,x01),x2,x02))))
    disp(double(subs(subs(g1x1x2,x1,x01),x2,x02)) + (double(subs(subs(g1x2x2,x1,x01),x2,x02))))
    %%%%

    x1c = x01+e+e
    x2c = x02+e+e
    x1n = x01;
    x2n = x02;
    
    iter = 0
    while(abs(x1n-x1c)>e || abs(x2n-x2c)>e)
        iter = iter + 1
        x1c = x1n
        x2c = x2n
        x2n = double(subs(subs(gx2,x1,x1c),x2,x2c));
        x1n = double(subs(subs(gx1,x1,x1c),x2,x2n));
    end
    res = [x1n,x2n,iter]
end

function res = NewtonMethod(x01,x02,f1,f2,e)
    syms x1 x2 real
    
    df1x1 = diff(f1,x1) 
    df1x2 = diff(f1,x2)
    df2x1 = diff(f2,x1)
    df2x2 = diff(f2,x2)
     
    x1c = x01+e+e
    x2c = x02+e+e
    x1n = x01;
    x2n = x02;

    iter = 0
    while(abs(x1n-x1c)>e || abs(x2n-x2c)>e)
        iter=iter+1;
        x1c = x1n
        x2c = x2n
        J=[double(subs(subs(df1x1,x1,x1c),x2,x2c)), double(subs(subs(df1x2,x1,x1c),x2,x2c));
           double(subs(subs(df2x1,x1,x1c),x2,x2c)), double(subs(subs(df2x2,x1,x1c),x2,x2c))]^-1
        f=[double(subs(subs(f1,x1,x1c),x2,x2c)), double(subs(subs(f2,x1,x1c),x2,x2c))]
        fJ=J*f';
        x1n=x1c-fJ(1,1);
        x2n=x2c-fJ(2,1);
    end
    res = [x1n,x2n,iter]
end


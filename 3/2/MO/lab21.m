function start
lab23()
end

function gradProjMethod()
    
end

function complexBoxMethod()

end

function lab22
syms x1 x2 x3
x = [x1,x2,x3]
f = x1^2+1/2*x2^2+2*x3^2

g = [1,1,1;-2,-1,-3;1,-1,0;1,1/2,-1;-1,2,-2;0,1,-3]
b = [2;-5;2;1;-3;-1]
xk = [0,0,2]

g*xk' - b % meet req

syms t
f0 = subs(f,x,xk)

end

function lab23
syms x1 x2
x = [x1,x2]
f = (x1-x2)^2+((x1+x2-10)/3)^2
x0 = [0,1]
eps = 0.001
%%Drawing levels of function
sx1 = -0.02:eps:5.02;
sx2 = -0.02:eps:5.02;
[X1,X2] = meshgrid(sx1,sx2);
Z = (X1-X2).^2+1/9*(X1+X2-10).^2;
contour(X1,X2,Z,50)

hold on
res1 = fastestSlope(f,x,x0,eps)
plot(res1.p(1,:),res1.p(2,:))

res2 = fletcherReaves(f,x,x0,eps)
plot(res2.p(1,:),res2.p(2,:))
hold off
disp(res1.i)
disp(res2.i)
end


function grd = GradInPoint(f,x,x0)
    grd = [];
    for xn = x
        dfxn = diff(f,xn);
        grd = [grd,double(subs(dfxn,x,x0))]
    end
    return;
end


function res = fastestSlope(f,x,xk,eps)
    mag = eps*2;
    syms t0
    resxk = [xk']
    iter = 0
    while(mag > eps)
        g = GradInPoint(f,x,xk)
        af = subs(f,x,(xk - t0*g));
        t = eps;
        tval = double(subs(af,t0,eps));
        for(nt = eps:eps:1)
            ntval = double(subs(af,t0,nt));
            if(ntval<tval)
                t = nt;
                tval = ntval;
            end
        end
        disp(t)
        xk = xk - t*g
        resxk = [resxk,xk']
        mag = norm(g);
        iter = iter + 1;
    end
    res = struct('p',resxk,'i',iter)
end

function res = fletcherReaves(f,x,xk,eps)
    mag = eps*2;
    syms t0
    resxk = [xk']    
    gn = GradInPoint(f,x,xk)
    iter = 0
    while(mag > eps)
        g = gn;
        af = subs(f,x,(xk - t0*g));
        t = eps;
        tval = double(subs(af,t0,eps));
        for(nt = eps:eps:1)
            ntval = double(subs(af,t0,nt));
            if(ntval<tval)
                t = nt;
                tval = ntval;
            end
        end
        xk = xk - t*g
        gn = GradInPoint(f,x,xk)
        w = norm(gn)^2/norm(g)^2
        gn = gn + w * g 
        resxk = [resxk,xk']
        mag = norm(gn);
        iter = iter + 1;
    end
    
    res = struct('p',resxk,'i',iter)
end
function DiffSolver
syms x y
e = 0.001

a = 0.5
m = 2

lb = 0
ub = 1

h = 0.01
h2 = h/2

y1 = a*(1-y^2)/((1+m)*x*x+y*y+1)

%%%%% Euler Method
e1 = EulerMethod(y1,lb,ub,h)
e2 = EulerMethod(y1,lb,ub,h2)
%%%%%

%%%%% RungeMethod
r1 = RungeMethod(y1,lb,ub,h)
r2 = RungeMethod(y1,lb,ub,h2)
%%%%%

%%%%% AdamsMethod
a1 = AdamsMetod(y1,lb,ub,h)
a2 = AdamsMetod(y1,lb,ub,h2)
%%%%% 

plot(e1.x,e1.y,e2.x,e2.y,r1.x,r1.y,r2.x,r2.y,a1.x,a1.y,a2.x,a2.y)

end

function res = AdamsMetod(f0,a,b,h)
    syms x y
    xk = a
    yk = 0
    yp = 0
    resx = [xk]
    resy = [yk]
    while(xk<b)
        xk = xk+h
        yn = double(subs(subs(f0,x,xk),y,yk))
        yk = yk+h*(1.5*yn-0.5*yp)
        yp = yn
        resx = [resx,xk]
        resy = [resy,yk]
    end
    res = struct('x',resx,'y',resy)
    return
end

function res = RungeMethod(f0,a,b,h)
    syms x y
    xk = a
    yk = 0
    resx = [xk]
    resy = [yk]
    while(xk<b)
        xk = xk+h
        k1 = h*double(subs(subs(f0,x,xk),y,yk))
        k2 = h*double(subs(subs(f0,x,xk+h*0.5),y,yk+k1/2))
        k3 = h*double(subs(subs(f0,x,xk+h*0.5),y,yk+k2/2))
        k4 = h*double(subs(subs(f0,x,xk+h),y,yk+h*k3))
        yk = yk + (k1+2*k2+2*k3+k4)/6
        resx = [resx,xk]
        resy = [resy,yk]
    end
    res = struct('x',resx,'y',resy)
    plot(resx,resy)
    return
end

function res = EulerMethod(f0,a,b,h)
    syms x y
    xk = a
    yk = 0
    resx = [xk]
    resy = [yk]
    
    while(xk<b)
        xk = xk+h
        yk = yk+h*double(subs(subs(f0,x,xk),y,yk)) 
        resx = [resx,xk]
        resy = [resy,yk]
    end
    res = struct('x',resx,'y',resy)
    return
end
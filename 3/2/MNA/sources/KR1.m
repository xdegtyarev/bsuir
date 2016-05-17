function KR1
%     KR1_6()
%     KR1_162()
    syms x
      KR1_26((x^3+4)^(1/2),2,12,1)
end

function KR1_6
    a = 6000
    p = 0.085
    t = 3
    disp(a*(1+p)^3) 
end

function KR1_16
    syms x
    a = 1.08
    b = 0.06
    c = 0.003
    d = 1.952
    f = a*x^2+b*x+c
    x = -2:0.01:2
    figure
    plot(x,eval(f))
    return
end

function KR1_162
    syms x
    a = 1.08
    b = 0.06
    c = 0.003
    d = 1.952
    f = a*x^3+b*x^2+c*x+d
    x = -2:0.01:2
    figure
    plot(x,eval(f))
    return
end

function res = KR1_26(f0,a,b,h)
    syms x
    xn = a
    i = 0
    summ = double(subs(f0,x,a)) + double(subs(f0,x,b))
    while(xn<(b-1))
        xn = xn + h
        i = i + 1
        partialSumm = double(subs(f0,x,xn));
        if(0 == mod(i,2))
            summ = summ + 2 * partialSumm;
        else
            summ = summ + 4 * partialSumm;
        end        
    end
    res = summ * h / 3
end

function KR1_36
    % 
    y1 = 3*x^2+6*y
    lb = 0.0
    ub = 0.5
    h = 0.1
    % 
    r = RungeMethod(y1,lb,ub,h)

    y2 = 3 + 18*x + 54*x^2 + 109*x^3 + 0.5*327*x^4
    resy2=[3,double(subs(y2,x,0.1)),
        double(subs(y2,x,0.2)),
        double(subs(y2,x,0.3)),
        double(subs(y2,x,0.4)),
        double(subs(y2,x,0.5))]

    plot(r.x,r.y,r.x,resy2)
    return
end

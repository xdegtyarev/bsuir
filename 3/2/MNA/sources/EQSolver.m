function EQSolver()
    syms x
    a = '20.2374';
    b = '-131.210';
    c = '-843.923';
    e = 0.0001;
    lb = -10;
    ub = 10;
%     f0 = x*x*x + a*x*x + b*x + c;
    a = '1.08'
    b = '0.06'
    c = '0.003'
    d = '1.952'
    f0 = a*x^3+b*x^2+c*x+d
%%%% Sturm Method
    disp(SturmMethod(f0,lb,ub));
%%%%
    disp(NewtonMethod(f0,-1.5,-1,0,0.000001))
    return
    
%%%% root separation
       for i=-10:0.1:10
        if SturmMethod(f0,i,i+0.1) > 0
           disp(i); 
        end
       end
%%%%

    

%%%% division by half method
     disp(BisectionMethod(f0,-10,0,0,e));
     disp(BisectionMethod(f0,0,10,0,e));
%%%%
     
%%%% hordes method
      disp(ChordMethod(f0,-10,0,0,e));
      disp(ChordMethod(f0,0,10,0,e));
%%%%

%%%% newton method
      disp(NewtonMethod(f0,-10,0,0,e));
      disp(NewtonMethod(f0,0,10,0,e));   
%%%%

%%%% plot
      figure
      x = lb:0.01:ub
      plot(x,eval(f0)); 
%%%%
end

function res = SturmMethod(f0,lb,ub)
    res = 0;
    syms x;
    
    f1 = diff(f0);
    [q,r] = quorem(f0,f1,x);
    f2 = -r;
    [q,r] = quorem(f1,f2,x);
    f3 = -r;
       
    if (0 >= double(subs(f0,x,lb))) ~= (0 >= double(subs(f1,x,lb)))
        res = res + 1;
    end
    
    if (0 >= double(subs(f1,x,lb))) ~= (0 >= double(subs(f2,x,lb)))
        res = res + 1;
    end
    
    if (0 >= double(subs(f2,x,lb))) ~= (0 >= double(subs(f3,x,lb)))
        res = res + 1;
    end

   
    if (0 >= double(subs(f0,x,ub))) ~= (0 >= double(subs(f1,x,ub)))
        res = res - 1;
    end
    
    if (0 >= double(subs(f1,x,ub))) ~= (0 >= double(subs(f2,x,ub)))
        res = res - 1;
    end
    
    if (0 >= double(subs(f2,x,ub))) ~= (0 >= double(subs(f3,x,ub)))
        res = res - 1;
    end
    %here we got a number of roots
end

function res = BisectionMethod(f0,a,b,iter,e)
    syms x
    xn = a
    xk = b;
    xi = a;
    dx = xk-xn
    while(abs(xk-xn)>e)
       iter = iter + 1
       disp(iter)
       dx = dx/2
       xi = xn + dx
       if(sign(subs(f0,x,xn)) ~= sign(subs(f0,x,xi)))
           xk = xi
       else
           xn = xi
       end
    end
    res = [xi,iter]
end

function res = ChordMethod(f0,a,b,iter,e)
        %% for ) curve
        syms x
        fa = subs(f0,x,a);
        fb = subs(f0,x,b);
        newa = a - fa / (fb - fa) * ( b - a )
        iter = iter+1
        if abs(newa-a) < e
            res = [a,iter]
            return
        else
            res = ChordMethod(f0,newa,b,iter,e)
            return
        end            
end

function res = NewtonMethod(f0,a,b,iter,e)
        syms x
        fa0 = subs(f0,x,a);
        fa1 = subs(diff(f0),x,a);
        newa = a - fa0 / fa1
        iter = iter+1
        if abs(newa-a) < e 
            res = [a,iter]
            return
        else
            res = NewtonMethod(f0,newa,b,iter,e)
            return
        end    
end

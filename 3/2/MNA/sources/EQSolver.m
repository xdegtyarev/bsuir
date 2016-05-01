function EQSolver()
    disp('Input formala');
    syms x
    a = '20.2374';
    b = '-131.210';
    c = '-843.923';
    e ='0.0001';
    lb = '-10';
    ub = '10';
    f0 = x*x*x + a*x*x + b*x + c;
    
%%%% Sturm Method
    disp(SturmMethod(f0,lb,ub));
%%%%     
    
%%%% root separation
%      for i=-10:0.1:10
%       if SturmMethod(f0,i,i+0.1) > 0
%          disp(i); 
%       end
%      end
%%%%

%%%% division by half method

%%%%
     
%%%% hordes method
     disp(ChordMethod(f0,-10,0));
     disp(ChordMethod(f0,0,10));
%%%%

%%%% newton method
     disp(NewtonMethod(f0,-5,-4));
     disp(NewtonMethod(f0,8,9));   
%%%%

%%%% plot
     figure
     plot(x,eval(f0),roots,0,'or'); 
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
    
%     disp(f0);
%     disp(f1);
%     disp(f2);
%     disp(f3);
       
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

function res = ChordMethod(f0,a,b)
        %% for ) curve
        syms x
        fa = subs(f0,x,a);
        fa = subs(f0,x,b);
        newa = a - fa / (fb - fa) * ( b - a )
        if abs(newa-a) < eps 
            res = a
            return
        else
            res = ChordMethod(f0,newa,b)
            return
        end            
end

function res = NewtonMethod(f0,a,b)
        syms x
        fa0 = subs(f0,x,a);
        fa1 = subs(diff(f0),x,a);
        newa = a - fa0 / fa1
        if abs(newa-a) < eps 
            res = a
            return
        else
            res = NewtonMethod(f0,newa,b)
            return
        end    
end

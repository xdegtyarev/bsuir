function start
quadSM()
end

function quadSM
    x = [17/10,12/5,0,1/4]
    Jb = [1,2,3]
    Jop = [1,2]
    c = [-8,-6,-4,-6]
    A = [1,0,2,1;0,1,-1,2]
    Ax = A(:,Jb)
 
    D = [2,1,1,0;1,1,0,0;1,0,1,0;0,0,0,0]
    
    cx = c'+D*x'
    cxt = cx'
    cop = cxt(:,Jop)
    Aop = A(:,Jop)
    Bop = Aop^-1
    
    ux = -cop * Bop
    d = A'*ux' + cx
    [dj0,j0] = min(d)
    j0 = 4
    
    DS = D(Jb,Jb)
    AS = A(Jop,Jb)
    Aj0 = A(:,j0)
    Dj0 = D(Jb,j0)
%     system 3.8
     BS = AS^-1;
     l = -Aj0'*BS
     y = -(Dj0+DS*l')'*BS' 
%     l = [3,-4,-2]
%     y = [0,1]
    
    dg = Dj0' * l' + Aj0' * y' + D(j0,j0)
    t1 = -x(Jb(1))/l(1)
    t2 = -x(Jb(2))/l(2)
%     t3 = -x(Jb(3))/l(3)
    tj0 = abs(d(j0)/dg)
    t0 = min(abs([t1,t2,tj0]))
    nx = x(Jb) + t0*l
end

function dualSM
     y = [90/8,-26/8]
     Jb = [3,4]
     A = [1,-1,3,-2;1,-5,11,-6]
     C = [1,1,-2,-3]
     b = [1;9]
     e = [0,0]
     Ab = A(:,Jb)
     B = Ab^(-1)   
     XB = B*b
     [djs,js] = min(XB)
     e(js) = 1;
     dy = e*B
     mj = dy*A
     [dj0,j0] = min(mj)
     aj0 = (A(:,j0))'
     sig = (C(:,j0)-(aj0 * y'))/mj(j0)
     ny = y + sig*dy
end

function directSM
     xj = [1,3]
     Jb = [2,4]
     A = [3,1,1,0,1,0;1,-2,0,1,0,1]
     C = [0,0,0,0,-1,-1]
     Ab = A(:,Jb)
     B = Ab^(-1)
     Cb = C(:,Jb)
     u = Cb*B
     d = u*A-C
     [dj0,j0] = min(d)   
       j0=2
     z = B*A(:,[j0])
     s = 2
     t = xj(s)/z(s)
     nxj = (xj'-(t*z))
end


% 21 A = [1,1,0,2,0; 0,-1,1,0,2; -1,0,1,-1,2] C = [1,1,1,1,1]
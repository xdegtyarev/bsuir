function start
% directSM();
dualSM()
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
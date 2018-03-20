%=============Functions=============
function res=PhiFunc(phi_x)
  res = phi_x^4;
end

function res=FX(X)
  r = zeros(size(X))
  for i = 1:size(X,2);
    if X(i)<-1
      r(i)=0;
    elseif X(i)>=-1 && X(i)<=3
      r(i)=((X(i)+1)/4);
    elseif X(i)>3
      r(i)=1
    end
  end
  res = r;
end

function res=fX(X)
   r = zeros(size(X))
  for i = 1:size(X,2);
    if X(i)<-1
      r(i)=0;
    elseif X(i)>=-1 && X(i)<=3
      r(i)=1/4;
    elseif X(i)>3
      r(i)=0;
    end
  end
  res = r;
end

function res=GY(Y)
   res = Y.^(1/4)/4+1/4;
end

function res=gY(lY)
   r = zeros(size(lY));
   for i = 1:size(lY,2)
       if lY(i)<=0
          r(i)=0;
        elseif lY(i)<=1
          r(i)=2/(16*(lY(i))^(3/4));
        elseif lY(i)>=1 && lY(i)<=81
          r(i)=1/(16*(lY(i))^(3/4));
       elseif lY(i)>81
          r(i)=0;
       end
   end
   res = r;
end
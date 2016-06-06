function start
lab22()
end

function lab22
syms x1 x2 x3
x = [x1,x2,x3]
f = x1^2+1/2*x2^2+2*x3^2

a = [1,1,1;-2,-1,-3;1,-1,0;1,1/2,-1;-1,2,-2;0,1,-3]
b = [2;-5;2;1;-3;-1]
x0 = [2,0,2]
eps = 0.001;

% drawArea(f,x,a,b,-2,2)


res = gradProjMethod(f,x,x0,a,b,eps)
plot3(res.p(1,:),res.p(2,:),res.p(3,:))

% res = complexBoxMethod(f,x,x0,a,b,-5,5,eps)
end

function bool = PointMeetsReq(a,b,xk)
bool = min(a*xk' - b)>0;
end

function res = gradProjMethod(f,x,xk,a,b,eps)
    mag = eps*2;
    syms t0
    resxk = [xk']
    iter = 0;
        
    while(mag > eps)
        iter = iter + 1;
        g = GradInPoint(f,x,xk);
        %%searching for active limitations%%
        act = a*xk'-b
        acti = [];
        i = 0;
        while i<length(act)
            i = i + 1;
            if(act(i)==0)
                acti = [acti,i];
            end
        end
            
        A = a(acti,[1,2,3])
        if(rank(A)==0)
            P = 1
        else
            P = eye(3)-A'*((A*A')^-1)*A;
        end
        
        if(norm(P)<eps)
            H = ((A*A')^-1)*A*g' ;
            [maxh,imaxh]=max(H)
            acti(imaxh)=[];
            A = a(acti,[1,2,3]);
            P = eye(3)-A'*((A*A')^-1)*A;
        end
        %%
        S = P*g';
        af = subs(f,x,(xk - t0*S'));
        t = eps;
        tval = double(subs(af,t0,eps));
        for(nt = eps:eps:1)
            ntval = double(subs(af,t0,nt));
            if(ntval<tval)
                t = nt;
                tval = ntval;
            end
        end
            prevxk = xk;
            xk = xk - t*S';
            resxk = [resxk,xk']
            mag = abs(norm(xk-prevxk));
    end
    res = struct('p',resxk,'i',iter)
end

function drawArea(f,x,a,b,l,u)
hold on
k = 0
i = 0
while (k<200) 
        p = l+rand(3,1000).*(u-l);
        i = 0;
        while(i<1000)
            i = i + 1;
            if(PointMeetsReq(a,b,p(:,i)'))
                scatter3(p(1,i),p(2,i),p(3,i))
                k = k+1
            end
        end
end
 hold off
end

function res = complexBoxMethod(f,x,x0,a,b,l,u,eps)
hold on
 n = length(x)*2;
 poly = [x0'];
 % creating random poly%
 k = 1;
 while (k<n) 
        p = l+rand(3,1).*(u-l);
        if(PointMeetsReq(a,b,p'))
            poly = [poly,p];
            k = k + 1;
        else
            % finding xc %
             xp = sum(poly,2);
             xc = xp/k;
             %%%%%
             attempt = 0;
             while(~PointMeetsReq(a,b,p') && attempt < 100)
                p = (p+xc)/2;
                attempt = attempt + 1;
             end
             if(PointMeetsReq(a,b,p'))
                poly = [poly,p];
                k = k + 1;
             end
        end
 end
 scatter3(poly(1,:),poly(2,:),poly(3,:))
 iter = 0
 while true
     iter = iter + 1
     % finding xh %
     ih = 0;
     maxf = 0;
     for i=1:1:k
         fxp = double(subs(f,x,poly(:,i)'));
         if(fxp>maxf)
             maxf = fxp;
             ih = i;
         end
     end 
     xh = poly(:,ih);
     %%%%%
     
     % finding xc %
     xc = (sum(poly,2)-xh) / (k-1);
     %%%%%
     alpha = 1.3;
     xr = xc-alpha*(xc-xh);
    
     while true
         if(~PointMeetsReq(a,b,xr'))
             for(i = 1:1:length(xr))
                 if(xr(i)<l)
                     xr(i)=l+eps;
                 end
                 if(xr(i)>u)
                     xr(i)=u-eps;
                 end
             end
             
             if(~PointMeetsReq(a,b,xr'))
                 xr = (xr + xc).*0.5; 
             end    
         else
             if(subs(f,x,xr')<subs(f,x,xh'))
                 poly(:,ih) = xr;
                 scatter3(poly(1,:),poly(2,:),poly(3,:))
                 fs = 0;

                 for i=1:1:length(poly)
                    fs = fs + double(subs(f,x,poly(:,i)'));
                    fs = fs - double(subs(f,x,xc'));
                 end   

                 sig = (fs*fs / (k-1))^ (1/2);
                 if(sig < eps)
                     res = poly;
                     hold off
                     return
                 else
                     break
                 end
             else
                 xr = (xr + xc).*0.5;  
             end
         end
     end
 end
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
        grd = [grd,double(subs(dfxn,x,x0))];
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
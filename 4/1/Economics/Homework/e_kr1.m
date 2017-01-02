function start
syms p;
Qd = 9 - p;
Qs = p + p - 6;
p1 = 0.0:0.1:20.0;
plot(p1,subs(Qd,p,p1),p1,subs(Qs,p,p1))
end
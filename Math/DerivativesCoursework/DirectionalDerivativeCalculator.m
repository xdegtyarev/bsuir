function res = DirectionalDerivativeCalculator()
    % listen to formula entrance type
    funcType = input('Select function type (implicit, explicit, parametric)','s');
    switch funcType
        case 'explicit'
            res = ExplicitDirectionalDerivativeCalculator();
        case 'implicit'
            res = ImplicitDirectionalDerivativeCalculator();
        case 'parametric'
            res = ParametricDirectionalDerivativeCalculator();
        otherwise
            disp('Wrong func type');
            res = 'ERROR';
    end
end

function res = ExplicitDirectionalDerivativeCalculator()
    disp('Counting Explicit Func Dir Derivative');
    func = input(strcat('Enter explicit function'),'s');
    % detect how many vars are there
    vars = unique(regexp(func,'[A-Za-z]\w*','match'));
    symbs = [];
    for v = vars
        symbs = horzcat(symbs,sym(v));
    end
    e = CreateVector(vars);
    esum = CountVectorLength(e);
    % function to calculate derivative by direction
    counteri = 1;
    for s = symbs    
        difres= diff(func,s);
        if(counteri == 1)
        res = difres * e(counteri)/esum;
        else
        res = res + (difres * e(counteri)/esum);
        end
        counteri = counteri + 1;
    end
    disp(res);
end

function res = ImplicitDirectionalDerivativeCalculator()
    disp('Counting Implicit Func Dir Derivative');
    func = input('Enter implicit function ','s');
    root = input('Enter root variable ','s');
    rootSym = sym(root);
    vars = unique(regexp(func,'[A-Za-z]\w*','match'));
    vars = setxor(root,vars);
    symbs = [];
    for v = vars
        symbs = horzcat(symbs,sym(v));
    end
    e = CreateVector(vars);
    esum = CountVectorLength(e);
    counteri = 1;
    rootDiff = diff(func,rootSym);
    for s = symbs    
        difres = diff(func,s)/rootDiff * (-1);
        if(counteri == 1)
        res = difres * e(counteri)/esum;
        else
        res = res + (difres * e(counteri)/esum);
        end
        counteri = counteri + 1;
    end
    pretty(res);
end

function res = ParametricDirectionalDerivativeCalculator()
    disp('Counting Parametric Func Dir Derivative');
    params = input('Enter number of parametric vars');
%   reading functions
    funcs = [];
    for i = 1:params            
        str = num2str(i);
        funcs = horzcat(funcs,sym(input(strcat('Enter parametric function',str,' :'),'s')));
    end     
    rootSym = sym(input('Enter root var','s'));
    e = CreateVector(params-1);
    esum = CountVectorLength(e);
    rootDiff = diff(funcs(1),rootSym);
    for i = 2:params
        difres = rootDiff/diff(funcs(i),rootSym);
        if(i == 2)
        res = difres * e(i-1)/esum;
        else
        res = res + (difres * e(i-1)/esum);
        end
    end
    pretty(res);
 end

function e = CreateVector(vars)
    e = [];
    for v = vars
        prompt = 'Enter coordinate for direction E_';
        e = horzcat(e,input(prompt));
    end
end

function esum = CountVectorLength(vector)
    esum = 0;
    for ecomp = vector
        esum = esum + ecomp^2;
    end
    esum = esum^(1/2);
end

       

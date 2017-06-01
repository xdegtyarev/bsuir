import std.stdio, std.getopt, std.stdio, std.file,std.utf,std.ascii, std.conv, std.regex;
import std.typecons : Yes;
import std.traits;

string inputFilePath = "test.c";
string outputFilePath = "out";
bool transliterate = false;
bool codegen = false;
bool verbose = false;
bool lex = false;

void main(string[] args)
{
	//parsing args
	GetoptResult helpInfo;
	try{
		helpInfo = getopt(args,
			std.getopt.config.required,
			"file|f","File to be compiled, test.c is checked by default",&inputFilePath,
			"verbose|v","This will print logs during execution",&verbose,
			"transliterate|t","This will read file and transliterate", &transliterate,
			"lexer|l","This will read file and make lexical analysis", &lex,
			"codegen|c","This will launch codegen demo and write bytecode to output file", &codegen);

	}catch(GetOptException exc){
		writeln(exc.msg);
		writeln("use -h to get help");
		return;
	}

	if(helpInfo.helpWanted){
		defaultGetoptPrinter("Test app usage is:", helpInfo.options);
	}else{
		if(transliterate){
			Transliterator();
		}else if(lex){
			LexicalAnalysis();
		}else if(codegen){
			IntermediateCodeGen();
		}
	}
}

public void Transliterator(){
	const string[wchar] transliterationMap = [
	'а':"a", 'А':"A", 'б':"b", 'Б':"B", 'в':"v", 'В':"V", 'г':"g", 'Г':"G",
	'д':"d", 'Д':"D", 'е':"e", 'Е':"E", 'ё':"e", 'Ё':"E", 'ж':"zh", 'Ж':"Zh",
	'з':"z", 'З':"Z", 'и':"i", 'И':"i", 'й':"i", 'Й':"I", 'к':"k", 'К':"K",
	'л':"l", 'Л':"L", 'м':"m", 'М':"M", 'н':"n", 'Н':"N", 'о':"o", 'О':"O",
	'п':"p", 'П':"P", 'р':"r", 'Р':"R", 'с':"s", 'С':"S", 'т':"t", 'Т':"T",
	'у':"u", 'У':"U", 'ф':"f", 'Ф':"F", 'х':"h", 'Х':"H", 'ц':"c", 'Ц':"C",
	'ч':"ch", 'Ч':"Ch", 'ш':"sh", 'Ш':"Sh", 'щ':"sc", 'Щ':"Sc", 'ъ':"", 'Ъ':"",
	'ы':"y", 'Ы':"y", 'ь':"", 'Ь':"", 'э':"e", 'Э':"e", 'ю':"iu", 'Ю':"Iu",
	'я':"ia", 'Я':"Ia"];

	auto inputFile = File(inputFilePath,"r");
	auto outputFile = File(outputFilePath,"w");

	string line;
	string tch;
	if(verbose)	writeln("--Start Reading File--");
	while(!inputFile.eof()){
		line = inputFile.readln();
		if(verbose)	writeln("line:\n",line);
		foreach(wchar ch; line){
			tch = (ch in transliterationMap)?transliterationMap[ch]:to!string(ch);
			if(verbose) writeln("|",ch," : ",tch);
			outputFile.write(tch);
		}
	}
	if(verbose)	writeln("--Ended Reading File--");
}

//COMPILER FRONT-END
//////////////////////
//CHARACTER SET:
//Any unicode character
//////////////////////
public class Token{
	static string[string] lexerRegExp;
	static string[] lexerKeyOrder = [
		"OPERATOR2","OPERATOR1","COMMA","KEYWORD","SEMICOLON","TYPE","INTEGERLITERAL","ID"
	];
	public static string splitterRegExp;

	public string content;
	public string type;

	this(string input){
		content = input;
		//maximal munch technic
		foreach(tokenType; lexerKeyOrder){
			auto pattern = regex(lexerRegExp[tokenType]);
			if(!matchFirst(content,pattern).empty){
				type = tokenType;
				break;
			}
		}

	}

	static this(){
		lexerRegExp = [
		"OPERATOR2" : r":=|==|<|>|!=|=|\+|-",
		"OPERATOR1" : r"\+\+|--", /*"*/
		"COMMA" : ",",
		"KEYWORD" : "begin|end|for|to|do",
		"SEMICOLON" : ";",
		"TYPE" : "int|float",
		"INTEGERLITERAL" : "[0-9]+",
		"ID" : "[a-zA-Z][a-zA-Z0-9]*"
		];

		splitterRegExp =
		"\u0020+|\u0009+|\u000B+|\u000C+|" ~ //WHITESPACE
		"\u0000+|\u001A+|" ~ //EOF
		"\u000D+|\u000A+|\u000D \u000A|\u2028+\u2029+|" ~ //EOL
		lexerRegExp["OPERATOR2"] ~ "|" ~
		lexerRegExp["OPERATOR1"] ~ "|" ~
		lexerRegExp["COMMA"] ~ "|" ~
		lexerRegExp["SEMICOLON"];

	}

	public override string toString(){
		return "token: " ~ content ~ " is: " ~ type;
	}
}

public Token[] LexicalAnalysis(){
	auto inputFile = File(inputFilePath,"r");
	auto splitPattern = regex(Token.splitterRegExp);
	auto nonemptyChunkPattern = regex(r"\S");
	/*
	"<-linterfix
	*/
	string line;
	Token[] tokenChain;
	if(verbose)	writeln("--Start Reading File--");
	while(!inputFile.eof()){
		line = inputFile.readln();
		if(verbose)	writeln("line: ",line);
		foreach(chunk; line.splitter!(Yes.keepSeparators)(splitPattern)){
			if(!matchFirst(chunk,nonemptyChunkPattern).empty){
				tokenChain ~= new Token(chunk);
				if(verbose)	writeln(tokenChain[$-1]);
			}
		}
	}
	if(verbose)	writeln("--Ended Reading File--");
	return tokenChain;
}

public void SyntaxAnalyzer(){
 	//syntax tree
}

public void SemanticAnalyzer(){
	//data compatability
	//annotated syntax tree
}

//ipr1 TODO:
//int float struct
//type id;
//type id = literal;
//lop = rop
//+ - / * == != < >

//AST-TREE has such order [OP|OPERAND1|OPERAND2|RES]
////COMPILER BACK-END
public void IntermediateCodeGen(){

	auto outputFile = File(outputFilePath,"w");
	ActivationNode[] codeDAG;
	//generarte intermediate lang
	auto testnode = new ActivationNode(new ActivationNode("op","-"),new ActivationNode("id","x","int"),new ActivationNode("literal","12"));
	codeDAG ~= testnode;
	auto memOffset = 0;
	//DAG
	foreach(node;codeDAG){
		writeln(node.toByteCode(false));
		//outputFile.write(node.toByteCode(false));
	}
}



public class ActivationNode{
	public string type;
	public string content;
	public string argument;
	public ActivationNode op;
	public ActivationNode loperand;
	public ActivationNode roperand;
	this(ActivationNode op, ActivationNode lop, ActivationNode rop){
		this.op = op;
		loperand = lop;
		roperand = rop;
	}

	this(string t, string cont, string arg = ""){
		type = t;
		content = cont;
		argument = arg;
	}
	//hosts in AST MANNER TREE INSIDE
	//MNEMONICS MODE
	string toByteCode(bool lop){
		//end node
		if(!op && !loperand && !roperand){
			switch(type){
				case "op":
					return getOpCode(content);
				case "id":
					if(lop){
						//get address to move
						return "store_0" ~ " " ~ content;
					}else{
						//TODO: we need to fetch runtime table to get proper reg
						return "load_1" ~ " " ~ content;
					}
				case "literal":
					if(lop){
						//no assignment to const
						return "ERROR!";
					}else{
						return "const_2" ~ " " ~ content;
					}
				default:
					assert(0);
			}
		}else{
			//traverse tree
			auto typecode = loperand.argument=="int"?"i_":"f_";
			return roperand.toByteCode(false) ~ "\n" ~
			((op.type == "assign") ?
			(typecode ~ op.toByteCode(false) ~ "\n" ~ loperand.toByteCode(true)) :
			(loperand.toByteCode(false) ~ "\n" ~ typecode ~ op.toByteCode(false)));
		}

		//here we can make some code opts but we need runtime data
	}

	static string getOpCode(string cont){
		switch(cont){
			case "+":
			return "add";
			case "-":
			return "neg \n add";
			case "*":
			return "mul";
			case "/":
			return "div";
			case "==":
			return "cmp";
			case "!=":
			return "cmp";
			case "<":
			return "cmp";
			case ">":
			return "cmp";
			default:
				assert(0);
		}
	}
}
//codeopt
//final codegen


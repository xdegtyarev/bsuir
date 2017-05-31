import std.stdio, std.getopt, std.stdio, std.file,std.utf,std.ascii, std.conv, std.regex;
import std.typecons : Yes;
import std.traits;

string inputFilePath = "test.c";
string outputFilePath = "out";
bool transliterate = false;
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
			"lexer|l","This will read file and make lexical analysis", &lex);

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

////COMPILER BACK-END

public void CodeGen(){
//generarte intermediate lang
}

//codeopt

//final codegen


import std.stdio, std.getopt;

string filePath = "test.c";
bool verbose = false;
void main(string[] args)
{
	auto helpInfo = getopt(args,
		"file|f","File to be compiled, test.c is checked by default",&filePath,
		"verbose|v","This will print lots of logs",&verbose);

	if(helpInfo.helpWanted){
		defaultGetoptPrinter("Test app usage is:", helpInfo.options);
	}else{
		writeln("Hello world.");
	}


}

public void ReadFile(){

}

public void LexicalAnalyzer(){

}

public void SyntaxAnalyzer(){

}

public void SemanticAnalyzer(){

}

public void CodeGen(){

}
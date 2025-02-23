int IncrementalProcessing(FILE *handle);
int NotIncrementalProcessing(FILE *handle);

int LexicalAnalyzer();
int SyntacticAnalyzer();
int SemanticAnalyzer();
int BytecodeGenerator();
int MachineCodeGenerator();
int IncrementalProcessing(FILE *handle);
int NotIncrementalProcessing(FILE *handle);

int LexicalAnalyzer();
int SyntacticAnalyzer();
int SemanticAnalyzer();
int CodeOptimizer();
int CodeGenerator();
//int BytecodeGenerator();
//int MachineCodeGenerator();
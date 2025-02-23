int IncrementalProcessing(FILE *handle);
int NotIncrementalProcessing(FILE *handle);
int LexicalAnalyzer();
int SyntacticAnalyzer(const unsigned char *token);
int SemanticAnalyzer(const unsigned char *token);
int BytecodeGenerator(const unsigned char *token);
int MachineCodeGenerator(const unsigned char *token);
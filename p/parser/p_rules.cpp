module p.rules;

namespace p::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 484771829123121153, "CommonParser.UnitName" },
        { 484771829123121154, "CommonParser.UsesClause" },
        { 484771829123121155, "CommonParser.Identifier" },
        { 484771829123121156, "CommonParser.IdentifierList" },
        { 2310102263856627713, "ExpressionParser.ConstantExpression" },
        { 2310102263856627714, "ExpressionParser.Expression" },
        { 2310102263856627715, "ExpressionParser.RelationalOperator" },
        { 2310102263856627716, "ExpressionParser.SimpleExpression" },
        { 2310102263856627717, "ExpressionParser.WeakOperator" },
        { 2310102263856627718, "ExpressionParser.Term" },
        { 2310102263856627719, "ExpressionParser.StrongOperator" },
        { 2310102263856627720, "ExpressionParser.Factor" },
        { 2310102263856627721, "ExpressionParser.Primary" },
        { 2310102263856627722, "ExpressionParser.UnsignedConstant" },
        { 2310102263856627723, "ExpressionParser.UnsignedNumber" },
        { 2310102263856627724, "ExpressionParser.Sign" },
        { 2310102263856627725, "ExpressionParser.CharacterString" },
        { 2310102263856627726, "ExpressionParser.Qualifier" },
        { 2310102263856627727, "ExpressionParser.Index" },
        { 2310102263856627728, "ExpressionParser.MemberSelection" },
        { 2310102263856627729, "ExpressionParser.ArgumentList" },
        { 2310102263856627730, "ExpressionParser.Argument" },
        { 2310102263856627731, "ExpressionParser.NewExpression" },
        { 4547404054656974849, "StatementParser.Statement" },
        { 4547404054656974850, "StatementParser.SimpleStatent" },
        { 4547404054656974851, "StatementParser.AssignmentStatement" },
        { 4547404054656974852, "StatementParser.ExpressionStatement" },
        { 4547404054656974853, "StatementParser.StructuredStatement" },
        { 4547404054656974854, "StatementParser.CompoundStatement" },
        { 4547404054656974855, "StatementParser.ConditionalStatement" },
        { 4547404054656974856, "StatementParser.IfStatement" },
        { 4547404054656974857, "StatementParser.CaseStatement" },
        { 4547404054656974858, "StatementParser.Case" },
        { 4547404054656974859, "StatementParser.ConstantRange" },
        { 4547404054656974860, "StatementParser.ElsePart" },
        { 4547404054656974861, "StatementParser.RepetitiveStatement" },
        { 4547404054656974862, "StatementParser.RepeatStatement" },
        { 4547404054656974863, "StatementParser.WhileStatement" },
        { 4547404054656974864, "StatementParser.Direction" },
        { 4547404054656974865, "StatementParser.ForStatement" },
        { 4547404054656974866, "StatementParser.ControlVariable" },
        { 4547404054656974867, "StatementParser.InitialValue" },
        { 4547404054656974868, "StatementParser.FinalValue" },
        { 5007228952788336641, "MainParser.Main" },
        { 636845565828661249, "ProgramParser.Program" },
        { 636845565828661250, "ProgramParser.ProgramParameterList" },
        { 109874149719015425, "UnitParser.Unit" },
        { 109874149719015426, "UnitParser.InterfacePart" },
        { 109874149719015427, "UnitParser.ProcedureAndFunctionHeadingPart" },
        { 109874149719015428, "UnitParser.ImplementationPart" },
        { 109874149719015429, "UnitParser.InitializationPart" },
        { 7470045152585711617, "BlockParser.Block" },
        { 7470045152585711618, "BlockParser.SubroutineBlock" },
        { 7470045152585711619, "BlockParser.DeclarationPart" },
        { 7470045152585711620, "BlockParser.ConstantDeclarationPart" },
        { 7470045152585711621, "BlockParser.TypeDeclarationPart" },
        { 7470045152585711622, "BlockParser.VariableDeclarationPart" },
        { 7470045152585711623, "BlockParser.SubroutineDeclarationPart" },
        { 7470045152585711624, "BlockParser.StatementPart" },
        { 7339729196215697409, "ConstantParser.ConstantDeclaration" },
        { 7339729196215697410, "ConstantParser.Constant" },
        { 7339729196215697411, "ConstantParser.ArrayConstant" },
        { 7339729196215697412, "ConstantParser.ObjectConstant" },
        { 1060958820211097601, "TypeParser.TypeName" },
        { 1060958820211097602, "TypeParser.TypeDeclaration" },
        { 1060958820211097603, "TypeParser.Type" },
        { 1060958820211097604, "TypeParser.SimpleType" },
        { 1060958820211097605, "TypeParser.OrdinalType" },
        { 1060958820211097606, "TypeParser.SubrangeType" },
        { 1060958820211097607, "TypeParser.EnumeratedType" },
        { 1060958820211097608, "TypeParser.OrdinalTypeName" },
        { 1060958820211097609, "TypeParser.RealType" },
        { 1060958820211097610, "TypeParser.StringType" },
        { 1060958820211097611, "TypeParser.PointerType" },
        { 1060958820211097612, "TypeParser.ObjectType" },
        { 1060958820211097613, "TypeParser.Heritage" },
        { 1060958820211097614, "TypeParser.GenericPart" },
        { 1060958820211097615, "TypeParser.ComponentList" },
        { 1060958820211097616, "TypeParser.ObjectFieldList" },
        { 1060958820211097617, "TypeParser.MethodList" },
        { 1060958820211097618, "TypeParser.ArrayType" },
        { 1060958820211097619, "TypeParser.Specialization" },
        { 6197679748483645441, "VariableParser.VariableDeclaration" },
        { 8014466136055742465, "SubroutineParser.ProcedureDeclaration" },
        { 8014466136055742466, "SubroutineParser.FunctionDeclaration" },
        { 8014466136055742467, "SubroutineParser.ConstructorDeclaration" },
        { 8014466136055742468, "SubroutineParser.ConstructorCall" },
        { 8014466136055742469, "SubroutineParser.Virtual" },
        { 8014466136055742470, "SubroutineParser.MethodHeading" },
        { 8014466136055742471, "SubroutineParser.SubroutineId" },
        { 8014466136055742472, "SubroutineParser.ProcedureHeading" },
        { 8014466136055742473, "SubroutineParser.FunctionHeading" },
        { 8014466136055742474, "SubroutineParser.ConstructorHeading" },
        { 5515974103946756097, "ParameterParser.ParameterList" },
        { 5515974103946756098, "ParameterParser.ParameterDeclaration" },
        { 5515974103946756099, "ParameterParser.ParameterQualifier" }
    };
    return &ruleNameMap;
}

} // p::rules

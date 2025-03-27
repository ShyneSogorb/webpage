export interface IClassDocumentation {
    name: string;
    functions: IFunctionDocumentation[];
    variables: IVariableDocumentation[];
    comment: ICommentDocumentation;
    umacro: IMacroDocumentation;
}

export interface IFunctionDocumentation {
    name: string;
    return_type: string;
    comment: ICommentDocumentation;
    parameters: IParameterDocumentation[];
    umacro: IMacroDocumentation;
}

export interface IParameterDocumentation {
    name: string;
    value: string;
}

export interface IVariableDocumentation {
    name: string;
    type: string;
    comment: ICommentDocumentation;
    umacro: IMacroDocumentation;
}

export interface ICommentDocumentation {
    description: IClassDocumentation[];
    parameters: IFunctionDocumentation[];
    return_type: IVariableDocumentation[];
}

export interface IMacroDocumentation {
    name: string;
    parameters: IParameterDocumentation[];
    metadata: IParameterDocumentation[];
}
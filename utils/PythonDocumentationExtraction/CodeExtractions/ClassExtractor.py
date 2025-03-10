import re

from CodeExtractions.FunctionsExtractor import extract_function_declarations
from CodeExtractions.VariablesExtractor import extract_variable_declarations
from .CommentClass import extract_comments
from .CommentClass import Comment
from .UnrealMacroClass import extract_uMacros
from .UnrealMacroClass import UnrealMacroClass
import json

function_pattern = re.compile(r'\w+[*&]{0,}\s\S+\(.*\)(;|)')
class_pattern = re.compile(r'\s*class\s(\w+\s|)\w+\s*((:\s*public\s+\w+)|)({|)\s*$')
comment_pattern = re.compile(r'^\s*((//.*)|\*\/)')
ufunction_pattern = re.compile(r'^\s*UFUNCTION\s*\(.*\)\s*$')
variable_pattern = re.compile(r'^\s*((const\s)|)\w+(<.+>|)\s*\w+([&*]+(\s+\w+)|)((\{.*\})|);')

class Class:
    
    def __init__(self, classLine=(str), comment=(str), uclassLine=(str)):
        lineSplit = classLine.split()
        self.name = lineSplit[1] if lineSplit[1].upper()!=lineSplit[1] else lineSplit[2] 
        self.body = classLine

        self.functions = []
        self.variables = []

        self.comment = comment
        self.umacro = uclassLine

    def __str__(self):
        functions = "FUNCTIONS {\n" + '\n'.join([str(f) for f in self.functions]) + "\n}"
        variables = "VARIABLES {\n" + '\n'.join([str(v) for v in self.variables]) + "\n}"
        return f'{self.comment}\n{self.umacro}\n{functions}\n{variables}'

    def extract_class_body(self, lines, start_index):
        class_body = []
        brace_count = 0
        hasEntered = False
        for i in range(start_index, len(lines)):
            line = lines[i]
            class_body.append(line)
            brace_count += line.count('{')
            brace_count -= line.count('}')
            if brace_count == 0 and hasEntered:
                break
            else:
                hasEntered = True
        self.body = '\n'.join(class_body)
        return i
    
    def autoCompleteClass(self):
        def sortName(val):
            return val.name
        self.functions = extract_function_declarations(self.body)
        self.functions.sort(key=sortName)
        self.variables = extract_variable_declarations(self.body)
        self.variables.sort(key=sortName)
        del self.body


    def to_dict(self):
        return {
            'name': self.name,
            'body': self.body,
            'functions': self.functions,
            'variables': self.variables,
            'comment': self.comment,
            'uclass': self.umacro
        }

    def to_json(self):
        return json.dumps(self.to_dict())

def extract_class_declarations(lines):
    foundFunctions = []

    lines = lines.split('\n')
    for i, line in enumerate(lines):
        if class_pattern.search(line):
            classLine = line

            classComment = extract_comments(lines, i)

            uclassLine = extract_uMacros(lines, i)
            newClass = Class(classLine, classComment, uclassLine)
            i = newClass.extract_class_body(lines, i)
            foundFunctions.append(newClass)

    return foundFunctions

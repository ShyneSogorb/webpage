import re
from .CommentClass import extract_comments
from .CommentClass import Comment
from .UnrealMacroClass import extract_uMacros
from .UnrealMacroClass import UnrealMacroClass
from .Patterns import FUNCTION_PATTERN
import json

class Function:
    def __init__(self, functionLine, comment, ufunctionLine):
        self.name = functionLine.split('(')[0].split()[-1]
        self.return_type = ' '.join(functionLine.split('(')[0].split()[:-1])
        self.parameters = functionLine.split('(')[1].split(')')[0].split(',')

        self.parameters = list(filter(len, self.parameters))

        self.comment = comment
        self.umacro = ufunctionLine

    def __str__(self):
        return f'{self.comment}\n{self.umacro}\n{self.return_type} {self.name}({", ".join(self.parameters)})'

    def to_dict(self):
        return {
            'name': self.name,
            'return_type': self.return_type,
            'parameters': [param.strip() for param in self.parameters],
            'comment': self.comment if isinstance(self.comment, dict) else self.comment.to_dict() if self.comment else None,
            'ufunction': self.umacro if isinstance(self.umacro, dict) else self.umacro.to_dict() if self.umacro else None
        }

    def to_json(self):
        return json.dumps(self.to_dict())

def extract_function_declarations(class_body):
    foundFunctions = []

    lines = class_body.split('\n')
    for i, line in enumerate(lines):
        if FUNCTION_PATTERN.search(line):
            functionLine = line

            functionComment = extract_comments(lines, i)

            ufunctionLine = extract_uMacros(lines, i)

            foundFunctions.append(Function(functionLine, functionComment, ufunctionLine))

    return foundFunctions

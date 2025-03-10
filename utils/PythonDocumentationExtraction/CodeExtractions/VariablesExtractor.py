from .CommentClass import extract_comments
from .CommentClass import Comment
from .UnrealMacroClass import extract_uMacros
from .UnrealMacroClass import UnrealMacroClass
from .Patterns import VARIABLE_PATTERN
import json

class Variable:
    def __init__(self, variableLine, comment, upropertyLine):
        self.name = variableLine.split('=')[0].split()[-1]
        self.type = ' '.join(variableLine.split('=')[0].split()[:-1])

        self.comment = comment.strip()
        self.umacro = upropertyLine

    def __str__(self):
        return f'{self.comment}\n{self.umacro}\n{self.type} {self.name}'

    def to_dict(self):
        return {
            'name': self.name,
            'type': self.type,
            'comment': self.comment,
            'uproperty': self.umacro
        }

    def to_json(self):
        return json.dumps(self.to_dict())

def extract_variable_declarations(class_body):
    foundVariables = []

    lines = class_body.split('\n')
    for i, line in enumerate(lines):
        if VARIABLE_PATTERN.search(line):
            variableLine = line

            variableComment = extract_comments(lines, i)

            uvariableLine = extract_uMacros(lines, i)

            foundVariables.append(Variable(variableLine, variableComment, uvariableLine))

    return foundVariables

import re
import json
from .Patterns import COMMENT_PARAM_PATTERN, COMMENT_RETURN_PATTERN, PARAM_NAME_PATTERN, FUNCTION_PATTERN, VARIABLE_PATTERN, COMMENT_PATTERN


class Comment:

    class Param:
        def __init__(self, paramLine):
            self.name = PARAM_NAME_PATTERN.search(paramLine).group(0)
            self.description = paramLine.replace('@param', '').replace(self.name, '').strip()

        def __str__(self):
            return f'@param {self.name} {self.description}'

        def to_dict(self):
            return {
                'name': self.name,
                'description': self.description
            }

    def __init__(self, commentLine):
        self.description = ""
        separatedLines = commentLine.split('\n')
        for i, line in enumerate(separatedLines):
            if not COMMENT_PARAM_PATTERN.search(line) and not COMMENT_RETURN_PATTERN.search(line):
                self.description += line + '\n'
            else:
                break

        self.parameters = []
        params = COMMENT_PARAM_PATTERN.findall(commentLine)
        for param in params:
            self.parameters.append(self.Param(param))

        self.return_type = str(COMMENT_RETURN_PATTERN.findall(commentLine)[0]) if COMMENT_RETURN_PATTERN.search(commentLine) else ""

    def __str__(self):
        
        if self.description == "":
            return ""

        needToClose = False
        if '/*' in self.description and '*/' not in self.description: 
            needToClose = True

        comment = ""
        comment += self.description + '\n'

        for param in self.parameters:
            comment += str(param) + '\n'

        if not self.return_type == "": comment += self.return_type

        if needToClose:
            comment += '\n*/'

        return comment
    
    def strip(self):
        return self.__str__().strip()

    def to_dict(self):
        return {
            'description': self.description.strip(),
            'parameters': [param.to_dict() for param in self.parameters],
            'return_type': self.return_type.strip()
        }

    def to_json(self):
        return json.dumps(self.to_dict(), ensure_ascii=False)


def extract_comments(lines, i):

    functionComment = ""
    i-=1
    while i >= 0 and not (FUNCTION_PATTERN.search(lines[i])):
        if(VARIABLE_PATTERN.search(lines[i])):
            break
        if(COMMENT_PATTERN.search(lines[i])):
            if('*/' in lines[i]):
                while '/*' not in lines[i]:
                    functionComment = lines[i].strip() + "\n" + functionComment
                    i -= 1
                functionComment = lines[i].strip() + "\n" + functionComment
            else:
                functionComment = lines[i].strip() + "\n" + functionComment
        i -= 1

    return Comment(functionComment)
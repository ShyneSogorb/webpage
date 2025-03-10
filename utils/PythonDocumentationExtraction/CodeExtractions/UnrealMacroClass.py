import re
import json
from .Patterns import UMACRO_PATTERN, FUNCTION_PATTERN, COMMENT_PATTERN, VARIABLE_PATTERN

class UnrealMacroClass:
    class Param:
        def __init__(self, paramLine):
            self.name = paramLine.split("=")[0].strip()
            self.value = paramLine.split("=")[1].strip() if "=" in paramLine else ""

        def __str__(self):
            return f'{self.name} = {self.value}' if self.value != "" else f'{self.name}'

        def to_dict(self):
            return {"name": self.name, "value": self.value}

    def __init__(self, MacroLine):
        self.name = MacroLine.split('(')[0].split()[-1]
        parameters = MacroLine.split('(')[1].split(')')[0].split("meta")[0].strip().split(',')
        if "meta" in MacroLine: 
            parameters.remove(parameters[-1])

        self.parameters = []
        for param in parameters:
            self.parameters.append(self.Param(param))

        metadata = MacroLine.split("meta")[1].split('(')[1].split(')')[0].split(",") if "meta" in MacroLine else ""
        self.metadata = []
        for meta in metadata:
            self.metadata.append(self.Param(meta.strip().replace("\"", "")))

    def __str__(self):
        params = ""
        for param in self.parameters:
            params += str(param) + ', '
        if len(params) > 0:
            params = params[:-2]

        metadata = ""
        for meta in self.metadata:
            metadata += meta + ', '
        if len(metadata) > 0:
            metadata = metadata[:-2]
            metadata = f'meta=({metadata})'

        if not metadata == "" and not params == "":
            params += ', ' + metadata

        return f'{self.name}({params})'

    def to_dict(self):
        return {
            "name": self.name,
            "parameters": [param.to_dict() for param in self.parameters],
            "metadata": [meta.to_dict() for meta in self.metadata]
        }

    def to_json(self):
        return json.dumps(self.to_dict())

            


    

def extract_uMacros(lines, i):
    i-=1
    while i >= 0 and not (FUNCTION_PATTERN.search(lines[i])):
        if(VARIABLE_PATTERN.search(lines[i]) or COMMENT_PATTERN.search(lines[i]) or FUNCTION_PATTERN.search(lines[i])):
            break
        if(UMACRO_PATTERN.search(lines[i])):
            return UnrealMacroClass(lines[i].strip())
        i -= 1

    return ""


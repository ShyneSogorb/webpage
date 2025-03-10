from CodeExtractions.ClassExtractor import extract_class_declarations
from CodeExtractions.ClassExtractor import Class
from CodeExtractions.FunctionsExtractor import extract_function_declarations
from CodeExtractions.VariablesExtractor import extract_variable_declarations
from json import JSONEncoder
import pathlib

classes: list[Class] = []

#desktop = pathlib.Path(input("path to read"))
desktop = pathlib.Path("C://Users//user//Documents//UnrealProjects//MyProject//Plugins//WeaponAssetSystem")
files = list(desktop.rglob("*.h"))
files.sort()

for file in files:

    file_path = file
    content = ""
    with open(file_path, 'r') as file:
        content = file.read()

    newClasses: list[Class] = extract_class_declarations(content)
    for i in newClasses: 
        classes.append(i)

output_file_path = './/was.documentation.json'
output_file_path = "C://Users//user//Documents//WebPortfolio//webpage//public//JSON//was.documentation.json"



for c in classes:
    c.autoCompleteClass()

def sortName(val):
    return val.name
classes.sort(key=sortName)


class MyEncoder(JSONEncoder):
        def default(self, o):
            return o.__dict__    

with open(output_file_path, 'w') as output_file:
    res = MyEncoder().encode(classes)

    output_file.write(res)
    print(f'Class body extracted to {output_file_path}')
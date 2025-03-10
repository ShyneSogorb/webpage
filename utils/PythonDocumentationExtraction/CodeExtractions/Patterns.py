import re

FUNCTION_PATTERN = re.compile(r'((const)|)\w+[*&]{0,}\s\S+\([\w\s&*,]*\)((\s+const\s+)|)(;|)')
COMMENT_PATTERN = re.compile(r'^\s*((//.*)|\*\/)')
UMACRO_PATTERN = re.compile(r'^\s*U([A-Z])+\s*\(.*\)\s*$')
VARIABLE_PATTERN = re.compile(r'^\s*((const\s)|)\w+(<.+>|)\s*\w+([&*]+(\s+\w+)|)((\{.*\})|);')

COMMENT_PARAM_PATTERN = re.compile(r'@param\s*\w+.*')
COMMENT_RETURN_PATTERN = re.compile(r'@return\s*\w+.*')

PARAM_NAME_PATTERN = re.compile(r'(?<=@param)\s*\w+')


import string

print('Hello, I can only speak Python, show me your code.')
code = input('Enter your code : ')
code = ''.join([c for c in code if c in string.printable])
for keyword in ['eval', 'exec', 'import', 'open', 'system',
                'os', 'builtins', "'", '""', '+', '*' ]:
    if keyword in code:
        print('This is not allowed:', keyword)
        break
else:
    try:
        print(eval(code, {'__builtins__': {'str': str}}))
    except Exception as e:
        print(f'I\'m confused - {e}')
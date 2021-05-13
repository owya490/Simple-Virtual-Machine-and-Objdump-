function_count = 0

string = ""
get_bin = lambda x, n: format(x, 'b').zfill(n)
tmp = input()
while (tmp != "quit"):
    tmp = input()
    if tmp == "FUNC": 
        tmp = int(input())
        if tmp == 0:
            string += "000"
        elif tmp == 1:
            string += "001"
        elif tmp == 2:
            string += "010"
        elif tmp == 3:
            string += "011"
        elif tmp == 4:
            string += "100"
        elif tmp == 5:
            string += "101"
        elif tmp == 6:
            string += "110"
        elif tmp == 7:
            string += "111"
    
    elif tmp == "MOV":
        function_count += 1
        string += "000"
    elif tmp == "CAL":
        function_count += 1
        string += "001"
    elif tmp == "RET":
        function_count += 1
        string += "010"
        string += get_bin(function_count, 5)
        function_count = 0
    elif tmp == "REF":
        function_count += 1
        string += "011"
    elif tmp == "ADD":
        function_count += 1
        string += "100"
    elif tmp == "PRINT":
        function_count += 1
        string += "101"
    elif tmp == "NOT":
        function_count += 1
        string += "110"
    elif tmp == "EQU":
        function_count += 1
        string += "111"


    elif tmp == "VAL":
        tmp = int(input())
        string += get_bin(tmp, 8)

        string += "00"
        
    elif tmp == "REG":
        tmp = int(input())
        string += get_bin(tmp, 3)
        string += "01"
    elif tmp == "STK":
        tmp = int(input())
        string += get_bin(tmp, 5)
        string += "10"
    elif tmp == "PTR":
        tmp = int(input())
        string += get_bin(tmp, 5)
        string += "11"

    

#string += get_bin(function_count, 5)

padding = 8 - len(string) % 8

while (padding != 0) :
    string = "0" + string
    padding -= 1

print(string)

print(len(string))
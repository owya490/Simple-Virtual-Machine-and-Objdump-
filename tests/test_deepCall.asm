FUNC LABEL 0
    MOV STK A VAL 3
    CAL VAL 1
    PRINT STK A
    RET
FUNC LABEL 1
    MOV STK A VAL 2
    CAL VAL 2
    PRINT STK A
    RET
FUNC LABEL 2
    MOV STK A VAL 1
    CAL VAL 3
    PRINT STK A
    RET
FUNC LABEL 3
    MOV STK A VAL 0
    PRINT STK A
    RET
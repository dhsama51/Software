#방법1: 분할 정복을 활용한 거듭제곱 계산
#n이 짝수일 때: A^n = A^(2/n) * A^(2/n)
#n이 홀수일 때: A^n = A^((n-1)/2) * A^((n-1)/2) * A
#n을 반씩 나눠가면서 계산

def matrix_multiply(A,B): #AxB 계산
    n = len(A)
    result = [[0] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            for k in range(n):
                result[i][j] += A[i][k] * B[k][j]
    return result

def matrix_power(A,n): #행렬 A의 n승 계산
    size = len(A)
    result = [[1 if i == j else 0 for j in range(size)] for i in range(size)] #항등행렬

    base = A
    while n > 0:
        if n % 2 == 1:
            result = matrix_multiply(result, base)
        base = matrix_multiply(base, base)
        n //= 2
    return result

def input_matrix(): #행렬 입력 함수
    try:
        n = int(input("nxn 정방행렬의 크기 n을 입력하세요: "))
        if n <= 0:
            print("양의 정수만 입력하세요.")
            return None
        
        matrix = []
        
        print(f"각 행을 공백으로 구분하여 입력하세요(총 {n}행, 정수 또는 실수 입력):")
        
        for i in range(n):
            row_input = input(f"{i + 1}번째 행 입력: ").split()
            try:
                row = list(map(float, row_input))
            except ValueError:
                print("각 행에는 숫자만 입력할 수 있습니다.")
                return None

            #입력된 행 길이 확인
            if len(row) != n:
                print("입력된 행의 길이가 맞지 않습니다. 정방행렬이 아닙니다.")
                return None

            matrix.append(row)
                
        return matrix
    
    except ValueError:
        print("양의 정수만 입력하세요.")
        return None

matrix = input_matrix()
if matrix is not None:
    try:
        power = int(input("행렬의 멱승을 계산할 지수를 입력하세요: "))
        if power < 0:
            print("양의 정수 또는 0만 입력하세요.")
        else:
            result = matrix_power(matrix , power)

            print(f"입력된 행렬의 {power}승 결과:")
            for row in result:
                print(" ".join(f"{int(value) if value.is_integer() else f'{value:.4f}'}" for value in row))
    except ValueError:
        print("양의 정수 또는 0만 입력하세요.")
        
"""
2025 P조 프로젝트 1 선형 연립방정식 해 구하기
20212052 이동훈, 20252377 김범준, 20252397 유현민
----------------------------------------------------------------------
- 가우스 조던: 해 판별용
- LU 분해: 실제 해 계산용
"""

def fmt(val):  #출력 보정용
    if abs(val - round(val)) < 1e-12:
        return str(int(round(val)))
    else:
        return f"{val:.6f}"

def forward_elimination(matrix):
    """해 판별용 전진 소거"""
    m = len(matrix)
    n = len(matrix[0]) - 1
    row = 0
    for col in range(n):
        pivot = None
        for r in range(row, m):
            if abs(matrix[r][col]) > 1e-12:
                pivot = r
                break
        if pivot is None:
            continue
        if pivot != row:
            matrix[row], matrix[pivot] = matrix[pivot], matrix[row]
        pivot_val = matrix[row][col]
        matrix[row] = [v / pivot_val for v in matrix[row]]
        #아래 행만 제거
        for r in range(row + 1, m):
            factor = matrix[r][col]
            matrix[r] = [matrix[r][c] - factor * matrix[row][c] for c in range(n + 1)]
        row += 1
        if row == m:
            break
    return matrix

def lu_decomposition(A):
    """LU 분해 (Doolittle, Scaled Partial Pivoting) -> L, U, piv"""
    n = len(A)
    U = [row[:] for row in A]  #A 복사
    L = [[0.0] * n for _ in range(n)]
    piv = list(range(n))  #행 순열 기록

    #행 스케일(최대 절대값) 미리 계산
    scale = []
    for r in range(n):
        s = max(abs(v) for v in U[r])
        if s == 0:
            raise ValueError("LU 분해 불가능: 영행(row) 존재")
        scale.append(s)

    for i in range(n):
        #스케일드 피벗 선택: argmax |U[r][i]| / scale[r]
        pivot = max(range(i, n), key=lambda r: abs(U[r][i]) / scale[r])
        if abs(U[pivot][i]) < 1e-12:
            raise ValueError("LU 분해 불가능: 0 피벗")
        if pivot != i:
            U[i], U[pivot] = U[pivot], U[i]
            piv[i], piv[pivot] = piv[pivot], piv[i]
            scale[i], scale[pivot] = scale[pivot], scale[i]
            if i > 0:
                L[i][:i], L[pivot][:i] = L[pivot][:i], L[i][:i]

        L[i][i] = 1.0
        for r in range(i + 1, n):
            L[r][i] = U[r][i] / U[i][i]
            for c in range(i, n):
                U[r][c] -= L[r][i] * U[i][c]

    return L, U, piv

def calc_solve(L, U, b):
    """LU 분해 결과로 Ax=b 해 구하기 (L은 단위하삼각)"""
    n = len(L)
    #Ly = b (forward)
    y = [0.0] * n
    for i in range(n):
        y[i] = b[i] - sum(L[i][j] * y[j] for j in range(i))
    #Ux = y (backward)
    x = [0.0] * n
    for i in reversed(range(n)):
        if abs(U[i][i]) < 1e-12:
            print("LU 해 계산 중 0 피벗 발생")
            return
        x[i] = (y[i] - sum(U[i][j] * x[j] for j in range(i + 1, n))) / U[i][i]
    return x

def is_solvable(matrix):
    """해 판별 및 LU 분해 기반 계산"""
    reduced = forward_elimination([row[:] for row in matrix])
    m = len(reduced)
    n = len(reduced[0]) - 1

    #해 없음
    for r in range(m):
        if all(abs(reduced[r][c]) < 1e-12 for c in range(n)) and abs(reduced[r][-1]) > 1e-12:
            return "no_solution", None, None

    #랭크/피벗열
    rank = 0
    pivot_cols = []
    for r in range(m):
        for c in range(n):
            if abs(reduced[r][c]) > 1e-12:
                rank += 1
                pivot_cols.append(c)
                break

    #유일한 해 -> LU 사용(+ 보정)
    if rank == n and m == n:
        A = [row[:n] for row in matrix]
        b = [row[-1] for row in matrix]
        L, U, piv = lu_decomposition(A)
        b_perm = [b[idx] for idx in piv]
        sol = calc_solve(L, U, b_perm)

        r_vec = []
        for i in range(n):
            Ai_dot_x = sum(A[i][j] * sol[j] for j in range(n))
            r_vec.append(b[i] - Ai_dot_x)

        delta = calc_solve(L, U, r_vec)
        sol = [sol[i] + delta[i] for i in range(n)]

        return "unique_LU", sol, reduced

    #무수히 많은 해
    if rank < n:
        free_vars = [c for c in range(n) if c not in pivot_cols]
        return "infinite", free_vars, reduced

    return "infinite", None, reduced

def format_infinite_solution(matrix, free_vars):
    """무수히 많은 해를 자유변수 식 형태로 올바르게 계산하여 출력"""
    m = len(matrix)
    n = len(matrix[0]) - 1
    epsilon = 1e-12

    free_names = [chr(115 + i) for i in range(len(free_vars))]  #s, t, u ...
    free_map = {fv: free_names[i] for i, fv in enumerate(free_vars)}

    #각 변수별 표현
    sol = ['0'] * n
    for fv in free_vars:
        sol[fv] = free_map[fv]

    #후진 대입
    for i in reversed(range(m)):
        pivot_col = None
        for j in range(n):
            if abs(matrix[i][j]) > epsilon:
                pivot_col = j
                break
        if pivot_col is None or pivot_col in free_vars:
            continue

        rhs = matrix[i][-1]
        expr_parts = []
        num_val = rhs

        for j in range(pivot_col + 1, n):
            coeff = matrix[i][j]
            if abs(coeff) < epsilon:
                continue
            term = sol[j]
            if any(ch.isalpha() for ch in term):  #자유변수 포함
                sign = '-' if coeff > 0 else '+'
                val = abs(coeff)
                if abs(val - 1) < epsilon:
                    expr_parts.append(f"{sign}{term}")
                else:
                    expr_parts.append(f"{sign}{fmt(val)}*{term}")
            else:
                num_val -= coeff * float(term)

        expr = ""
        if abs(num_val) > epsilon:
            expr += fmt(num_val)
        for term in expr_parts:
            if expr == "":
                #첫 항이면 부호 그대로
                expr += term.lstrip('+')
            else:
                expr += f" {term}"
        if expr == "":
            expr = "0"  #모든 항이 0일 때

        expr = expr.replace(' +', ' +').replace(' -', ' -').strip()
        sol[pivot_col] = expr

    lines = [f"x{i+1} = {sol[i]}" for i in range(n)]
    return "\n".join(lines)

def main():
    m = int(input("방정식의 개수를 입력하세요: "))
    n = int(input("변수의 개수를 입력하세요: "))
    print(f"각 방정식의 첨가행렬을 공백으로 구분하여 입력하세요(총 {m}행 {n+1}열):")
    matrix = []
    for i in range(m):
        row = list(map(float, input(f"{i+1}번째 식 입력: ").split()))
        if len(row) != n + 1:
            print(f"오류: {n+1}개의 숫자를 입력해야 합니다.")
            return
        matrix.append(row)

    result_type, data, reduced = is_solvable(matrix)

    print("\n--- 결과 ---")
    if result_type == "no_solution":
        print("해가 없습니다.")

    elif result_type == "unique_LU":
        print("유일한 해 (LU 분해로 계산):")

        A = [row[:n] for row in matrix]
        L, U, piv = lu_decomposition(A)

        print("\n[L 행렬]")
        for row in L:
            print(" ".join(fmt(v) for v in row))

        print("\n[U 행렬]")
        for row in U:
            print(" ".join(fmt(v) for v in row))

        print()
        #해 출력
        for i, val in enumerate(data):
            print(f"x{i+1} = {fmt(val)}")

    elif result_type == "infinite":
        print("무수히 많은 해 (자유변수 존재):")
        print(format_infinite_solution(reduced, data))
    else:
        print("문제가 발생했습니다.")
        return

if __name__ == "__main__":
    main()

def operatorpos_update(updated_logic): #calculate의 보조함수
    operatorpos =[]
    for i in range(len(updated_logic)):
        if updated_logic[i] not in [True, False]: operatorpos.append(i)
    return operatorpos

def calculate(logic): #우선순위에 맞추어 합성명제를 계산하는 함수
    temp = []

    print(logic[ppos[0]], end="\t")
    if qpos != []: print(logic[qpos[0]], end="\t")
    if rpos != []: print(logic[rpos[0]], end="\t")

    for i in operatorpos_update(logic):
        if logic[i] == operator[0]:
            logic[i+1] = not logic[i+1]
            print(logic[i+1],end="\t")
            temp.append(i)
    temp.reverse()
    for i in temp: del logic[i]
    temp=[]

    for i in operatorpos_update(logic):
        if logic[i] == operator[1]:
            logic[i+1] = logic[i-1] and logic[i+1]
            print(logic[i+1], end="\t")
            temp.append(i)
    temp.reverse()
    for i in temp: del logic[i-1:i+1]
    temp=[]

    for i in operatorpos_update(logic):
        if logic[i] == operator[2]:
            logic[i+1] = logic[i-1] or logic[i+1]
            print(logic[i+1], end="\t")
            temp.append(i)
    temp.reverse()
    for i in temp: del logic[i-1:i+1]
    temp=[]

    for i in operatorpos_update(logic):
        if logic[i] == operator[3]:
            logic[i+1] = logic[i-1] ^ logic[i+1]
            print(logic[i+1], end="\t")
            temp.append(i)
    temp.reverse()
    for i in temp: del logic[i-1:i+1]
    temp=[]

    for i in operatorpos_update(logic):
        if logic[i] == operator[4]:
            logic[i+1] = not logic[i-1] or logic[i+1]
            print(logic[i+1], end="\t")
            temp.append(i)
    temp.reverse()
    for i in temp: del logic[i-1:i+1]
    temp=[]

    for i in operatorpos_update(logic):
        if logic[i] == operator[5]:
            logic[i+1] = logic[i-1] or not logic[i+1]
            print(logic[i+1], end="\t")
            temp.append(i)
    temp.reverse()
    for i in temp: del logic[i-1:i+1]
    temp=[]

    for i in operatorpos_update(logic):
        if logic[i] == operator[6]:
            logic[i+1] = not (logic[i-1] ^ logic[i+1])
            print(logic[i+1], end="\t")
    temp.reverse()
    for i in temp: del logic[i-1:i+1]
    temp=[]
    print("")

def cal_left(i, left, logic): #print_col의 보조함수
    pos = 1
    while i-pos >= 0:
        if logic[i-pos] not in operator:
            left.append(logic[i-pos])
        elif operator.index(logic[i-pos]) <= operator.index(logic[i]):
            left.append(logic[i-pos])
        else: break
        pos += 1

    left.reverse()
    if len(left) > 2:
        left.insert(0, "(")
        left.append(")")

def cal_right(i, right, logic): #print_col의 보조함수
    pos = 1
    while i+pos < len(logic):
        if logic[i+pos] not in operator:
            right.append(logic[i+pos])
        elif operator.index(logic[i+pos]) < operator.index(logic[i]):
            right.append(logic[i+pos])
        else: break
        pos += 1

    if len(right) > 2:
        right.insert(0, "(")
        right.append(")")
    
def print_col(num, logic, operatorpos):
    print("P", end="\t")
    if qpos != []:
        print("Q", end="\t")
    if rpos != []:
        print("R", end="\t")

    for i in operatorpos:
        if logic[i] in operator[0]:
            print(logic[i]+logic[i+1], end=" "*(8-2))
    for j in range(1,7):
        for i in operatorpos:
            left=[]
            right=[]
            if logic[i] == operator[j]:
                cal_left(i, left, logic)
                cal_right(i, right, logic)
                
                print("".join(map(str, left))+logic[i]+"".join(map(str, right)), end=" "*(8-len(left)-1-len(right)))
    print("")
   
def revise():
    for i in range(len(given)):
        if given[i] in {"𝑃", "p"}: given[i] = "P"
        elif given[i] in {"𝑄", "q"}: given[i] = "Q"
        elif given[i] in {"𝑅", "r"}: given[i] = "R"
        elif given[i] == "∼": given[i] = "~"

#프로그램 시작
given = []
operator = ["~", "∧", "∨", "⊕", "→", "←", "↔"]
ppos = []
qpos = []
rpos = []
operatorpos_init = []

#합성명제 입력
switch = int(input("(1)파일 입출력 이용해 1번째 조건 입력\n(2)파일 입출력 이용해 2번째 조건 입력\n(3)직접 입력\n"))
if switch == 1:
    with open("condition1.txt", "r", encoding="utf8") as f:
        given = list(f.readline().replace("\n","").replace(" ",""))
elif switch == 2:
    with open("condition2.txt", "r", encoding="utf8") as f:
        given = list(f.readline().replace("\n","").replace(" ",""))
else:
    given = list(input("합성명제 전체를 입력하세요.\n").replace(" ",""))
    
#합성명제 출력
revise()
print("".join(map(str, given)) + " 에 대한 진리표를 출력합니다...")

for i in range(len(given)):
    if given[i] in {"P", "p"}: ppos.append(i)
    elif given[i] in {"Q", "q"}: qpos.append(i)
    elif given[i] in {"R", "r"}: rpos.append(i)
    else: operatorpos_init.append(i)

#진리표 열 출력
print_col(len(given), given, operatorpos_init)

#진리표 각 행 출력
num = 2
if qpos != []: num *= 2
if rpos != []: num *= 2

for i in range(num-1,-1,-1):
    backup=[]
    for j in range(len(given)):
        backup.append(given[j])

    for p in ppos:
        backup[p] = bool(i&(num//2))
    for q in qpos:
        backup[q] = bool(i&(num//4))
    for r in rpos:
        backup[r] = bool(i&(num//8))
    calculate(backup)

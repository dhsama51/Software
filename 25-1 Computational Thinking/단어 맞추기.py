import random

#1) words.txt 파일을 만들고, 영어 단어를 20개 저장한다.
#파일 입출력이 아니라 수기로, 따로 작성하여 파일로 첨부해둠
#같은 디렉터리에 두면 됨

#2) 단어 리스트에서 퀴즈에 사용할 단어는 랜덤으로 결정한다.
rand_int = random.randint(0, 19) + 1
f = open("./words.txt", "r")
#랜덤 횟수만큼 읽어오면 값이 덮어씌워지면서 원하는 번쨰의 값을 가져올 수 있게 됨
for i in range(rand_int):
    word = f.readline().strip()
#print(word, len(word))
opportunity = 10
unknown_pos = len(word)
answer_sheet = ["_" for i in range(len(word))]

#3) 단어의 길이는 _(언더바)로 출력한다.
while opportunity != 0 and unknown_pos != 0:
    ###한 턴을 반복하는 부분
    for i in range(len(word)):
        print(answer_sheet[i], end='')
    expect = input("\n단어를 추측하시오: ")

    right = False
    for i in range(len(word)):
        if expect == word[i]:
            right = True
            if answer_sheet[i] == '_':
                answer_sheet[i] = expect
                unknown_pos -= 1
    if right == False:
        opportunity -= 1
        print(f'=> 틀림 : 남은 기회 {opportunity}')
    print(unknown_pos)
    
if opportunity == 0:
    print(f"사용자 패배 정답은 {word}")

if unknown_pos == 0:
    print(f"{word}\n사용자 승리")
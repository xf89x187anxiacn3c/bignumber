#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//#define a b --> a를 b로 취급
//카드 종류
#define NUMBER 1
#define OPERATOR 2
#define USED 3

//기호들
#define PLUS -1
#define MINUS -2
#define MULT -3
#define DIV -4

typedef struct _card {
  int prop;
  int value;
} Card;

//@정렬함수: 숫자 정렬 후 기호가 +,-,*,/ 순으로 숫자 뒤에 오게 정렬
void sort(Card *deck) {
  Card temp;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 6 - i; j++) {
      if (deck[j].value > deck[j + 1].value) {
        temp = deck[j];
        deck[j] = deck[j + 1];
        deck[j + 1] = temp;
      }
    }
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 7; j < 9 - i; j++) {
      if (deck[j].value < deck[j + 1].value) {
        temp = deck[j];
        deck[j] = deck[j + 1];
        deck[j + 1] = temp;
      }
    }
  }
}

//덱 생성 함수
void seed(Card *deck) {
  for (int i = 0; i < 10; i++) {
    //카드 10개 무작위 생성, 7개는 숫자 (개수는 바뀔 수 있음)
    if (i < 7) {
      deck[i].prop = NUMBER;
      deck[i].value = (rand() % (13+i/4) + 1); //(1 ~ 13,13,13,13,14,14,14,14,15,15) -> 높은 수가 더 희귀해짐
      if ((i > 0)&&(deck[i-1].value == deck[i].value)){
        if(rand()%2){
          deck[i].value += 1;
          if( deck[i].value > 15){
            deck[i].value = (rand() % 15 + 1);
          }
        }else{
          deck[i].value /= 2;
          if( deck[i].value < 1){
            deck[i].value = (rand() % 15 + 1);
          }
        }
      }
    } else {
      //값을 키우는 기호랑 줄이는 기호 하나씩은 나오게 함
      deck[i].prop = OPERATOR;
    }
  }
  deck[7].value = ((rand() % 2 - 2) * 2) + 1; //(-1 or -3)
  deck[8].value = (rand() % 2 - 2) * 2;       //(-2 or -4)
  deck[9].value = rand() % 4 - 4;             //(-1 ~ -4)
  sort(deck);
}

//사용한 카드 재생성
void reseed(Card *deck){
  for (int i = 0;i < 10;i++){
    if(deck[i].prop == USED){
      if(deck[i].value > 0){
        deck[i].prop = NUMBER;
        deck[i].value = rand() % 15 + 1;
      }else{
        deck[i].prop = OPERATOR;
        deck[i].value = rand() % 4 - 4;
      }
    }
  }
}

//저장
void save(int myNumber) {
  FILE *file = fopen("save.txt", "wt");
  if (file != NULL) {
      fprintf(file, "%d", myNumber);
      fclose(file);
  }
}

//불러오기
int load() {
  int loadNumber = 0;
  FILE *file = fopen("save.txt", "rt");
  if (file != NULL) {
      fscanf(file, "%d", &loadNumber);
      fclose(file);
  }
  return loadNumber;
}

//덱 출력
void printDeck(Card *deck) {
  for (int i = 0; i < 10; i++) {
    if (deck[i].value >= 0) {
      printf("[%d] ", deck[i].value);
    } else {
      switch (deck[i].value) {
      case PLUS:
        printf("[+] ");
        break;
      case MINUS:
        printf("[-] ");
        break;
      case MULT:
        printf("[*] ");
        break;
      case DIV:
        printf("[/] ");
        break;
      }
    }
  }
  printf("\n");
  for (int i = 0; i < 10; i++) {
    if(deck[i].value > 9){
      printf("% 2d   ", i + 1);
    }else{
      printf("%2d  ", i + 1);
    }
  }
  printf("\n");
}

//사용자가 입력한 숫자들을 받아 배열로 반환
int getNumberInputs(int* numberInput){
  char userInput[128];
  int i;
  fgets(userInput, 128, stdin);
  if (strcmp(userInput, "\n") == 0){
    fgets(userInput, 128, stdin);
  }
  //입력이 숫자랑 띄어쓰기로만 이루어졌나 검사
  i = 0;
  while(userInput[i]){
    if( !((userInput[i] == ' ') || (userInput[i] == '\n') || ((userInput[i] >= '0') && (userInput[i] <= '9'))) ){
      printf("인식할 수 없는 문자가 포함되었습니다.\n");
      return 0;
    }
    i++;
  }

  //입력 내용에서 정수 개수 세기
  i = 0;
  int numberCount = 0, isNumber = 0, temp = 0;
  while(userInput[i]){
      if(userInput[i] != '\n'){
        isNumber = (userInput[i] == ' ') ? 0 : 1;
      }
    if ((isNumber != temp) && (isNumber)){
      numberCount++;
    }
    temp = isNumber;
    i++;
  }

  //입력에서 정수 가져오기
  int offset = 0;
  for(int j = 0 ; j < numberCount ; j++){
    sscanf((userInput+offset),"%d",&numberInput[j]);
    if ((numberInput[j] > 10) || (numberInput[j] < 1)) {
      printf( "%d번째 카드를 찾을 수 없습니다.\n", numberInput[j] );
      return 0;
    }else if (numberInput[j] == 10) {
      offset += 3;
    }else{
      offset += 2;
    }
  }
  return numberCount;
}


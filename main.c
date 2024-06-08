#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "card.h"

//메인메뉴 -> 시작
int gameMode1(){
  srand(time(NULL));
  Card deck[10] = {0,0,0}; //주어진 카드 더미
  seed(deck);
  int errorCheck = 0;
  
  int userInput[10] = {0}; //유저가 입력한 번호들
  int userInputN; //유저가 입력한 번호들의 개수
  Card userDeck[10] = {0,0,0}; //유저가 입력한 번호의 카드들

  int scoreDeck[10] = {0}; //계산이 진행될 공간
  int score = 0;
  int calculated = 0;
  
  for(int i=0;i<5;i++){
    printf("\033[2J\033[1;1f라운드 [%d/5], 점수: %d\n\n",i+1,score);
    printDeck(deck);
    printf("사용할 카드 번호를 순서대로 입력하세요\n");

    //입력받기
    while(1){
      for(int i=0;i<10;i++){
        userInput[i] = 0;
      }
      userInputN = 0;
      errorCheck = 0;
      calculated = 0;
      printf(">>> ");
      userInputN = getNumberInputs(userInput);
      if(userInputN == 0){
        continue;
      }
      for(int i = 0 ; i < userInputN ; i++){
        userDeck[i] = deck[userInput[i]-1];
        
      }
      
      //오류처리
      if (userDeck[0].prop == OPERATOR){
        printf("첫 카드는 숫자여야 합니다.\n");
        continue;
      }

      if (userDeck[userInputN-1].prop == OPERATOR){
        printf("마지막 카드는 숫자여야 합니다.\n");
        continue;
      }

      for(int i = 0 ; i < userInputN ; i++){
        //printf("%d",userInput[i]);
        for(int j = 0 ; j < userInputN ; j++){
          if ( (userInput[i] == userInput[j]) && (i != j) ){
            errorCheck = 1;
          }
        }
      }
      if (errorCheck){
        printf("같은 카드를 여러번 사용할 수 없습니다.\n");
        continue;
      }
      
      for(int i = 0 ; i < userInputN-1 ; i++){
        if (userDeck[i].prop == userDeck[i+1].prop){
          errorCheck = 1;
        }
      }
      if (errorCheck){
        printf("기호나 숫자를 연달아 사용할 수 없습니다.\n");
        continue;
      }
      
      break;
    }
    //입력한 카드 "사용됨" 처리
    for(int i=0;i<userInputN;i++){
      deck[userInput[i]-1].prop = USED;
    }

    //계산준비
    for(int i=0;i<userInputN;i++){
      scoreDeck[i] = userDeck[i].value;
    }
    //계산
    if(userInputN == 1){
      //연산자가 없는 경우
      score += scoreDeck[0];
    }else{
      //연산자가 있는 경우
      for( int N = userInputN ; N > 3 ; N -=2 ){
        calculated = 0;
        for(int i=0;i<N;i++){
          if(scoreDeck[i] == MULT){
            scoreDeck[i-1] = scoreDeck[i-1] * scoreDeck[i+1];
            calculated = 1;
          }else if(scoreDeck[i] == DIV){
            scoreDeck[i-1] = scoreDeck[i-1] / scoreDeck[i+1];
            calculated = 1;
          }
          if (calculated){
            for(int j=i;j<N-2;j++){
              scoreDeck[j] = scoreDeck[j+2];
            }
            break;
          }
        }
        if (calculated == 0){
          for(int i=0;i<N;i++){
            if(scoreDeck[i] == PLUS){
              scoreDeck[i-1] = scoreDeck[i-1] + scoreDeck[i+1];
              calculated = 1;
            }else if(scoreDeck[i] == MINUS){
              scoreDeck[i-1] = scoreDeck[i-1] - scoreDeck[i+1];
              calculated = 1;
            }
            if (calculated){
              for(int j=i;j<N-2;j++){
                scoreDeck[j] = scoreDeck[j+2];
              }
              break;
            }
          }
        }
      }
      //마지막 연산, 점수 증가
      switch(scoreDeck[1]){
        case PLUS: score += ( scoreDeck[0] + scoreDeck[2] ); break;
        case MINUS: score += ( scoreDeck[0] - scoreDeck[2] ); break;
        case MULT: score += ( scoreDeck[0] * scoreDeck[2] ); break;
        case DIV: score += ( scoreDeck[0] / scoreDeck[2] ); break;
      }
    }
    for(int i=0;i<10;i++){
      deck[i].isNew = 0;
    }
    reseed(deck);
    sort(deck);
  }
  printf("\033[2J\033[1;1f라운드 [~/5], 점수: %d\n\n",score);
  printDeck(deck);
  printf("나의 수가 %d만큼 증가했습니다!\n", score);
  printf("ENTER를 입력해 메인으로 넘어가기");
  getchar();
  return score;
}

//메인메뉴 -> 하는 법
int gameMode2(){
  printf("\033[2J\033[1;1f- 소개1. 제시된 카드를 이용하여 5라운드 동안 최대한 큰 수를 만드세요!\n2. 카드 번호를 입력할 때에는 \"1 9 2\"과 같이 띄어쓰기로 구별합니다.\n3. 만든 수는 점수에, 그리고 점수는 \"나의 수\"에 더해집니다.\n\n- 카드\n1. 카드는 숫자 카드 1~15, 기호 카드 4종이 있습니다.\n2. 숫자 카드는 언제나 7개, 기호 카드는 언제나 3개가 주어집니다.\n3. 사용하지 않은 카드는 다음 라운드에도 유지됩니다. 사용한 카드들은 무작위로 다시 뽑히며, 이렇게 새로 뽑힌 카드들은 빨간색으로 표시됩니다.\n\n");
  char next='\0';
  printf("ENTER를 입력해 메인으로 넘어가기");
  next = getchar();next = getchar();
  return 0;
}

int main(void) {
  int myNumber = load();
  int choice = 0;
  int welcome = 1;
  char message[1024] = "";
  //메인메뉴 코드
  while (1) {
    printf("\033[2J\033[0m");
    printf("\033[1;1f나의 수: %d\n", myNumber);
    if(welcome){
      if(myNumber == 0){
        strcpy(message, "처음 오신 것을 환영합니다. 번호를 입력하여 원하는 페이지로 이동하세요.");
      }else{
        strcpy(message, "돌아오신 것을 환영합니다.");
      }
      welcome = 0;
    }
    printf("- %s\n",message);
    printf("================================================\n");
    printf("1: 시작 2: 하는 법 3: 초기화 4: 종료\n");
    printf(">>> ");
    scanf(" %d", &choice);
    getchar();
      switch (choice) {
        case 1:
          printf("\033[2J");
          myNumber += gameMode1();
          save(myNumber);
          strcpy(message, "진행상황이 저장되었습니다.");
          break;
        case 2:
          printf("\033[2J");
          gameMode2();
          break;
        case 3:
          printf("\033[2J");
          printf("\033[1;1f초기화하려면 Y를, 아니라면 ENTER를 입력하세요.\n>>> ");
          switch(getchar()){
            case 'Y':
              save(0);
              myNumber = 0;
              break;
            case '\n':
              break;
            default:
              getchar();
          }
          break;
        case 4:
          return 0;
        default:
          strcpy(message,"번호를 다시 입력해주세요.");
          printf("\033[5;11f   ");
      }
    }
  return 0;
}

//*/
#include<stdio.h>
int main(void){
int i=0;
int test[5];/*=
                test[0]
                test[1]
                test[2]
                test[3]
                test[4]
*/
test[0]=10;
test[1]=20;
test[2]=30;
test[3]=40;
test[4]=50;
printf("表示したい配列要素の番号を入力してください");
scanf("%d",&i);
printf("その要素の数字は%d",test[i]);

    return 0;
}
//配列基礎

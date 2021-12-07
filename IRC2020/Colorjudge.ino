/*
void color_judgement(void){
  int judge_color =0;//認識した色を格納する　red 1 , blue 2 , yellow 3
  R_to_getColor=sqrt((R_vec[0]-get_vec[0])*(R_vec[0]-get_vec[0])+
                     (R_vec[1]-get_vec[1])*(R_vec[1]-get_vec[1])+
                     (R_vec[2]-get_vec[2])*(R_vec[2]-get_vec[2]));
  
  B_to_getColor=sqrt((B_vec[0]-get_vec[0])*(B_vec[0]-get_vec[0])+
                     (B_vec[1]-get_vec[1])*(B_vec[1]-get_vec[1])+
                     (B_vec[2]-get_vec[2])*(B_vec[2]-get_vec[2]));
 
  Y_to_getColor=sqrt((Y_vec[0]-get_vec[0])*(Y_vec[0]-get_vec[0])+
                     (Y_vec[1]-get_vec[1])*(Y_vec[1]-get_vec[1])+
                     (Y_vec[2]-get_vec[2])*(Y_vec[2]-get_vec[2]))+1000;//調整用の数値しばらく様子見る
                     
  if(R_to_getColor>B_to_getColor&&Y_to_getColor>B_to_getColor){
    judge_color=2;
    }else if(Y_to_getColor>R_to_getColor){
      judge_color=1;
      }else{
        judge_color=3;
            }
    switch(judge_color){
        case 1:
          Serial.print("recognizedColor    "); Serial.println("RED");
            break;
        case 2:
            Serial.print("recognizedColor    "); Serial.println("BLUE");
            break;
        case 3:   
            Serial.print("recognizedColor    "); Serial.println("YELLOW");
            break; 
      }        
     return 0;                                                                
  } */

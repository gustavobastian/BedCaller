#ifndef KEYBOARD_H_  
#define KEYBOARD_H_

/*Keyboard definitions*/
#define PRESSED 1
#define RELEASED 0

#define GPIO_COLUMN_1     22//21
#define GPIO_COLUMN_2     21//19
#define GPIO_COLUMN_3     19//18
#define GPIO_COLUMN_4     18//5


#define GPIO_ROW_1     5//5//4
#define GPIO_ROW_2     4//2
#define GPIO_ROW_3     2//15
#define GPIO_ROW_4     15//15//0

char topicCallerEvent[100];

extern int keyboard[];
extern int keyValue;

int keyboardInit();
int keyboardRefresh();
void initCaller();
void generateJson(char *buffer,int dispositivoId, char *nombre,char *ubicacion,int luz1, int luz2 ,float temp, float humedad);
void generateJsonKey(char *buffer,int key);



#endif
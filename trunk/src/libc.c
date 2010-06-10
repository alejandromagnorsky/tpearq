#include "../include/kernel.h"
#include "../include/kc.h"

int getchar(){
        return getc(stdin);
}

int getc(int fd){
        int character = -1;
        while (character == -1)
                read(fd, &character, 1);
        return character;
       
}


int atoi(const char *str){
        int i, acum, flagNegative = 0;
        for (i = 0, acum = 0; (!i && str[i]=='-') || isDigit(str[i]) && str[i] != '\0'; i++){
                if (str[i]=='-')
                        flagNegative = 1;
                else
                        acum = acum * 10 + str[i] - '0';
        }
        if (i==strlen(str))
                return flagNegative * -1 * acum + !flagNegative * acum;
        return 0;
}


/* Lee string hasta que se presione enter o se escriban más de MAX_STRLEN caracteres,
** en cuyo caso deja de cargarlo en la string
** Limitación por no tener malloc.
*/
void getString(char * ans){
        int i, j;
        char c;
        for (i=0, j=0; (c = getchar()) != '\n' ; j++){
                printf("%c", c);
                if(c == '\b'){
                        if (i)          
                                ans[--i] = '\0';
                } else if (i<MAX_STRLEN){
                        ans[i] = c;
                        i++;
                }
        }
        ans[i] = '\0';
}


/*
** COMENTAR BIEN CADA FUNCIÓN QUE HICE, EXPLICAR TOdO MEJOR, FIJAR SI SE PUEDE ACHICAR Y OPTIMIZAR EL CODIGO
**
** CONVERSAR SOBRE EL TEMA DE SIMILARIDAD CON K&R URGENTE!!!!!!!!!!!!!!!!!!!!!!
*/

int scanf(const char * str, ...){
        int i, j, k, acum, strLen, strTrueLen, strInLen, typeONegative;  // TYPE O NEGATVE CAMBIAR PARA LA ENTREGA POSTA
        /*      strLen:         longitud de la string str.
        **      strTrueLen:     longitud de la string str una vez reemplazados los % por los valores ingresados por el usuario.
        **      strInLen:       longitud de la string strIn, string que es ingresada por el usuario
        */
        char strIn[MAX_STRLEN+1]; //Le sumo uno al máximo de caracteres porque asi el último lo dejo para '\0'
        void ** argv = (void **)(&str);
       
        getString(strIn); /* El usuario ingresa una string */
        strLen = strlen(str);
        strTrueLen = strLen;    /* Inicialmente la string real tiene la misma longitud que la string del scanf */
        strInLen = strlen(strIn);

        for (i=0, j=0 ; i<strLen && j<strInLen; i++, j++){
                if ( str[i] == '%' )
                        switch(str[++i]){
                                case 'd':
                                        for(k=0, acum = 0, typeONegative = 0; ( strIn[j] == '-' && !k ) || isDigit(strIn[j])  ; k++, j++){
                                                if (strIn[j] != '-'){
                                                        acum = acum * 10 + strIn[j] -'0';
                                                } else
                                                        typeONegative = 1;
                                        }
                                        j--;
                                        if (!k){
                                                return -1;
                                        } else if (!isDigit(strIn[j]) && typeONegative){
                                                return -1;
                                        }
                                        *(*((int **)++argv)) = typeONegative * -1 * acum + !typeONegative * acum;
                                        strTrueLen = strTrueLen - 2 + k;  /* Resto 2 a strTrueLen (% y d) y sumo k (cant. dígitos leidos-1)*/
                                        break;
                                case 'c':
                                        *(*((char **)++argv)) = strIn[j];
                                        strTrueLen--;
                                        break;
                                case 's':                       /* FIJARME BIEN COMO REACCIONAR CON STRINGS */
                                        argv++;
                                        for (k=0; strIn[j] != str[i+1]; k++, j++){
                                                ((char *)(*((char **) argv)))[k] = strIn[j];
                                        }
                                        ((char *)(*((char **) argv)))[k] = '\0';
                                        j--;
                                        strTrueLen = strTrueLen -2 + k;
                                        break;
                                /* Hice el default así para que sea análogo del printf de mariano. Si recibe % y una letra que no es ni d ni c ni s,
                                ** entonces se saltea el porcentaje y dicha letra.
                                */
                                default:
                                        j--;
                                        strTrueLen-=2;
                                        break;
                        }
                else
                        if (str[i] != strIn[j]){
                             //   printf("**%c != %c**-1", str[i], strIn[j]);       /* PRINTF  DE DEBUGGEO */
                                return -1;
                        }
        }
        if (strInLen != strTrueLen){
        //      printf("%d", -1);       /* PRINTF  DE DEBUGGEO */
                return -1;
        }
        printf("\n");   // The user entered \n, so it must be used
        return 0;
}


int read(int fd, int * c, size_t count){
        return _int_80_hand(0, fd, c, count);
}


int strlen(const char * str){
        int i;
        for (i = 0; str[i] != '\0'; i++);
                return i;
}

int isDigit(int a){
        if( (a >= '0') && (a <= '9') )
                return 1;
        return 0;
}

int putchar(int c){
        return putc(stdout, c);
}

int putc(int fd, int c){
        return write(fd, &c, 1);
}

int printf(const char * str, ...){
        int c;
        int wait = 0;
        void ** argv = (void **)(&str);

        for(c=0;str[c] != NULL;c++){
                if(wait){
                        switch(str[c]){
                        case 'd':
                                putInt( *( (int*) argv));
                                break;
                        case 'c':
                                putchar(*((char*)argv));
                                break;
                        case 's':
                                printf(*((char**)argv));
                                break;
                        default:
                                break;
                        }
                        wait = 0;
                } else {
                        if(str[c] == '%'){
                                wait = 1;      
                                argv++;
                        }
                        else
                                putchar(str[c]);
                }
        }
}

int write(int fd, int * c, size_t count){
        return _int_80_hand(1, fd, c, count);
}

int pow(int n, int exp){
        int i;
        int out = 1;
        for(i=0;i<exp;i++)
                out*=n;
        return out;
}

void putInt(int n){

        int i,j;
       
        if(n<0){
                putchar('-');
                n = -n;
        }


        for(i=0;n/pow(10,i) > 0 ;i++); //cantidad de caracteres
        for(j=i-1;j>0;j--){
                putchar( (n/pow(10,j))+'0');
                n = n%pow(10,j);
        }
        putchar((n%10)+'0');
}

int strcmp(const char * str1, const char * str2){

        int i = 0;
        int out = 0;
        while(str1[i] != NULL || str2[i] != NULL){

                if( str1[i] != str2[i])
                        return str1[i] - str2[i];

                i++;
        }

        if(str1[i] == NULL && str2[i] != NULL)
                return -str2[i];
        else if( str1[i] != NULL && str2[i] == NULL)
                return str1[i];

        return out;
}

int rand(){
        _outport(0x70, 0);
        int seconds = _inport(0x71);
        _outport(0x70, 2);
        int minutes = _inport(0x71);
        _outport(0x70, 4);
        int hours = _inport(0x71);

        long num1;
        num1 = pow(2, hours) * pow(3, minutes) * pow(5, seconds);
        return num1;
}


/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*        Selector a cargar en el descriptor de interrupcion
*        Puntero a rutina de atencion de interrupcion  
*        Derechos de acceso del segmento
*        Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
                         byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}


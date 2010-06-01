#ifndef _video_
#define _video_

#define WHITE_FG 0x07 // Atributo de video. Letras blancas, fondo negro
#define ORANGE_FG 0x06
#define VIOLET_FG 0x05
#define RED_FG 0x04
#define SKY_FG 0x03
#define GREEN_FG 0x02
#define BLUE_FG 0x01
#define BLACK_FG 0x00

#define WHITE_BG 0x70 // Atributo de video. Letras blancas, fondo negro
#define ORANGE_BG 0x60
#define VIOLET_BG 0x50
#define RED_BG 0x40
#define SKY_BG 0x30
#define GREEN_BG 0x20
#define BLUE_BG 0x10
#define BLACK_BG 0x00

#define __MAX_CONSOLES 10
#define __TAB_LENGTH 4


int __write_test(char c, int pos, int attr);

#endif

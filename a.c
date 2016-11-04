#pragma sfr
#pragma DI
#pragma EI

#pragma interrupt INTTM000 timer_INTTM000
#pragma interrupt INTP5 inter_key RB1

int light=0;    //灯光类型
int mod=0;      //下一次闪光的类型
int alarm=1;    //蜂鸣器是否响
void initTimer();
void stopTimer();
void initLight();

//按键中断
void inter_key(){
    DI();
    initLight();
    mod=0;
    switch (light) {    //每次按键就修改灯光类型
        case 0:
            light=1;
            break;
        case 1:
            light=2;
            break;
        case 2:
            light=0;
            break;
    }
    EI();
}
//第一种灯光
void nextLight0(){
    switch (mod) {          //每次闪光就修改下次闪光类型
        case 0:
        case 1:
        case 2:
            P13.1=!P13.1;
            P13.0=!P13.0;
            P13.2=!P13.2;
            mod++;
            break;
        case 3:
        case 4:
        case 5:
	        P14.0=!P14.0;
            P13.3=!P13.3;
            P14.1=!P14.1;
            mod++;
            break;
        case 6:
        case 7:
        case 8:
            P14.3=!P14.3;
            P14.2=!P14.2;
            P15.0=!P15.0;
            mod++;
            break;
        case 9:
        case 10:
            P15.1=!P15.1;
	        P15.2=!P15.2;
            P15.3=!P15.3;
            mod++;
	    break;
        case 11:
            P15.1=!P15.1;
	        P15.2=!P15.2;
            P15.3=!P15.3;
            mod=0;
            break;
    }
}
//第二种灯光
void nextLight1(){
    switch (mod) {
        case 11:
            P13.0=!P13.0;
	        P13.1=!P13.1;
            mod=0;
            break;
        case 10:
            P13.1=!P13.1;
	        P13.0=!P13.0;
            mod++;
            break;
        case 9:
        case 8:
	        P13.2=!P13.2;
            P13.3=!P13.3;
            mod++;
            break;
        case 7:
        case 6:
	        P14.0=!P14.0;
            P14.1=!P14.1;
            mod++;
            break;
        case 5:
        case 4:
	        P14.2=!P14.2;
            P14.3=!P14.3;
            mod++;
            break;
        case 3:
        case 2:
	        P15.0=!P15.0;
            P15.1=!P15.1;
            mod++;
            break;
        case 1:
        case 0:
	        P15.2=!P15.2;
            P15.3=!15.3;
            mod++;
            break;
    }
}
//第三种灯光
void nextLight2(){
    switch (mod) {
        case 0:
        case 1:
            P13.1=!P13.1;
            mod++;
            break;
        case 2:
        case 3:
            P13.2=!P13.2;
            P13.0=!P13.0;
            mod++;
            break;
        case 4:
        case 5:
            P13.3=!P13.3;
            P15.3=!P15.3;
            mod++;
            break;
        case 6:
        case 7:
            P14.0=!P14.0;
            P15.2=!P15.2;
            mod++;
            break;
        case 8:
        case 9:
            P14.1=!P14.1;
            P15.1=!P15.1;
            mod++;
            break;
        case 10:
            P14.2=!P14.2;
            P15.0=!P15.0;
	        P14.3=!P14.3;
            mod++;
            break;
        case 11:
	        P14.2=!P14.2;
            P15.0=!P15.0;
            P14.3=!P14.3;
            mod=0;
            break;
    }
}
//16位计时器的中断
__interrupt void timer_INTTM000(){
    alarm=!alarm;           //没两次中断，蜂鸣器响一次
    switch (light) {
        case 0:
            CKS=0x80;       //打开蜂鸣器输出，不同灯光类型，响声不同
	        BZOE=alarm;
            nextLight0();
            break;
        case 1:
            CKS=0xA0;
	        BZOE=alarm;
            nextLight1();
            break;
        case 2:
            CKS=0xE0;
	        BZOE=alarm;
            nextLight2();
            break;
    }
}
//初始化计时器
void initTimer(){
    DI();
    CRC00.0=0;
    PRM00=0x04;
    CR000=0x7fff/4;     //间隔约为1/4秒
    TMMK010=1;
    TMMK000=0;
    TMC00=0x0c;
    EI();
}
//初始化按键
void initButton(){
    DI();
    PM3.0=1;
    PU3.0=1;
    EGP.5=1;
    EGN.5=0;
    PMK5=0;
    P3.4=0;
    PM3.4=0;
    EI();
}
//初始化LED
void initLight(){
    P13=0;
    PM13=0x00;
    P14=0;
    PM14=0x00;
    P15=0;
    PM15=0x00;
}
//初始化蜂鸣器
void initAlarm(){
    P3.3=0;
    PM3.3=0;
    BZOE=0;
}
void main(){
    initAlarm();
    initTimer();
    initButton();
    initLight();
    while(1){

    }
}

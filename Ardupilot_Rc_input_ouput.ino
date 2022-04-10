#include <AP_Common.h>
#include <AP_Math.h>
#include <AP_Param.h>
#include <AP_Progmem.h>
#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
const AP_HAL::HAL& hal=AP_HAL_AVR_APM2;
uint16_t channels[8]; /*배열 한칸당 16비트(2바이트) 로 이루어진 크기 8자리 채널 배열 생성*/
#define RC_MIN 1000 /*PWM 신호의 최솟값*/
#define RC_MAX 2000 /*PWM 신호의 최댓값*/
#define MOTOR 0 /*PWM 의 출력포트 번호*/

float map(float x, float in_min , float in_max , float out_min , float out_max)
{
  return (x-in_min)*(out_max - out_min)/(in_max-in_min) + out_min ;
}
void setup()
{
 hal.rcout -> set_freq(0xF,490) ; /*8개의 출력포트의 PWM 주파수를 490 HZ 로 설정하겠다*/
 hal.rcout -> enable_mask(0xFF) ;  /*APM 의 출력포트를 사용한다*/
}

void loop()
{
  float Throttle , Roll , Pitch , Yaw ;
  Throttle = map(channels[2],RC_MIN,RC_MAX,0,100) ; /* 채널로 받은 정보들을 물리적 의미를 가지도록 변환*/
  Roll = map(channels[0],RC_MIN,RC_MAX,-45,45) ; /*degree*/
  Pitch = map(channels[1],RC_MIN,RC_MAX,-45,45) ; /*degree*/
  Yaw = map(channels[3],RC_MIN,RC_MAX,-150,150) ; /*degree*/
  hal.rcin -> read(channels, 8 );   /*APM 에 전달된 채널값을 읽어오고, 저장하는 기능 수행*/
  hal.console -> print("Throttle = ");    hal.console -> print(channels[2]); /*Throttle 값 받아오고 출력*/
  hal.console -> print("\tRoll = ");     hal.console -> print(channels[0]); /*Roll 값 받아오고 출력*/
  hal.console -> print("\tPitch = ");    hal.console -> print(channels[1]); /*Pitch 값 받아오고 출력*/
  hal.console -> print("\tYaw = ");     hal.console -> println(channels[3]); /*Yaw 값 받아오고 출력*/
  hal.scheduler -> delay(1000) ;

  hal.rcout -> write(MOTOR,channels[2]); /*1번 출력포트로 선언된 MOTOR 에 2번채널값 Throttle 을 출력*/
  hal.rcout -> write(MOTOR,channels[0]); /*1번 출력포트로 선언된 MOTOR 에 0번채널값 Roll 을 출력*/
  hal.rcout -> write(MOTOR,channels[1]); /*1번 출력포트로 선언된 MOTOR 에 1번채널값 Pitch 을 출력*/
  hal.rcout -> write(MOTOR,channels[3]); /*1번 출력포트로 선언된 MOTOR 에 3번채널값 Yaw 을 출력*/
  
}
AP_HAL_MAIN(); /* Arduino 에서 C++으로 변환하기 위한 코드*/

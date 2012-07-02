#include "acquisition.h"

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"
//#include "nibo/display.h"
//#include "nibo/gfx.h"
#include "nibo/spi.h"
#include "nibo/copro.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"
//#include "nibo/xbee.h"

#include "behaviour.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)



extern int16_t tspeed_l;
extern int16_t tspeed_r;

extern uint16_t dist_l;
extern uint16_t dist_fl;
extern uint16_t dist_f;
extern uint16_t dist_fr;
extern uint16_t dist_r;

extern uint8_t ori; // Range: 0-31
extern uint16_t ori_deg; // Range: 0-360
extern uint8_t ori_opt;
extern uint16_t pos_x;
extern uint16_t pos_y;
extern uint16_t stress;

extern int8_t mode;
extern int8_t last_mode;
extern int8_t mode_cnt;

uint8_t dist_max_ls; // max leftside
uint8_t dist_max_fs; // max frontside
uint8_t dist_max_rs; // max rightside

uint16_t mode_time;

extern uint8_t headlight_target;
extern uint8_t headlight_act;

uint16_t abyss_counter;

//extern int16_t value_fl;
//extern int16_t value_fr;
//extern int16_t value_ll;
//extern int16_t value_lr;

uint16_t distances[32];

uint16_t line_detect_counter_l;
uint16_t line_detect_counter_r;


int16_t line_detect_dir;
int16_t line_detect_pos_l;
int16_t line_detect_pos_r;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
////                       OBSTACLE AVOIDANCE
////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


typedef enum {
  MODE_STOP,
  MODE_STRAIGHTON,
  MODE_TURN_LEFT,
  MODE_TURN_RIGHT,
  MODE_ROTATE_LEFT,
  MODE_ROTATE_RIGHT,
  MODE_BACK
} mode_t;

uint8_t minmodetime[] = {
   20,  //MODE_STOP,
   20,  //MODE_STRAIGHTON,
   20,  //MODE_TURN_LEFT,
   20,  //MODE_TURN_RIGHT,
   50,  //MODE_ROTATE_LEFT,
   50,  //MODE_ROTATE_RIGHT,
  200   //MODE_BACK
};


typedef enum {
  DIST_L  = 0x01,
  DIST_FL = 0x02,
  DIST_F  = 0x04,
  DIST_FR = 0x08,
  DIST_R  = 0x10,
  DIST_ALL = 0x1f
} distnames_t;



void update_distance (uint8_t direction, uint8_t dist) {
  uint8_t d = direction + 32 - 2 + ori;
  distances[(d++)%32] += dist/2;
  distances[(d++)%32] += dist;
  distances[(d++)%32] += dist;
  distances[(d++)%32] += dist;
  distances[(d++)%32] += dist/2;
}

void update_distances() {
  for (uint8_t dir = 0; dir < 32; dir++) {
    distances[dir] *= 7;
  }

  update_distance( 0, copro_distance[2]/256);
  update_distance( 4, copro_distance[1]/256);
  update_distance( 8, copro_distance[0]/256);
  update_distance(12, 255); // backward is blocked
  update_distance(16, 255); // backward is blocked
  update_distance(20, 255); // backward is blocked
  update_distance(24, copro_distance[4]/256);
  update_distance(28, copro_distance[3]/256);

  for (uint8_t dir = 0; dir < 32; dir++) {
    distances[dir] /= 8;
  }

  dist_l  = (distances[(ori+32-9)%32] + distances[(ori+32-8)%32] + distances[(ori+32-7)%32]) / 3;
  dist_fl = (distances[(ori+32-5)%32] + distances[(ori+32-4)%32] + distances[(ori+32-3)%32]) / 3;
  dist_f  = (distances[(ori+32-1)%32] + distances[(ori+32+0)%32] + distances[(ori+32+1)%32]) / 3;
  dist_fr = (distances[(ori+32+3)%32] + distances[(ori+32+4)%32] + distances[(ori+32+5)%32]) / 3;
  dist_r  = (distances[(ori+32+7)%32] + distances[(ori+32+8)%32] + distances[(ori+32+9)%32]) / 3;

  if ((copro_distance[4]/256)>dist_l)  dist_l  = copro_distance[4]/256;
  if ((copro_distance[3]/256)>dist_fl) dist_fl = copro_distance[3]/256;
  if ((copro_distance[2]/256)>dist_f)  dist_f  = copro_distance[2]/256;
  if ((copro_distance[1]/256)>dist_fr) dist_fr = copro_distance[1]/256;
  if ((copro_distance[0]/256)>dist_r)  dist_r  = copro_distance[0]/256;

  uint16_t minval=distances[ori_opt];
  for (uint8_t dir = 0; dir < 32; dir++) {
    if (distances[dir]<minval) {
      minval=distances[dir];
      ori_opt=dir;
    }
  }
}


uint8_t check_le(uint8_t dist, int8_t delta, uint8_t mask) {
  if (delta<0) {
    delta=-delta;
    dist = (dist>delta)? (dist-delta):0;
  } else {
    dist = (dist<255-delta)? (dist+delta):255;
  }

  return (!(mask&DIST_L) || (dist<=dist_l))
       & (!(mask&DIST_FL) || (dist<=dist_fl))
       & (!(mask&DIST_F) || (dist<=dist_f))
       & (!(mask&DIST_FR) || (dist<=dist_fr))
       & (!(mask&DIST_R) || (dist<=dist_r));
}


void behaviour_obstacle_avoidance_start() {
  copro_ir_startMeasure();
  for (uint8_t dir = 0; dir < 32; dir++) {
    distances[dir] = 32;
  }
  ori_opt = ori;
  distances[(32-1+ori_opt)%32] = 0;
  distances[(32+0+ori_opt)%32] = 0;
  distances[(32+1+ori_opt)%32] = 0;
  mode=MODE_STOP;
  abyss_counter=0;
  line_detect_counter_l=0;
  line_detect_counter_r=0;
}


uint8_t behaviour_obstacle_avoidance() {
  copro_ir_startMeasure();
  
  update_distances();

  dist_l = (dist_l<250)? (dist_l+5):255;
  dist_r = (dist_r<250)? (dist_r+5):255;
  dist_f = (dist_f>5)? (dist_f-5):0;

  dist_max_ls = (dist_l>dist_fl)?dist_l:dist_fl;
  dist_max_rs = (dist_r>dist_fr)?dist_r:dist_fr;
  dist_max_fs = (dist_fr>dist_fl)?dist_fr:dist_fl;
  if (dist_f>dist_max_fs) dist_max_fs = dist_f;

  
  uint8_t emergency = 0;
  
  int16_t speed_l = 0;
  int16_t speed_r = 0;
  int16_t speed = 0;

  uint8_t ledr = 0x00;
  uint8_t ledg = 0x00;

  uint16_t sum = dist_r + dist_fr
               + dist_f + dist_fl
               + dist_l;
  sum /= 5;
    
  uint8_t dmax = (sum<0xe0)?(sum+0x20):0xff;
  uint8_t dmin = (sum>0x20)?(sum-0x20):0x00;

  ledr=0;
  ledg=0;

  stress = (stress>10)?(stress-10):(0);

  stress += (copro_current_l>20)?(copro_current_l-5):0;
  stress += (copro_current_r>20)?(copro_current_r-5):0;

  uint8_t ori_corr = (32+ori-ori_opt)%32;
  //gfx_set_proportional(0);
  //gfx_move(100, 40);
  //print_hex(ori_corr);

  uint8_t proposed_mode = mode;
  static uint8_t candidate_mode = 0;
  static uint8_t candidate_cnt = 0;


  switch (mode) {
    case MODE_STOP:
      if (check_le(dist_l, 0, DIST_ALL))
        proposed_mode = MODE_ROTATE_LEFT;
      if (check_le(dist_r, 0, DIST_ALL))
        proposed_mode = MODE_ROTATE_RIGHT;
      if (check_le(dist_fl, 0, DIST_ALL))
        proposed_mode = MODE_TURN_LEFT;
      if (check_le(dist_fr, 0, DIST_ALL))
        proposed_mode = MODE_TURN_RIGHT;
      if (check_le(dist_f, 0, DIST_ALL))
        proposed_mode = MODE_STRAIGHTON;
      break;
      
    case MODE_STRAIGHTON:
        if (check_le(dist_fl, 0, DIST_FL|DIST_F|DIST_FR))
          proposed_mode = MODE_TURN_LEFT;
        if (check_le(dist_fr, 0, DIST_FL|DIST_F|DIST_FR))
          proposed_mode = MODE_TURN_RIGHT;
        if (check_le(dist_f, -2, DIST_FL|DIST_F|DIST_FR))
          proposed_mode = MODE_STRAIGHTON;
        if ((3<=ori_corr) && (ori_corr<=15))
          proposed_mode = MODE_TURN_LEFT;
        if ((29>=ori_corr) && (ori_corr>=17))
          proposed_mode = MODE_TURN_RIGHT;
        if (!check_le(dist_max_fs, -40, DIST_ALL)) {
          if (dist_max_rs>dist_max_ls)
            proposed_mode = MODE_ROTATE_LEFT;
          else if (dist_max_ls>dist_max_rs)
            proposed_mode = MODE_ROTATE_RIGHT;
          else
            proposed_mode = MODE_STOP;
        }
        if (dist_max_fs>0xa0)
          proposed_mode = MODE_BACK;
        break;
      
    case MODE_TURN_LEFT:
        if ((ori_corr>29) || (ori_corr<3))
          proposed_mode = MODE_STRAIGHTON;
        if (check_le(dist_l, 0, DIST_L|DIST_FL|DIST_F))
          proposed_mode = MODE_ROTATE_LEFT;
        if (check_le(dist_fl, -3, DIST_L|DIST_FL|DIST_F))
          proposed_mode = MODE_TURN_LEFT;
        if (check_le(dist_f, 0, DIST_L|DIST_FL|DIST_F))
          proposed_mode = MODE_STRAIGHTON;
        //if (!check_le(dist_max_fs, -40, DIST_ALL))
        //  proposed_mode = MODE_STOP;
        if (dist_max_fs>0xa0)
          proposed_mode = MODE_BACK;
        break;
      
    case MODE_TURN_RIGHT:
        if ((ori_corr>29) || (ori_corr<3))
          proposed_mode = MODE_STRAIGHTON;
        if (check_le(dist_r, 0, DIST_R|DIST_FR|DIST_F))
          proposed_mode = MODE_ROTATE_RIGHT;
        if (check_le(dist_fr, -3, DIST_R|DIST_FR|DIST_F))
          proposed_mode = MODE_TURN_RIGHT;
        if (check_le(dist_f, 0, DIST_R|DIST_FR|DIST_F))
          proposed_mode = MODE_STRAIGHTON;
        //if (!check_le(dist_max_fs, -40, DIST_ALL))
        //  proposed_mode = MODE_STOP;
        if (dist_max_fs>0xa0)
          proposed_mode = MODE_BACK;
        break;
      
    case MODE_ROTATE_LEFT:
        if (check_le(dist_l, 0, DIST_L|DIST_FL|DIST_F))
          proposed_mode = MODE_ROTATE_LEFT;
        if (check_le(dist_fl, -1, DIST_L|DIST_FL|DIST_F))
          proposed_mode = MODE_TURN_LEFT;
        if (check_le(dist_f, -1, DIST_L|DIST_FL|DIST_F))
          proposed_mode = MODE_TURN_LEFT;
        if (!check_le(dist_max_ls, -40, DIST_ALL))
          proposed_mode = MODE_BACK;
        break;
      
    case MODE_ROTATE_RIGHT:
        if (check_le(dist_r, 0, DIST_R|DIST_FR|DIST_F))
          proposed_mode = MODE_ROTATE_RIGHT;
        if (check_le(dist_fr, -1, DIST_R|DIST_FR|DIST_F))
          proposed_mode = MODE_TURN_RIGHT;
        if (check_le(dist_f, -1, DIST_R|DIST_FR|DIST_F))
          proposed_mode = MODE_TURN_RIGHT;
        if (!check_le(dist_max_rs, -40, DIST_ALL))
          proposed_mode = MODE_BACK;
        break;
      
    case MODE_BACK:
        proposed_mode = MODE_STOP;
      
        break;
  }

  if (mode_time<255) {
    mode_time++;
    if (mode_time>=minmodetime[mode]) {
      mode_time=255;
    }
  }


  if (mode_time==255) {
    if (mode!=proposed_mode) {
      if (proposed_mode==candidate_mode) {
        candidate_cnt++;
        if (candidate_cnt>5) {
          mode = proposed_mode;
          mode_time = 0;
        }
      } else {
        candidate_cnt = 0;
        candidate_mode = proposed_mode;
      }
    }
  }


  if (stress>200) {
    if (mode != MODE_BACK) {
      mode = MODE_BACK;
      distances[(32-3+ori)%32]=255;
      distances[(32-2+ori)%32]=255;
      distances[(32-1+ori)%32]=255;
      distances[(32+0+ori)%32]=255;
      distances[(32+1+ori)%32]=255;
      distances[(32+2+ori)%32]=255;
      distances[(32+3+ori)%32]=255;
    } else {
      mode = MODE_STOP;
      distances[(16-3+ori)%32]=255;
      distances[(16-2+ori)%32]=255;
      distances[(16-1+ori)%32]=255;
      distances[(16+0+ori)%32]=255;
      distances[(16+1+ori)%32]=255;
      distances[(16+2+ori)%32]=255;
      distances[(16+3+ori)%32]=255;
    }
    emergency = 1;
    stress = 0;
  }

  if (mode!=last_mode) {
    mode_cnt+=2;
    if (mode_cnt>80) {
      mode = MODE_BACK;
      emergency = 1;
    }
    last_mode=mode;
  } else {
    if (mode_cnt>0) {
      mode_cnt--;
    }
  }
  
  
  //gfx_move(100, 0);
  //print_hex(mode);

  uint8_t turn_factor;
      
  switch (mode) {
    case MODE_STOP:
        speed_l=0, speed_r=0, speed=0, ledg=0x00, ledr=0x84;
        turn_factor = 0;
        emergency = 1;
        break;
      
    case MODE_STRAIGHTON:
        headlight_target=10;
//        speed_l=100, speed_r=100, speed=50, ledg= 0x30;
        speed_l=100, speed_r=100, speed=80, ledg= 0x30;
        turn_factor = 60;
        break;
      
    case MODE_TURN_LEFT:
        headlight_target=10;
//        speed_l=70, speed_r=100, speed=40, ledg= 0x08;
        speed_l=70, speed_r=100, speed=60, ledg= 0x08;
        turn_factor = 60;
        break;
      
    case MODE_TURN_RIGHT:
        headlight_target=10;
//        speed_l=100, speed_r=70, speed=40, ledg= 0x40;
        speed_l=100, speed_r=70, speed=60, ledg= 0x40;
        turn_factor = 60;
        break;
      
    case MODE_ROTATE_LEFT:
        headlight_target=0;
        speed_l=-100, speed_r=100, speed=20, ledg= 0x04;
        turn_factor = 30;
        break;
      
    case MODE_ROTATE_RIGHT:
        headlight_target=0;
        speed_l=100, speed_r=-100, speed=20, ledg= 0x80;
        turn_factor = 30;
        break;
      
    case MODE_BACK:
        headlight_target=0;
        speed_l=-100, speed_r=-100, speed=20, ledg= 0x03, ledr=0x00;
        turn_factor = 20;
        break;
      
  }

      
  IO_LEDS_RED_PORT = ledr;
  IO_LEDS_GREEN_PORT = ledg;

  if (headlight_act<headlight_target)
    headlight_act++;
  if (headlight_act>headlight_target)
    headlight_act--;

  if (headlight_act)
    leds_set_headlights(1<<(headlight_act-1));
  else
    leds_set_headlights(0);

  if (mode==MODE_STRAIGHTON) {
    int16_t lval = 2*dist_fl+dist_l;
    int16_t rval = 2*dist_fr+dist_r;
    int16_t delta = lval-rval;

    if (abs(delta)>5) {
      if (lval>rval) speed_r-=10; // rechts mehr Platz
      if (rval>lval) speed_l-=10; // links mehr Platz
    } else {
      if (lval>rval) speed_r-=5; // rechts mehr Platz
      if (rval>lval) speed_l-=5; // links mehr Platz
    }

  } else if (mode==MODE_BACK) {
    uint16_t lval = dist_fl+2*dist_l;
    uint16_t rval = dist_fr+2*dist_r;
    if (lval>rval) speed_r-=15; // rechts mehr Platz
    if (rval>lval) speed_l-=15; // links mehr Platz

  } else {
    uint16_t lval = dist_fl+dist_l;
    uint16_t rval = dist_fr+dist_r;
    if (lval>rval) speed_r-=5; // rechts mehr Platz
    if (rval>lval) speed_l-=5; // links mehr Platz
  }

  if (ori_corr == 0) {
    // BESTER WEG LIEGT GERADEAUS
  } else if (ori_corr < 16) {
    // BESTER WEG LIEGT LINKS
    // ori_corr [1 ... 8]
    speed_l -= (turn_factor * (uint16_t)ori_corr)/8;
    speed_r += (turn_factor * (uint16_t)ori_corr)/8;
    speed = (speed * (16 - ori_corr))/16;
  } else {
    // BESTER WEG LIEGT RECHTS
    // ori_corr [24 ...31]
    ori_corr = 32-ori_corr;
    speed_l += (turn_factor * (uint16_t)ori_corr)/8;
    speed_r -= (turn_factor * (uint16_t)ori_corr)/8;
    speed = (speed * (16 - ori_corr))/16;
  }


  speed_l = (speed_l*speed)/128;
  speed_r = (speed_r*speed)/128;

#if 0

  if (abyss_counter<500) { // 5 sec
    if ((acquisition_getFloorState(AN_FLOOR_L)==FLOOR_LINE) || 
	    (acquisition_getFloorState(AN_FLOOR_R)==FLOOR_LINE) ||
        (acquisition_getFloorState(AN_LINE_L)==FLOOR_LINE) ||
        (acquisition_getFloorState(AN_LINE_R)==FLOOR_LINE) ){
	  // still in area
	  abyss_counter=0; 
	} else {
      abyss_counter++;
	}
    
  } else {
    if ((acquisition_getDiff(AN_FLOOR_L)<0x02)||(acquisition_getDiff(AN_FLOOR_R)<0x02)) {
      speed_l=0;
      speed_r=0;
      emergency = 1;
    }

//    if ((acquisition_getFloorState(AN_FLOOR_R)!=FLOOR_ABYSS) || (acquisition_getFloorState(AN_FLOOR_L)!=FLOOR_ABYSS)) {
  //    speed_l=0;
    //  speed_r=0;
//      emergency = 1;
  //  }

    if (line_detect_counter_l) {
      line_detect_counter_l--;
    } else if (acquisition_getFloorState(AN_FLOOR_L)!=FLOOR_ABYSS) {
      line_detect_counter_l = 300;
      if (line_detect_counter_r==0) {
        line_detect_pos_l = copro_ticks_l;
        line_detect_pos_r = copro_ticks_r;
      }
    }
    
    if (line_detect_counter_r) {
      line_detect_counter_r--;
    } else if (acquisition_getFloorState(AN_FLOOR_R)!=FLOOR_ABYSS) {
      line_detect_counter_r = 300;
      if (line_detect_counter_l==0) {
        line_detect_pos_l = copro_ticks_l;
        line_detect_pos_r = copro_ticks_r;
      }
    }

    if (line_detect_counter_l && line_detect_counter_r) {
      speed_l=0;
      speed_r=0;
      emergency = 1;
      line_detect_dir = line_detect_counter_l-line_detect_counter_r;
      line_detect_pos_l = copro_ticks_l-line_detect_pos_l;
      line_detect_pos_r = copro_ticks_r-line_detect_pos_r;
      return BEHAVIOUR_LINE;
    }
    

  }
#endif

  if (((tspeed_l<0) && (speed_l>=0)) ||
      ((tspeed_l>0) && (speed_l<=0)) ||
      ((tspeed_r<0) && (speed_r>=0)) ||
      ((tspeed_r>0) && (speed_r<=0)) ||
      emergency) {
    tspeed_l = 0;
    tspeed_r = 0;
    copro_stop();
    
  } else {
    tspeed_l = (3*tspeed_l+speed_l)/4;
    tspeed_r = (3*tspeed_r+speed_r)/4;
    if ((speed_l>0)&&(speed_l<tspeed_l)) {
      tspeed_l=speed_l;
    }
    if ((speed_r>0)&&(speed_r<tspeed_r)) {
      tspeed_r=speed_r;
    }

    copro_setSpeed(tspeed_l, tspeed_r);
  }
  return BEHAVIOUR_OBSTACLE;
}

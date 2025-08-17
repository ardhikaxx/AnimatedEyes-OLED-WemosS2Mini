#include <Wire.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SDA_PIN 9  // GPIO9 untuk SDA
#define SCL_PIN 8  // GPIO8 untuk SCL

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /*reset=*/ U8X8_PIN_NONE, SCL_PIN, SDA_PIN);

int COLOR_WHITE = 1;
int COLOR_BLACK = 0;

int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;

int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = SCREEN_WIDTH/2-ref_eye_width/2-ref_space_between_eye/2;
int left_eye_y = SCREEN_HEIGHT/2;
int right_eye_x = SCREEN_WIDTH/2+ref_eye_width/2+ref_space_between_eye/2;
int right_eye_y = SCREEN_HEIGHT/2;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;
int corner_radius = ref_corner_radius;

const int animation_sequence[] = {4, 5, 2, 3, 6, 4, 5};
int sequence_index = 0;
unsigned long last_animation_time = 0;
const int animation_delay = 2000;

void display_clearDisplay() {
  u8g2.clearBuffer();   
}

void display_fillRoundRect(int x, int y, int w, int h, int r, int color) {
  u8g2.setDrawColor(color);
  if(w < 2*(r+1)) r = (w/2)-1;
  if(h < 2*(r+1)) r = (h/2)-1;
  u8g2.drawRBox(x, y, w<1?1:w, h<1?1:h, r);
}

void display_display() {
  u8g2.sendBuffer();
}

void display_fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) {
  u8g2.setDrawColor(color);
  u8g2.drawTriangle(x0, y0, x1, y1, x2, y2);
}

void draw_eyes(bool update=true) {
    display_clearDisplay();        
    int x = int(left_eye_x-left_eye_width/2);
    int y = int(left_eye_y-left_eye_height/2);
    display_fillRoundRect(x, y, left_eye_width, left_eye_height, corner_radius, COLOR_WHITE);
    
    x = int(right_eye_x-right_eye_width/2);
    y = int(right_eye_y-right_eye_height/2);
    display_fillRoundRect(x, y, right_eye_width, right_eye_height, corner_radius, COLOR_WHITE);    
    
    if(update) display_display();
}

void reset_eyes(bool update=true) {
  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;
  
  left_eye_x = SCREEN_WIDTH/2-ref_eye_width/2-ref_space_between_eye/2;
  left_eye_y = SCREEN_HEIGHT/2;
  right_eye_x = SCREEN_WIDTH/2+ref_eye_width/2+ref_space_between_eye/2;
  right_eye_y = SCREEN_HEIGHT/2;

  corner_radius = ref_corner_radius;
  draw_eyes(update);
}

void sleep() {
  reset_eyes(false);
  left_eye_height = 2;
  left_eye_width = ref_eye_width;
  right_eye_height = 2;
  right_eye_width = ref_eye_width;
  corner_radius = 0;
  draw_eyes(true);  
}

void wakeup() {
  reset_eyes(false);
  sleep();
  
  for(int h=2; h <= ref_eye_height; h+=2) {
    left_eye_height = h;
    right_eye_height = h;
    corner_radius = h;
    if(corner_radius > ref_corner_radius) corner_radius = ref_corner_radius;
    draw_eyes(true);
  }
}

void blink(int speed=12) {
  int original_left_height = left_eye_height;
  int original_right_height = right_eye_height;
  int original_left_width = left_eye_width;
  int original_right_width = right_eye_width;
  
  for(int i=0; i<3; i++) {
    left_eye_height -= speed;
    right_eye_height -= speed;
    left_eye_width += 3;
    right_eye_width += 3;
    draw_eyes();
    delay(1);
  }
  
  for(int i=0; i<3; i++) {
    left_eye_height += speed;
    right_eye_height += speed;
    left_eye_width -= 3;
    right_eye_width -= 3;
    draw_eyes();
    delay(1);
  }

  left_eye_height = original_left_height;
  right_eye_height = original_right_height;
  left_eye_width = original_left_width;
  right_eye_width = original_right_width;
}

void happy_eye() {
  reset_eyes(false);
  int offset = ref_eye_height/2;
  
  for(int i=0; i<10; i++) {
    display_fillTriangle(
      left_eye_x-left_eye_width/2-1, left_eye_y+offset, 
      left_eye_x+left_eye_width/2+1, left_eye_y+5+offset, 
      left_eye_x-left_eye_width/2-1, left_eye_y+left_eye_height+offset, 
      COLOR_BLACK
    );    
    display_fillTriangle(
      right_eye_x+right_eye_width/2+1, right_eye_y+offset, 
      right_eye_x-left_eye_width/2-2, right_eye_y+5+offset, 
      right_eye_x+right_eye_width/2+1, right_eye_y+right_eye_height+offset, 
      COLOR_BLACK
    );
    offset -= 2;
    display_display();
    delay(1);
  }
  display_display();
  delay(1000);
}

void move_big_eye(int direction) {
  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for(int i=0; i<3; i++) {
    left_eye_x += direction_movement_amplitude*direction;
    right_eye_x += direction_movement_amplitude*direction;    
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    
    if(direction>0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  
  for(int i=0; i<3; i++) {
    left_eye_x += direction_movement_amplitude*direction;
    right_eye_x += direction_movement_amplitude*direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if(direction>0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }
    draw_eyes();
    delay(1);
  }

  delay(1000);

  for(int i=0; i<3; i++) {
    left_eye_x -= direction_movement_amplitude*direction;
    right_eye_x -= direction_movement_amplitude*direction;    
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    if(direction>0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
    draw_eyes();
    delay(1);
  }
  
  for(int i=0; i<3; i++) {
    left_eye_x -= direction_movement_amplitude*direction;
    right_eye_x -= direction_movement_amplitude*direction;    
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if(direction>0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
    draw_eyes();
    delay(1);
  }

  reset_eyes();
}

void move_right_big_eye() {
  move_big_eye(1);
}

void move_left_big_eye() {
  move_big_eye(-1);
}

void launch_animation(int animation_index) {
  switch(animation_index) {
    case 1:
      blink(12);
      break;
    case 2:
      move_right_big_eye();
      break;
    case 3:
      move_left_big_eye();
      break;
    case 4:
      blink(12);
      break;
    case 5:
      blink(12);
      break;
    case 6:
      happy_eye();
      break;
  }
}

void startupAnimation() {
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(500);

  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(10, 20, "Booting...");
  
  for(int i = 0; i <= 100; i+=5) {
    u8g2.clearBuffer();
    u8g2.drawStr(10, 20, "Starting...");
    u8g2.drawFrame(10, 30, 100, 10);
    u8g2.drawBox(10, 30, i, 10);
    u8g2.sendBuffer();
    delay(500);
  }
  delay(300);

  sleep();
  delay(500);
  wakeup();

  reset_eyes();
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  startupAnimation();
  reset_eyes();
}

void loop() {
  static bool first_run = true;
  
  if(first_run || millis() - last_animation_time > animation_delay) {
    first_run = false;
    
    if(sequence_index < sizeof(animation_sequence)/sizeof(animation_sequence[0])) {
      launch_animation(animation_sequence[sequence_index]);
      sequence_index++;
    } else {
      sequence_index = 0;
      reset_eyes();
    }
    
    last_animation_time = millis();
  }
}
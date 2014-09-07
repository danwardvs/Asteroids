#include<allegro.h>
#include<alpng.h>
#include<cmath>

BITMAP* buffer;
BITMAP* cursor;
BITMAP* spaceship;

float distance_to_mouse;
float angle_radians;
float angle_degrees;
float angle_allegro;

int player_x=400;
int player_y=300;

float angle_x;
float angle_y;
float vector_x;
float vector_y;

bool debugmode;

bool create_bullet;
int bullet_delay;

struct bullet{
    float x;
    float y;
    float vector_x;
    float vector_y;
    bool on_screen=false;
}bullets[100];

//A function that handles error messages
void abort_on_error(const char *message){
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}
//Finds distance between point 1 and point 2
int distance_to_object(int x_1, int y_1,int x_2,int y_2){
    return sqrt((pow(x_1-x_2,2))+(pow(y_1-y_2,2)));

}

//Finds angle of point 2 relative to point 1
float find_angle(int x_1, int y_1, int x_2, int y_2){
    float tan_1;
    float tan_2;
    if(x_1-x_2!=0 && y_1-y_2!=0){
        tan_1=y_1-y_2;
        tan_2=x_1-x_2;
    }

    return atan2(tan_1,tan_2);
}


void update(){

    //Calls the function, solves the math
    distance_to_mouse=distance_to_object(mouse_x,mouse_y,player_x,player_y);
    angle_radians=find_angle(player_x,player_y,mouse_x,mouse_y);

    //Converts radians to usable(by allegro) values
    angle_degrees=(angle_radians*57.2957795);
    angle_allegro=(angle_degrees/1.41176470588);

    angle_x=sin(angle_degrees);
    angle_y=cos(angle_degrees);

    //Moves the point


    //Draws the screen
    rectfill(buffer,0,0,800,600,makecol(10,0,0));
    if(key[KEY_F1])debugmode=true;
    if(key[KEY_F2])debugmode=false;
    if(debugmode){
        textprintf_ex(buffer,font,5,5,makecol(0,0,0),-1,"Mouse X:%i",mouse_x);
        textprintf_ex(buffer,font,5,15,makecol(0,0,0),-1,"Mouse Y:%i",mouse_y);
        textprintf_ex(buffer,font,5,25,makecol(0,0,0),-1,"Distance to mouse:%4.2f",distance_to_mouse);
        textprintf_ex(buffer,font,5,35,makecol(0,0,0),-1,"Radians:%4.2f,Degrees%4.2f,Allegro%4.2f",angle_radians,angle_degrees,angle_allegro);
        textprintf_ex(buffer,font,5,45,makecol(0,0,0),-1,"Vector X:%4.2f,Vector Y:%4.2f",angle_x,angle_y);
    }

    rotate_sprite(buffer,spaceship,player_x-46,player_y-38,itofix(angle_allegro));
    putpixel(buffer,player_x,player_y,makecol(0,0,0));
    for(int i=0; i<100; i++){
        if(bullets[i].on_screen){
            putpixel(buffer,bullets[i].x,bullets[i].y,makecol(255,0,0));
        }
    }
    draw_sprite(buffer,cursor,mouse_x-10,mouse_y-10);
    draw_sprite(screen,buffer,0,0);

    bullet_delay++;
    if(key[KEY_SPACE] && bullet_delay>9 || mouse_b & 1 && bullet_delay>9 ){
        create_bullet=true;
        bullet_delay=0;
    }



    vector_y=-2*sin(angle_radians);
    vector_x=-2*cos(angle_radians);
    if(key[KEY_LEFT] || key[KEY_W]){
        player_x+=vector_x;
        player_y+=vector_y;
    }


    for(int i=0; i<100; i++){
        if(bullets[i].on_screen){
           bullets[i].x+=bullets[i].vector_x;
           bullets[i].y+=bullets[i].vector_y;


           if(bullets[i].x>800 || bullets[i].y>600 || bullets[i].x<0 || bullets[i].y<0)bullets[i].on_screen=false;
        }else if(create_bullet==true){
            bullets[i].on_screen=true;
            create_bullet=false;
            bullets[i].x=player_x;
            bullets[i].y=player_y;
            bullets[i].vector_x=-2*cos(angle_radians);
            bullets[i].vector_y=-2*sin(angle_radians);
        }
    }
    rest(2);
}








void setup(){
    buffer=create_bitmap(800,600);

    if(!(cursor = load_bitmap("cursor.png",NULL))){
        abort_on_error( "Cannot find cursor.png.\n Please check your files and try again.");
    }
    if(!(spaceship = load_bitmap("spaceship.png",NULL))){
        abort_on_error( "Cannot find spaceship.png.\n Please check your files and try again.");
    }
}






int main(){

  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);

  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800,600, 0, 0);



  set_window_title("Angle Finder");
  setup();


    while(!key[KEY_ESC]){
        update();
  	}

	return 0;
}
END_OF_MAIN()

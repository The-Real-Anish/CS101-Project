#include <simplecpp>
#include "shooter.h"
#include "bubble.h"

/* Simulation Vars */
const double STEP_TIME = 0.02;

/* Game Vars */
const int PLAY_Y_HEIGHT = 450;
const int LEFT_MARGIN = 70;
const int TOP_MARGIN = 20;
const int BOTTOM_MARGIN = (PLAY_Y_HEIGHT+TOP_MARGIN);


void move_bullets(vector<Bullet> &bullets){
    // move all bullets
    for(unsigned int i=0; i<bullets.size(); i++){
        if(!bullets[i].nextStep(STEP_TIME)){
            bullets.erase(bullets.begin()+i);
        }
    }
}

void move_bubbles(vector<Bubble> &bubbles){
    // move all bubbles
    for (unsigned int i=0; i < bubbles.size(); i++)
    {
        bubbles[i].nextStep(STEP_TIME);
    }
}

vector<Bubble> create_bubbles(int lvl)
{
    // create initial bubbles in the game
    vector<Bubble> bubbles;
    bubbles.push_back(Bubble(WINDOW_X/2.0, BUBBLE_START_Y, PLAY_Y_HEIGHT, lvl - 1));
    if(lvl != 4) bubbles.push_back(Bubble(WINDOW_X/4.0, BUBBLE_START_Y, PLAY_Y_HEIGHT, lvl - 1, -BUBBLE_DEFAULT_VX));
    return bubbles;
}

bool checklife(Shooter shooter, vector<Bubble> &bubbles){
   for(unsigned int i = 0; i<bubbles.size(); i++){
      if(i == 0 && bubbles[0].get_level() == 3) i++;
      double r_head = sqrt(pow(bubbles[i].get_center_x() - shooter.get_head_center_x(),2) +
                           pow(bubbles[i].get_center_y() - shooter.get_head_center_y(),2));
      //distance between shooter head and bubble
      double body_x = max(shooter.get_body_center_x() - shooter.get_body_width()/2,
                          min(shooter.get_body_center_x() + shooter.get_body_width()/2, bubbles[i].get_center_x()));
      //algorithm to find the abscissa of the point on the body closest to the bubble
      double body_y = max(shooter.get_body_center_y() - shooter.get_body_height()/2,
                          min(shooter.get_body_center_y() + shooter.get_body_height()/2, bubbles[i].get_center_y()));
      //find ordinate of point on the body closest to the bubble
      double body_r = sqrt(pow(bubbles[i].get_center_x() - body_x, 2) +
                           pow(bubbles[i].get_center_y() - body_y, 2));
      if(r_head <= bubbles[i].get_radius() + shooter.get_head_radius()
         ||
         body_r <= bubbles[i].get_radius())
      return true;
      }
   return false;
   }

void split(vector<Bubble> &bubbles, int j){ //bubble splitting mechanism
    if(bubbles[j].get_level() == 3) bubbles[j].reduce_life();
    if((bubbles[j].get_level() == 3 && bubbles[j].get_life() == 0)
       ||
       (bubbles[j].get_level() != 3))
    {
       double vx = bubbles[j].get_vx(), vy = bubbles[j].get_vy(), cx = bubbles[j].get_center_x(), cy = bubbles[j].get_center_y();
       int l = bubbles[j].get_level();
       bubbles.erase(bubbles.begin() + j);
       if(l != 0)
       { //higher level bubble gets split into lower level bubbles
         if(vy > 0) vy = vy - 20;
         else vy = vy - 5;
         bubbles.push_back(Bubble(cx, cy, PLAY_Y_HEIGHT, l - 1, vx + 10, vy));
         bubbles.push_back(Bubble(cx, cy, PLAY_Y_HEIGHT, l - 1, -vx - 10, vy));
       }
    }
    }

void checkhit(vector<Bullet> &bullets, vector<Bubble> &bubbles){
   // register a bullet hitting a bubble
   for(unsigned int i=0; i<bullets.size(); i++){
      for (unsigned int j=0; j < bubbles.size(); j++){
         double bullet_x = max(bullets[i].get_center_x() - bullets[i].get_width()/2,
                               min(bullets[i].get_center_x() + bullets[i].get_width()/2, bubbles[j].get_center_x()));
            //abscissa of point on bullet closest to bubble
         double bullet_y = max(bullets[i].get_center_y() - bullets[i].get_height()/2,
                               min(bullets[i].get_center_y() + bullets[i].get_height()/2, bubbles[j].get_center_y()));
            //ordinate of point on bullet closest to bubble
         double bullet_r = sqrt(pow(bullet_x - bubbles[j].get_center_x(), 2) +
                                pow(bullet_y - bubbles[j].get_center_y(), 2));
         if(bullet_r < bubbles[j].get_radius())
            //checks if any bullet intersects with a bubble
         {
            bullets.erase(bullets.begin() + i); //removes the bullet hitting the bubble
            split(bubbles, j);
            return; //splits bubble
         }
         }
      }
   return;
   }

int main()
{
    int gamelvl = 1, bosstime = 0;

    initCanvas("Bubble Trouble", WINDOW_X, WINDOW_Y);

    Line b1(0, PLAY_Y_HEIGHT, WINDOW_X, PLAY_Y_HEIGHT);
    b1.setColor(COLOR(0, 0, 255));

    string msg_cmd("Cmd: _");
    string lvl_count("Level: 1");
    string boss_health ("Health: __");
    Text charPressed(LEFT_MARGIN, BOTTOM_MARGIN, msg_cmd);
    Text countLevel(WINDOW_X - LEFT_MARGIN, BOTTOM_MARGIN, lvl_count);
    Text health(WINDOW_X/2, BOTTOM_MARGIN, boss_health);
    health.setColor(COLOR(255,255,255));

    // Intialize the shooter
    Shooter shooter(SHOOTER_START_X, SHOOTER_START_Y, SHOOTER_VX);

    // Initialize the bubbles
    vector<Bubble> bubbles = create_bubbles(gamelvl);

    // Initialize the bullets (empty)
    vector<Bullet> bullets;

    XEvent event;

    // Main game loop
    while (true)
    {
        bool pendingEvent = checkEvent(event);
        if (pendingEvent)
        {
            // Get the key pressed
            char c = charFromEvent(event);
            msg_cmd[msg_cmd.length() - 1] = c;
            charPressed.setMessage(msg_cmd);

            // Update the shooter
            if(c == 'a')
                shooter.move(STEP_TIME, true);
            else if(c == 'd')
                shooter.move(STEP_TIME, false);
            else if(c == 'w')
                bullets.push_back(shooter.shoot());
            else if(c == 'q')
                return 0;
        }

        // Update the bubbles
        move_bubbles(bubbles);
        if(gamelvl == 4)
        {
           if(bubbles[0].get_life() != 0) health.setColor(COLOR(255, 0, 0));
           if(bubbles[0].get_life() != 0 && (bosstime == 0 || bosstime%250 == 0)){
             if(bubbles[0].get_vx() > 0)
             bubbles.push_back(Bubble(bubbles[0].get_center_x(), bubbles[0].get_center_y(), PLAY_Y_HEIGHT, 2, -BUBBLE_DEFAULT_VX, 0));
             else
             bubbles.push_back(Bubble(bubbles[0].get_center_x(), bubbles[0].get_center_y(), PLAY_Y_HEIGHT, 2, BUBBLE_DEFAULT_VX, 0));
             }
           if(bubbles[0].get_life() != 0){
             boss_health[boss_health.length() - 2] = bubbles[0].get_life()/10 + 48;
             boss_health[boss_health.length() - 1] = bubbles[0].get_life()%10 + 48;
             health.setMessage(boss_health);
             }
            else health.setColor(COLOR(255, 255, 255));
           bosstime++;
        }

        // Update the bullets
        move_bullets(bullets);

        // Check if bubble is hit
        checkhit(bullets, bubbles);

        // End game if shooter is hit
        if(checklife(shooter, bubbles)) break;

        //
        if(bubbles.size() == 0)
        {
           bullets.clear();
           gamelvl++;
           if(gamelvl == 5) break;
           Text win((TOP_MARGIN + BOTTOM_MARGIN)/2, PLAY_Y_HEIGHT/2, "Congrats! Next Level . . .");
           wait(2);
           lvl_count[lvl_count.length() - 1] = gamelvl + 48;
           countLevel.setMessage(lvl_count);
           bubbles = create_bubbles(gamelvl);
           continue;
        }

        wait(STEP_TIME);
    }
    if(gamelvl == 5){
       Text win((TOP_MARGIN + BOTTOM_MARGIN)/2, PLAY_Y_HEIGHT/2, "Congrats! You Won!");
       getClick();
       }
    else{
       bubbles.clear();
       bullets.clear();
       Text lose((TOP_MARGIN + BOTTOM_MARGIN)/2, PLAY_Y_HEIGHT/2, "Game Over");
       getClick();
       }

}

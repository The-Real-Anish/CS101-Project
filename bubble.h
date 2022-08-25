#include <simplecpp>

/* Bubble Vars */
const int BUBBLE_START_X = 250;
const int BUBBLE_START_Y = 50;
const int BUBBLE_DEFAULT_RADIUS = 20;
const int BUBBLE_RADIUS_THRESHOLD = 10;
const int BUBBLE_DEFAULT_VX = 150;
const int BOSS_VX = 200;
const int BUBBLE_DEFAULT_VY = 0;
const int g = 25;

class Bubble
{
private:
    Circle circle;  // the circle representing the bubble
    double vx, vy;  // velocity in x and y direction
    int a, Bounce, Level, Life; // acc and ground from which bubble bounces
    Color color;    // color of the bubble
    Rectangle mouth, l_eye, r_eye;
public:
    Bubble(double cx, double cy, int b, int l, double vx_=BUBBLE_DEFAULT_VX, double vy_=BUBBLE_DEFAULT_VY)
    {
        // Bubble constructor

        color = COLOR(255, 205 - 40*l, 245 - 20*l);
        circle = Circle(cx, cy, 5*pow(2, l));
        circle.setColor(color);
        circle.setFill(true);
        vx = vx_;
        vy = vy_;
        a = g;
        Bounce = b;
        Level = l;
        Life = 0;

        if(Level == 3)
        {
           double r = circle.getRadius();
           l_eye = Rectangle(cx - r/3, cy - 3*r/8, r/4, r/4);
           l_eye.setColor(COLOR(0, 255, 255));
           l_eye.setFill(true);
           r_eye = Rectangle(cx + r/3, cy - 3*r/8, r/4, r/4);
           r_eye.setColor(COLOR(0, 255, 255));
           r_eye.setFill(true);
           mouth = Rectangle(cx, cy + 3*r/8, 3*r/4, r/4);
           mouth.setColor(COLOR(0, 255, 255));
           mouth.setFill(true);
           vx = BOSS_VX;
           vy = 0;
           a = 0;
           Life = 20;
        }
    }

    void nextStep(double t)
    {
        // move the bubble

        vy = vy + a*t;
        double new_x = circle.getX() + vx*t;
        double new_y = circle.getY() + vy*t;

        if ((vx < 0 && new_x < (0 + circle.getRadius())) // bounce along X direction at left border
            ||
            (vx > 0 && new_x > (WINDOW_X - circle.getRadius()))) // bounce along X direction at right border
        {

            vx = -vx;
            new_x = circle.getX() + vx*t;

        }

        if (vy > 0 && new_y > (Bounce - circle.getRadius())) //bounce along Y direction at bottom margin
        {
            vy = - vy;
            new_y = circle.getY() + vy*t;
        }
        circle.moveTo(new_x, new_y);
        if(Level == 3)
        {
           double dx = vx*t;
           l_eye.move(dx, 0);
           r_eye.move(dx, 0);
           mouth.move(dx, 0);
        }
    }

    int get_level()
    {
       //return level of bubble
       return Level;
    }

    void reduce_life()
    {
       Life--;
    }

    int get_life()
    {
       //return life of boss bubble
       return Life;
    }

    double get_radius()
    {
        // return the radius of the bubble
        return circle.getRadius();
    }

    double get_center_x()
    {
        // return the x coordinate of the center of the bubble
        return circle.getX();
    }

    double get_center_y()
    {
        // return the y coordinate of the center of the bubble
        return circle.getY();
    }

    double get_vx()
    {
        // return the x velocity of the bubble
        return vx;
    }
    double get_vy()
    {
        // return the y velocity of the bubble
        return vy;
    }

    Color get_color()
    {
        // return the color of the bubble
        return color;
    }
};

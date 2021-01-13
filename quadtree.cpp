#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h> 

using namespace std;

sf::RenderWindow window(sf::VideoMode(600,600), "Quad Tree implementation");

 

class Particle
{   
    public:

    float x ;
    float y ;
    float r ;

    Particle(float cx , float cy,float R)
    {
        x = cx;
        y = cy;
        r = R;
    }
    
    sf::CircleShape display()
    {
        sf::CircleShape cshape(r);
        cshape.setFillColor(sf::Color::Green);
        cshape.setPosition(x, y);
        
        return cshape;
    }
};

class rectangle
{ 
    public:
    float x,y,w,h ; 


    rectangle (float cx =0 ,float cy = 0,float  wi= 0,float ht= 0) {
        x = cx ;
        y = cy;
        w = wi; 
        h = ht;

    }

    bool contains(Particle p1)
    {
        return ((p1.x >= x-w/2 )&& (p1.x <= x+ w/2 )&& (p1.y >= y-h/2) && (p1.y <= y+h/2)) ;
    }

   
};



class quadTree
{   
    private:
    
    rectangle* rec1;
    
    quadTree *northwest; 
    quadTree *northeast; 
    quadTree *southwest; 
    quadTree *southeast; 

    int capacity;
    bool divided = false;

    public:
    
    vector<Particle> p1;

    quadTree(rectangle* p, int n )
    {

        rec1 = p;
        capacity = n;
        
    }

    void subdivide()
    {
        float x1 = rec1->x;
        float y1 = rec1->y;
        float w1 = rec1->w;
        float h1 = rec1->h;

        rectangle* ne  = new rectangle(x1+w1/4, y1-h1/4 , w1/2,h1/2);
        northeast = new quadTree(ne,capacity);
        rectangle* nw  = new rectangle(x1-w1/4, y1-h1/4 , w1/2,h1/2);
        northwest = new quadTree(nw,capacity);
        rectangle* se  = new rectangle(x1+w1/4, y1+h1/4 , w1/2,h1/2);
        southeast = new quadTree(se,capacity);
        rectangle* sw  = new rectangle(x1-w1/4, y1+h1/4 , w1/2,h1/2);
        southwest = new quadTree(sw,capacity);
       
         divided = true;


    }

    bool insert(Particle x ) 
    {

        if (! rec1->contains(x))
        {
            return false ;
        }
        if (p1.size()< capacity)
        {
            p1.push_back(x);  
            return true;      
        }
        else
        {
            if (!divided) 
            {
                subdivide();
                
            }

            if (northeast->insert(x)) {
                return true;
            }
            else if (northwest->insert(x)) {
                return true;
            }
            else if (southeast->insert(x)) {
                return true;
            }
            else if (southwest->insert(x)) {
                return true;
            }
                  
        }
            
    }
  

     void qtdisplay()
    {
        
        sf::RectangleShape rshape(sf::Vector2f(rec1->w , rec1->h));
    
        rshape.setOrigin(rshape.getSize().x/2,rshape.getSize().y/2);
        rshape.setPosition(rec1->x,rec1->y);
        rshape.setFillColor(sf::Color::Transparent);
        rshape.setOutlineThickness(1.f);
        rshape.setOutlineColor(sf::Color(250, 150, 100));
        window.draw(rshape);
        if (divided)
        {
            northeast->qtdisplay();
            northwest->qtdisplay();
            southeast->qtdisplay();
            southwest->qtdisplay();
        }

        for (int i = 0 ; i<p1.size(); i++)
        { 
        sf::CircleShape cshape(p1[i].r);
        cshape.setFillColor(sf::Color::Green);
        cshape.setPosition(p1[i].x, p1[i].y);
        window.draw(cshape);
        } 
    }

   

    
};


int main ()
{

rectangle* r1 = new rectangle(300,300,600,600);

quadTree* qt  = new quadTree(r1 , 4);

vector <Particle> p2 ;

for (int j = 0 ; j< 20 ; j++)
{
    p2.push_back(Particle(rand()%500+20 , rand()%500+20,3));

    qt->insert(p2[j]) ;
}


qt->qtdisplay();   


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    window.setFramerateLimit(20);
    window.display();
    
    }


return 0;

}
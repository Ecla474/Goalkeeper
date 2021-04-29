#include <iostream>
#include <Grapic.h>
#include <cmath>
using namespace grapic;
using namespace std;

// LES CONSTANTES

const int DIMW = 500;
const int CONST_MAX = 30;
const float FRICTION = 0.6f;
const float G = 9.81f;
const int MAX_BALL = 50;

// LES STRUCTURES

struct Vec2
{
	float x, y;
};

struct Particle
{
	Vec2 p;         // position(p.x, p.y)
	Vec2 v;         // vitesse en m/s (v.x,v.y)
	Vec2 f;         // force en N (f.x, f.y)
	float m;        // masse en kg
	Image terrain;
	int score;
};

struct Gardien
{
    Particle goal;
    Image imgoal;
};

Vec2 make_vec2(float x, float y)
{
    Vec2 v;
    v.x = x;
    v.y = y;
    return v;
}

// LES OPERATEURS

Vec2 operator+(const Vec2& a, const Vec2& b)
{
	Vec2 r;
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	return r;
}

Vec2 operator/(Vec2 a,float b)
{
    Vec2 v;
    v.x = a.x / b;
    v.y = a.y / b;
    return v;
}

Vec2 operator-(const Vec2& a, const Vec2& b)
{
	Vec2 r;
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	return r;
}

Vec2 operator+=(Vec2& a, const Vec2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vec2 operator*(float a, const Vec2& b)
{
	Vec2 r;
	r.x = a * b.x;
	r.y = a * b.y;
	return r;
}

// LES PROCEDURES INIT

void initBallon(Particle & part)
{
    part.m = 1.0;
    part.p.x = 10;
    part.p.y = 250;
    part.v.x = frand(80.f, 130.f);  // C'est ici que la balle va tirer automatiquement et dans diffrents cotés
    part.v.y = frand(30.f, 90.f);
}

void initGardien (Gardien & g)
{
    g.goal.p = make_vec2(DIMW/2+30, DIMW/2+30); // On place le gardien sur sa ligne
    g.imgoal = image("/data/PONG/Gardien.png");
}
void init(Particle & part, Gardien & g)
{
    part.score = 0;
    initGardien(g);
    initBallon(part);
    part.terrain = image("/data/PONG/Terrain.jpg");
}

// LES PROCEDURES UPDATE

void updateParticle(Particle& part)
{
    const float dt = 0.007;
    if (part.m>0)
    {
        part.v = part.v + (dt/part.m)*part.f;     // mise à jour de la vitesse
        part.p = part.p + dt*part.v;              // mise à jour de la position
        part.f.x = 0,8;
        part.f.y = 0,5;
    }
}

void collision(Particle& part, Gardien &g)
{
	int i;

		if (part.p.x < 0) // Si la balle par hors des limites de la fenetre, on relance la balle
		{
			initBallon(part);
		}

		if (part.p.y < 200) // La collision se fait a un certain niveau pour plus de réalisme
		{
			part.p.y = 200;
			part.v.y = -part.v.y;
			part.v = FRICTION * part.v;
		}

		if (part.p.x >= DIMW)
		{
            if (part.p.y <= 400 && part.p.y >= 250) // 400 et 250 des valeurs où est implanté les cages. Si la balle se trouve dans cette position, c'est qu'il y a but
            {
                part.score--;
                initBallon(part); // Apres le but, on relance la balle et on décremente un point pour le joueur. On peut avoir un score négatif
                cout << "Et c'est le but !!" << endl;
            }
            initBallon(part);  // Si le tir n'est pas cadré (ça arrive et fait volontairement), on relance la balle
		}

		if (part.p.y >= DIMW)
		{
            initBallon(part);
		}

        if (part.p.x >= g.goal.p.x+180 && part.p.y >= g.goal.p.y && part.p.y < g.goal.p.y+80) // Quand le gardien arrete la balle, on incrémente d'un point le joueur
        {
            part.score++;
            part.v.x=-part.v.x;
            cout<<"Quel arret!!!"<<endl;
        }
}


void computeParticleForceGravityEarth(Particle& part)
{
    part.f = part.f + make_vec2(0, -part.m * G); // pour mettre en place la gravitation de la balle
}

void update_gardien (Gardien & g)
{
    const float d = 1.0f;
        if (isKeyPressed(SDLK_UP)) // Cela permet de deplacer le gardien
    {
    if (g.goal.p.y < DIMW) g.goal.p.y+=d;
        if (g.goal.p.y > DIMW-150) g.goal.p.y=DIMW-150;
    }

    if (isKeyPressed(SDLK_DOWN))  // Cela permet de deplacer le gardien
    {
        if (g.goal.p.y < 270) g.goal.p.y=-g.goal.p.y*d;
        if (g.goal.p.y > 270) g.goal.p.y-=d;
    }
}

void update(Particle& part, Gardien & g)

{
    computeParticleForceGravityEarth(part);
    updateParticle(part);
    update_gardien(g);
    collision(part, g);
}

// LA FONCTION DRAW

void draw(Particle& part, Gardien & g)
{
    image_draw(part.terrain, 0, 0, DIMW, DIMW); // Le terrain de foot
    image_draw(g.imgoal , g.goal.p.x +150  ,g.goal.p.y-40  ,45 ,110); // Le gardien de but

    color(255, 255, 255);
    circleFill(part.p.x, part.p.y, 4); // Le ballon de foot

    print (400, 480, "Score :");
    print (440, 480, part.score);

}

// LA FONCTION MAIN

int main(int , char ** )
{
    Particle part;
    Gardien g;
    bool stop=false;
	winInit("Particles", DIMW, DIMW);
    setKeyRepeatMode(true);
	init(part, g);

    Menu menu;
    menu_add( menu, "Recommencer" );
    menu_add( menu, "Jouer");

    menu_setSelect(menu, 0);
    backgroundColor( 0, 150, 50 );

	while( !stop )
    {
        winClear();
        switch( menu_select(menu))
        {
            case 0: init(part, g); menu_setSelect(menu,1); break;
            case 1:
                draw(part, g);
                update(part, g);
                break;

        }

        menu_draw( menu );
        stop = winDisplay();
    }
    winQuit();
	return 0;
}


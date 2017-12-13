#include <GL/glut.h>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <deque>

using namespace std;

int scene = 0;
unsigned int counter = 0;
int HP = 3;

class Point
{
  public:
	double x;
	double y;
	double z;
	Point(double a, double b, double c) : x(a), y(b), z(c) {}
};

class Color
{
  public:
	double r;
	double g;
	double b;
	Color(double x, double y, double z) : r(x), g(y), b(z) {}
};

class Vector
{
  public:
	double x;
	double y;
	double z;
	Vector() : x(0), y(0), z(0) {}
	Vector(double a, double b, double c) : x(a), y(b), z(c) {}
	Vector(const Vector &orig)
	{
		x = orig.x;
		y = orig.y;
		z = orig.z;
	}
	Vector operator+() const { return *this; }
	Vector operator-() const { return Vector(-x, -y, -z); }
	double dot(const Vector &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	Vector cross(const Vector &v) const
	{
		double x = y * v.z - z * v.y;
		double y = z * v.x - x * v.z;
		double z = x * v.y - y * v.x;
		return Vector(x, y, z);
	}
	double length() const
	{
		double r = x * x + y * y + z * z;
		return sqrt(r);
	}
	double cos(const Vector &v) const
	{
		return this->dot(v) / this->length() / v.length();
	}
	Vector norm() const
	{
		double l = this->length();
		return Vector(x / l, y / l, z / l);
	}
	Vector operator+(const Vector &v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}
	Vector operator-(const Vector &v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}
	Vector operator*(double d) const
	{
		return Vector(x * d, y * d, z * d);
	}
	bool Vector::operator==(const Vector &v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}
	bool Vector::operator!=(const Vector &v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}
};

Point *look_place = new Point(0, 0, 0);
Point *look_point = new Point(0, 0, 1.0);

class Obj
{
  public:
	Point *p;
	Color *c;
	double hp;

	Obj(Point *pp, Color *cc) : p(pp), c(cc), hp(100) {}
	virtual ~Obj()
	{
		delete p;
		delete c;
	}
};
class Bullet : public Obj
{
  public:
	//�����ł�Point*p�͕`����W�ł͂Ȃ��A���e�_���W
	Point *drawp;
	double r; //�e�ۂ̑傫��
	Bullet(double rr, Point *pp, Point *fromp, Color *cc) : Obj(pp, cc), r(rr), drawp(fromp) {}
	void changeZ(double zz) { drawp->z = zz; }
	void draw()
	{
		//drawp->x += p->x / 100;
		//drawp->y += p->y / 100;
		glColor3f(c->r, c->g, c->b);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 100; i++)
		{
			double rate = (double)i / 100;
			double cx = r * cos(2 * 3.141592 * rate);
			double cy = r * sin(2 * 3.141592 * rate);
			glVertex3f(cx + drawp->x, cy + drawp->y, drawp->z);
		}
		glEnd();
	}
	bool in_circle(Point *pp, double dd)
	{
		double xxx = pp->x;
		double yyy = pp->y;
		double dist = sqrt((drawp->x - xxx) * (drawp->x - xxx) + (drawp->y - yyy) * (drawp->y - yyy));
		return dist < dd + r;
	}
};
deque<Bullet *> bullets2;
class Target : public Obj
{
  public:
	double d;
	static int cnt;
	int number;
	static int counter2;
	Target(double dd, Point *pp, Color *cc) : Obj(pp, cc), d(dd), number(rand() % 3000) {}
	~Target() {}
	void draws()
	{
		glColor3f(c->r, c->g, c->b);
		glBegin(GL_TRIANGLES);
		glVertex3f(p->x, p->y + d * sqrt(3) / 3, p->z);
		glVertex3f(p->x - d / 2, p->y - sqrt(3) / 6, p->z);
		glVertex3f(p->x + d / 2, p->y - sqrt(3) / 6, p->z);
		glEnd();
	}
	void draw()
	{
		glColor3f(c->r, c->g, c->b);
		glBegin(GL_POLYGON);
		glVertex3f(0.05 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.1 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.05 + p->x, 0.05 + p->y, p->z);
		glVertex3f(0.1 + p->x, 0.05 + p->y, p->z); //1
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.1 + p->x, 0 + p->y, p->z);
		glVertex3f(0.15 + p->x, 0 + p->y, p->z);
		glVertex3f(0.1 + p->x, 0.1 + p->y, p->z);
		glVertex3f(0.15 + p->x, 0.1 + p->y, p->z); //2
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(0.15 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.2 + p->x, 0 + p->y, p->z);
		glVertex3f(0.15 + p->x, -0.05 + p->y, p->z);
		glVertex3f(0.2 + p->x, -0.05 + p->y, p->z); //3
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.15 + p->x, -0.15 + p->y, p->z);
		glVertex3f(0.2 + p->x, -0.15 + p->y, p->z);
		glVertex3f(0.15 + p->x, -0.1 + p->y, p->z);
		glVertex3f(0.2 + p->x, -0.1 + p->y, p->z);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(0.2 + p->x, 0 + p->y, p->z);
		glVertex3f(0.2 + p->x, -0.1 + p->y, p->z);
		glVertex3f(0.25 + p->x, 0 + p->y, p->z);
		glVertex3f(0.25 + p->x, -0.1 + p->y, p->z); //4
		glVertex3f(0.2 + p->x, 0.05 + p->y, p->z);
		glVertex3f(0.25 + p->x, 0.05 + p->y, p->z);
		glVertex3f(0.2 + p->x, 0.15 + p->y, p->z);
		glVertex3f(0.25 + p->x, 0.15 + p->y, p->z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.2 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.25 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.2 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.25 + p->x, 0.25 + p->y, p->z);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(0.25 + p->x, 0.15 + p->y, p->z);
		glVertex3f(0.25 + p->x, -0.05 + p->y, p->z);
		glVertex3f(0.4 + p->x, 0.15 + p->y, p->z);
		glVertex3f(0.4 + p->x, -0.05 + p->y, p->z); //5,6,7
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(0.4 + p->x, 0 + p->y, p->z);
		glVertex3f(0.4 + p->x, -0.1 + p->y, p->z);
		glVertex3f(0.45 + p->x, 0 + p->y, p->z);
		glVertex3f(0.45 + p->x, -0.1 + p->y, p->z); //8
		glVertex3f(0.4 + p->x, 0.05 + p->y, p->z);
		glVertex3f(0.45 + p->x, 0.05 + p->y, p->z);
		glVertex3f(0.4 + p->x, 0.15 + p->y, p->z);
		glVertex3f(0.45 + p->x, 0.15 + p->y, p->z);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(0.45 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.5 + p->x, 0 + p->y, p->z);
		glVertex3f(0.45 + p->x, -0.05 + p->y, p->z);
		glVertex3f(0.5 + p->x, -0.05 + p->y, p->z); //9
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.45 + p->x, -0.15 + p->y, p->z);
		glVertex3f(0.5 + p->x, -0.15 + p->y, p->z);
		glVertex3f(0.45 + p->x, -0.1 + p->y, p->z);
		glVertex3f(0.5 + p->x, -0.1 + p->y, p->z);
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.5 + p->x, 0 + p->y, p->z);
		glVertex3f(0.55 + p->x, 0 + p->y, p->z);
		glVertex3f(0.5 + p->x, 0.1 + p->y, p->z);
		glVertex3f(0.55 + p->x, 0.1 + p->y, p->z); //2
		glEnd();
		glBegin(GL_POLYGON);
		glVertex3f(0.55 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.6 + p->x, 0.2 + p->y, p->z);
		glVertex3f(0.55 + p->x, 0.05 + p->y, p->z);
		glVertex3f(0.6 + p->x, 0.05 + p->y, p->z); //11
		glEnd();
	}

	void move(double *x, double *z, double *X, double *XX, double *XXX, double *XXXX, double *XXXXX)
	{
		if (counter % 1000 == 0)
		{ //�܂��͂��߂ɂǂ����Ɉړ����邩�ŏꍇ��������B
			if (cnt % 2 != 0)
			{
				if (*X >= 3.9)
				{ //��ԍ�������������
					cnt++;
					*XXXXX = 0.3;
					*XXXX = 0.9;
					*XXX = 1.5;
					*XX = 2.1;
					*x = 2.7;
					*X = 3.3;
					z--;
				}
				else if (p->x != 3.9)
				{
					*x += 0.6;
					*X += 0.6;
					*XX += 0.6;
					*XXX += 0.6;
					*XXXX += 0.6;
					*XXXXX += 0.6;
				}
			}
			else if (cnt == 0 || cnt % 2 == 0)
			{
				if (*XXXXX <= -4.5)
				{ 
					cnt++;
					*XXXXX = -3.9;
					*XXXX = -3.3;
					*XXX = -2.7;
					*XX = -2.1;
					*x = -1.5;
					*X = -0.9;
					z--;
				}
				else if (p->x != -4.5)
				{
					*x -= 0.6;
					*X -= 0.6;
					*XX -= 0.6;
					*XXX -= 0.6;
					*XXXX -= 0.6;
					*XXXXX -= 0.6;
					cout << p->x << endl;
					cout << cnt << endl;
				}
			}
		}
	}
	void shoot()
	{
		if (counter % 3000 == number % 3000)
		{
			if (bullets2.size() < 50)
			{
				bullets2.push_front(new Bullet(0.05, new Point(p->x, p->y, 1.0), new Point(p->x, p->y, p->z), new Color(1, 1, 0)));
			}
		}
	}

	void moves()
	{
		if (counter % 500 == 0)
		{
			double dist;
			if (cnt % 2 == 0)
				dist = -0.6;
			else
				dist = 0.6;
			p->x += dist;
		}
	}
	void ahead()
	{
		p->z -= 0.5;
		if (p->z <= 1.5)
		{
			scene = 3;
			glutPostRedisplay();
		}
	}
};
int Target::counter2 = 0;
class Wall : public Obj
{
  public:
	double width;
	double height;
	double depth;
	bool exist;
	Wall(Point *pp, Color *cc, double x, double y, double z) : Obj(pp, cc), width(x), height(y), depth(z), exist(true) {}
	~Wall() {}
	void draw()
	{
		glBegin(GL_QUADS);
		glColor3f(c->r, c->g, c->b);
		//��O
		glVertex3f(width / 2 + p->x, height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, -height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(width / 2 + p->x, -height / 2 + p->y, depth / 2 + p->z);
		//��
		glVertex3f(width / 2 + p->x, height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(width / 2 + p->x, height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(width / 2 + p->x, -height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(width / 2 + p->x, -height / 2 + p->y, depth / 2 + p->z);
		//�E
		glVertex3f(-width / 2 + p->x, height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, -height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, -height / 2 + p->y, -depth / 2 + p->z);
		//��
		glVertex3f(width / 2 + p->x, height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, -height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(width / 2 + p->x, -height / 2 + p->y, -depth / 2 + p->z);
		//��
		glVertex3f(width / 2 + p->x, height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(width / 2 + p->x, height / 2 + p->y, -depth / 2 + p->z);
		//��
		glVertex3f(width / 2 + p->x, -height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, -height / 2 + p->y, depth / 2 + p->z);
		glVertex3f(-width / 2 + p->x, -height / 2 + p->y, -depth / 2 + p->z);
		glVertex3f(width / 2 + p->x, -height / 2 + p->y, -depth / 2 + p->z);
		glEnd();
	}
	void damagedraw(double damage, double width, double height, double depth)
	{
		hp -= damage;
		if (hp < (100 / 3) * 2)
		{
			glColor3f(1, 1, 0);
			glBegin(GL_QUADS);
			//�O
			glNormal3f(0.0, 0.0, -1.0);
			glVertex3f(width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, depth / 2);
			glVertex3f(width / 2, -height / 2, depth / 2);
			//��
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f(width / 2, height / 2, depth / 2);
			glVertex3f(width / 2, height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, depth / 2);

			//�E
			glNormal3f(-1.0, 0.0, 0.0);
			glVertex3f(-width / 2, height / 2, -depth / 2);
			glVertex3f(-width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, -depth / 2);

			//��
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(width / 2, height / 2, -depth / 2);
			glVertex3f(-width / 2, height / 2, -depth / 2);
			glVertex3f(-width / 2, -height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, -depth / 2);

			//��
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, height / 2, -depth / 2);
			glVertex3f(width / 2, height / 2, -depth / 2);

			//��
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(width / 2, -height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, -depth / 2);
			glEnd();
		}
		else if (hp < 100 / 3)
		{
			glColor3f(1, 0, 0);
			glBegin(GL_QUADS);
			//�O
			glNormal3f(0.0, 0.0, -1.0);
			glVertex3f(width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, depth / 2);
			glVertex3f(width / 2, -height / 2, depth / 2);
			//��
			glNormal3f(1.0, 0.0, 0.0);
			glVertex3f(width / 2, height / 2, depth / 2);
			glVertex3f(width / 2, height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, depth / 2);
			//�E
			glNormal3f(-1.0, 0.0, 0.0);
			glVertex3f(-width / 2, height / 2, -depth / 2);
			glVertex3f(-width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, -depth / 2);
			//��
			glNormal3f(0.0, 0.0, 1.0);
			glVertex3f(width / 2, height / 2, -depth / 2);
			glVertex3f(-width / 2, height / 2, -depth / 2);
			glVertex3f(-width / 2, -height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, -depth / 2);
			//��
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, height / 2, depth / 2);
			glVertex3f(-width / 2, height / 2, -depth / 2);
			glVertex3f(width / 2, height / 2, -depth / 2);

			//��
			glNormal3f(0.0, -1.0, 0.0);
			glVertex3f(width / 2, -height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, depth / 2);
			glVertex3f(-width / 2, -height / 2, -depth / 2);
			glVertex3f(width / 2, -height / 2, -depth / 2);
			glEnd();
		}
	}
	void hit(double damage)
	{
		hp -= damage;
		if (hp <= 0)
		{
			exist = false;
		}
		else if (hp < 100 / 3)
		{
			Color *cc = new Color(1, 0, 0);
			delete c;
			c = cc;
		}
		else if (hp < 100 * 2 / 3)
		{
			Color *cc = new Color(1, 1, 0);
			delete c;
			c = cc;
		}
	}
	bool jadge(Point *bp, double br)
	{
		if (exist)
		{
			if (width / 2 + p->x > bp->x + br && bp->x - br > -1 * width / 2 + p->x)
			{
				if (height / 2 + p->y > bp->y + br && bp->y - br > -1 * height / 2 + p->y)
				{
					if (depth / 2 + p->z > bp->z + br && bp->z - br > -1 * depth / 2 + p->z)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
};

class Health : public Obj
{
  public:
	Health(Point *pp, Color *cc) : Obj(pp, cc) {}
	void draw()
	{
		glColor3f(c->r, c->g, c->b);
		glBegin(GL_POLYGON);
		double n = 0.7;
		for (int i = 0; i < 100; i++)
		{
			double rate = (double)i / 100;
			double cx = 0.5 * cos(2 * 3.141592 * rate);
			double cy = 0.5 * sin(2 * 3.141592 * rate);
			double z = p->z;
			glVertex3f((cx - p->x) * 0.1 + look_place->x, (cy - p->y) * 0.1 + look_place->y, z);
		}
		glEnd();
		glBegin(GL_POLYGON);
		for (int i = 0; i < 100; i++)
		{
			double rate = (double)i / 100;
			double cx = 0.5 * cos(2 * 3.141592 * rate);
			double cy = 0.5 * sin(2 * 3.141592 * rate);
			double z = p->z;
			glVertex3f((cx + n - p->x) * 0.1 + look_place->x, (cy - p->y) * 0.1 + look_place->y, z);
		}
		glEnd();
		glBegin(GL_POLYGON);
		double z = p->z;
		double k = 0.9;
		glVertex3f((-(p->x) + 1.18 * k) * 0.1 + look_place->x, (-(p->y) - 0.35) * 0.1 + look_place->y, z);
		glVertex3f((-(p->x) + k / 2.75) * 0.1 + look_place->x, (-(p->y) - 1.15 * k) * 0.1 + look_place->y, z);
		glVertex3f((-(p->x) - 0.4 * k) * 0.1 + look_place->x, (-(p->y) - 0.35) * 0.1 + look_place->y, z);
		glEnd();
	}
};

void render_string(double x, double y, double z, const string &str)
{
	glRasterPos3d(x, y, z);
	for (int i = 0; i < str.size(); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
	}
}

int Target::cnt = 0;
bool bul1 = true;
bool bul2 = true;
deque<Bullet *> bullets;
deque<Target *> enemies;
double theta = 0;
double x1 = 3.3;
double yy1 = 0.5;
double z1 = 10.0;
double x2 = 3.9;
double y2 = 0.5;
double z2 = 10.0;

double x3 = 2.7;
double yy3 = 0.5;
double z3 = 10.0;
double x4 = 2.1;
double y4 = 0.5;
double z4 = 10.0;
double x5 = 1.5;
double yy5 = 0.5;
double z5 = 10.0;
double x6 = 0.9;
double y6 = 0.5;
double z6 = 10.0;
double speed = 0;
bool setting = true;

void start()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(1.0, 1.0, 1.0);
	render_string(0.5, -0.75, 1.5, "PRESS SPACE TO START");
	glutSwapBuffers();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//��
	static Wall *wall1 = new Wall(new Point(1.5, 0, 1.5), new Color(0, 0, 1), 0.8, 1.0, 0.25);
	if (wall1->exist)
		wall1->draw();
	static Wall *wall2 = new Wall(new Point(-1.5, 0, 1.5), new Color(0, 0, 1), 0.8, 1.0, 0.25);
	if (wall2->exist)
		wall2->draw();

	double d = 0.6;

	Color *C2 = new Color(1, 0, 0.5); //���C�t�|�C���g�֌Wby�Â�����

	double x = 3.6;
	double y = -0.9;
	double z = 4;
	int n = 1;

	if (setting)
	{
		for (x; x > 0.6; x = x - 0.7)
		{
			for (y; y < 1.2; y = y + 0.3)
			{
				for (z; z < 6; z = z + 0.5)
				{
					Color *E = new Color(n % 3, 0, (n - 1) % 3);
					Target *t = new Target(d, new Point(x, y, z), E);
					n++;
					enemies.push_back(t);
					//delete t;
				}
				z = 4;
			}
			y = -0.8;
		}
		setting = false;
	}

	bool j = false; //�O�i����́H
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->p->x - 0.6 < -4.5 && enemies[i]->cnt % 2 == 0)
		{
			j = true;
			break;
		}
		else if (enemies[i]->p->x + 0.6 > 4.5 && enemies[i]->cnt % 2 == 1)
		{
			j = true;
			break;
		}
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		if (j)
		{
			enemies[i]->ahead();
		}
		else
		{
			enemies[i]->moves();
		}
		enemies[i]->draw();
		enemies[i]->shoot();
	}
	if (j)
		enemies[0]->cnt++;

	//���C�t�|�C���g
	Point *HPp1 = new Point(13, -9, 1.001);
	Point *HPp2 = new Point(13, -7, 1.001);
	Point *HPp3 = new Point(13, -5, 1.001);
	Health *h1 = new Health(HPp1, C2);
	Health *h2 = new Health(HPp2, C2);
	Health *h3 = new Health(HPp3, C2);
	switch (HP)
	{
	case 3:
		h1->draw();
	case 2:
		h2->draw();
	case 1:
		h3->draw();
		break;
	case 0:
		scene = 3;
		glutPostRedisplay();
	}

	//�����̋�
	for (int i = 0; i < int(bullets.size()); i++)
	{
		if (bullets[i]->drawp->z <= 5.5)
		{
			bool del = false;
			bullets[i]->changeZ(bullets[i]->drawp->z + 0.01);
			if (wall1->jadge(bullets[i]->drawp, bullets[i]->r))
			{
				bullets[i]->hp = 0;
				wall1->hit(5);
				del = true;
			}
			if (wall2->jadge(bullets[i]->drawp, bullets[i]->r))
			{
				bullets[i]->hp = 0;
				wall2->hit(5);
				del = true;
			}
			for (int j = 0; j < enemies.size(); j++)
			{
				if (bullets[i]->drawp->z - 0.02 < enemies[j]->p->z && bullets[i]->drawp->z + 0.02 > enemies[j]->p->z)
				{
					if (bullets[i]->in_circle(enemies[j]->p, enemies[j]->d))
					{
						delete enemies[j];
						enemies.erase(enemies.begin() + j);
						del = true;
						break;
					}
				}
			}
			if (del)
			{
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
				if (enemies.empty())
				{
					scene = 2;
					glutPostRedisplay();
				}
			}
			else
			{
				bullets[i]->draw();
			}
		}
		else
		{
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}

	//����̋�
	for (int i = 0; i < int(bullets2.size()); i++)
	{
		if (bullets2[i]->drawp->z > 1.0)
		{
			bool del = false;
			bullets2[i]->changeZ(bullets2[i]->drawp->z - 0.01);
			if (wall1->jadge(bullets2[i]->drawp, bullets2[i]->r))
			{
				bullets2[i]->hp = 0;
				wall1->hit(0.05);
				del = true;
			}
			if (wall2->jadge(bullets2[i]->drawp, bullets2[i]->r))
			{
				bullets2[i]->hp = 0;
				wall2->hit(2);
				del = true;
			}
			if (bullets2[i]->drawp->z < 1.1 && bullets2[i]->drawp->z > 1.0)
			{
				if (bullets2[i]->in_circle(look_place, 0.1))
				{
					HP--;
					del = true;
				}
			}
			if (del)
			{
				delete bullets2[i];
				bullets2.erase(bullets2.begin() + i);
			}
			else
			{
				bullets2[i]->draw();
			}
		}
		else
		{
			delete bullets2[i];
			bullets2.erase(bullets2.begin() + i);
		}
	}

	//glRotatef(0.1, 0.0, 0.0, 1.0);
	glutSwapBuffers();
	if (counter < UINT_MAX)
	{
		counter++;
	}
	else
	{
		counter = 0;
	}
}

void clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	glColor3d(1.0, 1.0, 0.0);
	render_string(0.5, 0.0, 1.5, "CONGRATULATIONS!!");
	glColor3d(1.0, 1.0, 1.0);
	render_string(-1.0, -0.75, 1.5, "PRESS ESC TO EXIT");
	glutSwapBuffers();
}

void gameover()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	glColor3d(1.0, 0.0, 0.0);
	render_string(0.5, 0.0, 1.5, "GAME OVER");
	glColor3d(1.0, 1.0, 1.0);
	render_string(-1.0, -0.75, 1.5, "PRESS ESC TO EXIT");
	glutSwapBuffers();
}

void master()
{
	switch (scene)
	{
	case 0:
		start();
		break;
	case 1:
		display();
		break;
	case 2:
		clear();
		break;
	case 3:
		gameover();
		break;
	}
}

void idle()
{
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == '\033')
	{
		exit(0);
	}
	if (scene == 0)
	{
		if (key == ' ')
		{
			scene = 1;
		}
	}
	else if (scene == 1)
	{
		if (key == 'w')
		{
			if (look_place->y < 1.0)
			{
				look_place->y += 0.02;
				look_point->y += 0.02;
			}
		}
		else if (key == 's')
		{
			if (look_place->y > -1.0)
			{
				look_place->y -= 0.02;
				look_point->y -= 0.02;
			}
		}
		else if (key == 'a')
		{
			if (look_place->x < 1.5)
			{
				look_place->x += 0.02;
				look_point->x += 0.02;
			}
		}
		else if (key == 'd')
		{
			if (look_place->x > -1.5)
			{
				look_place->x -= 0.02;
				look_point->x -= 0.02;
			}
		}
		if (key == ' ')
		{
			if (bullets.size() < 50)
			{
				bullets.push_front(new Bullet(0.05, new Point(look_place->x, look_place->y, 10.0), new Point(look_place->x, look_place->y, 1.0), new Color(1, 1, 1)));
				speed += 0.02;
			}
		}
		glLoadIdentity();
		gluLookAt(look_place->x, look_place->y, look_place->z, look_point->x, look_point->y, look_point->z, 0, 1, 0);
	}
}

void special(int key, int x, int y)
{
}

void mouse(int button, int state, int x, int y)
{
	if (scene == 1)
	{
		double px = double(x - 540) / 360 + look_place->x;
		double py = double(y - 360) / 360 + look_place->y;
		if (bullets.size() < 50)
		{
			bullets.push_front(new Bullet(0.05, new Point(px, py, 10.0), new Point(look_place->x, look_place->y, 1.0), new Color(1, 1, 1)));
			speed += 0.0002;
		}
	}
}

void openGLInit(int *argc, char **argv)
{
	glutInit(argc, argv); //���܂��Ȃ�
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(1080, 720);
	glutCreateWindow("sample");

	glutDisplayFunc(master);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.5, 1.5, -1.0, 1.0, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	gluLookAt(0, 0, 0, 0, 0, 1, 0, 1, 0);
}

int main(int argc, char **argv)
{
	openGLInit(&argc, argv);
	glutMainLoop();
}

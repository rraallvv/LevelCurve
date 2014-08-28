#include <vector>
#include <unordered_map>
#include <assert.h>
#include <OpenGL/gl.h>

typedef struct {
	float x;
	float y;
} Vec2;

class LevelCurveAPI
{
public:
	bool debug;
	
	Vec2 interpolation(Vec2 u, Vec2 v, float a) {
		Vec2 p;
		p.x = (1.0-a)*u.x+a*v.x;
		p.y = (1.0-a)*u.y+a*v.y;
		return p;
	}
	
	static LevelCurveAPI& getInstance()
	{
		static LevelCurveAPI    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
	
	void setProf(int p) {
		if (p > maxProf) prof = 0;
		else if (p < 0) prof = maxProf;
		else prof = p;
	}
	
	int getProf() {
		return prof;
	}
	
private:
	int maxProf = 7;
	int prof;
	
	LevelCurveAPI(): prof(4), debug(true)
	{};
	
	LevelCurveAPI(LevelCurveAPI const&);
	void operator=(LevelCurveAPI const&);
};

class Square {
	Vec2 A;
	bool preEvaluated = false;
	std::unordered_map<int, float> coeff;
	std::vector<Vec2> dualVertices;
	
public:
	float deltaX;
	float deltaY;
	
	Vec2 primVertices(int i) {
		switch (i) {
			case 0:
				return A;
			case 1:
				return (Vec2){A.x+deltaX, A.y};
			case 2:
				return (Vec2){A.x+deltaX, A.y+deltaY};
			case 3:
				return (Vec2){A.x, A.y+deltaY};
			default:
				assert(0);
		}
	}
	
	int edgesValue = -1;
	
	Square(Vec2 A, float deltaX, float deltaY)
	:A(A), deltaX(deltaX), deltaY(deltaY)
	{
	}
	
	void preEvaluate(float (*density)(Vec2))
	{
		if (!preEvaluated) {
			edgesValue = 0;
			for (int i = 0 ; i < 4; ++i) {
				float zde = density(primVertices(i));
				coeff[i] = zde;
				edgesValue += (zde < 0 ? 1 : 0) << i;
			}
			preEvaluated = true;
		}
	}
	
	void evaluate(float (*density)(Vec2)) {
		preEvaluate(density);
		for (int i = 0 ; i < 4; i++)
		{
			// interest point
			int from = i;
			int to = (from < 3) ? from+1 : 0;
			// calculate cross position
			float c1 = coeff[from];
			float c2 = coeff[to];
			float a = c1/(c1-c2);
			Vec2 p1 = primVertices(from);
			Vec2 p2 = primVertices(to);
			// building edge :
			if (c1<0) dualVertices.push_back(p1);
			if (c1*c2<0) dualVertices.push_back(LevelCurveAPI::getInstance().interpolation(p1, p2, a));
		}
	}
	
	void drawEdges() {
		glColor3f(.5, .5, .5);
		glBegin(GL_LINE_LOOP);
		for (int i = 0 ; i < 4 ; ++i) {
			glVertex2f(primVertices(i).x, primVertices(i).y);
		}
		glEnd();
	}
	
	void draw() {
		glColor3f(0, 0, 1);
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_POLYGON);
		for (size_t i = 0 ; i < dualVertices.size(); ++i)
			glVertex2f(dualVertices[i].x, dualVertices[i].y);
		glEnd();
		
		glColor3f(0, 1, 0);
		glBegin(GL_LINE_LOOP);
		for (size_t i = 0 ; i < dualVertices.size(); ++i)
			glVertex2f(dualVertices[i].x, dualVertices[i].y);
		glEnd();
	}
};

// adding method to CanvasRenderingContext2D
/*
 * @params:
 *  equation : must take a 2-array as input and return a float example : function (p) { return p[0] ; }
 *  x, y, width, height: rectangle where you want draw your curve
 *  prof [optional] : integer (7 by default). Put higher values to get more precision (may be time consumer)
 *  debug [optional] : draw the computed quadtree in a light mode.
 */

class Quadtree {
	Square square;
	std::vector<Quadtree> childs;
	int prof;
	
public:
	Quadtree(Square square, float (*density)(Vec2), int prof)
	:square(square), prof(prof)
	{
		if (LevelCurveAPI::getInstance().debug)
			square.drawEdges();
		
		if (prof > 0)
		{
			square.preEvaluate(density);
			// s'il faut splitter
			if ((square.edgesValue != 0b0000 && square.edgesValue != 0b1111) ||
				prof == LevelCurveAPI::getInstance().getProf())
			{
				Vec2 a = square.primVertices(0);
				float dx = square.deltaX * 0.5;
				float dy = square.deltaY * 0.5;
				Square tl = Square(a, dx, dy);
				Square tr = Square((Vec2){a.x + dx, a.y}, dx, dy);
				Square bl = Square((Vec2){a.x, a.y+dy}, dx, dy);
				Square br = Square((Vec2){a.x + dx, a.y+dy}, dx, dy);
				Quadtree topLeft = Quadtree(tl, density, prof-1);
				Quadtree topRight = Quadtree(tr, density, prof-1);
				Quadtree bottomRight = Quadtree(br, density, prof-1);
				Quadtree bottomLeft = Quadtree(bl, density, prof-1);
				childs.push_back(topLeft);
				childs.push_back(topRight);
				childs.push_back(bottomLeft);
				childs.push_back(bottomRight);
			} else if (square.edgesValue == 0b1111) {
				square.evaluate(density);
				square.draw();
			}
		} // sinon on dessine
		else {
			square.evaluate(density);
			square.draw();
		}
	}
};

Quadtree levelCurve(float (*equation)(Vec2), float x, float y, float width, float height)
{
	Square zou = Square((Vec2){x, y}, width, height);
	return Quadtree(zou, equation, LevelCurveAPI::getInstance().getProf());
}

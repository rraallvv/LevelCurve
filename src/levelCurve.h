#include <vector>
#include <unordered_map>
#include <string>
#include <assert.h>
#include <OpenGL/gl.h>

typedef struct {
	float x;
	float y;
} Vec2;

class LevelCurveAPI
{
	bool debug = false;
	
public:
	int maxProf = 7;
	std::unordered_map<std::string, std::vector<int>> lookup_edges;
	
	Vec2 lib_vector(Vec2 u, Vec2 v, float a, float b) {
		return (Vec2){a*u.x + b*v.x, a*u.y + b*v.y};
	}
	
	static LevelCurveAPI& getInstance()
	{
		static LevelCurveAPI    instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
	
private:
	LevelCurveAPI() {
		lookup_edges["0000"] = std::vector<int>(0);
		
		int v1[] = {2, 3};
		lookup_edges["0001"] = std::vector<int>(v1, v1 + 2);
		
		int v2[] = {1, 2};
		lookup_edges["0010"] = std::vector<int>(v2, v2 + 2);
		
		int v3[] = {0, 1};
		lookup_edges["0100"] = std::vector<int>(v3, v3 + 2);
		
		int v4[] = {3, 0};
		lookup_edges["1000"] = std::vector<int>(v4, v4 + 2);
		
		int v5[] = {1, 3};
		lookup_edges["0011"] = std::vector<int>(v5, v5 + 2);
		
		int v6[] = {0, 1, 2, 3};
		lookup_edges["0101"] = std::vector<int>(v6, v6 + 4);
		
		int v7[] = {0, 2};
		lookup_edges["1001"] = std::vector<int>(v7, v7 + 2);
		
		int v8[] = {0, 2};
		lookup_edges["0110"] = std::vector<int>(v8, v8 + 2);
		
		int v9[] = {1, 2, 3, 0};
		lookup_edges["1010"] = std::vector<int>(v9, v9 + 4);
		
		int v10[] = {1, 3};
		lookup_edges["1100"] = std::vector<int>(v10, v10 + 2);
		
		int v11[] = {3, 0};
		lookup_edges["0111"] = std::vector<int>(v11, v11 + 2);
		
		int v12[] = {0, 1};
		lookup_edges["1011"] = std::vector<int>(v12, v12 + 2);
		
		int v13[] = {1, 2};
		lookup_edges["1101"] = std::vector<int>(v13, v13 + 2);
		
		int v14[] = {2, 3};
		lookup_edges["1110"] = std::vector<int>(v14, v14 + 2);
		
		lookup_edges["1111"] = std::vector<int>(0);
	};                   // Constructor? (the {} brackets) are needed here.
	// Dont forget to declare these two. You want to make sure they
	// are unaccessable otherwise you may accidently get copies of
	// your singleton appearing.
	LevelCurveAPI(LevelCurveAPI const&);              // Don't Implement
	void operator=(LevelCurveAPI const&); // Don't implement
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
	
	std::string edgesValue = "";
	
	Square(Vec2 A, float deltaX, float deltaY)
	:A(A), deltaX(deltaX), deltaY(deltaY)
	{
	}
	
	void preEvaluate(float (*density)(Vec2))
	{
		if (!preEvaluated) {
			for (int i = 0 ; i < 4; ++i) {
				float zde = density(primVertices(i));
				coeff[i] = zde;
				edgesValue += (zde < 0) ? '1' : '0';
			}
			preEvaluated = true;
		}
	}
	
	void evaluate(float (*density)(Vec2)) {
		preEvaluate(density);
		std::vector<int> ed = LevelCurveAPI::getInstance().lookup_edges[edgesValue];
		for (int i = 0 ; i < ed.size(); i++)
		{
			// interest point
			int from = ed[i];
			int to = (from < 3) ? from+1 : 0;
			// coeffs
			float c1 = abs(coeff[from]);
			float c2 = abs(coeff[to]);
			float a = c1 / (c1 + c2);
			float b = c2 / (c1 + c2);
			// building edge :
			dualVertices.push_back(LevelCurveAPI::getInstance().lib_vector(primVertices(from), primVertices(to), b, a));
		}
	}
	
	void draw(bool debug)
	{
		if (debug) {
			glColor3f(.5, .5, .5);
			glBegin(GL_LINE_LOOP);
			for (int i = 0 ; i < 4 ; ++i) {
				glVertex2f(primVertices(i).x, primVertices(i).y);
			}
			glEnd();
		}
		
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		for (size_t i = 0 ; i < dualVertices.size(); i+=2)
		{
			glVertex2f(dualVertices[i].x, dualVertices[i].y);
			glVertex2f(dualVertices[i+1].x, dualVertices[i+1].y);
		}
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
	Quadtree(Square square, float (*density)(Vec2), int prof, bool debug)
	:square(square), prof(prof)
	{
		if (prof > 0)
		{
			if (debug) {
				square.draw(true);
			}
			square.preEvaluate(density);
			// s'il faut splitter
			if ((square.edgesValue.compare("0000") != 0 && square.edgesValue.compare("1111") != 0) || prof == LevelCurveAPI::getInstance().maxProf)
			{
				Vec2 a = square.primVertices(0);
				float dx = square.deltaX * 0.5;
				float dy = square.deltaY * 0.5;
				Square tl = Square(a, dx, dy);
				Square tr = Square((Vec2){a.x + dx, a.y}, dx, dy);
				Square bl = Square((Vec2){a.x, a.y+dy}, dx, dy);
				Square br = Square((Vec2){a.x + dx, a.y+dy}, dx, dy);
				Quadtree topLeft = Quadtree(tl, density, prof-1, debug);
				Quadtree topRight = Quadtree(tr, density, prof-1, debug);
				Quadtree bottomRight = Quadtree(br, density, prof-1, debug);
				Quadtree bottomLeft = Quadtree(bl, density, prof-1, debug);
				childs.push_back(topLeft);
				childs.push_back(topRight);
				childs.push_back(bottomLeft);
				childs.push_back(bottomRight);
			}
		} // sinon on dessine
		else {
			square.evaluate(density);
			square.draw(debug);
		}
	}
};

Quadtree levelCurve(float (*equation)(Vec2), float x, float y, float width, float height, int prof = 0, bool debug = false)
{
	if (prof != 0) {
		LevelCurveAPI::getInstance().maxProf = prof;
	}
	Square zou = Square((Vec2){x, y}, width, height);
	return Quadtree(zou, equation, LevelCurveAPI::getInstance().maxProf, debug);
}

// iso-surfaces tests
float surface(Vec2 p) {
	float x= (p.x-300);
	float y = (p.y-300);
	return cos(x/50)*x*x+ y*y - 10000;
}

float surface2(Vec2 p) {
	float x= (p.x-300);
	float y = (p.y-300);
	return 5*x*x + y*y - 10000;
}
